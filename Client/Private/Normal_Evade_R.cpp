#include "pch.h"
#include "Normal_Evade_R.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CNormal_Evade_R::CNormal_Evade_R()
{
}

HRESULT CNormal_Evade_R::Initialize()
{
	return S_OK;
}

void CNormal_Evade_R::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	//_vector vBackDir = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK) * -1.f; 
	//pGameObject->Get_Transfrom()->Go_Backward_With_Navi(fTimeDelta * m_fWalkSpeed, pNavigation);
}

void CNormal_Evade_R::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CNormal_Evade_R::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CNormal_Evade_R* CNormal_Evade_R::Create()
{
	CNormal_Evade_R* pInstance = new CNormal_Evade_R();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Normal_Evade_R");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNormal_Evade_R::Free()
{
	__super::Free();
}
