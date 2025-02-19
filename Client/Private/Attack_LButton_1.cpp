#include "pch.h"
#include "Attack_LButton_1.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Player.h"


CAttack_LButton_1::CAttack_LButton_1()
{
}

HRESULT CAttack_LButton_1::Initialize()
{
	return S_OK;
}

void CAttack_LButton_1::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;


	_vector vLook = XMVector3Normalize(XMVector3Cross(CamRight, vUp)); // 외적을 하면 각이 2개 생기긴 하는데 흠.. 이건 나중에 고민하기. 

	// vLook과 캐릭터의 look 의 내적구하기 
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

	float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, vLook);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f) {
		Radian = -Radian;
	}


	pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
	vCurPosition.m128_f32[1] += pNavigation->Compute_Height(vCurPosition);
	pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);


	//dynamic_cast<CPlayer*>(pGameObject)->Set_ParentPhaseState(CPlayer::PHASE_IDLE); 

}

void CAttack_LButton_1::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CAttack_LButton_1::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CAttack_LButton_1* CAttack_LButton_1::Create()
{
	CAttack_LButton_1* pInstance = new CAttack_LButton_1();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Attack_LButton_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttack_LButton_1::Free()
{
	__super::Free();
}
