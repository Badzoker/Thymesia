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

	if (FAILED(Ready_Components()))
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
		_float2 TextSize = m_pGameInstance->Get_TextSize(pUseFontName, pUseText.c_str());//텍스트 가로 세로 길이

		/*m_fTextPosition.x = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x - TextSize.x / 2;
		m_fTextPosition.y = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y - TextSize.y / 2;*/
		m_fTextPosition.x = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).x;
		m_fTextPosition.y = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION).y;

		////그림자
		//m_pGameInstance->Render_Font(pUseFontName, pUseText, { m_fTextPosition.x + 1.0f, m_fTextPosition.y + 1.0f }, Colors::Black);
		//m_pGameInstance->Render_Font(pUseFontName, pUseText, { m_fTextPosition.x - 1.0f, m_fTextPosition.y - 1.0f }, Colors::Black);

		m_pGameInstance->Render_Font(pUseFontName, pUseText.c_str(), m_fTextPosition, XMLoadFloat4(&m_fTextCloro));


		/*vector<UI_TextInfo>::iterator it;
		for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
		{*/
		
									// 사용 폰트 이름, 텍스트 내용, 그려질 좌표(출력되는 이미지 중점기준), 컬러, 회전 , 모르겠다, 스케일, 레이어뎁스, 이펙트 

		//m_pGameInstance->Render_Font(it->strFontName.c_str(), it->srtTextContent.c_str(), it->fTextStartPos);

		//}
	}
	return S_OK;
	
	
	/*if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTexNumber)))
		return E_FAIL;


	m_pShaderCom->Begin(m_iShaderPassNum);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();*/

	return S_OK;
}

HRESULT CUI_TextBox::Ready_Components()
{
	///* Com_Texture */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SceneBackground"),
	//	TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	///* Com_Shader */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
	//	TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
	//	return E_FAIL;

	///* Com_VIBuffer */
	//if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
	//	TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
	//	return E_FAIL;


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

	//Safe_Release(m_pShaderCom);
	//Safe_Release(m_pVIBufferCom);
	//Safe_Release(m_pTextureCom);
}
