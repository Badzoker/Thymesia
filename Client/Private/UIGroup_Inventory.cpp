#include "pch.h"
#include "UIGroup_Inventory.h"
#include "UI_Scene.h"
#include "UI_Button.h"
#include "UI_Text.h"
#include "UI_Image.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "UI_ItemBackground.h"
#include "UI_Item_Icon.h"
#include "UI_ButtonHighlight.h"
#include "UI_SquareFrame.h"
#include "UI_SquareFrame_Hover.h"
#include "UI_UnderLine.h"
#include "UI_Arrow.h"
#include "UIGroup_PlayerScreen.h"

#include "Player.h"

CUIGroup_Inventory::CUIGroup_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_Inventory::CUIGroup_Inventory(const CUIGroup_Inventory& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_Inventory::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_Inventory::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	m_pMyBaseScene = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase");
	m_pItemScene = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_Inven_ItemUse");

	// 선행 조건이 필요한 ui들 최초에는 off 하도록 설정
	m_pItemUsePopUp = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemUse_PopUp");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemUsePopUp, false);

	m_pItemTypePopUp = m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_ItemType_PopUp");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemTypePopUp, false);

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"), "PLAYER");
	
	m_pGroupPlayerScreen = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_PlayerScreen"), "PlayerScreen");


	Ready_MiniView_ItemInfo(); // 아이템 정보 띄우는 부분 상시 바뀌니깐 별도로 저장해서 사용
	Set_Item_Default_Info(); // 아이템 ui 정보 연결을 위해 정보를 가진 벡터 컨테이너 하나 준비

	return S_OK;
}

void CUIGroup_Inventory::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	// 그룹에서 가장 메인이 되는 씬이 켜지는 경우 다른 것들도 켜지도록 => 해당 씬들의 업데이트를 켠다는 것
	if (m_pMyBaseScene->Get_Scene_Render_State())
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemScene, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemUsePopUp, true); // 자동으로 uiobj를 가진 scene 랜더도 오픈되도록 함수가 있음
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemTypePopUp, true);
	}
	else
	{
		m_bItemUsePopOpen = false; // 아이템 사용/버리기 팝업 off
		m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemUsePopUp, false);
		m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItemTypePopUp, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemScene, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemUsePopUp, false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pItemTypePopUp, false);
		m_iMouseOnLastSlot = 0; // 끄는 시점에 값 초기화
	}
}

void CUIGroup_Inventory::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	Change_UI_Item_Tab(); // 아이템 탭안의 아이템 종류 탭을 변경 시 마다 슬롯 정보 변경


	if (m_bRenderOpen)
	{
		if (!m_bItemUsePopOpen)// 아이템 사용/ 버리기 팝업이 켜져 있지 않다면
		{
			Update_Get_ItemMgr(); //아이템 매니저가 갖고 있는 보유 아이템 목록을 가져옴
			Update_ItemInfo(); // 보유 아이템 타입 정보를 토대로 아이템의 부가적인 설명들 채워넣기

			if (!m_bItemTypePopOpen) // 아이템 타입 팝업이 켜져 있지 않다면
			{
				if (m_bCommonOpen) // 아이템 탭 켜져있을 때
					Slot_Button_MouseOn_Check(m_InvenItemCommon); // 슬롯을 마우스로 가리키는지
				if (m_bSkillOpen) // 스킬 탭 켜져 있을 때
					Slot_Button_MouseOn_Check(m_InvenItemSkill);
			}
			// 마우스 On 값이 true 인 녀석을 찾아 값을 집어 넣자

			/* 아이템 타입 팝업은 모달이기에 여기서 슬롯 버튼 누르기 설정을 하고 있음*/
			if (m_bCommonOpen)
				Slot_Button_Select_Check(m_InvenItemCommon); // 슬롯을 마우스로 누르는지
			if (m_bSkillOpen)
				Slot_Button_Select_Check(m_InvenItemSkill);
			
			if (m_bItemTypePopOpen)
				ItemType_PopUP_Button(); // 아이템 타입 팝업이 켜져 있다면 버튼 누르기 활성화
		}
		else
		{
			if (1 == m_iPopUpOpenNum)// 아이템 사용 팝업이 켜져 있다면 아이템 사용 함수 체크
				ItemUse_PopUP_Use_Button(); // 아이템 버리기 팝업이 켜져 있다면 아이템 버리기 함수 체크
			else if (2 == m_iPopUpOpenNum)
				ItemUse_PopUP_Drop_Button();
		}
	}
}

