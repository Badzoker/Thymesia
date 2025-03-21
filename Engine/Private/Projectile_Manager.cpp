#include "Projectile_Manager.h"
#include "GameInstance.h"

CProjectile_Manager::CProjectile_Manager()
	:m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CProjectile_Manager::Initialize()
{
	return S_OK;
}

void CProjectile_Manager::Priority_Update(_float _fTimeDelta)
{
	for (auto& iter : m_pFireProjectiles)
		iter->Priority_Update(_fTimeDelta);
}

void CProjectile_Manager::Update(_float _fTimeDelta)
{
	for (auto& iter : m_pFireProjectiles)
		iter->Update(_fTimeDelta);
}

void CProjectile_Manager::Late_Update(_float _fTimeDelta)
{
	for (auto& iter = m_pFireProjectiles.begin(); iter != m_pFireProjectiles.end();)
	{
		if ((*iter)->Get_IsFire() == false)
		{
			(*iter)->Reset_Projectile();

			if (m_pFireProjectiles.size() == 1)
			{
				m_pFireProjectiles.clear();
				break;
			}
			iter = m_pFireProjectiles.erase(iter);
		}
		else
		{
			(*iter)->Late_Update(_fTimeDelta);
			iter++;
		}
	}
}

HRESULT CProjectile_Manager::Add_Projectile(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, PROJECTILE_CATEGORY _eCategory, void* _pArg)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, _iPrototypeLevelIndex, _strPrototypeTag, _pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	CProjectile* pProjectile = dynamic_cast<CProjectile*>(pGameObject);
	if (pProjectile == nullptr)
	{
		Safe_Release(pProjectile);
		return E_FAIL;
	}

	m_pProjectiles[_eCategory].push_back(pProjectile);

	return S_OK;
}


HRESULT CProjectile_Manager::Fire_Projectile(PROJECTILE_CATEGORY _eCategory, _fvector vStartPos, _fvector vEndPos, _uint iCount)
{
	if (iCount <= 1)
	{
		for (auto& iter : m_pProjectiles[_eCategory])
		{
			//iCount에 따라 1발 쏠껀지 5발? 쏠건지 아마 카드만 5발일듯?
			if (!iter->Get_IsFire())
			{
				iter->Set_IsFire(true);
				iter->Set_Target(vStartPos, vEndPos);
				m_pFireProjectiles.push_back(iter);
				break;
			}
		}
	}
	else
	{
		_uint pCount = {};
		for (auto& iter : m_pProjectiles[_eCategory])
		{
			if (!iter->Get_IsFire())
			{
				iter->Set_IsMultiFire(true);
				iter->Set_Target(vStartPos, vEndPos);
				m_pFireProjectiles.push_back(iter);
				pCount++;
				if (pCount == iCount)
					break;
			}
		}
	}
	return S_OK;
}

CProjectile_Manager* CProjectile_Manager::Create()
{
	CProjectile_Manager* pInstance = new CProjectile_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CProjectile_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CProjectile_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_pProjectiles)
	{
		for (auto& iter : Pair.second)
		{
			Safe_Release(iter);
		}
		Pair.second.clear();
	}

	m_pProjectiles.clear();

	Safe_Release(m_pGameInstance);
}
