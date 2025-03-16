#include "pch.h"
#include "UIGroup_Loading.h"
#include "GameInstance.h"
#include "Level_Loading.h"

#include "UI_Scene.h"
#include "UI_Text.h"
#include "UI_TextBox.h"
#include "UI_Button.h"
#include "UI_Image.h"
#include "UI_LoadingScreen.h"


#include "UI_ButtonHighlight.h"

CUIGroup_Loading::CUIGroup_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_Loading::CUIGroup_Loading(const CUIGroup_Loading& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUIGroup_Loading::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_Loading::Initialize(void* pArg)
{
	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	m_pMyScene = m_pGameInstance->Find_UIScene(UISCENE_LOADING, L"UIScene_Loading");

	
	return S_OK;
}

void CUIGroup_Loading::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	if (m_bRenderOpen)
	{
		for (auto& Image : m_pMyScene->Find_UI_Image())
		{
			if (0 == Image->Get_UI_GroupID())
			{
				m_iTexNumber =  dynamic_cast<CUI_LoadingScreen*>(Image)->Get_ImageTex();
			}


		}
	}
}

void CUIGroup_Loading::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_bRenderOpen)
	{
		Texture_Num_Check();
	}
}

void CUIGroup_Loading::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUIGroup_Loading::Render()
{
	
	return S_OK;
}

void CUIGroup_Loading::Texture_Num_Check()
{
	for (auto& TextBox : m_pMyScene->Find_UI_TextBox())
	{
		if (1 == TextBox->Get_UI_GroupID()) // 제목
		{
			for (auto& Text : m_TextInfo)
			{
				if (m_iTexNumber == Text.iTextID)
				{
					TextBox->Set_FontName(Text.strFontName.c_str());
					TextBox->Set_Content(Text.srtTextContent.c_str());
					break;
				}
				else
				{
					TextBox->Set_FontName(L"");
					TextBox->Set_Content(L"");
				}
			}
		}

		if (10 == TextBox->Get_UI_GroupID()) // 내용
		{
			for (auto& Text : m_TextInfo)
			{
				if ((m_iTexNumber + 10) == Text.iTextID)
				{
					TextBox->Set_FontName(Text.strFontName.c_str());
					TextBox->Set_Content(Text.srtTextContent.c_str());
					break;
				}
				else
				{
					TextBox->Set_FontName(L"");
					TextBox->Set_Content(L"");
				}
			}
		}
	}
}

HRESULT CUIGroup_Loading::Ready_UIObject()
{
	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_STATIC, UISCENE_LOADING, L"UIScene_Loading");
	m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_Loading", m_TextInfo);
	return S_OK;
}

CUIGroup_Loading* CUIGroup_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_Loading* pInstance = new CUIGroup_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_Loading::Clone(void* pArg)
{
	CUIGroup_Loading* pInstance = new CUIGroup_Loading(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_Loading::Free()
{
	__super::Free();
}
