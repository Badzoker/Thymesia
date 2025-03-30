#include "pch.h"
#include "UIGroup_PlayerTalent.h"
#include "UI_Scene.h"
#include "GameInstance.h"
#include "UI_Button.h"

CUIGroup_PlayerTalent::CUIGroup_PlayerTalent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_PlayerTalent::CUIGroup_PlayerTalent(const CUIGroup_PlayerTalent& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_PlayerTalent::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_PlayerTalent::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	m_pMySceneBase = m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_0");
	m_pSlot_LightAttack = m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_1");

	Slot_Setting();

	return S_OK;
}

void CUIGroup_PlayerTalent::Priority_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_0")), true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_1")), true);
	}
}

void CUIGroup_PlayerTalent::Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		/*마우스 커서 위치에 따른 호버 이미지 이펙트 이미지 반응*/

		Slot_Update_State();


		for (auto& Button : m_pMySceneBase->Find_UI_Button())
		{
			if (Button->Get_Mouse_Select_OnOff())
			{
				if (40 == Button->Get_UI_GroupID())
				{
					Button->Set_Mouse_Select_OnOff(false);
					
					m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerTalent"), false);
					m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_0")), false);
					m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent_1")), false);
					m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerMenu"), true);
					m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
				}
			}
		}


	}

}

void CUIGroup_PlayerTalent::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{

	}
}

HRESULT CUIGroup_PlayerTalent::Render()
{
	if (m_bRenderOpen)
	{
	}
	return S_OK;
}

HRESULT CUIGroup_PlayerTalent::Ready_UIObject()
{
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent_0");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent_1");

	return S_OK;
}

HRESULT CUIGroup_PlayerTalent::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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

void CUIGroup_PlayerTalent::Slot_Setting()
{
	/* 슬롯 정보를 따로 컨테이너에 저장한다*/
	for (auto& Slot : m_pSlot_LightAttack->Find_UI_Button())
	{
		CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(Slot);


		if (100 < Slot->Get_UI_GroupID() &&
			200 > Slot->Get_UI_GroupID())
		{
			pSlot->Set_Slot_State(SLOT_OPEN_OFF);
			pSlot->Set_TalentPoint(0);
			m_mapSlot_LightAttack.emplace(Slot->Get_UI_GroupID(), make_pair(true, pSlot)); // 기본으로 주어지는 특성
		}
		else
		{
			pSlot->Set_Slot_State(SLOT_CLOSE_OFF);
			pSlot->Set_TalentPoint((pSlot->Get_UI_GroupID() - (pSlot->Get_UI_GroupID() - 100)) / 100);
			m_mapSlot_LightAttack.emplace(Slot->Get_UI_GroupID(), make_pair(false, pSlot)); // 특성 포인트가 필요한 특성
		}
	}

}

void CUIGroup_PlayerTalent::Slot_Update_State()
{
	/* map에 저장한 button의 정보에 따라 이미지를 반응 설정*/

	for (auto& SlotEffect : m_mapSlot_LightAttack)
	{
		CUI_Frame* pSlot = dynamic_cast<CUI_Frame*>(SlotEffect.second.second);

		Slot_Update_State_Value(pSlot->Get_Slot_State(), pSlot);

		if (1 >= pSlot->Get_Slot_State())
			SlotEffect.second.first = true;
		else
			SlotEffect.second.first = false;

	}

}

void CUIGroup_PlayerTalent::Slot_Update_State_Value(SLOTSTATE eSteteNum, CUI_Frame* pSlotUIObj)
{
	switch (eSteteNum)
	{
	case Client::SLOT_OPEN_ON:
		pSlotUIObj->Set_TexSlot(3);
		pSlotUIObj->Set_TexIconOff(false);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);
		pSlotUIObj->Set_TexEffectOff(false);
		pSlotUIObj->Set_TexEffect(1);
		break;
	case Client::SLOT_OPEN_OFF:
		pSlotUIObj->Set_TexSlot(3);
		pSlotUIObj->Set_TexIconOff(false);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(2);
		pSlotUIObj->Set_TexEffectOff(false);
		pSlotUIObj->Set_TexEffect(0);
		break;
	case Client::SLOT_CLOSE_ON:
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexIconOff(true); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);   // 이미지 랜더를 켜야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1); // 이미지 랜더를 꺼야 함
		break;
	case Client::SLOT_CLOSE_OFF:
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexIconOff(true); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexEdgeOff(true);
		pSlotUIObj->Set_TexEdge(2);  // 이미지 랜더를 꺼야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1);// 이미지 랜더를 꺼야 함
		break;
	case Client::SLOT_CLOSE_CONDITION:
		pSlotUIObj->Set_TexSlot(0);
		pSlotUIObj->Set_TexIconOff(true); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);  // 이미지 랜더를 켜야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1);// 이미지 랜더를 꺼야 함
		break;
	
	}

}


CUIGroup_PlayerTalent* CUIGroup_PlayerTalent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_PlayerTalent* pInstance = new CUIGroup_PlayerTalent(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_PlayerTalent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_PlayerTalent::Clone(void* pArg)
{
	CUIGroup_PlayerTalent* pInstance = new CUIGroup_PlayerTalent(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_PlayerTalent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_PlayerTalent::Free()
{
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_TALENT);
}
