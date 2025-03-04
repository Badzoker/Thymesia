#include "pch.h" 
#include "UI_TextBox.h"
#include "GameInstance.h"

CUI_TextBox::CUI_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Text{ pDevice, pContext }
{
}

CUI_TextBox::CUI_TextBox(const CUI_TextBox& Prototype)
	: CUI_Text(Prototype)
{
}

HRESULT CUI_TextBox::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_TextBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_TextBox::Priority_Update(_float fTimeDelta)
{
}

void CUI_TextBox::Update(_float fTimeDelta)
{
}

void CUI_TextBox::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
	}
}

HRESULT CUI_TextBox::Render()
{
	if (m_bRenderOpen)
	{
		_float2 TextSize = m_pGameInstance->Get_TextSize(m_strFontName, m_strContentText.c_str());//텍스트 가로 세로 길이

		m_fTextPosition.x = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x;
		m_fTextPosition.y = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y;

		switch (m_eRenderType)
		{
		case Client::CUI_TextBox::FONT_DEFALUT:
			m_pGameInstance->Render_Font(m_strFontName, m_strContentText.c_str(), m_fTextPosition, m_fTextColor);
			break;
		case Client::CUI_TextBox::FONT_SHADOW:
			m_pGameInstance->Render_Shadow(m_strFontName, m_strContentText.c_str(), m_fTextPosition, m_fTextColor);
			break;
		case Client::CUI_TextBox::FONT_OUTLINE:
			m_pGameInstance->Render_Outline(m_strFontName, m_strContentText.c_str(), m_fTextPosition, m_fTextColor);
			break;
		}

	}
	return S_OK;
}

CUI_TextBox* CUI_TextBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_TextBox* pInstance = new CUI_TextBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_TextBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_TextBox::Clone(void* pArg)
{
	CUI_TextBox* pInstance = new CUI_TextBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_TextBox");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_TextBox::Free()
{
	__super::Free();

}
