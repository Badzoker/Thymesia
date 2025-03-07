#include "pch.h"
#include "Normal_Evade_BL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_BL::CNormal_Evade_BL()
{
}

HRESULT CNormal_Evade_BL::Initialize()
{
	return S_OK;
}

void CNormal_Evade_BL::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	//_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f; 
	//pGameObject->Get_Transfrom()->Go_Backward_With_Navi(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CNormal_Evade_BL::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_BL::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_BL* CNormal_Evade_BL::Create()
{
	CNormal_Evade_BL* pInstance = new CNormal_Evade_BL();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_BL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_BL::Free()
{
	__super::Free();
}
