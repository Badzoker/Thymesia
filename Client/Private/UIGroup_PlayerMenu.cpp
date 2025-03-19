#include "pch.h"
#include "UIGroup_PlayerMenu.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "UI_ButtonHighlight.h"
#include "UI_KeyBox_Long.h"

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
	if (m_bRenderOpen)
	{
	}

}

void CUIGroup_PlayerMenu::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bRenderOpen)	
	{
		MenuButton_Check();
	}
}

void CUIGroup_PlayerMenu::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
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
		if (Button->Get_Mouse_Select_OnOff())
		{
			if (100 == Button->Get_UI_GroupID()) // 레벨업
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerLevelUP"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), true);
			}
			if (101 == Button->Get_UI_GroupID()) // 특성 해제
			{
			}
			if (102 == Button->Get_UI_GroupID()) // 역병무기
			{
			}
			if (103 == Button->Get_UI_GroupID()) // 물약
			{
			}
			if (104 == Button->Get_UI_GroupID()) // 잊혀진 깃털 사용
			{
			}
			if (105 == Button->Get_UI_GroupID()) //기억 되살리기 중단	
			{
			}
			if (106 == Button->Get_UI_GroupID()) //게임 재개
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);

			}
			if (120 == Button->Get_UI_GroupID()) //게임 재개
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);

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
