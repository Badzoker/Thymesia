#include "pch.h"
#include "Normal_Evade_FL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_FL::CNormal_Evade_FL()
{
}

HRESULT CNormal_Evade_FL::Initialize()
{
	return S_OK;
}

void CNormal_Evade_FL::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	//_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f; 
	//pGameObject->Get_Transfrom()->Go_Backward_With_Navi(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CNormal_Evade_FL::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_FL::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_FL* CNormal_Evade_FL::Create()
{
	CNormal_Evade_FL* pInstance = new CNormal_Evade_FL();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_FL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_FL::Free()
{
	__super::Free();
}
