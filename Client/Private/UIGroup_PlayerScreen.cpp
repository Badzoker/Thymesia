#include "pch.h"
#include "UIGroup_PlayerScreen.h"
#include "UI_Scene.h"
#include "GameInstance.h"

CUIGroup_PlayerScreen::CUIGroup_PlayerScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_PlayerScreen::CUIGroup_PlayerScreen(const CUIGroup_PlayerScreen& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_PlayerScreen::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_PlayerScreen::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CUIGroup_PlayerScreen::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUIGroup_PlayerScreen::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
	}

}

void CUIGroup_PlayerScreen::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
}

HRESULT CUIGroup_PlayerScreen::Render()
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

HRESULT CUIGroup_PlayerScreen::Ready_UIObject()
{

	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_GAMEPLAY, UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen");
	//m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_PlayerScreen", m_TextInfo);

	return S_OK;
}

CUIGroup_PlayerScreen* CUIGroup_PlayerScreen::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_PlayerScreen* pInstance = new CUIGroup_PlayerScreen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_PlayerScreen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_PlayerScreen::Clone(void* pArg)
{
	CUIGroup_PlayerScreen* pInstance = new CUIGroup_PlayerScreen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_PlayerScreen");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_PlayerScreen::Free()
{
	__super::Free();
}
