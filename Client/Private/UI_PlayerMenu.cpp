#include "pch.h"
#include "UI_PlayerMenu.h"
#include "UI_Scene.h"
#include "GameInstance.h"

CUI_PlayerMenu::CUI_PlayerMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_PlayerMenu::CUI_PlayerMenu(const CUI_PlayerMenu& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_PlayerMenu::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerMenu::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CUI_PlayerMenu::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_PlayerMenu::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUI_PlayerMenu::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_PlayerMenu::Render()
{
	return S_OK;
}

HRESULT CUI_PlayerMenu::Ready_UIObject()
{
	LoadData_UI_Scene(UISCENE_MENU, L"UIScene_PlayerMenu");
	return S_OK;
}

HRESULT CUI_PlayerMenu::LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName)
{
	HANDLE		hFile = CreateFile(L"../Bin/DataFiles/UISave/PlayerMenu.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load UI_PlayerMenu File", _T("Fail"), MB_OK);
		return S_OK;
	}

	DWORD	dwByte(0);
	CUIObject::UIOBJECT_DESC Desc{};
	_float3  m_fPos = {};
	_float2  m_fSize = {};
	_uint  iLen = {};
	_wstring szSaveName = {};
	_uint iUIType = {};
	_uint iShaderNum = {};

	while (true)
	{
		ReadFile(hFile, &m_fPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &m_fSize, sizeof(_float2), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szSaveName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szSaveName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		Desc.fX = m_fPos.x;
		Desc.fY = m_fPos.y;
		Desc.fZ = m_fPos.z;
		Desc.fSizeX = m_fSize.x;
		Desc.fSizeY = m_fSize.y;
		Desc.szProtoName = szSaveName;
		Desc.iShaderPassNum = iShaderNum;

		if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, szSaveName, iSceneIndex, szSceneName, iUIType, &Desc)))
			return E_FAIL;


		//ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, L"Load 완료", _T("성공"), MB_OK);
	return S_OK;
}

CUI_PlayerMenu* CUI_PlayerMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PlayerMenu* pInstance = new CUI_PlayerMenu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_PlayerMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerMenu::Clone(void* pArg)
{
	CUI_PlayerMenu* pInstance = new CUI_PlayerMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_PlayerMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerMenu::Free()
{
	__super::Free();
}
