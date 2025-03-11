#include "pch.h" 
#include "UI_Effect_SlotFrame.h"
#include "GameInstance.h"

CUI_Effect_SlotFrame::CUI_Effect_SlotFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Image{ pDevice, pContext }
{
}

CUI_Effect_SlotFrame::CUI_Effect_SlotFrame(const CUI_Effect_SlotFrame& Prototype)
	: CUI_Image(Prototype)
{
}

HRESULT CUI_Effect_SlotFrame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Effect_SlotFrame::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_Effect_SlotFrame::Priority_Update(_float fTimeDelta)
{
}

void CUI_Effect_SlotFrame::Update(_float fTimeDelta)
{
}

void CUI_Effect_SlotFrame::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_Effect_SlotFrame::Render()
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

HRESULT CUI_Effect_SlotFrame::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Effect_SlotFrame"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	return S_OK;
}

CUI_Effect_SlotFrame* CUI_Effect_SlotFrame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Effect_SlotFrame* pInstance = new CUI_Effect_SlotFrame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_Effect_SlotFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Effect_SlotFrame::Clone(void* pArg)
{
	CUI_Effect_SlotFrame* pInstance = new CUI_Effect_SlotFrame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_Effect_SlotFrame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Effect_SlotFrame::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
