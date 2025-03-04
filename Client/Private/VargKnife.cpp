#include "pch.h"
#include "VargKnife.h"
#include "GameInstance.h"

CVargKnife::CVargKnife(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CVargKnife::CVargKnife(const CVargKnife& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CVargKnife::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CVargKnife::Initialize(void* pArg)
{
    strcpy_s(m_szName, "VARG_KNIFE");

    VARGKNIFE_DESC* pDesc = static_cast<VARGKNIFE_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_bCollider_ON_OFF = pDesc->bCollider_ON_OFF;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER_WEAPON, this);

    m_pTransformCom->Scaling(_float3{ 0.5f, 0.5f, 0.5f });


    return S_OK;
}

void CVargKnife::Priority_Update(_float fTimeDelta)
{
    if (*m_bCollider_ON_OFF)
    {
        m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER_WEAPON, this);
    }
    else
    {
        m_pGameInstance->Sub_ObjCollider(GROUP_TYPE::MONSTER_WEAPON, this);
    }
}

void CVargKnife::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );
    for (auto pCollider : m_pColliderCom)
        pCollider->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));
}

void CVargKnife::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CVargKnife::Render()
{
#ifdef _DEBUG
    if (*m_bCollider_ON_OFF)
    {
        for (auto pCollider : m_pColliderCom)
            pCollider->Render();
    }
#endif 
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CVargKnife::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Varg_Knife"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Com_Collider */
    for (size_t i = 0; i < 6; i++)
    {
        CCollider* pCollider = { nullptr };

        CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};

        SphereDesc.fRadius = 50.f;
        SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius - 50.f, -100.f - (i * 100.f));

        wstring strName = TEXT("Com_Collider") + to_wstring(i);

        if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
            strName.c_str(), reinterpret_cast<CComponent**>(&pCollider), &SphereDesc)))
            return E_FAIL;

        pCollider->Set_Collider_Name("Monster_Weapon");

        m_pColliderCom.push_back(pCollider);
    }

    return S_OK;
}

HRESULT CVargKnife::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CVargKnife::OnCollisionEnter(CGameObject* _pOther)
{
}

void CVargKnife::OnCollision(CGameObject* _pOther)
{
}

void CVargKnife::OnCollisionExit(CGameObject* _pOther)
{
}

CVargKnife* CVargKnife::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVargKnife* pInstance = new CVargKnife(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CVargKnife");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVargKnife::Clone(void* pArg)
{
    CVargKnife* pInstance = new CVargKnife(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CVargKnife");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVargKnife::Free()
{
    __super::Free();

    for (auto pCollider : m_pColliderCom)
        Safe_Release(pCollider);

    m_pColliderCom.clear();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
