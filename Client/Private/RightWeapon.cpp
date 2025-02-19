#include "pch.h" 
#include "RightWeapon.h"
#include "GameInstance.h"
#include "Player.h"

CRightWeapon::CRightWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CRightWeapon::CRightWeapon(const CRightWeapon& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CRightWeapon::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CRightWeapon::Initialize(void* pArg)
{

    strcpy_s(m_szName, "PLAYER_RIGHT_WEAPON");

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    //m_pTransformCom->Scaling(_float3(0.1f, 0.1f, 0.1f));
    //m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));
    m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));

    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.8f, 0.f, 0.f, 1.f));

    //m_pSwordEffect = CSwordEffect::Create(m_pDevice, m_pContext);
    //if (m_pSwordEffect == nullptr)  
    //    return E_FAIL;  

    return S_OK;

}

void CRightWeapon::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

}

void CRightWeapon::Update(_float fTimeDelta)
{

    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    //for (size_t i = 0; i < 3; i++)  
    //    SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);  

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );

    m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));

}

void CRightWeapon::Late_Update(_float fTimeDelta)
{

    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

}

HRESULT CRightWeapon::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
    //if(*m_pParentState & (CPlayer::STATE_ATTACK | CPlayer::STATE_ATTACK2) && m_AccColliderLife < 0.4f)
    //    m_pColliderCom->Render();
    //if ((*m_pParentState & CPlayer::STATE_ATTACK) && m_AccColliderLifeAttack1 < 0.4f || (*m_pParentState & CPlayer::STATE_ATTACK2) && m_AccColliderLifeAttack2 < 0.2f)  
    //    m_pColliderCom->Render();       
#endif // DEBUG 


    return S_OK;
}

HRESULT CRightWeapon::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Right_Weapon"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


    SphereDesc.fRadius = 25.f;
    SphereDesc.vCenter = _float3(50.f, 0.f, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
        return E_FAIL;


    m_pColliderCom->Set_Collider_Name("PLAYER_RIGHT_WEAPON");
    //m_pGameInstance->Add_ObjCollider(GROUP_TYPE::PLAYER_WEAPON, this);      


    return S_OK;
}

HRESULT CRightWeapon::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CRightWeapon::OnCollisionEnter(CGameObject* _pOther)
{


}

void CRightWeapon::OnCollision(CGameObject* _pOther)
{

}

void CRightWeapon::OnCollisionExit(CGameObject* _pOther)
{

}

CRightWeapon* CRightWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRightWeapon* pInstance = new CRightWeapon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CRightWeapon::Clone(void* pArg)
{
    CRightWeapon* pInstance = new CRightWeapon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CRightWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRightWeapon::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
