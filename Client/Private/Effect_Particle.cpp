#include "pch.h"
#include "Effect_Particle.h"
#include "GameInstance.h"

CEffect_Particle::CEffect_Particle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CEffect{ _pDevice, _pContext }
{
}

CEffect_Particle::CEffect_Particle(const CEffect_Particle& _Prototype)
    : CEffect(_Prototype)
{
}

HRESULT CEffect_Particle::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Particle::Initialize(void* _pArg)
{
    EFFECT_PARTICLE_DESC* pDesc = static_cast<EFFECT_PARTICLE_DESC*>(_pArg);

    m_iDiffuse = pDesc->iDiffuse;
    m_iParticle_Count = pDesc->iParticle_Count;
    m_fMaxTimer = pDesc->fMaxTimer;
    m_vRGB = pDesc->vRGB;
    m_iShaderPass = pDesc->iShaderPass;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    /* Com_Shader*/
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, pDesc->szShaderName,
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, pDesc->szBufferName,
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pBufferCom))))
        return E_FAIL;

    m_pTransformCom->Scaling(pDesc->vScale);
    m_pTransformCom->Rotation(XMConvertToRadians(pDesc->vRot.x), XMConvertToRadians(pDesc->vRot.y), XMConvertToRadians(pDesc->vRot.z)); //이부분은 Tool이랑 뭔가 이상함
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(XMLoadFloat3(&pDesc->vTranslation), 1.f));


    return S_OK;
}

void CEffect_Particle::Priority_Update(_float _fTimeDelta)
{
}

void CEffect_Particle::Update(_float _fTimeDelta)
{
    m_pBufferCom->Compute_Shader(m_pShaderCom, 2, 1, 1);

    __super::Update(_fTimeDelta);
}

void CEffect_Particle::Late_Update(_float _fTimeDelta)
{
    switch (m_iShaderPass)
    {
    case 0:
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
        break;

    case 1:
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_WEIGHTBLEND, this);
        break;

    case 2:
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
        break;

    case 3:
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
        break;

    case 4:
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this); //아마 Bloom
        break;
    }
}

HRESULT CEffect_Particle::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (3 == m_iShaderPass) //BLOOD
    {
        if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
            return E_FAIL;
    }


    m_pShaderCom->Begin(m_iShaderPass);

    m_pBufferCom->Bind_InputAssembler();

    m_pBufferCom->Render();

    return S_OK;
}

HRESULT CEffect_Particle::Render_WeightBlend()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    
    m_pShaderCom->Begin(m_iShaderPass); //WeightBlend

    m_pBufferCom->Bind_InputAssembler();

    m_pBufferCom->Render();

    return S_OK;
}

void CEffect_Particle::Set_IsPlaying(_bool _bIsPlaying)
{
    m_bIsPlaying = _bIsPlaying;
    if (true == _bIsPlaying) //시작해라 느낌
    {
        m_fTimerX = 0.f;
        m_fTimerY = 0.f;
        m_fDissolve = 0.f;
        m_fTimer_Timelag = 0.f;
        m_pBufferCom->Compute_Shader_Reset(m_pShaderCom, 2, 1, 1);
    }
}

HRESULT CEffect_Particle::Ready_Components()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Particle_Image"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffect_Particle::Bind_ShaderResources()
{
    //if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
    //    return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_matCombined)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iDiffuse)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vRGB", &m_vRGB, sizeof(_float3))))
        return E_FAIL;

    return S_OK;
}

CEffect_Particle* CEffect_Particle::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CEffect_Particle* pInstance = new CEffect_Particle(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed Create : Effect_Particle_Prototype");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CEffect_Particle::Clone(void* _pArg)
{
    CEffect_Particle* pInstance = new CEffect_Particle(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed Clone : Effect_Particle");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CEffect_Particle::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pBufferCom);
}
