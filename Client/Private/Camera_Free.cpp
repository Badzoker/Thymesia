﻿#include "pch.h" 
#include "Camera_Free.h"
#include "GameInstance.h"
#include "Player.h"
#include "Layer.h"


// 보간(Fade) 함수: 부드러운 변화
float Fade(float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

// 선형 보간(Lerp) 함수	
float Lerp(float a, float b, float t) {
	return a + t * (b - a);
}

// Gradient 계산 함수
float Grad(int hash, float x, float y) {
	int h = hash & 3;  // 0~3 값 사용
	float u = h < 2 ? x : y;
	float v = h < 2 ? y : x;
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

// 펄린 노이즈 계산 함수 (2D)
float PerlinNoise(float x, float y) {
	// 정수 좌표 찾기
	int xi = (int)floor(x) & 255;
	int yi = (int)floor(y) & 255;

	// 소수점 부분 (격자 내 위치)
	float xf = x - floor(x);
	float yf = y - floor(y);

	// 보간 함수 적용
	float u = Fade(xf);
	float v = Fade(yf);

	// 해시 테이블 (임의 값)
	static int p[512];
	static bool init = false;
	if (!init) {
		for (int i = 0; i < 256; ++i) p[256 + i] = p[i] = i;
		init = true;
	}

	// 네 개의 코너 포인트
	int aa = p[p[xi] + yi];
	int ab = p[p[xi] + yi + 1];
	int ba = p[p[xi + 1] + yi];
	int bb = p[p[xi + 1] + yi + 1];

	// Gradient 값 적용
	float gradAA = Grad(aa, xf, yf);
	float gradBA = Grad(ba, xf - 1, yf);
	float gradAB = Grad(ab, xf, yf - 1);
	float gradBB = Grad(bb, xf - 1, yf - 1);

	// 보간 (Interpolation)
	float lerpX1 = Lerp(gradAA, gradBA, u);
	float lerpX2 = Lerp(gradAB, gradBB, u);
	return Lerp(lerpX1, lerpX2, v);
}

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

	strcpy_s(m_szName, "Camera_Free");

	FREE_CAMERA_DESC* pDesc = static_cast<FREE_CAMERA_DESC*>(pArg);

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	map<const _wstring, class CLayer*>* mapLayer = m_pGameInstance->Get_Layers();
	auto& LevelLayer = mapLayer[3];	/* 애가 LEVEL_GAMEPLAY의 Layer 집합 이고 */

	CLayer* pLayer = LevelLayer.find(TEXT("Layer_Player"))->second;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pLayer->Get_GameObject_List().front());
	m_pPlayer = pPlayer;

	m_pPlayerTransformCom = dynamic_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")));
	Safe_AddRef(m_pPlayerTransformCom);

	_fvector CameraPos =
	{
		 XMVectorGetX(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
		 XMVectorGetY(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)) + 1.0f, // 머리 높이 보정
		 XMVectorGetZ(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
		 1.0f
	};

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, CameraPos);

	m_plistMonster = m_pGameInstance->Get_LayerGameObject(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));


	return S_OK;
}


