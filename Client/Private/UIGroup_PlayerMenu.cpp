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
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerLevelUP"), true);
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
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);

			}
			if (120 == Button->Get_UI_GroupID()) //게임 재개
			{
				Button->Set_Mouse_Select_OnOff(false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerMenu"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);

			}

		}
	}
}

HRESULT CUIGroup_PlayerMenu::Ready_UIObject()
{
	//m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_MENU, L"UIScene_PlayerMenu");
	//m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_PlayerMenu", m_TextInfo);
	LoadData_UIObject(LEVEL_STATIC, UISCENE_MENU, L"UIScene_PlayerMenu");
	return S_OK;
}

HRESULT CUIGroup_PlayerMenu::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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
