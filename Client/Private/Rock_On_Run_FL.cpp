#include "pch.h"
#include "Rock_On_Run_FL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_FL::CRock_On_Run_FL()
{
}

HRESULT CRock_On_Run_FL::Initialize()
{
	return S_OK;
}

void CRock_On_Run_FL::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CRock_On_Run_FL::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_FL::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_FL* CRock_On_Run_FL::Create()
{
	CRock_On_Run_FL* pInstance = new CRock_On_Run_FL();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_FL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_FL::Free()
{
	__super::Free();
}
