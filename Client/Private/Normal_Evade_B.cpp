#include "pch.h"
#include "Normal_Evade_B.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_B::CNormal_Evade_B()
{
}

HRESULT CNormal_Evade_B::Initialize()
{
	return S_OK;
}

void CNormal_Evade_B::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	//_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f; 
	//pGameObject->Get_Transfrom()->Go_Backward_With_Navi(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CNormal_Evade_B::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_B::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_B* CNormal_Evade_B::Create()
{
	CNormal_Evade_B* pInstance = new CNormal_Evade_B();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_B");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_B::Free()
{
	__super::Free();
}
