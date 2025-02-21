#include "pch.h"
#include "UI_AttributeButton.h"
#include "GameInstance.h"

CUI_AttributeButton::CUI_AttributeButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Button_Player{ pDevice, pContext }
{
}

CUI_AttributeButton::CUI_AttributeButton(const CUI_AttributeButton& Prototype)
	: CUI_Button_Player(Prototype)
{
}

HRESULT CUI_AttributeButton::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_AttributeButton::Initialize(void* pArg)
{
	BUTTON_DESC* pDesc = static_cast<BUTTON_DESC*>(pArg);

	pDesc->fNear = 0.f;
	pDesc->fFar = 1.f;

	pDesc->fSpeedPerSec = 5.f;
	pDesc->fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_AttributeButton::Priority_Update(_float fTimeDelta)
{
}

void CUI_AttributeButton::Update(_float fTimeDelta)
{
	__super::On_Mouse_UI(g_hWnd, m_fPos.x, m_fPos.y, m_iTexNumber);
}

void CUI_AttributeButton::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_AttributeButton::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;


	m_pShaderCom->Begin(m_iShaderPassNum);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_AttributeButton::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Slot_Attribute"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	return S_OK;
}

CUI_AttributeButton* CUI_AttributeButton::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_AttributeButton* pInstance = new CUI_AttributeButton(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_AttributeButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_AttributeButton::Clone(void* pArg)
{
	CUI_AttributeButton* pInstance = new CUI_AttributeButton(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_AttributeButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_AttributeButton::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
