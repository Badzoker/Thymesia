#include "pch.h"
#include "Weapon_Magician_Sword.h"
#include "GameInstance.h"
#include "Boss_Magician.h"
#include "Animation.h"

CWeapon_Magician_Sword::CWeapon_Magician_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CWeapon_Magician_Sword::CWeapon_Magician_Sword(const CWeapon_Magician_Sword& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CWeapon_Magician_Sword::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Magician_Sword::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");

    MAGICIAN_SWORD_DESC* pDesc = static_cast<MAGICIAN_SWORD_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pTransformCom->Scaling(_float3{ 0.5f, 0.5f, 0.5f });
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
    m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(180.f));
    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.8f,0.15f }, _float3{ 0.f,1.f,0.f }, 90.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,100.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    return S_OK;
}

void CWeapon_Magician_Sword::Priority_Update(_float fTimeDelta)
{
}

void CWeapon_Magician_Sword::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
    {
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 100.f, 0.f,-350.f,1.f });
    }

}

void CWeapon_Magician_Sword::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CWeapon_Magician_Sword::Render()
{
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

HRESULT CWeapon_Magician_Sword::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Magician_Sword"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CWeapon_Magician_Sword::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CWeapon_Magician_Sword::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
}

void CWeapon_Magician_Sword::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CWeapon_Magician_Sword::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CWeapon_Magician_Sword* CWeapon_Magician_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Magician_Sword* pInstance = new CWeapon_Magician_Sword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWeapon_Magician_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon_Magician_Sword::Clone(void* pArg)
{
    CWeapon_Magician_Sword* pInstance = new CWeapon_Magician_Sword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CWeapon_Magician_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon_Magician_Sword::Free()
{
    __super::Free();


    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
