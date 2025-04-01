#include "pch.h"
#include "Normal_ScytheM.h"
#include "Body_ScytheM.h"
#include "Decorative_Hat.h"
#include "Weapon_Monster_Scythe.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Monster_HP_Bar.h"
#include "Locked_On.h"
#include "Player.h"

CNormal_ScytheM::CNormal_ScytheM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster(pDevice, pContext)
{
}

CNormal_ScytheM::CNormal_ScytheM(const CNormal_ScytheM& Prototype)
    :CMonster(Prototype)
{
}

HRESULT CNormal_ScytheM::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNormal_ScytheM::Initialize(void* pArg)
{
    m_fHP_Bar_Height = 500.f;
    m_fRootDistance = 1.f;
    m_fSpawn_Distance_Max = 10.f;
    m_fActive_Distance = 5.f;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    if (FAILED(Ready_PartObjects(pArg)))
        return E_FAIL;

    m_pNavigationCom->Set_CurrentNaviIndex(XMLoadFloat4(&m_vSpawnPoint));
    m_iSpawn_Cell_Index = m_pNavigationCom->Get_CurCellIndex();
    m_Player_Attack = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower_Ptr();
    m_Player_Phase = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PhaseState_Ptr();
    m_Player_State = dynamic_cast<CPlayer*>(m_pPlayer)->Get_State_Ptr();

    m_pState_Manager = CState_Machine<CNormal_ScytheM>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    m_pStunActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,0.f,1.f }, 0.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON | GROUP_TYPE::MONSTER;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);
    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pStunActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pStunActor, _fvector{ 0.f,22.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    return S_OK;

}

void CNormal_ScytheM::Priority_Update(_float fTimeDelta)
{
    if (*m_Player_Phase & CPlayer::PHASE_DEAD)
        m_Is_Player_Dead = true;
    else
        m_Is_Player_Dead = false;

    __super::Priority_Update(fTimeDelta);
}

void CNormal_ScytheM::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
        m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CNormal_ScytheM::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CNormal_ScytheM::Render()
{
    return S_OK;
}

HRESULT CNormal_ScytheM::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;

    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CNormal_ScytheM::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    CBody_ScytheM::BODY_SCYTHEM_DESC BodyDesc = {};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pParentState = &m_iMonster_State;
    BodyDesc.bDead = &m_bDead;
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_ScytheM"), LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_ScytheM_Body"), &BodyDesc)))
        return E_FAIL;

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_ScytheM"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
        return E_FAIL;

    CWeapon_Monster_Scythe::WEAPON_Monster_Scythe_DESC		Weapon_Desc = {};
    Weapon_Desc.pParent = this;
    Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
    Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Weapon_Desc.pParentModel = m_pModelCom;
    Weapon_Desc.pParentState = &m_iMonster_State;
    Weapon_Desc.iAttack = &m_iMonster_Attack_Power;
    Weapon_Desc.fSpeedPerSec = 0.f;
    Weapon_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon_Monster_Scythe"), LEVEL_STATIC, TEXT("Prototype_GameObject_Weapon_Monster_Scythe"), &Weapon_Desc)))
        return E_FAIL;

    CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
    Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("spine_02");
    Locked_On_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Locked_On_Desc.pParentState = &m_iMonster_State;
    Locked_On_Desc.bLocked_On_Active = &m_bLocked_On;
    Locked_On_Desc.fSpeedPerSec = 0.f;
    Locked_On_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Locked_On"), LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_Locked_On"), &Locked_On_Desc)))
        return E_FAIL;

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

    CDecorative_Hat::DECORATIVE_HAT_DESC Deco_Desc{};
    Deco_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("Bip001-HeadNub");
    Deco_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Deco_Desc.pParentState = &m_iMonster_State;
    Deco_Desc.fSpeedPerSec = 0.f;
    Deco_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Decorative_Hat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Decorative_Hat"), &Deco_Desc)))
        return E_FAIL;

    return S_OK;
}

void CNormal_ScytheM::State_Update(_float fTimeDelta)
{
    m_pState_Manager->State_Update(fTimeDelta, this);
}