void CUIGroup_Inventory::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		__super::Late_Update(fTimeDelta);
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUIGroup_Inventory::Render()
{
	return S_OK;
}

HRESULT CUIGroup_Inventory::Ready_UIObject()
{
	// ESC 메뉴 배경
	if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INVEN, L"UIScene_EscMenuBase")))
		return E_FAIL;

	// 인벤 아이템 슬롯 영역
	if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INVEN, L"UIScene_Inven_ItemUse")))
		return E_FAIL;

	// 인벤 아이템 사용 버리기 최종 팝업
	if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INVEN, L"UIScene_ItemUse_PopUp")))
		return E_FAIL;

	//인벤 아이템 타입에 따른 팝업
	if (FAILED(m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INVEN, L"UIScene_ItemType_PopUp")))
		return E_FAIL;

	return S_OK;
}

void CUIGroup_Inventory::Change_UI_Item_Tab()
{
	if (m_bCommonOpen)
		Itme_View_Inventory(m_InvenItemCommon); // 일반 아이템을 슬롯에 띄우기

	if (m_bSkillOpen)
		Itme_View_Inventory(m_InvenItemSkill);// 스킬 아이템을 슬롯에 띄우기

	for (auto& Button : m_pItemScene->Find_UI_Button())
	{
		CUI_UnderLine* pButton = dynamic_cast<CUI_UnderLine*>(Button);

		if (10 == Button->Get_UI_GroupID()) // 일반 
		{
			if (pButton->Get_Mouse_Select_OnOff()) // 일반 버튼을 누를 시
			{
				m_bCommonOpen = true;
				m_bSkillOpen = false;
			}

		}
		if (20 == Button->Get_UI_GroupID()) {} // 이야기
		if (30 == Button->Get_UI_GroupID()) // 기술의 파편
		{
			if (pButton->Get_Mouse_Select_OnOff()) // 기술의 파편 버튼을 누를 시
			{
				m_bCommonOpen = false;
				m_bSkillOpen = true;

			}
		}
		if (40 == Button->Get_UI_GroupID()) {} // 재료

	}
}

void CUIGroup_Inventory::Ready_MiniView_ItemInfo()
{
	// 자주 쓰는 기본 정보객체 미리 가져와서 저장하기
	for (auto& TextBox : m_pItemScene->Find_UI_TextBox())
	{
		_tchar* pTemp = { L"" };
		if (200 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxName = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxName)->Set_Content(pTemp);
		}
		if (210 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxType = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxType)->Set_Content(pTemp);
		}
		if (220 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxCount = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxCount)->Set_Content(pTemp);
		}
		if (221 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxCountNum = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxCountNum)->Set_Content(pTemp);
		}

		if (230 == TextBox->Get_UI_GroupID())
		{
			m_pItemTextBoxDesc = TextBox;
			dynamic_cast<CUIObject*>(m_pItemTextBoxDesc)->Set_Content(pTemp);
		}
		if (nullptr != m_pItemTextBoxName &&
			nullptr != m_pItemTextBoxDesc)
		{
			break;
		}
	}

	for (auto& image : m_pItemScene->Find_UI_Image())
	{
		if (200 <= image->Get_UI_GroupID() && 250 >= image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_OnOff(false);
		}

	}
}

