#include "pch.h"
#include "HurtMFL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CHurtMFL::CHurtMFL()
{
}

HRESULT CHurtMFL::Initialize()
{
	return S_OK;
}

void CHurtMFL::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

	_vector vLookFront = XMVector3Normalize(XMVector3Cross(CamRight, vUp));
	_vector vLookBack = vLookFront * -1.f;
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

}

void CHurtMFL::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CHurtMFL::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CHurtMFL* CHurtMFL::Create()
{
	CHurtMFL* pInstance = new CHurtMFL();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CHurtMFL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHurtMFL::Free()
{
	__super::Free();
}
