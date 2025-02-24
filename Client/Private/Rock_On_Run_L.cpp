#include "pch.h"
#include "Rock_On_Run_L.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_L::CRock_On_Run_L()
{
}

HRESULT CRock_On_Run_L::Initialize()
{
	return S_OK;
}

void CRock_On_Run_L::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CRock_On_Run_L::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_L::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_L* CRock_On_Run_L::Create()
{
	CRock_On_Run_L* pInstance = new CRock_On_Run_L();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_L");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_L::Free()
{
	__super::Free();
}
