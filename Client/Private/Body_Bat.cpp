#include "pch.h"
#include "Body_Bat.h"
#include "GameInstance.h"
#include "Animation.h"

CBody_Bat::CBody_Bat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CBody_Bat::CBody_Bat(const CBody_Bat& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CBody_Bat::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Bat::Initialize(void* pArg)
{
	CBody_Bat::BODY_BAT_DESC* pDesc = static_cast<CBody_Bat::BODY_BAT_DESC*>(pArg);


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->SetUp_Animation(20, true);

	return S_OK;
}

void CBody_Bat::Priority_Update(_float fTimeDelta)
{
}

void CBody_Bat::Update(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);

	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));
}

void CBody_Bat::Late_Update(_float fTimeDelta)
{
	//XMVectorCatmullRom()
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CBody_Bat::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_iPassNum = 0;
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;
		m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);


		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네
			return E_FAIL;


		m_pShaderCom->Begin(m_iPassNum);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CBody_Bat::Render_Shadow()
{
	return S_OK;
}

HRESULT CBody_Bat::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Bat_Body"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Bat::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CBody_Bat* CBody_Bat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Bat* pInstance = new CBody_Bat(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBody_Bat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Bat::Clone(void* pArg)
{
	CBody_Bat* pInstance = new CBody_Bat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBody_Bat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Bat::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
