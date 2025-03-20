#include "pch.h"
#include "ArchiveSitLight_Up.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CArchiveSitLight_Up::CArchiveSitLight_Up()
{
}

HRESULT CArchiveSitLight_Up::Initialize()
{
	return S_OK;
}

void CArchiveSitLight_Up::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CArchiveSitLight_Up::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CArchiveSitLight_Up::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CArchiveSitLight_Up* CArchiveSitLight_Up::Create()
{
	CArchiveSitLight_Up* pInstance = new CArchiveSitLight_Up();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : ArchiveSitLight_Up");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CArchiveSitLight_Up::Free()
{
	__super::Free();
}