void CNormal_ScytheM::PatternCreate()
{
    if (!m_bPatternProgress && m_bActive)
    {
        m_fDelayTime += m_fTimeDelta;
        if (m_fDelayTime >= 1.f && m_fDistance <= 1.5f)
        {
            _uint iRandom = rand() % 2;
            switch (iRandom)
            {
            case 0:
                m_pState_Manager->ChangeState(new Attack_ComboA, this);
                break;
            case 1:
                m_pState_Manager->ChangeState(new Attack_ComboB, this);
                break;
            case 2:
                m_pState_Manager->ChangeState(new Attack_ComboC, this);
                break;
            }

            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
        }
    }
}

void CNormal_ScytheM::Active()
{
    m_bActive = true;
    if (!m_bFirstActive)
    {
        m_bFirstActive = true;
        m_pState_Manager->ChangeState(new CNormal_ScytheM::Intro_State(), this);
    }
}

void CNormal_ScytheM::Return_To_Spawn()
{
    m_pState_Manager->ChangeState(new Return_To_SpawnPoint_State(), this);
}

void CNormal_ScytheM::Stun()
{
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
    m_pState_Manager->ChangeState(new CNormal_ScytheM::Stun_State(), this);
#pragma region Effect_Stun
    m_pGameInstance->Play_Effect_Dir(EFFECT_NAME::EFFECT_PARTICLE_SPARK, Get_Transfrom()->Get_State(CTransform::STATE_POSITION), Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
#pragma endregion
}

void CNormal_ScytheM::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) && m_fMonsterCurHP > 0.f)
    {
        _uint m_iNoDamage = 1;
        if (m_iHitCount >= 3.f)
        {
            m_iHitCount = 0;
            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
            m_pState_Manager->ChangeState(new Parry_State(), this);
            m_iNoDamage = 0;
        }
        m_fRecoveryTime = 0.f;
        m_bCanRecovery = false;
        m_bHP_Bar_Active = true;
        m_fHP_Bar_Active_Timer = 0.f;
        m_fMonsterCurHP -= *m_Player_Attack * 0.5f * m_iNoDamage;  //나중에 플레이어의 공격력 받아오기
        m_fShieldHP -= (*m_Player_Attack) * 0.5f * 1.5f * m_iNoDamage;
        if (m_bCanHit)
        {
            _uint iRandom = rand() % 2;
            while (true)
            {
                if (iRandom == m_iHit_Motion_Index)
                {
                    iRandom = rand() % 2;
                }
                else
                {
                    m_iHit_Motion_Index = iRandom;
                    break;
                }
            }

            m_iHitCount += 1;
            m_pState_Manager->ChangeState(new CNormal_ScytheM::Hit_State(m_iHit_Motion_Index), this);
        }
    }
}

void CNormal_ScytheM::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    if ((!strcmp("MONSTER", _pOther->Get_Name()) || (!strcmp("PLAYER", _pOther->Get_Name()))) &&
        m_iMonster_State != STATE_STUN &&
        m_iMonster_State != STATE_EXECUTION &&
        m_fMonsterCurHP > 0.f)
    {
        m_bMove = false;
        m_pTransformCom->Sliding_Move(m_fTimeDelta, m_pNavigationCom, _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));
    }
}

void CNormal_ScytheM::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_bMove = true;
}

CNormal_ScytheM* CNormal_ScytheM::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNormal_ScytheM* pInstance = new CNormal_ScytheM(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CNormal_ScytheM");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNormal_ScytheM::Clone(void* pArg)
{
    CNormal_ScytheM* pInstance = new CNormal_ScytheM(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CNormal_ScytheM");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNormal_ScytheM::Free()
{
    __super::Free();

    Safe_Release(m_pState_Manager);
}

#pragma region Intro_State

void CNormal_ScytheM::Intro_State::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 22;

    pObject->m_bActive = true;
    pObject->m_bFirstActive = true;
    pObject->m_bPatternProgress = true;

    pObject->m_iMonster_State = STATE_INTRO;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_ScytheM::Intro_State::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Idle_State(), pObject);
    }
}

void CNormal_ScytheM::Intro_State::State_Exit(CNormal_ScytheM* pObject)
{
}
#pragma endregion

#pragma region Idle_State