void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if (m_pGameInstance->isMouseEnter(DIM_MB))
	{
		m_bLockOnOff = !m_bLockOnOff;
		m_bFirst = true;
		m_bLockOnCameraFirst = false;
	}

	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

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
	_vector vNewCamPos = m_vPlayerHeadPos - vCamDir * m_fCurCamDistance;

	_vector vLerpCamPos = XMVectorLerp(vCamPosition, vNewCamPos, 0.075f);


	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vLerpCamPos);	 // 위치를 이렇게 세팅하니깐 다시 위에서는 vCamPosition 값이 증가하게되어 보간효과 o 

	/* 이동속도를 줌으로써 계산을 진행? */
	float CosTheta = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	float OppostieCosTheta = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f, XMVectorSet(0.f, -1.f, 0.f, 0.f)));
	//angleRadians = acosf(cosTheta); // 라디안 값	
	float AngleDegrees = XMConvertToDegrees(acosf(CosTheta));         // 각도를 도(degree)로 변환							
	float OppositeAngleDegrees = XMConvertToDegrees(acosf(OppostieCosTheta)); // 각도를 도(degree)로 변환			




	/* 평상시 카메라 */
	if (!m_bLockOnOff)
	{

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
	}
	/* ======================================================= */

	/* 락온 카메라 */
	else // Lock On 카메라 상태 
	{
		///* 일단 몬스터 클래스들을 다 찾아야겠네*/
		if (m_bFirst)
		{
			m_pTargetMonster = Find_LockOnTarget();
			if (m_pTargetMonster == nullptr)
			{
				m_bLockOnOff = !m_bLockOnOff;
			}

			m_bFirst = false;
		}

		if (m_bLockOnOff)
		{
			/* 플레이어 회전 관련 */
			_vector PlayerPos = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector MonsterPos = m_pTargetMonster->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

			_vector vDir = (MonsterPos - PlayerPos);

			_vector vPlayerLook = m_pPlayerTransformCom->Get_State(CTransform::STATE_LOOK);

			_vector vLookPlayerXZ = XMVector2Normalize(_fvector{ vPlayerLook.m128_f32[0] ,vPlayerLook.m128_f32[2] }); // 이게 플레이어 xz 룩방향
			_vector vDirLook = XMVector2Normalize(_fvector{ vDir.m128_f32[0],vDir.m128_f32[2] });// 이게 현재 플레이어의 위치에서 몬스터 방향

			_float  dotResult = XMVectorGetX(XMVector2Dot(vLookPlayerXZ, vDirLook));
			dotResult = max(-1.0f, min(dotResult, 1.0f));
			float Radian = acosf(dotResult);

			_vector crossResult = XMVector2Cross(vDirLook, vLookPlayerXZ);
			float crossY = XMVectorGetY(crossResult);
			if (crossY < 0.0f) {
				Radian = -Radian;

			}
			/* =============================================== */


			/* 카메라 회전 관련 */





			/* 카메라 방향 전환 관련 */

			//m_pTransformCom->Orbit_Move_Once(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian* fTimeDelta, m_vLerpPlayerHeadPos);	
			LockOnCameraTurn(fTimeDelta);



			/* 플레이어 방향 전환 */
			//if (abs(Radian) > 0.1f)
			m_pPlayerTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, Radian * fTimeDelta * 6.f);
			m_bLockOnCameraFirst = true;


		}
	}
	/* ===================================================================== */

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



	/* 여기에 이벤트가 들어와야함. */
	if (m_bShakeOnOff)
	{
		_vector CamPos = Camera_Shake(fTimeDelta, m_pTransformCom->Get_State(CTransform::STATE_POSITION));

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, CamPos);
	}


	if (m_bZoomIn)
	{
		if (m_fCamCloseLimitDistance < m_fCurCamDistance)
			m_fCurCamDistance -= fTimeDelta;

		_vector vNewCamPos = m_vLerpPlayerHeadPos - vCamDir * m_fCurCamDistance;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewCamPos);
	}


	if (m_bZoomOut)
	{
		if (m_fCamFarLimitDistance > m_fCurCamDistance)
			m_fCurCamDistance += fTimeDelta * 20.f;

		_vector vNewCamPos = m_vLerpPlayerHeadPos - vCamDir * m_fCurCamDistance;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewCamPos);
	}



	m_bShakeOnOff = false;
	m_bZoomIn = false;
	m_bZoomOut = false;


	__super::Priority_Update(fTimeDelta);
}

void CCamera_Free::Update(_float fTimeDelta)
{

}

void CCamera_Free::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera_Free::Render()
{

	return S_OK;
}

CGameObject* CCamera_Free::Find_LockOnTarget()
{
	m_maptMonsterDistance.clear();
	/* 여기서 가장 가까운 몬스터를 리턴 해 줘야한다.*/
	for (auto& iter : *m_plistMonster)
	{
		// 절두체에 안에 있어야 하며, 그 중 가장 거리가 가까운놈을 가져와야함. 
		_vector MonsterPosition = iter->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

		if (m_pGameInstance->isIn_Frustum_WorldSpace(MonsterPosition))
		{
			_vector PlayerPosition = m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION);

			// 절두체 컬링을 해도 화면에 가려지느냐 안가려지느냐를 체크 안했네. 
			_float  Distance = XMVectorGetX(XMVector3Length(MonsterPosition - PlayerPosition));
			// 거리 제한 
			if (Distance < 25.f)
				m_maptMonsterDistance.emplace(Distance, iter);
		}
	}

	if (m_maptMonsterDistance.size() >= 1)
	{
		auto& Pair = m_maptMonsterDistance;

		m_pPlayer->Set_Lockon(true);
		return Pair.begin()->second;
	}

	else
	{
		m_pPlayer->Set_Lockon(false);
		return nullptr;
	}
}

