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

	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_GAMEPLAY, UISCENE_TALENT, L"UIScene_PlayerTalent");
	//m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_PlayerTalent", m_TextInfo);

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
