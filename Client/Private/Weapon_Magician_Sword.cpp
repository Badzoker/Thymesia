#include "pch.h"
#include "Weapon_Magician_Sword.h"
#include "GameInstance.h"
#include "Boss_Magician.h"
#include "Animation.h"

CWeapon_Magician_Sword::CWeapon_Magician_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
    for (_uint i = COLLIDER_SWORD; i < COLLIDER_END; i++)
    {
        ZeroMemory(&m_pActor[i], sizeof(PxRigidDynamic));
    }
}

CWeapon_Magician_Sword::CWeapon_Magician_Sword(const CWeapon_Magician_Sword& Prototype)
    :CPartObject(Prototype)
{
    for (_uint i = COLLIDER_SWORD; i < COLLIDER_END; i++)
    {
        ZeroMemory(&m_pActor[i], sizeof(PxRigidDynamic));
    }
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

    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_IsDissolveOn = pDesc->IsDissolveOn;
    m_IsDissolveOff = pDesc->IsDissolveOff;
    m_iMonster_Attack = pDesc->iAttack;
    m_bCane_Collider_On = pDesc->bCane_Collider_On;
    m_Is_Change_Sword_Bone = pDesc->Is_Change_Sword_Bone;
    m_bSpecial_Skill_Progress = pDesc->bSpecial_Skill_Progress;
    m_pSocket_Right_Matrix = pDesc->pSocketMatrix;
    m_bCatch_Special_Attack = pDesc->bCatch_Special_Attack;

    m_pSocket_Left_Matrix = m_pParentModelCom->Get_BoneMatrix("weapon_l_Sword");
    m_pSocket_Leg_Matrix = m_pParentModelCom->Get_BoneMatrix("calf_r");


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(_float3{ 0.5f, 0.5f, 0.5f });
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
    m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(180.f));

    m_pActor[COLLIDER_SWORD] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.4f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_KICK] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.4f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_SLASH] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 0.4f,0.8f,10.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_SPECIAL] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);
    m_pActor[COLLIDER_PARRY] = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 2.f,2.f,2.f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_SWORD], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_KICK], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_SLASH], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_SPECIAL], _fvector{ 0.f,0.f,100.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pActor[COLLIDER_PARRY], _fvector{ 0.f,0.f,100.f,1.f });
    //막는 것들
    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_SWORD], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_KICK], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    //못막는 것들
    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_SLASH], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_SPECIAL], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    m_pGameInstance->Set_CollisionGroup(m_pActor[COLLIDER_PARRY], GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);
    return S_OK;
}

void CWeapon_Magician_Sword::Priority_Update(_float fTimeDelta)
{
    if (*m_IsDissolveOn)
    {
        m_fDissolveOn_Timer += fTimeDelta * 3.f;
        m_fDissolveOn_FinishTime += fTimeDelta * 3.f;
    }
    else
    {
        m_fDissolveOn_Timer = 0.f;
        m_fDissolveOn_FinishTime = 0.f;
    }

    if (*m_IsDissolveOff)
    {
        m_fDissolveOff_Timer -= fTimeDelta * 3.f;
        m_fDissolveOff_FinishTime -= fTimeDelta * 3.f;
        if (m_fDissolveOff_Timer <= 0.f)
        {
            m_fDissolveOff_Timer = 0.f;
            m_fDissolveOff_FinishTime = 0.f;
        }
    }
    else
    {
        m_fDissolveOff_Timer = 1.f;
        m_fDissolveOff_FinishTime = 1.f;
    }
}

void CWeapon_Magician_Sword::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = {};

    if (!(*m_Is_Change_Sword_Bone))
        SocketMatrix = XMLoadFloat4x4(m_pSocket_Right_Matrix);
    else
        SocketMatrix = XMLoadFloat4x4(m_pSocket_Left_Matrix);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_SWORD])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_SWORD], XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ -100.f, 0.f, 0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_KICK])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_KICK], XMLoadFloat4x4(m_pSocket_Leg_Matrix) * XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 50.f, 0.f,0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_SLASH])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_SLASH], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0.f, 500.f, 4000.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_SPECIAL])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_SPECIAL], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0.f, 500.f, 0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor[COLLIDER_PARRY])))
        m_pGameInstance->Update_Collider(m_pActor[COLLIDER_PARRY], XMLoadFloat4x4(m_pParentWorldMatrix), _vector{ 0.f, 1000.f, 0.f,1.f });

    if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD && !*m_bCane_Collider_On)
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                //내가 넣은 콜라이더 시간에 진입했을때
                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate)
                {
                    if (*m_pParentState == STATE_SPECIAL_ATTACK)
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_SPECIAL]);
                    else if (*m_pParentState == STATE_SPECIAL_ATTACK2)
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_SLASH]);
                    else if (*m_pParentState == STATE_KICK_ATTACK)
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_KICK]);
                    else if (*m_pParentState == STATE_PARRY)
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_PARRY]);
                    else
                        m_pGameInstance->Add_Actor_Scene(m_pActor[COLLIDER_SWORD]);

                    iter.isPlay = true;
                }
            }
            else
            {
                //내가 넣은 콜라이더 시간이 끝났을때나 플레이어한테 닿아서 데미지를 입혔을경우. 콜라이더를 꺼라.
                if ((iter.eType == EVENT_COLLIDER && !iter.isEventActivate) || m_bColliderOff)
                {
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SWORD]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SLASH]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_KICK]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SPECIAL]);
                    m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_PARRY]);

                    m_bColliderOff = false;
                    if (!iter.isEventActivate)
                    {
                        iter.isPlay = false;
                    }
                }
            }
            //문제 iter가 2개니까 첫 iter는 add를 했는데 다음 iter가 자기 프레임 타이밍이 아니니 Sub를 해버린다.
            //다단히트 해결하기 = 충돌나면 bool 값이 ColliderOff-> true가 되는데 이때 sub하면되는디 다시 추가되는느낌?
        }
    }
    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SWORD]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SLASH]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_KICK]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_SPECIAL]);
        m_pGameInstance->Sub_Actor_Scene(m_pActor[COLLIDER_PARRY]);
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

        if (*m_IsDissolveOn)
        {
            m_iPassNum = 9;
            if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDissolveOn_Timer, sizeof(_float))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveOn_FinishTime, sizeof(_float))))
                return E_FAIL;
        }
        else if (*m_IsDissolveOff)
        {
            m_iPassNum = 9;
            if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDissolveOff_Timer, sizeof(_float))))
                return E_FAIL;

            if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fDissolveOff_FinishTime, sizeof(_float))))
                return E_FAIL;
        }
        else
            m_iPassNum = 0;

        m_pShaderCom->Begin(m_iPassNum);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CWeapon_Magician_Sword::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boss_Magician_Sword"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),
        TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
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
    if (!strcmp("PLAYER", _pOther->Get_Name()))
    {
        m_bColliderOff = true;
    }
    if (!strcmp("PLAYER", _pOther->Get_Name()) && *m_bSpecial_Skill_Progress)
        *m_bCatch_Special_Attack = true;
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
    Safe_Release(m_pTextureCom);
}
