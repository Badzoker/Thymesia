#include "pch.h"
#include "UIGroup_PlayerMenu.h"
#include "UI_Scene.h"
#include "GameInstance.h"

CUIGroup_PlayerMenu::CUIGroup_PlayerMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_PlayerMenu::CUIGroup_PlayerMenu(const CUIGroup_PlayerMenu& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_PlayerMenu::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_PlayerMenu::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CUIGroup_PlayerMenu::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUIGroup_PlayerMenu::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
	}
}

void CUIGroup_PlayerMenu::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
}

HRESULT CUIGroup_PlayerMenu::Render()
{
	if (m_bRenderOpen)
	{
		vector<UI_TextInfo>::iterator it;
		for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
		{
			m_pGameInstance->Render_Font(it->strFontName.c_str(), it->srtTextContent.c_str(), it->fTextStartPos);

		}
	}
	return S_OK;
}

HRESULT CUIGroup_PlayerMenu::Ready_UIObject()
{
	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_GAMEPLAY, UISCENE_MENU, L"UIScene_PlayerMenu");
	//m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_PlayerMenu", m_TextInfo);
	return S_OK;
}

CUIGroup_PlayerMenu* CUIGroup_PlayerMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_PlayerMenu* pInstance = new CUIGroup_PlayerMenu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_PlayerMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_PlayerMenu::Clone(void* pArg)
{
	CUIGroup_PlayerMenu* pInstance = new CUIGroup_PlayerMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_PlayerMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_PlayerMenu::Free()
{
	__super::Free();
}
