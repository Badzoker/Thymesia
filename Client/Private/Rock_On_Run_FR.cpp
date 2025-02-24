#include "pch.h"
#include "Rock_On_Run_FR.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_FR::CRock_On_Run_FR()
{
}

HRESULT CRock_On_Run_FR::Initialize()
{
	return S_OK;
}

void CRock_On_Run_FR::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CRock_On_Run_FR::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_FR::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_FR* CRock_On_Run_FR::Create()
{
	CRock_On_Run_FR* pInstance = new CRock_On_Run_FR();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_FR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_FR::Free()
{
	__super::Free();
}
