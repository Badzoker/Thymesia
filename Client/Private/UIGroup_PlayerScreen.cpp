#include "pch.h"
#include "UIGroup_PlayerScreen.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "UI_Text.h"
#include "UI_Image.h"
#include "Player.h"
#include "UI_ItemBackground.h"
#include "UIGroup_Inventory.h"

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

	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen");
	m_pItmeScreen = m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen_1");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItmeScreen, false);

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"), "PLAYER");
	m_pGroupInven = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_PlayerInventory"), "Inventory");
	for (auto& Textbox : m_pMyScene->Find_UI_TextBox())
	{
		if (101 == Textbox->Get_UI_GroupID())
		{
			Textbox->Set_OnOff(false);
		}
	}
	return S_OK;
}

void CUIGroup_PlayerScreen::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
	// 그룹에서 가장 메인이 되는 씬이 켜지는 경우 다른 것들도 켜지도록 => 해당 씬들의 업데이트를 켠다는 것
	if (m_pMyScene->Get_Scene_Render_State())
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItmeScreen, true);
	}
	else
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItmeScreen, false);
	}
	
}

void CUIGroup_PlayerScreen::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	Player_Info_GageBar();

	if (0 != dynamic_cast<CUIGroup_Inventory*>(m_pGroupInven)->Get_Drop_Item_Info().size())// 버린 아이템이 있는 경우
	{
		if (m_bDrop) // 버린걸 먹기 전까지 다시 이 구간을 사용할 수 없는 기형적인 상태가 되어버림
		{
			Item_Drop_Info((*dynamic_cast<CUIGroup_Inventory*>(m_pGroupInven)->Get_Drop_Item_Info().begin()).second);
			m_bDrop = false;
		}
	}
	else if (0 != m_pGameInstance->Get_Item_Save_Info().size()) // 획득한 아이템이 있는 경우
	{
		UI_Direction_Item_Nudge();

		_uint iNum = {};

		if (0 != m_pGameInstance->Get_Item_Save_Info().size())
		{
			for (auto& SaveInfo : m_pGameInstance->Get_Item_Save_Info())
			{
				if (!m_bNudgeUse[0]) // 1번 넛지 꺼져 있으면
				{
					Item_Save_Info(SaveInfo, 1); // 추가해서 재생 
					m_bNudgeUse[0] = true;
				}
				/*else if (!m_bNudgeUse[1])
				{
					Item_Save_Info(SaveInfo, 2);
					m_bNudgeUse[0] = true;
				}
				else if (!m_bNudgeUse[2])
				{
					Item_Save_Info(SaveInfo, 3);
					m_bNudgeUse[0] = true;
				}
				else if (!m_bNudgeUse[3])
				{
					Item_Save_Info(SaveInfo, 4);
					m_bNudgeUse[0] = true;
				}*/
			}

		}
		m_pGameInstance->Get_Item_Save_Info().clear();
	}
	if (m_bNudgeUse[0] || m_bNudgeUse[1])
	{
		m_fTimeCheck += fTimeDelta;
		if (m_fTimeCheck > 2)
		{
			m_bNudgeUse[0] = false;
			m_bNudgeUse[1] = false;
			m_fTimeCheck = 0;
			for (auto& Image : m_pItmeScreen->Find_UI_Image())
			{
				if (1 == Image->Get_UI_GroupID()) // 배경 이미지 설정
				{
					dynamic_cast<CUI_Image*>(Image)->Set_OnOff(false);
				}
				if (1 + 10 == Image->Get_UI_GroupID()) // 획득 아이템 아이콘 설정
				{
					dynamic_cast<CUI_Image*>(Image)->Set_OnOff(false);
				}
			}
			for (auto& TextBox : m_pItmeScreen->Find_UI_TextBox()) // 아이템 이름 
			{
				if (1 == TextBox->Get_UI_GroupID())
				{
					dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(false);
				}
				if (1 + 10 == TextBox->Get_UI_GroupID()) // 아이템 카테고리 설명
				{
					dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(false);
				}
			}

		}
	}
	
}

void CUIGroup_PlayerScreen::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_bRenderOpen)
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
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

void CUIGroup_PlayerScreen::Player_Info_GageBar()
{
	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%d";
	const _tchar* CountTextDouble = L"%d / %d";

	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (10 == TextBox->Get_UI_GroupID()) // 현재 HP
		{
			wsprintf(ChangeText, CountTextDouble, dynamic_cast<CPlayer*>(m_pPlayer)->Get_CurrentHp(), dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullHp());
			TextBox->Set_Content(ChangeText);
		}
		if (20 == TextBox->Get_UI_GroupID()) // 현재 MP
		{
			wsprintf(ChangeText, CountTextDouble, dynamic_cast<CPlayer*>(m_pPlayer)->Get_CurrentMp(), dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullMp());
			TextBox->Set_Content(ChangeText);
		}
		if (12 == TextBox->Get_UI_GroupID()) // 물약 드링킹 시 플러스되는 수치
		{
			TextBox->Set_OnOff(false);
		}
		if (30 == TextBox->Get_UI_GroupID()) // 현재 물약 개수
		{
			wsprintf(ChangeText, CountTextDouble, 3, 3);
			TextBox->Set_Content(ChangeText);
		}
		if (22 == TextBox->Get_UI_GroupID()) // 
		{
			TextBox->Set_OnOff(false);
		}
		if (40 == TextBox->Get_UI_GroupID()) // 현재 깃털 개수
		{
			wsprintf(ChangeText, CountTextDouble, 3, 3);
			TextBox->Set_Content(ChangeText);
		}
		if (100 == TextBox->Get_UI_GroupID()) // 현재 기억의 파편 개수
		{
			wsprintf(ChangeText, CountText, dynamic_cast<CPlayer*>(m_pPlayer)->Get_MemoryFragment());
			TextBox->Set_Content(ChangeText);
		}

	}
}

