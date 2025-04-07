#include "pch.h"
#include "Stand_Stack_Sword.h"
#include "GameInstance.h"

CStand_Stack_Sword::CStand_Stack_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CStand_Stack_Sword::CStand_Stack_Sword(const CStand_Stack_Sword& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CStand_Stack_Sword::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CStand_Stack_Sword::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");
    STAND_STACK_SWORD_DESC* pDesc = static_cast<STAND_STACK_SWORD_DESC*>(pArg);

    m_vPlayer_Position = pDesc->pPlayerPos;
    m_bNeed_Memory_Position = pDesc->bNeed_Memory_Position;
    m_bIs_Equipped_To_LeftHand = pDesc->bIs_Equipped_To_LeftHand;
    m_bIs_Stand_In_Ground = pDesc->bIs_Stand_In_Ground;
    m_bNeed_Fly_To_Player = pDesc->bNeed_Fly_To_Player;
    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentModelCom = pDesc->pParentModel;

    m_pLeft_SocketMatrix = m_pParentModelCom->Get_BoneMatrix("weapon_l");

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_pTransformCom->Scaling(_float3(0.5f, 0.5f, 0.5f));

    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
    m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(180.f));


    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.3f,0.3f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,100.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    return S_OK;
}

void CStand_Stack_Sword::Priority_Update(_float fTimeDelta)
{

}

void CStand_Stack_Sword::Update(_float fTimeDelta)
{
    Store_CombinedMatrix();
    //왼손에 있지도 땅에 있지도 날아갈 필요도 없으면 그냥 허리에 차있으면 돼
    if (!*m_bIs_Equipped_To_LeftHand && !*m_bIs_Stand_In_Ground && !*m_bNeed_Fly_To_Player)
    {
        _matrix			SocketMatrix = {};
        SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
        XMStoreFloat4x4(&m_CombinedWorldMatrix,
            XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *
            SocketMatrix *
            XMLoadFloat4x4(m_pParentWorldMatrix)
        );
    }
    else if (*m_bIs_Equipped_To_LeftHand)
    {
        //왼손에 있어야 할 타이밍이면 socket 왼손으로 바꿔
        _matrix			SocketMatrix = {};
        SocketMatrix = XMLoadFloat4x4(m_pLeft_SocketMatrix);
        XMStoreFloat4x4(&m_CombinedWorldMatrix,
            XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *
            SocketMatrix *
            XMLoadFloat4x4(m_pParentWorldMatrix)
        );
    }
    else if (*m_bIs_Stand_In_Ground)
    {
        //땅에 박혀 있어야할때 땅에 박혀있었을때의 컴바인드 넣어
        m_CombinedWorldMatrix = m_Store_CombinedMatrix;
    }
    else if (*m_bNeed_Fly_To_Player)
    {
        Fly_To_Player(fTimeDelta);
    }
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 0.f, 0.f,1.f });
}

void CStand_Stack_Sword::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CStand_Stack_Sword::Render()
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

HRESULT CStand_Stack_Sword::Ready_Components(void* pArg)
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Urd_Sword"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CStand_Stack_Sword::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CStand_Stack_Sword::Store_CombinedMatrix()
{
    if (*m_bNeed_Memory_Position)
    {
        *m_bNeed_Memory_Position = false;
        m_Store_CombinedMatrix = m_CombinedWorldMatrix;
    }
}

void CStand_Stack_Sword::Fly_To_Player(_float fTimeDelta)
{
    if (!m_bHasStartedFlying)
    {
        m_bHasStartedFlying = true;
        m_fSpeed = 10.f;
        XMStoreFloat4(&m_fStartPos, XMLoadFloat4x4(&m_CombinedWorldMatrix).r[3]);
        _float fPlayerY = XMVectorGetY(XMLoadFloat4(m_vPlayer_Position)) + 0.5f;
        XMStoreFloat4(m_vPlayer_Position, XMVectorSetY(XMLoadFloat4(m_vPlayer_Position), fPlayerY));
        m_fEndPos = *m_vPlayer_Position;
    }

    _vector vNewPos = XMVectorLerp(XMLoadFloat4(&m_fStartPos), XMLoadFloat4(&m_fEndPos), m_fLinearTime);
    m_fLinearTime += fTimeDelta * m_fSpeed;

    XMMATRIX matCombined = XMLoadFloat4x4(&m_CombinedWorldMatrix);
    matCombined.r[3] = XMVectorSet(XMVectorGetX(vNewPos), XMVectorGetY(vNewPos), XMVectorGetZ(vNewPos), 1.f);
    XMStoreFloat4x4(&m_CombinedWorldMatrix, matCombined);

    if (m_fLinearTime >= 1.f)
    {
        m_bHas_Finished_Flying = true;
        *m_bNeed_Fly_To_Player = false;
        *m_bNeed_Memory_Position = true;
        *m_bIs_Stand_In_Ground = true;

        Store_CombinedMatrix();
    }
}

void CStand_Stack_Sword::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
}

void CStand_Stack_Sword::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CStand_Stack_Sword::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CStand_Stack_Sword* CStand_Stack_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CStand_Stack_Sword* pInstance = new CStand_Stack_Sword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CProjectile_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CStand_Stack_Sword::Clone(void* pArg)
{
    CStand_Stack_Sword* pInstance = new CStand_Stack_Sword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CProjectile_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStand_Stack_Sword::Free()
{
    __super::Free();


    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
