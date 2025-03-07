#include "pch.h"
#include "Normal_Evade_BR.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_BR::CNormal_Evade_BR()
{
}

HRESULT CNormal_Evade_BR::Initialize()
{
	return S_OK;
}

void CNormal_Evade_BR::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	//_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f; 
	//pGameObject->Get_Transfrom()->Go_Backward_With_Navi(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CNormal_Evade_BR::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_BR::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_BR* CNormal_Evade_BR::Create()
{
	CNormal_Evade_BR* pInstance = new CNormal_Evade_BR();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_BR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_BR::Free()
{
	__super::Free();
}
