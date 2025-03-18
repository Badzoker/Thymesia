#include "pch.h" 
#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "UI_Scene.h"
#include "UI_Image.h"
#include "UI_Text.h"
#include "UI_TextBox.h"
#include "UI_LoadingScreen.h"
#include "UI_LoadingIcon.h"
#include "Blackscreen.h"

CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Loading::Initialize(LEVELID eNextLevelID, _uint iLoadingNum, _bool bCheck)
{
	m_eNextLevelID = eNextLevelID;
	m_iLoadingeNum = iLoadingNum;

	if (bCheck)
	{
  		if (FAILED(Ready_Layer_UIGroup_Loading(TEXT("Layer_Loading"))))
			return E_FAIL;	
		if (FAILED(Ready_Layer_Mouse(TEXT("Layer_Mouse"))))
			return E_FAIL;	

		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), true);

		/*if (FAILED(Ready_Layer_Fade(TEXT("Layer_Loading"))))
			return E_FAIL;

		m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_OUT);
		m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_IN);*/
		
	}
	m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.2f);

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	if (m_eNextLevelID > LEVEL_STATIC)
	{
		/*CGameObject* pUIgroupLoading = m_pGameInstance->Get_LayerGameObject(LEVEL_LOADING, TEXT("Layer_Loading"))->back();
		dynamic_cast<CUIGroup_Loading*>(pUIgroupLoading)*/
		
		CUI_Scene* pScene = m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading");
		
		for (auto& Image : pScene->Find_UI_Image())
		{
			if (0 == Image->Get_UI_GroupID())
			{
				dynamic_cast<CUI_LoadingScreen*>(Image)->Set_TexNumber(m_iLoadingeNum);
				break;
			}
		}
		
		for (auto& TextBox : pScene->Find_UI_TextBox())
		{
			if (0 == TextBox->Get_UI_GroupID())
			{
				TextBox->Set_OnOff(false);
				break;
			}
		}



		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Loading"), true);
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading")), true);


		
	}
	
	if (true == m_pLoader->isFinished())
	{



		if (m_eNextLevelID == LEVEL_STATIC)
		{
			m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOGO,0, true));
		}
		if (m_eNextLevelID == LEVEL_LOGO) 
		{
			m_pGameInstance->Open_Level(m_eNextLevelID, CLevel_Logo::Create(m_pDevice, m_pContext));
		}
		if (m_eNextLevelID > LEVEL_LOGO)
		{

			CUI_Scene* pScene = m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading");
			for (auto& Image : pScene->Find_UI_Image())
			{
				if (1 == Image->Get_UI_GroupID())
				{
					dynamic_cast<CUI_LoadingIcon*>(Image)->Set_OnOff(false);
					break;
				}
			}
			for (auto& TextBox : pScene->Find_UI_TextBox())
			{
				if (0 == TextBox->Get_UI_GroupID())

				{
					TextBox->Set_OnOff(true);
					break;
				}
			}
			if (m_pGameInstance->isAnyEnter())
			{
				//m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 0.2f);

				switch (m_eNextLevelID)
				{
				case LEVEL_GAMEPLAY:
					
					m_pGameInstance->UIGroup_Render_OnOff(LEVEL_LOADING, TEXT("Layer_Loading"), false);
					m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading")), false);
					m_pGameInstance->Open_Level(m_eNextLevelID, CLevel_GamePlay::Create(m_pDevice, m_pContext));
					break;
				}

			}
		}

	}
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->SetUp_WindowText();

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_UIGroup_Loading(const _tchar* pLayerTag)
{
 	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Loading"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Mouse(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_MouseCursor"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_Fade(const _tchar* pLayerTag)
{
	CBlackScreen::BLACKSCREEN_DESC BlackScreenDesc = {};

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Black"), LEVEL_STATIC, pLayerTag, &BlackScreenDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID, _uint iLoadingNum, _bool bCheck)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

  	if (FAILED(pInstance->Initialize(eNextLevelID, iLoadingNum, bCheck)))
	{
		MSG_BOX("Failed To Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);

}
