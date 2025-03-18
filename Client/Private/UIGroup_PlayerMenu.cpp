#include "pch.h"
#include "UIGroup_PlayerMenu.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "UI_ButtonHighlight.h"

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

	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu");

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
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUIGroup_PlayerMenu::Render()
{
	if (m_bRenderOpen)
	{
		
	}
	return S_OK;
}

void CUIGroup_PlayerMenu::MenuButton_Check()
{

	for (auto& Button : m_pMyScene->Find_UI_Button())
	{
		if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
		{
			if (1 == Button->Get_UI_GroupID()) // 게임 계속하기
			{
				m_pGameInstance->Set_NextLevel_Open(true);
			}

			if (2 == Button->Get_UI_GroupID()) // 새게임
			{

			}

			if (3 == Button->Get_UI_GroupID()) // 게임 불러오기
			{

			}

			if (4 == Button->Get_UI_GroupID()) // 설정
			{

			}

			if (5 == Button->Get_UI_GroupID()) // 제작진
			{

			}

			if (6 == Button->Get_UI_GroupID()) // 종료
			{
				DestroyWindow(g_hWnd);
			}
		}


	}
}

HRESULT CUIGroup_PlayerMenu::Ready_UIObject()
{
	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_MENU, L"UIScene_PlayerMenu");
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
