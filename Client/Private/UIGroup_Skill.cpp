#include "pch.h"
#include "GameInstance.h"
#include "UI_Scene.h"
#include "UIGroup_Skill.h"

#include "UI_Button.h"
#include "UI_TextBox.h"

CUIGroup_Skill::CUIGroup_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_Skill::CUIGroup_Skill(const CUIGroup_Skill& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_Skill::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_Skill::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	m_pBaseScene = m_pGameInstance->Find_UIScene(UISCENE_SKILL, L"UIScene_PlayerSkill");
	m_pEquipWeapon = m_pGameInstance->Find_UIScene(UISCENE_SKILL, L"UIScene_PlayerSkill_1Equip");
	m_pEquipCondition = m_pGameInstance->Find_UIScene(UISCENE_SKILL, L"UIScene_PlayerSkill_1Condition");


	return S_OK;
}

void CUIGroup_Skill::Priority_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pEquipWeapon, true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pEquipCondition, true);

		//m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pMapChangePop, true);
	}
}

void CUIGroup_Skill::Update(_float fTimeDelta)
{
}

void CUIGroup_Skill::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
	}
}

HRESULT CUIGroup_Skill::Render()
{

	return S_OK;
}

void CUIGroup_Skill::Slot_Update_State_Value(SkILL_SLOTSTATE eSteteNum, CUI_Skill_Slot* pSlotUIObj)
{
	switch (eSteteNum)
	{
	case Client::SKILL_OPEN_ON:
		pSlotUIObj->Set_TexIconOff(false); 
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(2);
		break;
	case Client::SKILL_OPEN_OFF:
		pSlotUIObj->Set_TexIconOff(true);
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexEdgeOff(true);
		pSlotUIObj->Set_TexEdge(1);
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(0);
		break;
	case Client::SKILL_CLOSE_ON:
		pSlotUIObj->Set_TexIconOff(false); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexSlot(0);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(1);   // 이미지 랜더를 켜야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1); // 이미지 랜더를 꺼야 함
		break;
	case Client::SKILL_CLOSE_OFF:
		pSlotUIObj->Set_TexSlot(2);
		pSlotUIObj->Set_TexIconOff(true); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexEdgeOff(true);
		pSlotUIObj->Set_TexEdge(1);  // 이미지 랜더를 꺼야 함
		pSlotUIObj->Set_TexEffectOff(true);
		pSlotUIObj->Set_TexEffect(1);// 이미지 랜더를 꺼야 함
		break;
	case Client::SKILL_OPEN_IDLE:
		pSlotUIObj->Set_TexIconOff(false); // 이미지 흐리게 처리
		pSlotUIObj->Set_TexSlot(0);
		pSlotUIObj->Set_TexEdgeOff(false);
		pSlotUIObj->Set_TexEdge(2);  // 이미지 랜더를 켜야 함
		pSlotUIObj->Set_TexEffectOff(false);
		pSlotUIObj->Set_TexEffect(2);// 이미지 랜더를 꺼야 함
		break;

	}


}

HRESULT CUIGroup_Skill::Ready_UIObject()
{
	LoadData_UIObject(LEVEL_STATIC, UISCENE_SKILL, L"UIScene_PlayerSkill");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_SKILL, L"UIScene_PlayerSkill_1Equip");
	LoadData_UIObject(LEVEL_STATIC, UISCENE_SKILL, L"UIScene_PlayerSkill_1Condition");
	return S_OK;
}

HRESULT CUIGroup_Skill::LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
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

HRESULT CUIGroup_Skill::LoadData_UIText_Info(const _tchar* szSceneName)
{
	char   szDir[MAX_PATH] = "../Bin/DataFiles/UISave/";
	_char   szFileName[MAX_PATH] = "";

	WideCharToMultiByte(CP_ACP, 0, szSceneName, (_int)(wcslen(szSceneName)), szFileName, MAX_PATH, nullptr, nullptr);

	_char   szPlusText[MAX_PATH] = "_Text";
	_char   szEXT[MAX_PATH] = ".dat";

	_char   szFullPath[MAX_PATH] = "";
	strcat_s(szFullPath, szDir);
	strcat_s(szFullPath, szFileName);
	strcat_s(szFullPath, szPlusText);
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
	UI_TextInfo TextInfo = {};
	_uint iLen = {};

	while (true)
	{

		ReadFile(hFile, &TextInfo.iTextID, sizeof(_uint), &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		TextInfo.strFontName.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(TextInfo.strFontName.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &iLen, sizeof(_uint), &dwByte, nullptr);
		TextInfo.srtTextContent.resize(iLen);
		ReadFile(hFile, const_cast<wchar_t*>(TextInfo.srtTextContent.data()), sizeof(_tchar) * iLen, &dwByte, nullptr);

		ReadFile(hFile, &TextInfo.fTextStartPos, sizeof(_float2), &dwByte, nullptr);
		ReadFile(hFile, &TextInfo.fTextSize, sizeof(_float2), &dwByte, nullptr);


		if (0 == dwByte)
		{
			break;
		}


		m_TextInfo.push_back(TextInfo);

	}

	CloseHandle(hFile);

	return S_OK;
}

CUIGroup_Skill* CUIGroup_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_Skill* pInstance = new CUIGroup_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_Skill::Clone(void* pArg)
{
	CUIGroup_Skill* pInstance = new CUIGroup_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_Skill::Free()
{
	__super::Free();
	m_pGameInstance->UIScene_Clear(UISCENE_MAP);
}
