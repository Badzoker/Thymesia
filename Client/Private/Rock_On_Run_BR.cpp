#include "pch.h"
#include "Rock_On_Run_BR.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CRock_On_Run_BR::CRock_On_Run_BR()
{
}

HRESULT CRock_On_Run_BR::Initialize()
{
	return S_OK;
}

void CRock_On_Run_BR::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{

}

void CRock_On_Run_BR::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CRock_On_Run_BR::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CRock_On_Run_BR* CRock_On_Run_BR::Create()
{
	CRock_On_Run_BR* pInstance = new CRock_On_Run_BR();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CRock_On_Run_BR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRock_On_Run_BR::Free()
{
	__super::Free();
}
