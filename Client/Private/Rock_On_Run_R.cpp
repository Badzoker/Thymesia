#include "pch.h"
#include "Rock_On_Run_R.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_R::CRock_On_Run_R()
{
}

HRESULT CRock_On_Run_R::Initialize()
{
	return S_OK;
}

void CRock_On_Run_R::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

	_vector vLookFront = XMVector3Normalize(XMVector3Cross(CamRight, vUp));
	_vector vLookBack = XMVector3Normalize(XMVector3Cross(vUp, CamRight));

	// vLook과 캐릭터의 look 의 내적구하기 
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));


	float dotResult = XMVectorGetX(XMVector3Dot(vLookFront, PlayerLook));
	dotResult = max(-1.0f, min(dotResult, 1.0f));
	float Radian = acosf(dotResult);

	_vector crossResult = XMVector3Cross(PlayerLook, vLookFront);
	float crossY = XMVectorGetY(crossResult);
	if (crossY < 0.0f) {
		Radian = -Radian;
	}

	//pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);	
	//pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed, pNavigation);
	pGameObject->Get_Transfrom()->Go_Dir(CamRight, pNavigation, fTimeDelta * m_fWalkSpeed);

}

void CRock_On_Run_R::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_R::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_R* CRock_On_Run_R::Create()
{
	CRock_On_Run_R* pInstance = new CRock_On_Run_R();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_R::Free()
{
	__super::Free();
}