void CUIGroup_Inventory::Itme_View_Inventory(vector<UI_Item>& vecContainer)
{
	// 보여줄 아이템 순서를 정리한다
	sort(vecContainer.begin(), vecContainer.end(), [](UI_Item a, UI_Item b) {return a.ItemType < b.ItemType; }); // 아이템 타입 기준 오름차순 정렬

	// 인벤토리에 획득한 아이템들을 보여준다

	_int iOpenSlotCount = 100; // 슬롯 번호

	for (auto& InvenItem : vecContainer) // 아이템 정보 돌면서 슬롯에 넣기
	{
		for (auto& Slot : m_pItemScene->Find_UI_Button()) // 슬롯 가져오기
		{
			if (iOpenSlotCount == Slot->Get_UI_GroupID()) // 첫 번째 슬롯부터 값을 채운다
			{
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon(InvenItem.ItemIconNum);
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon_OnOff(true);

				_tchar ChangeText[MAX_PATH] = {};
				const _tchar* Text = { L"x%d" };

				wsprintf(ChangeText, Text, InvenItem.ItemCount);
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Content(ChangeText);
				iOpenSlotCount++;
				break;
			}
		}

	}
	// 아이템 컨테이너 정보가 없을 때 모두 빈 슬롯으로 나오도록 설정
	iOpenSlotCount = 100;
	if (vecContainer.empty())
	{
		for (auto& Slot : m_pItemScene->Find_UI_Button()) // 슬롯 가져오기
		{
			if (iOpenSlotCount == Slot->Get_UI_GroupID()) // 첫 번째 슬롯부터 값을 채운다
			{
				_tchar ChangeText[MAX_PATH] = { L"" };

				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon_OnOff(false);
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Content(ChangeText);
				iOpenSlotCount++;

			}
		}
	}
}

void CUIGroup_Inventory::Slot_Button_Select_Check(vector<UI_Item>& vecContainer)
{
	for (auto& Button : m_pItemScene->Find_UI_Button())
	{
		if (100 <= Button->Get_UI_GroupID() && 125 > Button->Get_UI_GroupID()) // slotbutton
		{
			CUI_ItemBackground* pButton = dynamic_cast<CUI_ItemBackground*>(Button);
			if (pButton->Get_Mouse_Select_OnOff())
			{
				// 버튼들 중 마우스가 셀렉한 녀석이 있다!
				pButton->Set_Mouse_Select_OnOff(false); // 1회 체크만 할 것이기에 값을 끄고

				for (auto& Item : vecContainer)
				{
					if (Item.ItemIconNum == pButton->Get_Item_Icon())
					{
						m_CurrentItemInfo = Item; // 선택한 아이템 저장
						_float3 ButtonPos = dynamic_cast<CTransform*>(pButton->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
						ItemType_PopUP_Open(Item.ItemType, ButtonPos);
						break;
					}
				}
			}
		}
	}

	
}

void CUIGroup_Inventory::Slot_Button_MouseOn_Check(vector<UI_Item>& vecContainer)
{
	for (auto& Slot : m_pItemScene->Find_UI_Button())
	{
		CUI_ItemBackground* pSlot = dynamic_cast<CUI_ItemBackground*>(Slot);

		if (100 <= Slot->Get_UI_GroupID() && 125 > Slot->Get_UI_GroupID())
		{
			if (pSlot->Get_Slot_Mouse_OnOff()) // 해당 슬롯에 마우스가 올려져 있다
			{
				for (auto& Image : m_pItemScene->Find_UI_Image())
				{
					if (100 == Image->Get_UI_GroupID()) // 호버 이미지를 찾아서
					{
						_float3 fPos = dynamic_cast<CTransform*>(Slot->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);

						CUI_SquareFrame_Hover* pImage = dynamic_cast<CUI_SquareFrame_Hover*>(Image);
						dynamic_cast<CTransform*>(pImage->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fPos.x,fPos.y });
						pImage->Set_Hover_ImageOn(true);
						m_iMouseOnLastSlot = pSlot->Get_UI_GroupID();
					}
				}

				if (pSlot->Get_Item_Icon_OnOff()) // 슬롯에 아이템이 보여지고 있는지
				{
					for (auto& Item : vecContainer)
					{
						if (Item.ItemIconNum == pSlot->Get_Item_Icon()) // 슬롯에 있는 아이콘의 이미지와 같은 정보를 가진 아이템을
							Connect_MiniView_ItemInfo(Item); // ui로 연결
					}
				}
				else
				{
					DisConnect_MiniView_ItemInfo();
				}

			}

		}

	}


	for (auto& Slot : m_pItemScene->Find_UI_Button())
	{
		if (m_iMouseOnLastSlot == Slot->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ItemBackground*>(Slot)->Get_Item_Icon_OnOff()) // 슬롯에 아이템이 보여지고 있는지
			{
				for (auto& Item : vecContainer)
				{
					if (Item.ItemIconNum == dynamic_cast<CUI_ItemBackground*>(Slot)->Get_Item_Icon()) // 슬롯에 있는 아이콘의 이미지와 같은 정보를 가진 아이템을
						Connect_MiniView_ItemInfo(Item); // ui로 연결
				}
			}
			else
			{
				DisConnect_MiniView_ItemInfo();
			}
		}
	}
}

