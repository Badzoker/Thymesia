#include "pch.h"
#include "UIGroup_GameIntro.h"
#include "UI_Scene.h"
#include "GameInstance.h"

CUIGroup_GameIntro::CUIGroup_GameIntro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_GameIntro::CUIGroup_GameIntro(const CUIGroup_GameIntro& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_GameIntro::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_GameIntro::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CUIGroup_GameIntro::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUIGroup_GameIntro::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), true);
	}
}

void CUIGroup_GameIntro::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
}

HRESULT CUIGroup_GameIntro::Render()
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

HRESULT CUIGroup_GameIntro::Ready_UIObject()
{
	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_GAMEPLAY, UISCENE_INTRO, L"UIScene_Intro");
//	m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_Intro", m_TextInfo);
	return S_OK;
}

CUIGroup_GameIntro* CUIGroup_GameIntro::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_GameIntro* pInstance = new CUIGroup_GameIntro(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_GameIntro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_GameIntro::Clone(void* pArg)
{
	CUIGroup_GameIntro* pInstance = new CUIGroup_GameIntro(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_GameIntro");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_GameIntro::Free()
{
	__super::Free();
}
