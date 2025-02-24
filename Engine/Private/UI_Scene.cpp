#include "UI_Scene.h"

#include "GameObject.h"

#include "UI_Button.h"
#include "UI_Button_Player.h"
#include "UI_Image.h"
#include "UI_Text_PlayerInfo.h"


CUI_Scene::CUI_Scene()
{
}

void CUI_Scene::Priority_Update(_float fTimeDelta)
{

	for (auto& Image : m_Image)
		Image->Priority_Update(fTimeDelta);
	
	for (auto& Button : m_Button)
		Button->Priority_Update(fTimeDelta);

	for (auto& ButtonPlayer : m_ButtonPlayer)
		ButtonPlayer->Priority_Update(fTimeDelta);

	for (auto& TextPlayerInfo : m_TextPlayerInfo)
		TextPlayerInfo->Priority_Update(fTimeDelta);
}

void CUI_Scene::Update(_float fTimeDelta)
{

	for (auto& Image : m_Image)
		Image->Update(fTimeDelta);
	
	for (auto& Button : m_Button)
		Button->Update(fTimeDelta);

	for (auto& ButtonPlayer : m_ButtonPlayer)
		ButtonPlayer->Update(fTimeDelta);

	for (auto& TextPlayerInfo : m_TextPlayerInfo)
		TextPlayerInfo->Update(fTimeDelta);
}

void CUI_Scene::Late_Update(_float fTimeDelta)
{
	for (auto& Image : m_Image)
		Image->Late_Update(fTimeDelta);
	
	for (auto& Button : m_Button)
		Button->Late_Update(fTimeDelta);

	for (auto& ButtonPlayer : m_ButtonPlayer)
		ButtonPlayer->Late_Update(fTimeDelta);

	for (auto& TextPlayerInfo : m_TextPlayerInfo)
		TextPlayerInfo->Late_Update(fTimeDelta);
}

HRESULT CUI_Scene::Add_UIObject_Type(_int iUIType, CGameObject* pUIObj)
{
	if (nullptr == pUIObj)
		return E_FAIL;

	switch (iUIType)
	{
	case UI_BUTTON:
		m_Button.push_back(dynamic_cast<CUI_Button*>(pUIObj));
		break;
	case UI_BUTTONPLAYER:
		m_ButtonPlayer.push_back(dynamic_cast<CUI_Button_Player*>(pUIObj));
		break;
	case UI_IMAGE:
		m_Image.push_back(dynamic_cast<CUI_Image*>(pUIObj));
		break;
	case UI_TEXTPLAYER:
		m_TextPlayerInfo.push_back(dynamic_cast<CUI_Text_PlayerInfo*>(pUIObj));
		break;
	}

	return S_OK;
}

HRESULT CUI_Scene::UIScene_UIObject_Render_OnOff(_bool bOpen)
{
	for (auto& Image : m_Image)
		Image->Set_Render_OnOff(bOpen);

	for (auto& Button : m_Button)
		Button->Set_Render_OnOff(bOpen);

	for (auto& ButtonPlayer : m_ButtonPlayer)
		ButtonPlayer->Set_Render_OnOff(bOpen);

	for (auto& TextPlayerInfo : m_TextPlayerInfo)
		TextPlayerInfo->Set_Render_OnOff(bOpen);

	return S_OK;
}

void CUI_Scene::Clear_Last(_uint iUIType)
{
	switch (iUIType)
	{
	case UI_BUTTON:
		if (!m_Button.empty())
		{
			Safe_Release(m_Button.back());
			m_Button.pop_back();
		}
		break;
	case UI_BUTTONPLAYER:
		if (!m_ButtonPlayer.empty())
		{
			Safe_Release(m_ButtonPlayer.back());
			m_ButtonPlayer.pop_back();
		}
		break;
	case UI_IMAGE:
		if (!m_Image.empty())
		{
			Safe_Release(m_Image.back());
			m_Image.pop_back();
		}
		break;
	case UI_TEXTPLAYER:
		if (!m_TextPlayerInfo.empty())
		{
			Safe_Release(m_TextPlayerInfo.back());
			m_TextPlayerInfo.pop_back();
		}
		break;
	}
}

void CUI_Scene::Clear_Choice(_uint iUIType, CUIObject* pUIObj)
{
	switch (iUIType)
	{
	case UI_BUTTON:
		if (!m_Button.empty())
			m_Button.erase(remove(m_Button.begin(), m_Button.end(), pUIObj), m_Button.end());
		break;
	case UI_BUTTONPLAYER:
		if (!m_ButtonPlayer.empty())
			m_ButtonPlayer.erase(remove(m_ButtonPlayer.begin(), m_ButtonPlayer.end(), pUIObj), m_ButtonPlayer.end());
		break;
	case UI_IMAGE:
		if (!m_Image.empty())
			m_Image.erase(remove(m_Image.begin(), m_Image.end(), pUIObj), m_Image.end());
		break;
	case UI_TEXTPLAYER:
		if (!m_TextPlayerInfo.empty())
			m_TextPlayerInfo.erase(remove(m_TextPlayerInfo.begin(), m_TextPlayerInfo.end(), pUIObj), m_TextPlayerInfo.end());
		break;
	}
}

CUI_Scene* CUI_Scene::Create()
{
	return new CUI_Scene();
}

void CUI_Scene::Free()
{
	__super::Free();

	for (auto& pUIObj : m_Button)
		Safe_Release(pUIObj);
	for (auto& pUIObj : m_ButtonPlayer)
		Safe_Release(pUIObj);
	for (auto& pUIObj : m_Image)
		Safe_Release(pUIObj);
	for (auto& pUIObj : m_TextPlayerInfo)
		Safe_Release(pUIObj);

	m_Button.clear();
	m_ButtonPlayer.clear();
	m_Image.clear();
	m_TextPlayerInfo.clear();
}