void CUIGroup_Inventory::ItemType_PopUP_Open(ITEM_TYPE eItemType, _float3 fSetPos)
{
	_float3 fImagePos = {};

	switch (eItemType)
	{
	case ITEM_TYPE::ITEM_KEY1: // 아무것도 안 뜸
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;
		break;
	case ITEM_TYPE::ITEM_KEY2: // 아무것도 안 뜸
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;
		break;
	case ITEM_TYPE::ITEM_MEMORY:// 사용 버리기 종료 뜸
		for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		{
			if (300 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
				fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
				Image->Set_OnOff(true);
			}
			if (400 == Image->Get_UI_GroupID())
			{
				Image->Set_OnOff(true);
			}
		}
		for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		{
			if (310 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
				Button->Set_OnOff(true);
			}
			if (320 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
				Button->Set_OnOff(true);
			}
			if (330 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y + 40.f });
				Button->Set_OnOff(true);
			}
		}
		m_bItemTypePopOpen = true;
		break;
	case ITEM_TYPE::ITEM_FORGIVEN:// 아무것도 안 뜸
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;
		break;
	case ITEM_TYPE::ITEM_SKILLPIECE:// 버리기, 종료 뜸
		for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		{
			if (300 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
				fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
				Image->Set_OnOff(true);
			}
			if (400 == Image->Get_UI_GroupID())
			{
				Image->Set_OnOff(true);
			}
		}
		for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		{
			if (310 == Button->Get_UI_GroupID())
			{
				Button->Set_OnOff(false);
			}
			if (320 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
				Button->Set_OnOff(true);
			}
			if (330 == Button->Get_UI_GroupID())
			{
				dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
				Button->Set_OnOff(true);
			}
		}
		m_bItemTypePopOpen = true;
		break;
	default: //빈슬롯
		ItemType_PopUP_State(false);
		m_bItemTypePopOpen = false;
		break;
		//case 3: //ITEM_ONLYUSE // 사용하기, 종료 뜸 // 그 신호기로 가는 칼 아이템
		//	for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
		//	{
		//		if (300 == Image->Get_UI_GroupID())
		//		{
		//			dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fSetPos.x + 125,fSetPos.y + 75 });
		//			fImagePos = dynamic_cast<CTransform*>(Image->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION);
		//			Image->Set_OnOff(true);
		//		}
		//		if (400 == Image->Get_UI_GroupID())
		//		{
		//			Image->Set_OnOff(true);
		//		}
		//	}
		//	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
		//	{
		//		if (310 == Button->Get_UI_GroupID())
		//		{
		//			dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y - 40.f });
		//			Button->Set_OnOff(true);
		//		}
		//		if (320 == Button->Get_UI_GroupID())
		//		{
		//			Button->Set_OnOff(false);
		//		}
		//		if (330 == Button->Get_UI_GroupID())
		//		{
		//			dynamic_cast<CTransform*>(Button->Find_Component(TEXT("Com_Transform")))->Set_State_UIObj(CTransform::STATE_POSITION, { fImagePos.x ,fImagePos.y });
		//			Button->Set_OnOff(true);
		//		}
		//	}
		//	m_bItemTypePopOpen = true;
		//	break;
	}


	
}
void CUIGroup_Inventory::ItemType_PopUP_State(_bool bOpen)
{

	for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
	{
		if (300 == Image->Get_UI_GroupID())
		{
			Image->Set_OnOff(bOpen);
		}
		if (400 == Image->Get_UI_GroupID())
		{
			Image->Set_OnOff(bOpen);
		}
	}
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
		if (310 == Button->Get_UI_GroupID())
		{
			Button->Set_OnOff(bOpen);
		}
		if (320 == Button->Get_UI_GroupID())
		{
			Button->Set_OnOff(bOpen);
		}
		if (330 == Button->Get_UI_GroupID())
		{
			Button->Set_OnOff(bOpen);
		}
	}
}

