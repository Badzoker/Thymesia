#include "pch.h"
#include "UI_PlayerAttribute.h"
#include "UI_Scene.h"
#include "GameInstance.h"

CUI_PlayerAttribute::CUI_PlayerAttribute(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_PlayerAttribute::CUI_PlayerAttribute(const CUI_PlayerAttribute& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_PlayerAttribute::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerAttribute::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CUI_PlayerAttribute::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUI_PlayerAttribute::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CUI_PlayerAttribute::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CUI_PlayerAttribute::Render()
{
	return S_OK;
}

HRESULT CUI_PlayerAttribute::Ready_UIObject()
{
	LoadData_UI_Scene(UISCENE_ATTRIBUTE, L"UIScene_PlayerAttribute");
	return S_OK;
}

HRESULT CUI_PlayerAttribute::LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName)
{
	HANDLE		hFile = CreateFile(L"../Bin/DataFiles/UISave/PlayerAttribute.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load UI_PlayerAttribute File", _T("Fail"), MB_OK);
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


	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, L"Load 완료", _T("성공"), MB_OK);
	return S_OK;
}

CUI_PlayerAttribute* CUI_PlayerAttribute::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PlayerAttribute* pInstance = new CUI_PlayerAttribute(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_PlayerAttribute");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerAttribute::Clone(void* pArg)
{
	CUI_PlayerAttribute* pInstance = new CUI_PlayerAttribute(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_PlayerAttribute");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerAttribute::Free()
{
	__super::Free();
}
