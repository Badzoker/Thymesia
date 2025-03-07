#include "pch.h"
#include "Normal_Evade_FR.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_FR::CNormal_Evade_FR()
{
}

HRESULT CNormal_Evade_FR::Initialize()
{
	return S_OK;
}

void CNormal_Evade_FR::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	//_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f; 
	//pGameObject->Get_Transfrom()->Go_Backward_With_Navi(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CNormal_Evade_FR::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_FR::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_FR* CNormal_Evade_FR::Create()
{
	CNormal_Evade_FR* pInstance = new CNormal_Evade_FR();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_FR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_FR::Free()
{
	__super::Free();
}
