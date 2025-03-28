#include "pch.h"
#include "UIGroup_PlayerScreen.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "UI_Text.h"
#include "UI_TextBox.h"
#include "UI_Image.h"
#include "Player.h"
#include "UI_ItemBackground.h"
#include "UIGroup_Inventory.h"
#include "UI_PlunderSlotFrame.h"
#include "UI_HPBar3_MainBar.h"
#include "UI_HPBar5_Track.h"

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


	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	m_eLevelID = static_cast<LEVELID>(pDesc->iCurLevel);

	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen");
	m_pItmeScreen = m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen_1");
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pItmeScreen, false);

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(m_eLevelID, TEXT("Layer_Player"), "PLAYER");
	m_pGroupInven = m_pGameInstance->Get_GameObject_To_Layer(m_eLevelID, TEXT("Layer_PlayerInventory"), "Inventory");
	for (auto& Textbox : m_pMyScene->Find_UI_TextBox())
	{
		if (101 == Textbox->Get_UI_GroupID())
		{
			Textbox->Set_OnOff(false);
			m_pMonsterText = Textbox;
		}
	}

	Ready_Skill_Slot();
	Ready_Player_GageBar();


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
	Button_Skill();

	Player_Info_GageBar();
	UI_Direction_HPBar();
	if (m_pMonsterText->Get_OnOff())
	{
		m_fMonsterTextOnTime += fTimeDelta;
		if (m_fMonsterTextOnTime > 2)
		{
			m_pMonsterText->Set_OnOff(false);
			m_fMonsterTextOnTime = 0;

		}
	}
	
	if (0 != m_pGameInstance->Get_Item_Drop_Info().size()) // 버리는 아이템이 있는경우
	{
		for (auto& SaveInfo : m_pGameInstance->Get_Item_Drop_Info())
		{
			Item_Drop_Info(SaveInfo);
		}
		m_pGameInstance->Get_Item_Drop_Info().clear();
	}
	if (0 != m_pGameInstance->Get_Item_Save_Info().size()) // 획득한 아이템이 있는 경우
	{
		UI_Direction_Item_Nudge();

		_uint iNum = {};

		if (0 != m_pGameInstance->Get_Item_Save_Info().size())
		{
			for (auto& SaveInfo : m_pGameInstance->Get_Item_Save_Info())
			{
				if (!m_bNudgeUse[0]) // 1번 넛지 꺼져 있으면
				{
					Item_Save_Info(SaveInfo); // 추가해서 재생 
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
	if (m_bRenderOpen)
	{
	}
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
		if (22 == TextBox->Get_UI_GroupID()) // mp 플러스 되는 수치 인 것 같음
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

void CUIGroup_PlayerScreen::UI_Direction_HPBar()
{

	dynamic_cast<CUI_HPBar3_MainBar*>(m_pHPGageBar)->Set_PlayerHP_Info((_float)dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullHp(), (_float)dynamic_cast<CPlayer*>(m_pPlayer)->Get_CurrentHp());

	if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_CurrentHp() == dynamic_cast<CPlayer*>(m_pPlayer)->Get_FullHp())
	{
		dynamic_cast<CUI_HPBar5_Track*>(m_pHPGageTrack)->Set_Open_Image(false);
	}
	else
	{
		_float fX = dynamic_cast<CTransform*>(dynamic_cast<CUI_HPBar3_MainBar*>(m_pHPGageBar)->Find_Component(TEXT("Com_Transform")))->Get_State_UIObj(CTransform::STATE_POSITION).x + 129;
		dynamic_cast<CUI_HPBar5_Track*>(m_pHPGageTrack)->Set_ChangeX(fX);
		dynamic_cast<CUI_HPBar5_Track*>(m_pHPGageTrack)->Set_Open_Image(true);
	}









}

void CUIGroup_PlayerScreen::Item_Save_Info(ITEM_TYPE eItemType)
{
	UI_Item MakeInfo = {};
	for (auto& ItemInfo : dynamic_cast<CUIGroup_Inventory*>(m_pGroupInven)->Get_Vector_Itme_default_Info())
	{
		if (eItemType == ItemInfo.ItemType)
		{
			MakeInfo = ItemInfo;
			break;
		}
	}

	_uint iTexNum = {};
	switch (MakeInfo.ItemType)
	{
	case ITEM_TYPE::ITEM_KEY1:
		MakeInfo.ItemDesc = L"- 일반 아이템 -";
		iTexNum = 0;
		break;
	case ITEM_TYPE::ITEM_KEY2:
		MakeInfo.ItemDesc = L"- 일반 아이템- ";
		iTexNum = 0;
		break;
	case ITEM_TYPE::ITEM_MEMORY:
		MakeInfo.ItemDesc = L"- 소비 아이템 -";
		iTexNum = 0;
		break;
	case ITEM_TYPE::ITEM_FORGIVEN:
		MakeInfo.ItemDesc = L"- 소비 아이템 -";
		iTexNum = 3;
		break;
	case ITEM_TYPE::ITEM_SKILLPIECE:
		MakeInfo.ItemDesc = L"- 기술의 파편 -";
		iTexNum = 2;
		break;

	}

	// 여기서 시작, 도착 좌표해서 보간하면 될 듯
	_uint iNum = { 1 };
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
			dynamic_cast<CUI_Image*>(Image)->Set_TexNumber(MakeInfo.ItemIconNum);
		}
	}
	for (auto& TextBox : m_pItmeScreen->Find_UI_TextBox()) // 아이템 이름 
	{
		if (iNum == TextBox->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(MakeInfo.ItemName);
		}
		if (iNum +10 == TextBox->Get_UI_GroupID()) // 아이템 카테고리 설명
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(MakeInfo.ItemDesc);
		}
	}


}