void CUIGroup_Inventory::ItemType_PopUP_Button()
{
	// 팝업 종료 조건 => 이미지 바깥 선택
	for (auto& Image : m_pItemTypePopUp->Find_UI_Image())
	{
		if (300 == Image->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_SquareFrame*>(Image)->Get_Mouse_Side_Select_OnOff())
			{
				ItemType_PopUP_State(false);
				m_bItemTypePopOpen = false;
				m_CurrentItemInfo = {};
			}
		}
	}
	// 팝업 종료 조건 => 종료 버튼 클릭
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
		if (330 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				ItemType_PopUP_State(false);
				m_bItemTypePopOpen = false;
				m_CurrentItemInfo = {};
			}
		}
	}

	// 사용 버튼 클릭
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
		if (310 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iPopUpOpenNum = 1;
				ItemUse_PopUP_Open();

			}
		}
	}
	// 버리기 버튼 클릭
	for (auto& Button : m_pItemTypePopUp->Find_UI_Button())
	{
		if (320 == Button->Get_UI_GroupID())
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iPopUpOpenNum = 2;
				ItemUse_PopUP_Open();




			}
		}
	}
}

void CUIGroup_Inventory::ItemUse_PopUP_Open()
{
	_float3 fImagePos = {};


	for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
	{
		Image->Set_OnOff(true);
	}

	for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
	{
		Button->Set_OnOff(true);
	}

	for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
	{
		TextBox->Set_OnOff(true);
	}


	m_bItemUsePopOpen = true;
}

void CUIGroup_Inventory::ItemUse_PopUP_Use_Button()
{
	m_iItemMaxCount = m_CurrentItemInfo.ItemCount;

	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%s";
	const _tchar* CountNum = L"%d";
	for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
	{
		if (0 == TextBox->Get_UI_GroupID())
		{
			TextBox->Set_Content(L"사용");
		}
		if (1 == TextBox->Get_UI_GroupID())
		{
			wsprintf(ChangeText, CountText, m_CurrentItemInfo.ItemName);
			TextBox->Set_Content(ChangeText);
			TextBox->Set_Change_TextColor(FONT_GREEN);
		}
		if (2 == TextBox->Get_UI_GroupID())
		{
			wsprintf(ChangeText, CountNum, m_iItemStatCount);
			TextBox->Set_Content(ChangeText);

		}
	}
	for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
	{
		if (10 == Button->Get_UI_GroupID()) // 개수 감소
		{
			if (dynamic_cast<CUI_Arrow*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iItemStatCount -= 1;
				if (m_iItemStatCount == 0)
				{
					m_iItemStatCount = m_iItemMaxCount;
				}

			}

		}
		if (20 == Button->Get_UI_GroupID()) // 개수 증가
		{
			if (dynamic_cast<CUI_Arrow*>(Button)->Get_Mouse_Select_OnOff())
			{
				if (m_iItemMaxCount > m_iItemStatCount)
				{
					m_iItemStatCount += 1;
				}
			}
		}


		if (30 == Button->Get_UI_GroupID()) // 아이템 사용
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				if (m_pGameInstance->Use_Item(m_CurrentItemInfo.ItemType, m_iItemStatCount))
				{
					for (auto It = m_InvenItemCommon.begin(); It != m_InvenItemCommon.end();)
					{
						if (It->ItemType == m_CurrentItemInfo.ItemType)
							It = m_InvenItemCommon.erase(It);
						else
							++It;
					}
				}
				Item_Use_Set(m_CurrentItemInfo.ItemType, m_iItemStatCount);
				ItemUse_Update();
				ItemType_PopUP_State(false);
				m_iItemStatCount = 1;
				m_bItemTypePopOpen = false;
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}
			}
		}
		if (40 == Button->Get_UI_GroupID())// 창 종료
		{

			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iItemStatCount = 1;
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}
			}
		}
	}
}