void CCamera_Free::LockOnCameraTurn(_float fTimeDelta)
{
	///* 카메라 회전 관련 */
	_vector vCamCurPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector MonsterPos = m_pTargetMonster->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
	_vector vUp = _fvector{ 0.f,1.f,0.f,0.f };
	_vector vCamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector vCamRightXZ = _fvector{ vCamRight.m128_f32[0],0.f,vCamRight.m128_f32[2] };


	// 플레이어의 look 의 x,z 방향 벡터와 몬스터의 look의 x,z 방향 벡터를 가져와서 내적하고 회전하면 될 것 같다. 
	_vector vDir = (MonsterPos - vCamCurPos);

	_vector vFrontLook = XMVector3Normalize(XMVector3Cross(vCamRightXZ, vUp));
	_vector vDirLook = XMVector3Normalize(_fvector{ vDir.m128_f32[0],0.f,vDir.m128_f32[2] });// 이게 현재 카메라의 위치에서 몬스터 방향					

	// XZ 평면의 2D 벡터 생성 (x는 그대로, y에 z 값을 넣음)
	_vector vFrontLook2D = _fvector{ vFrontLook.m128_f32[0], vFrontLook.m128_f32[2], 0.f, 0.f };
	_vector vDirLook2D = _fvector{ vDirLook.m128_f32[0],   vDirLook.m128_f32[2],   0.f, 0.f };

	_float dotResult = XMVectorGetX(XMVector2Dot(vFrontLook2D, vDirLook2D));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	// 2D cross: XMVector2Cross returns a vector where the z component is the scalar cross product.
	// 하지만 XMVector2Cross는 결과를 (0,0, cross, 0)로 넣습니다.
	_vector crossResult = XMVector2Cross(vDirLook2D, vFrontLook2D);
	float crossScalar = XMVectorGetZ(crossResult);  // 여기서는 Z 성분을 사용합니다.	
	if (crossScalar < 0.0f)
	{
		Radian = -Radian;
	}

	m_pTransformCom->Orbit_Move_Once(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * fTimeDelta * 3.f, m_vLerpPlayerHeadPos);

}

void CCamera_Free::ShakeOn(_float _fXaxisShakeSpeed, _float _fZaxisShakeSpeed, _float _fXaxisMoveAmount, _float _fZaxisMoveAmount)
{
	m_bShakeOnOff = true;
	m_fXaxisShakeSpeed = _fXaxisShakeSpeed;
	m_fZaxisShakeSpeed = _fZaxisShakeSpeed;
	m_fXaxisMoveAmount = _fXaxisMoveAmount;
	m_fZaxisShakeSpeed = _fZaxisMoveAmount;
}

void CCamera_Free::ResetZoomInCameraPos()
{
	if (m_fCamDistance > m_fCurCamDistance)
		m_fCurCamDistance += m_fTimeDelta;
}

void CCamera_Free::ResetZoomOutCameraPos()
{
	if (m_fCamDistance < m_fCurCamDistance)
		m_fCurCamDistance -= m_fTimeDelta * 20.f;
}

_vector CCamera_Free::Camera_Shake(float deltaTime, XMVECTOR& cameraPosition)
{
	static float shakeTime = 0.0f;
	shakeTime += deltaTime;

	// 펄린 노이즈 기반 흔들림 값 생성
	float offsetX = PerlinNoise(shakeTime * m_fXaxisShakeSpeed, 0.5f) * deltaTime * m_fXaxisMoveAmount;
	float offsetY = 0.f;//PerlinNoise(shakeTime * 240.0f, 1.0f) * deltaTime * 4.f;	
	float offsetZ = PerlinNoise(shakeTime * m_fZaxisShakeSpeed, 1.5f) * deltaTime * m_fZaxisMoveAmount;

	XMVECTOR shakeOffset = XMVectorSet(offsetX, offsetY, offsetZ, 0.0f);
	XMVECTOR newCameraPos = XMVectorAdd(cameraPosition, shakeOffset);


	return newCameraPos;
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

