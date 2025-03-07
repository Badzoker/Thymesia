#include "pch.h"
#include "Normal_Evade_F.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_F::CNormal_Evade_F()
{
}

HRESULT CNormal_Evade_F::Initialize()
{
	return S_OK;
}

void CNormal_Evade_F::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	//_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f; 
	//pGameObject->Get_Transfrom()->Go_Backward_With_Navi(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CNormal_Evade_F::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_F::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_F* CNormal_Evade_F::Create()
{
	CNormal_Evade_F* pInstance = new CNormal_Evade_F();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_F");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_F::Free()
{
	__super::Free();
}