void CUIGroup_PlayerScreen::Item_Save_Info(UI_Item SaveItem, _uint iNum)
{
	_uint iTexNum = {};
	switch (SaveItem.ItemType)
	{
	case ITEM_TYPE::ITEM_KEY1:
		SaveItem.ItemDesc = L"- 일반 아이템 -";
		iTexNum = 0;
		break;
	case ITEM_TYPE::ITEM_KEY2:
		SaveItem.ItemDesc = L"- 일반 아이템- ";
		iTexNum = 0;
		break;
	case ITEM_TYPE::ITEM_MEMORY:
		SaveItem.ItemDesc = L"- 소비 아이템 -";
		iTexNum = 0;
		break;
	case ITEM_TYPE::ITEM_FORGIVEN:
		SaveItem.ItemDesc = L"- 소비 아이템 -";
		iTexNum = 3;
		break;
	case ITEM_TYPE::ITEM_SKILLPIECE:
		SaveItem.ItemDesc = L"- 기술의 파편 -";
		iTexNum = 2;
		break;

	}

	// 여기서 시작, 도착 좌표해서 보간하면 될 듯

	for (auto& Image : m_pItmeScreen->Find_UI_Image())
	{
		if (iNum == Image->Get_UI_GroupID()) // 배경 이미지 설정
		{
			dynamic_cast<CUI_Image*>(Image)->Set_OnOff(true);
			dynamic_cast<CUI_Image*>(Image)->Set_TexNumber(iTexNum);
		}
		if (iNum +10 == Image->Get_UI_GroupID()) // 획득 아이템 아이콘 설정
		{
			dynamic_cast<CUI_Image*>(Image)->Set_OnOff(true);
			dynamic_cast<CUI_Image*>(Image)->Set_TexNumber(SaveItem.ItemIconNum);
		}
	}
	for (auto& TextBox : m_pItmeScreen->Find_UI_TextBox()) // 아이템 이름 
	{
		if (iNum == TextBox->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(SaveItem.ItemName);
		}
		if (iNum +10 == TextBox->Get_UI_GroupID()) // 아이템 카테고리 설명
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(SaveItem.ItemDesc);
		}
	}


}

void CUIGroup_PlayerScreen::Item_Drop_Info(UI_Item DropItem)
{

	DropItem.ItemDesc = L"- 떨어뜨림 -";
	m_bNudgeUse[1] = true;
	_uint iNum = { 1 };
	for (auto& Image : m_pItmeScreen->Find_UI_Image())
	{
		if (iNum == Image->Get_UI_GroupID()) // 배경 이미지 설정
		{
			dynamic_cast<CUI_Image*>(Image)->Set_OnOff(true);
			dynamic_cast<CUI_Image*>(Image)->Set_TexNumber(1);
		}
		if (iNum + 10 == Image->Get_UI_GroupID()) // 획득 아이템 아이콘 설정
		{
			dynamic_cast<CUI_Image*>(Image)->Set_OnOff(true);
			dynamic_cast<CUI_Image*>(Image)->Set_TexNumber(DropItem.ItemIconNum);
		}
	}
	for (auto& TextBox : m_pItmeScreen->Find_UI_TextBox()) // 아이템 이름 
	{
		if (iNum == TextBox->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(DropItem.ItemName);
		}
		if (iNum + 10 == TextBox->Get_UI_GroupID()) // 아이템 카테고리 설명
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(DropItem.ItemDesc);
		}
	}


	//dynamic_cast<CUIGroup_Inventory*>(m_pGroupInven)->Get_Drop_Item_Info().clear();

}

void CUIGroup_PlayerScreen::Item_In_Out_Pop()
{
	for (auto& ItemInfo : m_pItmeScreen->Find_UI_Image())
	{
		if (1 == ItemInfo->Get_UI_GroupID())
		{

		}
		if ((10) == ItemInfo->Get_UI_GroupID())
		{

		}
	}


}

void CUIGroup_PlayerScreen::UI_Direction_Monster_MemoryGet()
{
}

void CUIGroup_PlayerScreen::UI_Direction_Item_Nudge()
{
	// 아이템 획득, 버리기 알림을 띄운다
	// 아이템 알림은 최대 한 번에 4개까지 출력할 수 있으며 => 4개가 종료된 이후에 첫 번째 알림부터 사용
	





}

HRESULT CUIGroup_PlayerScreen::Ready_UIObject()
{
	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen");
 	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen_1");

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