void CUIGroup_Inventory::ItemUse_PopUP_Drop_Button()
{
	m_iItemMaxCount = m_CurrentItemInfo.ItemCount;

	_tchar ChangeText[MAX_PATH] = {};
	const _tchar* CountText = L"%s";
	const _tchar* CountNum = L"%d";
	for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
	{
		if (0 == TextBox->Get_UI_GroupID())
		{
			TextBox->Set_Content(L"버리기");
		}
		if (1 == TextBox->Get_UI_GroupID())
		{
			wsprintf(ChangeText, CountText, m_CurrentItemInfo.ItemName);
			TextBox->Set_Content(ChangeText);
			TextBox->Set_Change_TextColor(FONT_GREEN);
		}
		if (2 == TextBox->Get_UI_GroupID())
		{
			wsprintf(ChangeText, CountNum, m_iItemStatCount);
			TextBox->Set_Content(ChangeText);

		}
	}
	for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
	{
		if (10 == Button->Get_UI_GroupID()) // 개수 감소
		{
			if (dynamic_cast<CUI_Arrow*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iItemStatCount -= 1;
				if (m_iItemStatCount == 0)
				{
					m_iItemStatCount = m_iItemMaxCount;
				}

			}

		}
		if (20 == Button->Get_UI_GroupID()) // 개수 증가
		{
			if (dynamic_cast<CUI_Arrow*>(Button)->Get_Mouse_Select_OnOff())
			{
				if (m_iItemMaxCount > m_iItemStatCount)
				{
					m_iItemStatCount += 1;
				}
			}
		}


		if (30 == Button->Get_UI_GroupID()) // 아이템 버리기
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				if (m_pGameInstance->Use_Item(m_CurrentItemInfo.ItemType, m_iItemStatCount)) // 아이템이 저장되어 있는 컨테이너에서 Item_type이 맞는 녀석의 개수를 빼기
				{

					Update_Get_ItemMgr(); // 컨테이너에서 정보 뺐으니깐 인벤토리 슬롯 정보 다시 업데이트

					//dynamic_cast<CUIGroup_PlayerScreen*>(m_pGroupPlayerScreen)->Set_m_bDrop(true);
				}


				UI_Item tagDrop = m_CurrentItemInfo;
				tagDrop.ItemCount = m_iItemStatCount;
				m_mapDropItemInfo.emplace(m_CurrentItemInfo.ItemType, tagDrop); // 버릴 아이템 정보 전달용
				
				m_pGameInstance->Drop_Item(m_CurrentItemInfo.ItemType, dynamic_cast<CTransform*>(m_pPlayer->Find_Component(TEXT("Com_Transform")))->Get_State(CTransform::STATE_POSITION), m_pPlayer);

				ItemUse_Update();
				ItemType_PopUP_State(false);
				m_iItemStatCount = 1;
				m_bItemTypePopOpen = false;
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}
			}
		}
		if (40 == Button->Get_UI_GroupID())// 창 종료
		{
			if (dynamic_cast<CUI_ButtonHighlight*>(Button)->Get_Mouse_Select_OnOff())
			{
				m_iItemStatCount = 1;
				m_bItemUsePopOpen = false;
				for (auto& Image : m_pItemUsePopUp->Find_UI_Image())
				{
					Image->Set_OnOff(false);
				}

				for (auto& Button : m_pItemUsePopUp->Find_UI_Button())
				{
					Button->Set_OnOff(false);
				}

				for (auto& TextBox : m_pItemUsePopUp->Find_UI_TextBox())
				{
					TextBox->Set_OnOff(false);
				}
			}
		}
	}
}

void CUIGroup_Inventory::ItemUse_Update()
{
	_int iOpenSlotCount = 100; // 슬롯 번호
	for (auto& Slot : m_pItemScene->Find_UI_Button()) // 슬롯 가져오기
	{
		if (iOpenSlotCount == Slot->Get_UI_GroupID())
		{
			const _tchar* Text = { L"x%d" };

			if (lstrcmp(Slot->Get_Content().c_str(), L"x0"))
			{
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Item_Icon_OnOff(false);
				dynamic_cast<CUI_ItemBackground*>(Slot)->Set_Content(L"");
				iOpenSlotCount++;
			}
		}
	}
	if (m_bCommonOpen)
		Itme_View_Inventory(m_InvenItemCommon); // 일반 아이템을 슬롯에 띄우기

	if (m_bSkillOpen)
		Itme_View_Inventory(m_InvenItemSkill);// 스킬 아이템을 슬롯에 띄우기

}

