#include "UI_Manager.h"
#include "UI_Scene.h"

#include "GameInstance.h"
#include "GameObject.h"

CUI_Manager::CUI_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CUI_Manager::Initialize(_uint iNumScenes)
{
	m_iNumScenes = iNumScenes;

	m_pScenes = new map<const _wstring, class CUI_Scene*>[m_iNumScenes];

	return S_OK;
}
void CUI_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
		{
			Pair.second->Priority_Update(fTimeDelta);
		}
	}
}
void CUI_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
			Pair.second->Update(fTimeDelta);
	}
}
void CUI_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
			Pair.second->Late_Update(fTimeDelta);
	}
}
void CUI_Manager::Clear(_uint iScenelIndex)
{
	for (auto& Pair : m_pScenes[iScenelIndex])
		Safe_Release(Pair.second);

	m_pScenes[iScenelIndex].clear();
}

void CUI_Manager::Clear_Choice(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag, CUIObject* pUIObj)
{
	CUI_Scene* pUIScene = Find_UIScene(iScenelIndex, strSceneTag);
	if (nullptr != pUIScene)
	{
		pUIScene->Clear_Choice(iUIType, pUIObj);
	}
}

void CUI_Manager::Clear_Last(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag)
{
	CUI_Scene* pUIScene = Find_UIScene(iScenelIndex, strSceneTag);
	if (nullptr != pUIScene)
	{
		pUIScene->Clear_Last(iUIType);
	}
}

void CUI_Manager::Clear_ALL()
{
	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
			Safe_Release(Pair.second);

		m_pScenes[i].clear();
	}

}

CUI_Scene* CUI_Manager::Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag)
{
	auto	iter = m_pScenes[iSceneIndex].find(strSceneTag);

	if (iter == m_pScenes[iSceneIndex].end())
		return nullptr;

	return iter->second;
}

map<const _wstring, class CUI_Scene*>* CUI_Manager::Find_UIScene_ALL()
{
	return m_pScenes;
}

HRESULT CUI_Manager::Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag,_uint iUIType, void* pArg)
{
	/* UIScene에 저장할 UIObj 복제해오기 */
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	CUI_Scene* pScene = Find_UIScene(iSceneIndex, strSceneTag);

	if (nullptr == pScene)
	{
		pScene = CUI_Scene::Create();

		pScene->Add_UIObject_Type(iUIType, pGameObject);
		m_pScenes[iSceneIndex].emplace(strSceneTag, pScene);
	}
	else
	{
		pScene->Add_UIObject_Type(iUIType, pGameObject); 
	}
	
	return S_OK;
}

HRESULT CUI_Manager::UIScene_UIObject_Render_OnOff(CUI_Scene* pScene, _bool bOpen)
{
	return pScene->UIScene_UIObject_Render_OnOff(bOpen);
}

CUI_Manager* CUI_Manager::Create(_uint iNumScenes)
{
	CUI_Manager* pInstance = new CUI_Manager();

	if (FAILED(pInstance->Initialize(iNumScenes)))
	{
		MSG_BOX("Failed To Created : CUI_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumScenes; i++)
	{
		for (auto& Pair : m_pScenes[i])
			Safe_Release(Pair.second);

		m_pScenes[i].clear();
	}

	Safe_Delete_Array(m_pScenes);
	Safe_Release(m_pGameInstance);
}
