#include "pch.h" 
#include "UI_InteractableIndicator.h"
#include "GameInstance.h"

CUI_InteractableIndicator::CUI_InteractableIndicator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI_Image{ pDevice, pContext }
{
}

CUI_InteractableIndicator::CUI_InteractableIndicator(const CUI_InteractableIndicator& Prototype)
	: CUI_Image(Prototype)
{
}

HRESULT CUI_InteractableIndicator::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InteractableIndicator::Initialize(void* pArg)
{
	InterAct_Desc* pDesc = static_cast<InterAct_Desc*>(pArg);
	pDesc->fSizeX = 50.f;
	pDesc->fSizeY = 50.f;
	pDesc->fX = 100.f;
	pDesc->fY = 100.f;
	pDesc->fZ = 0.0f;
	pDesc->fNear = 0.0f;
	pDesc->fFar = 1.0f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CUI_InteractableIndicator::Priority_Update(_float fTimeDelta)
{
}

void CUI_InteractableIndicator::Update(_float fTimeDelta)
{
}

void CUI_InteractableIndicator::Late_Update(_float fTimeDelta)
{
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_UI, this);
}

HRESULT CUI_InteractableIndicator::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;


	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Bind_InputAssembler();

	m_pVIBufferCom->Render();


// 텍스트 출력
	m_strFontName = TEXT("Font_NotoSansKR18"); // 사용하는 폰트 정보
	m_strContentText = TEXT("E"); // 출력할 텍스트 내용
	_float3 fMyPos = m_pTransformCom->Get_State_UIObj(CTransform::STATE_POSITION); // 뷰포트 기준 X,Y,Z 위치 값을 가져온다

	_float2 TextSize = m_pGameInstance->Get_TextSize(m_strFontName, m_strContentText.c_str());//텍스트 가로 세로 길이
	// 객체 중점 기준 텍스트의 중점을 맞춰 출력되도록 설정
	m_fTextPosition.x = (fMyPos.x - TextSize.x / 2) - 3.f; // 묘하게 중점이 맞지 않아 임의로 상수값 넣어 조정
	m_fTextPosition.y = (fMyPos.y - TextSize.y / 2) - 1.f; // 묘하게 중점이 맞지 않아 임의로 상수값 넣어 조정
	m_fTextPosition.z = fMyPos.z;
	m_pGameInstance->Render_World(m_strFontName, m_strContentText.c_str(), { m_fTextPosition.x,m_fTextPosition.y }, { 1.f,1.f,1.f,1.f }, 0.0f, { 0.0f,0.0f }, 1.0f); 


	return S_OK;
}

HRESULT CUI_InteractableIndicator::Ready_Components()
{
	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_InteractableIndicator"),
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

CUI_InteractableIndicator* CUI_InteractableIndicator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InteractableIndicator* pInstance = new CUI_InteractableIndicator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUI_InteractableIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InteractableIndicator::Clone(void* pArg)
{
	CUI_InteractableIndicator* pInstance = new CUI_InteractableIndicator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUI_InteractableIndicator");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InteractableIndicator::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
