#include "pch.h"
#include "Normal_Evade_L.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_L::CNormal_Evade_L()
{
}

HRESULT CNormal_Evade_L::Initialize()
{
	return S_OK;
}

void CNormal_Evade_L::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	//_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f; 
	//pGameObject->Get_Transfrom()->Go_Backward_With_Navi(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CNormal_Evade_L::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_L::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_L* CNormal_Evade_L::Create()
{
	CNormal_Evade_L* pInstance = new CNormal_Evade_L();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_L::Free()
{
	__super::Free();
}
