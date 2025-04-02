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
			if (1 == Button->Get_UI_GroupID()) // 게임 계속하기 
			{
				m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.6f);
				m_pGameInstance->Set_NextLevel_Open(true, LEVEL_TUTORIAL); //Tutorial로 가라
			}

			if (2 == Button->Get_UI_GroupID()) // 새게임
			{
				m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.6f);
				m_pGameInstance->Set_NextLevel_Open(true, LEVEL_SEAOFTREES); //Tutorial로 가라
			}

			if (3 == Button->Get_UI_GroupID()) // 게임 불러오기
			{
				m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.6f);
				m_pGameInstance->Set_NextLevel_Open(true, LEVEL_HILL); //Tutorial로 가라
			}

			if (4 == Button->Get_UI_GroupID()) // 설정
			{
				//m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.6f);
				//m_pGameInstance->Set_NextLevel_Open(true, LEVEL_ROYALGARDEN); //Tutorial로 가라
			}

			if (5 == Button->Get_UI_GroupID()) // 제작진
			{
				//m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.6f);
				//m_pGameInstance->Set_NextLevel_Open(true, LEVEL_FORTRESS); //Tutorial로 가라
			}

			if (6 == Button->Get_UI_GroupID()) // 종료
			{
				DestroyWindow(g_hWnd);
			}
		}


	}

}

HRESULT CUIGroup_GameIntro::Ready_UIObject()
{
	//m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_INTRO, L"UIScene_Intro");
//	m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_Intro", m_TextInfo);
	LoadData_UIObject(LEVEL_STATIC, UISCENE_INTRO, L"UIScene_Intro");
	return S_OK;
}

HRESULT CUIGroup_GameIntro::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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
	_uint iTextureNum = { 0 };
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
		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szFontName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szFontName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szContentText.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szContentText.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

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


	//MessageBox(g_hWnd, L"Load 완료", _T("성공"), MB_OK);
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
	m_pGameInstance->UIScene_Clear(UISCENE_INTRO);
}
