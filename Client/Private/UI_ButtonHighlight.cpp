#include "pch.h" 
#include "UI_ButtonHighlight.h"
#include "GameInstance.h"

CUI_ButtonHighlight::CUI_ButtonHighlight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI_Button{ pDevice, pContext }
{
}

CUI_ButtonHighlight::CUI_ButtonHighlight(const CUI_ButtonHighlight& Prototype)
    : CUI_Button(Prototype)
{
}

HRESULT CUI_ButtonHighlight::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_ButtonHighlight::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_ButtonHighlight::Priority_Update(_float fTimeDelta)
{
}

void CUI_ButtonHighlight::Update(_float fTimeDelta)
{
	if (m_bRenderOpen) // UI 가 보여지고 있을 때에만 기능 작동
	{
		if (__super::On_Mouse_UI(g_hWnd))
			m_bImageOn = true;
		else
			m_bImageOn = false;

		if (m_bImageOn)
		{
			if (__super::Mouse_Select(g_hWnd))
			{
				m_bMouseSelected = true; // 최초에 마우스 클릭이 있는지 체크
			}
		}
	}
}

void CUI_ButtonHighlight::Late_Update(_float fTimeDelta)
{
	if (m_bRenderOpen)
	{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
	}
}

HRESULT CUI_ButtonHighlight::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_bImageOn", &m_bImageOn,sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;


	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_ButtonHighlight::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ButtonHighlight"),
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

CUI_ButtonHighlight* CUI_ButtonHighlight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ButtonHighlight* pInstance = new CUI_ButtonHighlight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_ButtonHighlight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ButtonHighlight::Clone(void* pArg)
{
	CUI_ButtonHighlight* pInstance = new CUI_ButtonHighlight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_ButtonHighlight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ButtonHighlight::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