void CUIGroup_PlayerScreen::Item_Drop_Info(ITEM_TYPE eItemType)
{
	UI_Item MakeInfo = {};
	for (auto& ItemInfo : dynamic_cast<CUIGroup_Inventory*>(m_pGroupInven)->Get_Vector_Itme_default_Info())
	{
		if (eItemType == ItemInfo.ItemType)
		{
			MakeInfo = ItemInfo;
			break;
		}
	}

	MakeInfo.ItemDesc = L"- 떨어뜨림 -";
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
			dynamic_cast<CUI_Image*>(Image)->Set_TexNumber(MakeInfo.ItemIconNum);
		}
	}
	for (auto& TextBox : m_pItmeScreen->Find_UI_TextBox()) // 아이템 이름 
	{
		if (iNum == TextBox->Get_UI_GroupID())
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(MakeInfo.ItemName);
		}
		if (iNum + 10 == TextBox->Get_UI_GroupID()) // 아이템 카테고리 설명
		{
			dynamic_cast<CUI_Text*>(TextBox)->Set_OnOff(true);
			dynamic_cast<CUI_Text*>(TextBox)->Set_Content(MakeInfo.ItemDesc);
		}
	}

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

void CUIGroup_PlayerScreen::Button_Skill()
{

	if (m_pGameInstance->isKeyEnter(DIK_K))
	{
		/* 고정 스킬*/
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pFixSkill_1)->Set_SkillOn(true);
	}
	if (m_pGameInstance->isKeyEnter(DIK_L))
	{
		/* 약탈 스킬*/
		dynamic_cast<CUI_PlunderSlotFrame*>(m_pPlunderSkill)->Set_SkillOn(true);
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
	LoadData_UIObject(LEVEL_STATIC, UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen_1");

	return S_OK;
}

void CUIGroup_PlayerScreen::Ready_Skill_Slot()
{
	/*스킬 사용 연출을 위해 미리 멤버 변수로 설정하기*/
	for (auto& Slot : m_pMyScene->Find_UI_Image())
	{
		if (50 == Slot->Get_UI_GroupID())
		{
			m_pFixSkill_1 = Slot;
		}
		if (60 == Slot->Get_UI_GroupID())
		{
			m_pPlunderSkill = Slot;
		}
	}

}

void CUIGroup_PlayerScreen::Ready_Player_GageBar()
{
	/*HP,MP 연출을 위해 미리 멤버 변수로 설정하기*/
	for (auto& Gage : m_pMyScene->Find_UI_Image())
	{
		if (10 == Gage->Get_UI_GroupID())
			m_pHPGageBar = Gage;
		if (11 == Gage->Get_UI_GroupID())
			m_pHPGageTrack = Gage;
	}


}

HRESULT CUIGroup_PlayerScreen::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
{
	char   szDir[MAX_PATH] = "../Bin/DataFiles/UISave/";
	_char   szFileName[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, 0, szSceneName, (_int)(wcslen(szSceneName)), szFileName, MAX_PATH, nullptr, nullptr);

	_char   szEXT[MAX_PATH] = ".dat";

	_char   szFullPath[MAX_PATH] = "";
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, szEXT);

	_tchar  szLastPath[MAX_PATH] = {};
	MultiByteToWideChar(CP_ACP, 0, szFullPath, (_int)strlen(szFullPath), szLastPath, MAX_PATH);


	HANDLE		hFile = CreateFile(szLastPath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, szLastPath, TEXT("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	CUIObject::UIOBJECT_DESC Desc{};
	_float3  fPos = {};
	_float2  fSize = {};
	_float3  fRotation = {};
	_uint  iLen = {};
	_wstring szFontName = {};
	_wstring szContentText = {};
	_wstring szSaveName = {};
	_uint iUIType = {};
	_uint iShaderNum = {};
	_uint iTextureNum = {};
	_uint iGroupID = {};

	while (true)
	{
		ReadFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &fSize, sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szSaveName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szSaveName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		if (iUIType == UI_TEXT || iUIType == UI_BUTTON)
		{
			ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
			szFontName.resize(iLen);
			ReadFile(hFile, const_cast<wchar_t*>(szFontName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

			ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
			szContentText.resize(iLen);
			ReadFile(hFile, const_cast<wchar_t*>(szContentText.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		}

		ReadFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iTextureNum, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iGroupID, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		Desc.fNear = 0.f;
		Desc.fFar = 1.f;

		Desc.fX = fPos.x;
		Desc.fY = fPos.y;
		Desc.fZ = fPos.z;
		Desc.fSizeX = fSize.x;
		Desc.fSizeY = fSize.y;

		Desc.strFontName = szFontName;
		Desc.strContent = szContentText;

		Desc.strProtoName = szSaveName;
		Desc.iShaderPassNum = iShaderNum;
		Desc.iTexNumber = iTextureNum;
		Desc.iGroupID = iGroupID;
		Desc.fRotation = fRotation;
		if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(iLevelIndex, szSaveName, iSceneIndex, szSceneName, iUIType, &Desc)))
			return E_FAIL;

	}

	CloseHandle(hFile);

	//MessageBox(hWnd, L"Load 완료", TEXT("성공"), MB_OK);
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
	m_pGameInstance->UIScene_Clear(UISCENE_PLAYERSCREEN);
}
