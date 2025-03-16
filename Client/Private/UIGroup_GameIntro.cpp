#include "pch.h"
#include "UIGroup_GameIntro.h"
#include "GameInstance.h"
#include "Level_Loading.h"

#include "UI_Scene.h"
#include "UI_Text.h"
#include "UI_TextBox.h"
#include "UI_Button.h"


#include "UI_ButtonHighlight.h"

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

	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro");

	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		dynamic_cast<CUI_TextBox*>(TextBox)->Set_Change_TextColor(FONT_GRAY);
		dynamic_cast<CUI_TextBox*>(TextBox)->Set_TextRenderType(Engine::CUI_Text::FONT_OUTLINE);
	}
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
		Button_Check();
	}
}

void CUIGroup_GameIntro::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUIGroup_GameIntro::Render()
{
	if (m_bRenderOpen)
	{
	}
	return S_OK;
}

void CUIGroup_GameIntro::Button_Check()
{
	for (auto& Button : m_pMyScene->Find_UI_Button())
	{
		if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
		{
			if (1 == Button->Get_UI_GroupID())
			{
				m_pGameInstance->Set_NextLevel_Open(true);
			}

			if (2 == Button->Get_UI_GroupID())
			{

			}

			if (3 == Button->Get_UI_GroupID())
			{

			}

			if (4 == Button->Get_UI_GroupID())
			{

			}

			if (5 == Button->Get_UI_GroupID())
			{

			}

			if (6 == Button->Get_UI_GroupID())
			{

			}
		}


	}

}

HRESULT CUIGroup_GameIntro::Ready_UIObject()
{
	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INTRO, L"UIScene_Intro");
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
