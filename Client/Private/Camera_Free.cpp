#include "pch.h" 
#include "Camera_Free.h"

#include "GameInstance.h"
#include "Player.h"
#include "Layer.h"

CCamera_Free::CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCamera_Free::CCamera_Free(const CCamera_Free& Prototype)
	: CCamera(Prototype)
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera_Free::Initialize(void* pArg)
{
	FREE_CAMERA_DESC* pDesc = static_cast<FREE_CAMERA_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	map<const _wstring, class CLayer*>* mapLayer = m_pGameInstance->Get_Layers();
	auto& LevelLayer = mapLayer[3];	/* 애가 LEVEL_GAMEPLAY의 Layer 집합 이고 */

	CLayer* pLayer = LevelLayer.find(TEXT("Layer_Player"))->second;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pLayer->Get_GameObject_List().front());

	m_pPlayerTransformCom = dynamic_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")));
	Safe_AddRef(m_pPlayerTransformCom);

	_fvector CameraPos =
	{
		 XMVectorGetX(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
		 XMVectorGetY(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)) + 40.0f, // 머리 높이 보정
		 XMVectorGetZ(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)) - 30.0f,
		 1.0f
	};

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, CameraPos);


	return S_OK;
}


void CCamera_Free::Priority_Update(_float fTimeDelta)
{

	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	const float distance = 2.0f; // 플레이어와 카메라 거리		

	// 플레이어의 충돌체를 기준으로할까.	
	m_vPlayerHeadPos = XMVectorSet(
		XMVectorGetX(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
		XMVectorGetY(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)) + 1.f, // 머리 높이 보정	
		XMVectorGetZ(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
		1.0f
	);

	m_vLerpPlayerHeadPos = XMVectorLerp(m_vLerpPlayerHeadPos, m_vPlayerHeadPos, 0.075f);	// 자기 자신을 보간하므로 계속해서 값이 증가하거나 감소해서 변함

	// 카메라 이동 처리
	_vector vCamDir = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
	_vector vCamPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vNewCamPos = m_vPlayerHeadPos - vCamDir * distance;

	_vector vLerpCamPos = XMVectorLerp(vCamPosition, vNewCamPos, 0.075f);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLerpCamPos);	 // 위치를 이렇게 세팅하니깐 다시 위에서는 vCamPosition 값이 증가하게되어 보간효과 o 

	/* 이동속도를 줌으로써 계산을 진행? */
	float CosTheta = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	float OppostieCosTheta = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f, XMVectorSet(0.f, -1.f, 0.f, 0.f)));
	//angleRadians = acosf(cosTheta); // 라디안 값	
	float AngleDegrees = XMConvertToDegrees(acosf(CosTheta));         // 각도를 도(degree)로 변환							
	float OppositeAngleDegrees = XMConvertToDegrees(acosf(OppostieCosTheta)); // 각도를 도(degree)로 변환			


	if (m_pGameInstance->isKeyEnter(DIK_TAB))
	{
		if (m_bStop)
			m_bStop = false;
		else
			m_bStop = true;
	}

	if (!m_bStop)
	{

		if (MouseMoveX != 0 || MouseMoveY != 0)
		{
			// X축(Y축 기준 회전)
			if (MouseMoveX != 0)
			{
				m_pTransformCom->Orbit_Move(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMoveX * fTimeDelta * m_fMouseSensor, m_vLerpPlayerHeadPos);
			}

			// Y축(Right 축 기준 회전)
			if (MouseMoveY != 0)
			{

				if (AngleDegrees > 60.f && OppositeAngleDegrees > 80.f)
					m_pTransformCom->Orbit_Move(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMoveY * fTimeDelta * m_fMouseSensor, m_vLerpPlayerHeadPos);


				else if (AngleDegrees < 60.f && MouseMoveY < 0.f)
				{
					m_pTransformCom->Orbit_Move(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMoveY * fTimeDelta * m_fMouseSensor, m_vLerpPlayerHeadPos);
				}

				else if (OppositeAngleDegrees < 80.f && MouseMoveY > 0.f)
				{
					m_pTransformCom->Orbit_Move(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMoveY * fTimeDelta * m_fMouseSensor, m_vLerpPlayerHeadPos);
				}
			}
		}
	}

	// Look 벡터 갱신 (플레이어 머리 위치를 바라보도록 설정)

	_vector CamDir = XMVectorSetW(XMVector3Normalize(m_vLerpPlayerHeadPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)), 0.f);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, CamDir);

	// Right 벡터 갱신 (Up × Look)
	_vector vUp = m_pPlayerTransformCom->Get_State(CTransform::STATE_UP);
	_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, CamDir));

	// Up 벡터 갱신 (Look × Right)
	_vector vNewUp = XMVector3Normalize(XMVector3Cross(CamDir, vRight));

	// Transform 갱신
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vNewUp);





	__super::Priority_Update(fTimeDelta);
}

void CCamera_Free::Update(_float fTimeDelta)
{

	//m_vPrePlayerHeadPos = m_vPlayerHeadPos;	
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{


}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

CCamera_Free* CCamera_Free::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Free::Clone(void* pArg)
{
	CCamera_Free* pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CCamera_Free");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerTransformCom);
}
