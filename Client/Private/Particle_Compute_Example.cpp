#include "pch.h"
#include "Particle_Compute_Example.h"
#include "GameInstance.h"

CParticle_Compute_Example::CParticle_Compute_Example(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{ pDevice, pContext }
{
}

CParticle_Compute_Example::CParticle_Compute_Example(const CParticle_Compute_Example& Prototype)
    : CGameObject(Prototype)
{
}

HRESULT CParticle_Compute_Example::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CParticle_Compute_Example::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    // 시작 지점의 플레이어 위치 1_23일 
    _float fRandomY = m_pGameInstance->Compute_Random(-300.f, 300.f);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 112.6f + fRandomY, fRandomY, 107.1f, 1.f });

    return S_OK;
}

void CParticle_Compute_Example::Priority_Update(_float fTimeDelta)
{
}

void CParticle_Compute_Example::Update(_float fTimeDelta)
{
    m_pBufferCom->Compute_Shader(m_pShaderCom, 2, 1, 1);
}

void CParticle_Compute_Example::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

HRESULT CParticle_Compute_Example::Render()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pBufferCom->Bind_InputAssembler();

    m_pBufferCom->Render();

    return S_OK;
}

HRESULT CParticle_Compute_Example::Ready_Components()
{
    /* Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Drop"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Particle_Example"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Compute"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pBufferCom))))
        return E_FAIL;

    return S_OK;
}

CParticle_Compute_Example* CParticle_Compute_Example::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CParticle_Compute_Example* pInstance = new CParticle_Compute_Example(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed Create : Particle_Compute_Example_Prototype");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CParticle_Compute_Example::Clone(void* pArg)
{
    CParticle_Compute_Example* pInstance = new CParticle_Compute_Example(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed Clone : Particle_Compute_Example");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CParticle_Compute_Example::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pBufferCom);
}
