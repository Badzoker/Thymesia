#include "pch.h"
#include "Circus_Balloon.h"
#include "Player.h"
#include "Locked_On.h"
#include "Monster_HP_Bar.h"
#include "GameInstance.h"

CCircus_Balloon::CCircus_Balloon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject(pDevice, pContext)
{
}

CCircus_Balloon::CCircus_Balloon(const CCircus_Balloon& Prototype)
    :CContainerObject(Prototype)
{
}

HRESULT CCircus_Balloon::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCircus_Balloon::Initialize(void* pArg)
{
    GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    pDesc->fScaling = _float3(0.1f, 0.1f, 0.1f);

    m_fHP_Bar_Height = 500.f;
    m_fMonsterMaxHP = 100.f;
    m_fMonsterCurHP = m_fMonsterMaxHP;
    m_fShieldHP = m_fMonsterMaxHP;

    m_fDistance = 500.f;

    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_Components(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_PartObjects(pDesc)))
        return E_FAIL;


    m_pNavigationCom->Set_CurCellIndex(m_pNavigationCom->Find_Closest_Cell(m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
    //m_iSpawn_Cell_Index = m_pNavigationCom->Get_CurCellIndex();
    m_Player_Attack = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower_Ptr();
    m_Player_Phase = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PhaseState_Ptr();
    m_Player_State = dynamic_cast<CPlayer*>(m_pPlayer)->Get_State_Ptr();


    m_pTransformCom->Scaling(_float3(0.0025f, 0.0025f, 0.0025f));
    //m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(90.f));

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.5f,0.5f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON | GROUP_TYPE::MONSTER;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);



    return S_OK;
}

void CCircus_Balloon::Priority_Update(_float fTimeDelta)
{
    if (m_fMonsterCurHP <= 0.f && !m_bDead)
    {
        m_bDead = true;
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
        m_pGameInstance->Add_DeadObjects(TEXT("Layer_Monster_Building"), this, m_pGameInstance->Get_Current_Level_Index());
        return;
    }
}

void CCircus_Balloon::Update(_float fTimeDelta)
{
    if (m_bDead)
        return;
    _vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 300.f,0.f,1.f });
}

void CCircus_Balloon::Late_Update(_float fTimeDelta)
{
    if (m_bDead)
        return;

    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_FOG, this);
}

HRESULT CCircus_Balloon::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pSecondModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pSecondModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pSecondModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        m_pShaderCom->Begin(0);
        m_pSecondModelCom->Render(i);
    }

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    iNumMeshes = m_pModelCom->Get_NumMeshes();
    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;
        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }
    return S_OK;
}

HRESULT CCircus_Balloon::Render_Fog_Front()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pFogShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pFogShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pFogShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    _float3 vBoxExtents = _float3(35.f, 1.5f, 35.f); // x 길이 : 70 y 길이 5 z 길이 : 70

    if (FAILED(m_pFogShaderCom->Bind_RawValue("g_vCubeExtents", &vBoxExtents, sizeof(_float3))))
        return E_FAIL;

    m_pFogShaderCom->Begin(0);

    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    m_pContext->Draw(1, 0); // 정점 버퍼가 없을때 최소한의 정보만 넣어주기
    return S_OK;
}

HRESULT CCircus_Balloon::Render_Fog_Back()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pFogShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pFogShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pFogShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    m_pFogShaderCom->Begin(1);

    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
    m_pContext->Draw(1, 0);

    return S_OK;
}

HRESULT CCircus_Balloon::Render_Fog_Final(ID3D11ShaderResourceView* pNoiseSRV)
{
    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_RangeFog_Front"), m_pFogShaderCom, "g_FrontTexture")))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_RangeFog_Back"), m_pFogShaderCom, "g_BackTexture")))
        return E_FAIL;


    //if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Shadow_Final"), m_pFogShaderCom, "g_FinalTexture")))
    //    return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pFogShaderCom, "g_DepthTexture")))
        return E_FAIL;

    //if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_RangeFog_Final"), m_pFogShaderCom, "g_PreviousFogTexture")))
    //    return E_FAIL;


    if (FAILED(m_pFogShaderCom->Bind_SRV("g_VolumeFogTexture", pNoiseSRV)))
        return E_FAIL;

    if (FAILED(m_pFogShaderCom->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pFogShaderCom->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pFogShaderCom->Bind_RawValue("g_vCamPos", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    _float3 vBoxExtents = _float3(35.f, 1.5f, 35.f);

    if (FAILED(m_pFogShaderCom->Bind_RawValue("g_vCubeExtents", &vBoxExtents, sizeof(_float3))))
        return E_FAIL;

    _float4 vCubeCenterPos;
    _vector vCubePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vCubePos.m128_f32[1] += vBoxExtents.y * 0.5f;
    XMStoreFloat4(&vCubeCenterPos, vCubePos);

    if (FAILED(m_pFogShaderCom->Bind_RawValue("g_vCubePos", &vCubeCenterPos, sizeof(_float4))))
        return E_FAIL;

    _float3 vFogColor = _float3(0.3f, 0.2f, 0.1019f);

    if (FAILED(m_pFogShaderCom->Bind_RawValue("g_vFogColor", &vFogColor, sizeof(_float3))))
        return E_FAIL;

    m_pFogShaderCom->Begin(2);

    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_pContext->IASetInputLayout(NULL);
    m_pContext->Draw(4, 0);

    return S_OK;
}

HRESULT CCircus_Balloon::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;

    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Building_Circus_Balloon"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Building_Circus_Balloon2"),
        TEXT("Com_Model2"), reinterpret_cast<CComponent**>(&m_pSecondModelCom))))
        return E_FAIL;

    /* Com_FogShader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Fog"),
        TEXT("Com_FogShader"), reinterpret_cast<CComponent**>(&m_pFogShaderCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CCircus_Balloon::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    CMonster_HP_Bar::Monster_HP_Bar_DESC Monster_HP_Bar_Desc = {};
    Monster_HP_Bar_Desc.pMonsterMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Monster_HP_Bar_Desc.fMaxHP = &m_fMonsterMaxHP;
    Monster_HP_Bar_Desc.fCurHP = &m_fMonsterCurHP;
    Monster_HP_Bar_Desc.fShieldHP = &m_fShieldHP;
    Monster_HP_Bar_Desc.bHP_Bar_Active = &m_bHP_Bar_Active;
    Monster_HP_Bar_Desc.bDead = &m_bDead;
    Monster_HP_Bar_Desc.fHeight = &m_fHP_Bar_Height;
    Monster_HP_Bar_Desc.fSpeedPerSec = 0.f;
    Monster_HP_Bar_Desc.fRotationPerSec = 0.f;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_HP_Bar"), iLevel, TEXT("Layer_MonsterHP"), &Monster_HP_Bar_Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CCircus_Balloon::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CCircus_Balloon::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) && m_fMonsterCurHP > 0.f)
    {
        m_bHP_Bar_Active = true;
        m_fHP_Bar_Active_Timer = 0.f;
        m_fMonsterCurHP -= *m_Player_Attack;
        m_fShieldHP -= (*m_Player_Attack);

    }
}

void CCircus_Balloon::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CCircus_Balloon::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CCircus_Balloon* CCircus_Balloon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCircus_Balloon* pInstance = new CCircus_Balloon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CCircus_Balloon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCircus_Balloon::Clone(void* pArg)
{
    CCircus_Balloon* pInstance = new CCircus_Balloon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CCircus_Balloon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCircus_Balloon::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pNavigationCom);

    Safe_Release(m_pSecondModelCom);

    Safe_Release(m_pFogShaderCom);
}
