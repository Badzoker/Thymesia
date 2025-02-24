#include "pch.h"
#include "UI_PlayerLevelUP.h"
#include "UI_Scene.h"
#include "GameInstance.h"
CUI_PlayerLevelUP::CUI_PlayerLevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUIObject{ pDevice, pContext }
{
}

CUI_PlayerLevelUP::CUI_PlayerLevelUP(const CUI_PlayerLevelUP& Prototype)
    : CUIObject(Prototype)
{
}

HRESULT CUI_PlayerLevelUP::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_PlayerLevelUP::Initialize(void* pArg)
{
    if (FAILED(Ready_UIObject()))
        return E_FAIL;

    return S_OK;
}

void CUI_PlayerLevelUP::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CUI_PlayerLevelUP::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CUI_PlayerLevelUP::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CUI_PlayerLevelUP::Render()
{
    return S_OK;
}

HRESULT CUI_PlayerLevelUP::Ready_UIObject()
{
    LoadData_UI_Scene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP");
    return S_OK;
}

HRESULT CUI_PlayerLevelUP::LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName)
{
	HANDLE		hFile = CreateFile(L"../Bin/DataFiles/UISave/PlayerLevelUP.dat", GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load UI_PlayerLevelUP File", _T("Fail"), MB_OK);
		return S_OK;
	}



	DWORD	dwByte(0);
	CUIObject::UIOBJECT_DESC Desc{};
	_float3  fPos = {};
	_float2  fSize = {};
	_float3  fRotation = {};
	_uint  iLen = {};
	_wstring szSaveName = {};
	_uint iUIType = {};
	_uint iShaderNum = {};

	while (true)
	{
		ReadFile(hFile, &fPos, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &fSize, sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &fRotation, sizeof(_float3), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		szSaveName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(szSaveName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iUIType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iShaderNum, sizeof(_uint), &dwByte, nullptr);

		if (0 == dwByte)
		{
			break;
		}

		Desc.fX = fPos.x;
		Desc.fY = fPos.y;
		Desc.fZ = fPos.z;
		Desc.fSizeX = fSize.x;
		Desc.fSizeY = fSize.y;
		Desc.szProtoName = szSaveName;
		Desc.iShaderPassNum = iShaderNum;
		Desc.fRotation = fRotation;
		if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, szSaveName, iSceneIndex, szSceneName, iUIType, &Desc)))
			return E_FAIL;

	}

	CloseHandle(hFile);

	//MessageBox(g_hWnd, L"Load 완료", _T("성공"), MB_OK);
	return S_OK;
}

CUI_PlayerLevelUP* CUI_PlayerLevelUP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_PlayerLevelUP* pInstance = new CUI_PlayerLevelUP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_PlayerLevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerLevelUP::Clone(void* pArg)
{
	CUI_PlayerLevelUP* pInstance = new CUI_PlayerLevelUP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_PlayerLevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerLevelUP::Free()
{
	__super::Free();
}