void CUIGroup_Inventory::Item_Use_Set(ITEM_TYPE etype, _uint iCount)
{
	switch (etype)
	{
	case Engine::ITEM_TYPE::ITEM_KEY1:
		break;
	case Engine::ITEM_TYPE::ITEM_KEY2:
		break;
	case Engine::ITEM_TYPE::ITEM_MEMORY:
		dynamic_cast<CPlayer*>(m_pPlayer)->Increase_MemoryFragment(iCount * 100);
		break;
	case Engine::ITEM_TYPE::ITEM_FORGIVEN:
		break;
	case Engine::ITEM_TYPE::ITEM_SKILLPIECE:
		break;
	case Engine::ITEM_TYPE::ITEM_END:
		break;
	default:
		break;
	}


}

void CUIGroup_Inventory::Update_Get_ItemMgr()
{
	UI_Item ItemBox = {};
	m_InvenItemCommon.clear();
	m_InvenItemSkill.clear();

	m_ItemMgrContainerRef = m_pGameInstance->Get_Item_Info(); // 아이템 매니저에서 정보 가져오고

	for (auto& ItemInfo : m_ItemMgrContainerRef)
	{
		ItemBox.ItemType = ItemInfo.first;
		ItemBox.ItemCount = ItemInfo.second.first;

		if (0 != ItemBox.ItemCount)
		{
			if (ItemBox.ItemType == ITEM_TYPE::ITEM_SKILLPIECE)
				m_InvenItemSkill.push_back(ItemBox);
			else
				m_InvenItemCommon.push_back(ItemBox);

		}
	}
}

void CUIGroup_Inventory::Update_ItemInfo()
{
	for (auto& InvenInfo : m_InvenItemCommon)
	{
		switch (InvenInfo.ItemType)
		{
		case ITEM_TYPE::ITEM_KEY1:
			InvenInfo.ItemIconNum = 1;
			InvenInfo.ItemName = L"열쇠A";
			InvenInfo.ItemDesc = L"열쇠 입니다.";
			break;
		case ITEM_TYPE::ITEM_KEY2:
			InvenInfo.ItemIconNum = 3;
			InvenInfo.ItemName = L"열쇠B";
			InvenInfo.ItemDesc = L"열쇠 입니다.";
			break;
		case ITEM_TYPE::ITEM_MEMORY:
			InvenInfo.ItemIconNum = 5;
			InvenInfo.ItemName = L"기억의 모음집";
			InvenInfo.ItemDesc = L"이 아이템을 사용하면 기억의 파편을 획득합니다.";
			break;
		case ITEM_TYPE::ITEM_FORGIVEN:
			InvenInfo.ItemIconNum = 4;
			InvenInfo.ItemName = L"잊혀진 깃털";
			InvenInfo.ItemDesc = L"잊혀진 깃털을 사용하면, 아이세미와 대화하거나 신호기를 사용하여\n코르버스의 레벨과 상태를 초기화할 수 있습니다.";
			break;
		}
	}
	for (auto& InvenInfo : m_InvenItemSkill)
	{
		switch (InvenInfo.ItemType)
		{
		case ITEM_TYPE::ITEM_SKILLPIECE:
			InvenInfo.ItemIconNum = 7;
			InvenInfo.ItemName = L"단도";
			InvenInfo.ItemDesc = L"기술의 파편을 충분히 수집하여 신호기에서 역병 무기를 해제하거나\n업그레이드 하세요";
			break;
		}
	}
}

