#include "pch.h"
#include "UIGroup_PlayerTalent.h"
#include "UI_Scene.h"
#include "GameInstance.h"

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

	return S_OK;
}

void CUIGroup_PlayerTalent::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUIGroup_PlayerTalent::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent")), true);
	}

}

void CUIGroup_PlayerTalent::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
}

HRESULT CUIGroup_PlayerTalent::Render()
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

HRESULT CUIGroup_PlayerTalent::Ready_UIObject()
{

	//m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent");
	//m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_PlayerTalent", m_TextInfo);
	LoadData_UIObject(LEVEL_STATIC, UISCENE_TALENT, L"UIScene_PlayerTalent");

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
}