void CNormal_ScytheM::Idle_State::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 14;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_ScytheM::Idle_State::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    pObject->RotateDegree_To_Player();

    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
        pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Move_State(), pObject);
}

void CNormal_ScytheM::Idle_State::State_Exit(CNormal_ScytheM* pObject)
{
}
#pragma endregion

#pragma region Move_State
void CNormal_ScytheM::Move_State::State_Enter(CNormal_ScytheM* pObject)
{
    if (pObject->m_fDistance > 0.5f)
        m_iIndex = 33;
    else
    {
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            m_iIndex = 30;
            break;
        case 1:
            m_iIndex = 37;
            break;
        case 2:
            m_iIndex = 38;
            break;
        }
    }
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_ScytheM::Move_State::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    pObject->RotateDegree_To_Player();

    if (pObject->m_fDistance >= 5.f)
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);

    else if (pObject->m_fDistance < 5.f && pObject->m_bMove)
    {
        if (m_iIndex == 33)
            pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 30)
            pObject->m_pTransformCom->Go_Backward_With_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 37)
            pObject->m_pTransformCom->Go_Left_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else
            pObject->m_pTransformCom->Go_Right_Navi(fTimeDelta, pObject->m_pNavigationCom);
    }
}

void CNormal_ScytheM::Move_State::State_Exit(CNormal_ScytheM* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}
#pragma endregion

#pragma region Run_State
void CNormal_ScytheM::Run_State::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 18;
    pObject->m_iMonster_State = STATE_RUN;
    m_pPlayerNavi = static_cast<CNavigation*>(pObject->m_pPlayer->Find_Component(TEXT("Com_Navigation")));
    pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_ScytheM::Run_State::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (pObject->m_fDistance >= 5.f || pObject->m_pNavigationCom->Get_BestListSize() >= 1)
    {
        pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    }

    _vector vDir = XMVectorSetY(pObject->m_pNavigationCom->MoveAstar(pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), bCheck), 0.f);
    if (bCheck && pObject->m_bMove)
    {
        pObject->m_pTransformCom->LookAt_Astar(vDir);
        pObject->m_pTransformCom->Go_Straight_Astar(fTimeDelta * 2.f, pObject->m_pNavigationCom);
    }
    else
    {
        pObject->RotateDegree_To_Player();
        pObject->m_pTransformCom->Go_Straight(fTimeDelta * 2.f, pObject->m_pNavigationCom);
    }
}

void CNormal_ScytheM::Run_State::State_Exit(CNormal_ScytheM* pObject)
{

}
#pragma endregion

#pragma region Stun_State
void CNormal_ScytheM::Stun_State::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 13;
    pObject->m_iMonster_State = STATE_STUN;
    pObject->m_bMove = true;
    pObject->m_bCan_Move_Anim = true;

    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1;
}

void CNormal_ScytheM::Stun_State::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (m_iIndex == 12 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
        m_fTime += fTimeDelta;


    if (m_iIndex == 13 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 12;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

    if (m_iIndex == 12 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && m_fTime >= 5.f)
    {
        m_iIndex = 11;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 12 &&
        pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex &&
        *pObject->m_Player_State == CPlayer::STATE_STUN_EXECUTE &&
        pObject->m_bIsClosest)
    {
        pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Execution_State(), pObject);
    }

    if (m_iIndex == 11 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_fShieldHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_IsStun = false;

        pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);
        pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pActor);

        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
    }
}

