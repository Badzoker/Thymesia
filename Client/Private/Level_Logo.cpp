#include "pch.h" 
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "BackGround.h"
#include "BlackScreen.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Logo::Initialize()
{
	
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_UIGroup_GameIntro(TEXT("Layer_GameIntro"))))
		return E_FAIL;

	return S_OK;
}

void CLevel_Logo::Update(_float fTimeDelta)
{
	/*m_fRenderTime += fTimeDelta;
	if (5 < m_fRenderTime)
	{
	*/	
	if (!m_bNextLevelOpen)
	{
		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_LOGO, TEXT("Layer_GameIntro"), true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), true);
	}
	else
	{
		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_LOGO, TEXT("Layer_GameIntro"), false);
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), false);
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY, 2, false));
	}
	
}

HRESULT CLevel_Logo::Render()
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CBackGround::BACKGROUND_DESC		Desc{};	

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		LEVEL_LOGO, pLayerTag, &Desc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_GameIntro"), LEVEL_LOGO, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Logo::Free()
{
	__super::Free();

}