void CUIGroup_Inventory::Connect_MiniView_ItemInfo(UI_Item ItemInfo)
{
	_tchar* pTemp = {};

	// 마우스가 올라 갔을 때 해당 아이템의 정보를 가져오자
	// 이름 설명을 연결

	//텍스트 박스 n번에 아이템 이름 출력
	dynamic_cast<CUIObject*>(m_pItemTextBoxName)->Set_Content(ItemInfo.ItemName);
	dynamic_cast<CUIObject*>(m_pItemTextBoxDesc)->Set_Content(ItemInfo.ItemDesc);

	_tchar* pSor = { L"%d" };
	_tchar pDust[MAX_PATH] = {};
	wsprintf(pDust, pSor, ItemInfo.ItemCount);
	pTemp = L"수량";
	dynamic_cast<CUIObject*>(m_pItemTextBoxCount)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxCountNum)->Set_Content(pDust);

	switch (ItemInfo.ItemType)
	{
	case ITEM_TYPE::ITEM_KEY1:
		pTemp = L"일반";
		break;
	case ITEM_TYPE::ITEM_KEY2:
		pTemp = L"일반";
		break;
	case ITEM_TYPE::ITEM_MEMORY:
		pTemp = L"소비 아이템";
		break;
	case ITEM_TYPE::ITEM_FORGIVEN:
		pTemp = L"소비 아이템";
		break;
	case ITEM_TYPE::ITEM_SKILLPIECE:
		pTemp = L"기술의 파편";
		break;
	}
	dynamic_cast<CUIObject*>(m_pItemTextBoxType)->Set_Content(pTemp);

	for (auto& image : m_pItemScene->Find_UI_Image())
	{
		if (200 <= image->Get_UI_GroupID() && 250 >= image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_OnOff(true);
		}
		if (250 == image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_TexNumber(ItemInfo.ItemIconNum);
		}
	}

}

void CUIGroup_Inventory::DisConnect_MiniView_ItemInfo()
{
	_tchar* pTemp = { L"" };

	dynamic_cast<CUIObject*>(m_pItemTextBoxName)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxDesc)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxCount)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxCountNum)->Set_Content(pTemp);
	dynamic_cast<CUIObject*>(m_pItemTextBoxType)->Set_Content(pTemp);
	for (auto& image : m_pItemScene->Find_UI_Image())
	{
		if (200 <= image->Get_UI_GroupID() && 250 >= image->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Image*>(image)->Set_OnOff(false);
		}

	}

}

CUIGroup_Inventory* CUIGroup_Inventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_Inventory* pInstance = new CUIGroup_Inventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_Inventory::Clone(void* pArg)
{
	CUIGroup_Inventory* pInstance = new CUIGroup_Inventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_Inventory::Free()
{
	__super::Free();
}

void CUIGroup_Inventory::Set_Item_Default_Info()
{
	UI_Item SaveData = {};
	SaveData.ItemType = ITEM_TYPE::ITEM_KEY1;
	SaveData.ItemIconNum = 1;
	SaveData.ItemName = L"열쇠A";
	SaveData.ItemDesc = L"열쇠 입니다.";
	SaveData.ItemCount = 0;

	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_KEY2;
	SaveData.ItemIconNum = 3;
	SaveData.ItemName = L"열쇠B";
	SaveData.ItemDesc = L"열쇠 입니다.";
	SaveData.ItemCount = 0;

	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_MEMORY;
	SaveData.ItemIconNum = 5;
	SaveData.ItemName = L"기억의 모음집";
	SaveData.ItemDesc = L"이 아이템을 사용하면 기억의 파편을 획득합니다.";
	SaveData.ItemCount = 0;

	m_vecItemDefaultInfo.push_back(SaveData);

	SaveData.ItemType = ITEM_TYPE::ITEM_FORGIVEN;
	SaveData.ItemIconNum = 4;
	SaveData.ItemName = L"잊혀진 깃털";
	SaveData.ItemDesc = L"잊혀진 깃털을 사용하면, 아이세미와 대화하거나 신호기를 사용하여\n코르버스의 레벨과 상태를 초기화할 수 있습니다.";
	SaveData.ItemCount = 0;

	m_vecItemDefaultInfo.push_back(SaveData);


	SaveData.ItemType = ITEM_TYPE::ITEM_SKILLPIECE;
	SaveData.ItemIconNum = 7;
	SaveData.ItemName = L"단도";
	SaveData.ItemDesc = L"기술의 파편을 충분히 수집하여 신호기에서 역병 무기를 해제하거나\n업그레이드 하세요";
	SaveData.ItemCount = 0;
}