void CNormal_ScytheM::Stun_State::State_Exit(CNormal_ScytheM* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Hit_State

CNormal_ScytheM::Hit_State::Hit_State(_uint pHitNum)
{
    m_iHitNum = pHitNum;
}
void CNormal_ScytheM::Hit_State::State_Enter(CNormal_ScytheM* pObject)
{
    switch (m_iHitNum)
    {
    case 0:
        m_iIndex = 9;
        break;
    case 1:
        m_iIndex = 10;
        break;
    }
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_HIT;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bMove = true;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_ScytheM::Hit_State::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_ScytheM::Hit_State::State_Exit(CNormal_ScytheM* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Return_To_SpawnPoint_State
void CNormal_ScytheM::Return_To_SpawnPoint_State::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 33;
    pObject->m_fDelayTime = 0.f;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_ScytheM::Return_To_SpawnPoint_State::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    pObject->m_pNavigationCom->Start_Astar(pObject->m_iSpawn_Cell_Index);
    _vector vDir = XMVectorSetY(pObject->m_pNavigationCom->MoveAstar(pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), bCheck), 0.f);
    if (bCheck)
    {
        pObject->m_pTransformCom->LookAt_Astar(vDir);
        pObject->m_pTransformCom->Go_Straight_Astar(fTimeDelta, pObject->m_pNavigationCom);
    }

    if (pObject->m_fDistance <= 1.f)
    {
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
    }

    if (pObject->m_fSpawn_Distance <= 3.f)
    {
        pObject->m_pState_Manager->ChangeState(new NotActive_Idle(), pObject);
    }
}

void CNormal_ScytheM::Return_To_SpawnPoint_State::State_Exit(CNormal_ScytheM* pObject)
{
    pObject->m_bActive = false;
}
#pragma endregion

#pragma region NotActive_Idle
void CNormal_ScytheM::NotActive_Idle::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 15;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_ScytheM::NotActive_Idle::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance <= 5.f)
    {
        m_iIndex = 16;
        pObject->m_bActive = true;
        pObject->RotateDegree_To_Player();
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    if (m_iIndex == 16 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Idle_State(), pObject);
    }
}

void CNormal_ScytheM::NotActive_Idle::State_Exit(CNormal_ScytheM* pObject)
{
}

#pragma endregion

#pragma region Execution_State
void CNormal_ScytheM::Execution_State::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 27;
    pObject->m_iMonster_State = STATE_EXECUTION;
    pObject->m_bMove = true;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_bHP_Bar_Active = false;
    pObject->m_bExecution_Start = false;

    _float teleportDistance = 1.f;
    _vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
    _vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
    _vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

    vPlayerLook = XMVector3Normalize(vPlayerLook);

    _vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));

    pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
    pObject->RotateDegree_To_Player();

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);

    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_ScytheM::Execution_State::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_iMonster_State = STATE_DEAD;
    }
}

void CNormal_ScytheM::Execution_State::State_Exit(CNormal_ScytheM* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Parry_State
void CNormal_ScytheM::Parry_State::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 17;
    pObject->m_bCanHit = false;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 0;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_PARRY;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_ScytheM::Parry_State::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 44.f)
    {
        pObject->m_pState_Manager->ChangeState(new Attack_ComboC(), pObject);
    }

}

void CNormal_ScytheM::Parry_State::State_Exit(CNormal_ScytheM* pObject)
{
    pObject->m_bCanHit = true;
}
#pragma endregion

#pragma region Attack_ComboA
void CNormal_ScytheM::Attack_ComboA::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 0;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 95;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_ScytheM::Attack_ComboA::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (m_iIndex == 0 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Move_State(), pObject);
            break;
        }
    }
}

void CNormal_ScytheM::Attack_ComboA::State_Exit(CNormal_ScytheM* pObject)
{
}
#pragma endregion

#pragma region Attack_ComboB
void CNormal_ScytheM::Attack_ComboB::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 1;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 95;
    pObject->m_bCanHit = false;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_ScytheM::Attack_ComboB::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (m_iIndex == 1 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Move_State(), pObject);
            break;
        }
    }
}

void CNormal_ScytheM::Attack_ComboB::State_Exit(CNormal_ScytheM* pObject)
{
    pObject->m_bCanHit = true;
}
#pragma endregion

#pragma region Attack_ComboC
void CNormal_ScytheM::Attack_ComboC::State_Enter(CNormal_ScytheM* pObject)
{
    m_iIndex = 2;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_Attack_Power = 119;
    pObject->m_bCanHit = false;
    pObject->m_iMonster_State = MONSTER_STATE::STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_ScytheM::Attack_ComboC::State_Update(_float fTimeDelta, CNormal_ScytheM* pObject)
{
    if (m_iIndex == 2 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CNormal_ScytheM::Move_State(), pObject);
            break;
        }
    }
}

void CNormal_ScytheM::Attack_ComboC::State_Exit(CNormal_ScytheM* pObject)
{
    pObject->m_bCanHit = true;
}
#pragma endregion