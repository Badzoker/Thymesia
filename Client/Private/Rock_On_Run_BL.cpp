#include "pch.h"
#include "Rock_On_Run_BL.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_BL::CRock_On_Run_BL()
{
}

HRESULT CRock_On_Run_BL::Initialize()
{
	return S_OK;
}

void CRock_On_Run_BL::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CRock_On_Run_BL::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_BL::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_BL* CRock_On_Run_BL::Create()
{
	CRock_On_Run_BL* pInstance = new CRock_On_Run_BL();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Rock_On_Run_BL");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_BL::Free()
{
	__super::Free();
}
