#include "pch.h"
#include "Normal_VillageM1.h"
#include "Player.h"
#include "Weapon_Axe.h"
#include "Weapon_Shield.h"
#include "Body_VillageM1.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Monster_HP_Bar.h"
#include "Locked_On.h"

CNormal_VillageM1::CNormal_VillageM1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CMonster(pDevice, pContext)
{
}

CNormal_VillageM1::CNormal_VillageM1(const CNormal_VillageM1& Prototype)
    :CMonster(Prototype)
{
}

HRESULT CNormal_VillageM1::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNormal_VillageM1::Initialize(void* pArg)
{
    m_fHP_Bar_Height = 500.f;
    m_fRootDistance = 1.f;
    m_fSpawn_Distance_Max = 10.f;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Layer_Player"), "PLAYER");
    m_pNavigationCom->Set_CurrentNaviIndex(XMLoadFloat4(&m_vSpawnPoint));
    m_iSpawn_Cell_Index = m_pNavigationCom->Get_CurCellIndex();
    m_Player_Attack = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower_Ptr();

    m_pState_Manager = CState_Machine<CNormal_VillageM1>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint setting_Body_ColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON | GROUP_TYPE::MONSTER;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, setting_Body_ColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    return S_OK;
}

void CNormal_VillageM1::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CNormal_VillageM1::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    m_pState_Manager->State_Update(fTimeDelta, this);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CNormal_VillageM1::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CNormal_VillageM1::Render()
{
    return S_OK;
}

HRESULT CNormal_VillageM1::Ready_Components()
{
    /* Com_Navigation */
    CNavigation::NAVIGATION_DESC   Desc{};

    Desc.iCurrentCellIndex = 0;

    if (FAILED(__super::Add_Component(LEVEL_TUTORIAL, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
        return E_FAIL;


    return S_OK;
}

HRESULT CNormal_VillageM1::Ready_PartObjects()
{
    CBody_VillageM1::BODY_VillageM1_DESC BodyDesc = {};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pParentState = &m_iMonster_State;
    BodyDesc.bDead = &m_bDead;
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_VillageM1"), LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Normal_VillageM1_Body"), &BodyDesc)))
        return E_FAIL;

    CWeapon_Axe::WEAPON_AXE_DESC		Weapon_Desc = {};

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_VillageM1"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
        return E_FAIL;

    Weapon_Desc.pParent = this;
    Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
    Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Weapon_Desc.pParentModel = m_pModelCom;
    Weapon_Desc.pParentState = &m_iMonster_State;
    Weapon_Desc.fSpeedPerSec = 0.f;
    Weapon_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon_Axe"), LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Weapon_Axe"), &Weapon_Desc)))
        return E_FAIL;

    CWeapon_Shield::WEAPON_SHIELD_DESC		Weapon2_Desc = {};

    Weapon2_Desc.pParent = this;
    Weapon2_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("lowerarm_l");
    Weapon2_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Weapon2_Desc.pParentModel = m_pModelCom;
    Weapon2_Desc.pParentState = &m_iMonster_State;
    Weapon2_Desc.fSpeedPerSec = 0.f;
    Weapon2_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon_Shield"), LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Weapon_Shield"), &Weapon2_Desc)))
        return E_FAIL;

    CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
    Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("spine_02");
    Locked_On_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Locked_On_Desc.pParentState = &m_iMonster_State;
    Locked_On_Desc.bLocked_On_Active = &m_bLocked_On;
    Locked_On_Desc.fSpeedPerSec = 0.f;
    Locked_On_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Locked_On"), LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Monster_Locked_On"), &Locked_On_Desc)))
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

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Monster_HP_Bar"), LEVEL_TUTORIAL, TEXT("Layer_MonsterHP"), &Monster_HP_Bar_Desc)))
        return E_FAIL;

    return S_OK;
}

void CNormal_VillageM1::PatternCreate()
{

    if (!m_bPatternProgress && m_bActive)
    {
        m_fDelayTime += m_fTimeDelta;
        if (m_fDelayTime >= 1.f && m_fDistance <= 1.5f)
        {
            _uint iRandom = rand() % 3;
            switch (iRandom)
            {
            case 0:
                m_pState_Manager->ChangeState(new Attack_01_State(), this);
                break;
            case 1:
                m_pState_Manager->ChangeState(new Attack_02_State(), this);
                break;
            case 2:
                m_pState_Manager->ChangeState(new Attack_03_State(), this);
                break;
            }

            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
        }
    }
}

void CNormal_VillageM1::Active()
{
    m_bActive = true;
    m_pState_Manager->ChangeState(new CNormal_VillageM1::Idle_State(), this);
}

void CNormal_VillageM1::Return_To_Spawn()
{
    m_pState_Manager->ChangeState(new CNormal_VillageM1::Return_To_SpawnPoint_State(), this);
}

void CNormal_VillageM1::Stun()
{
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
    m_pState_Manager->ChangeState(new CNormal_VillageM1::Stun_State(), this);
}

void CNormal_VillageM1::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) && m_fMonsterCurHP > 0.f)
    {
        _uint m_iNoDamage = 1;
        if (m_iHitCount >= 4.f)
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
        m_iHitCount += 1;
        if (m_bCanHit)
        {
            m_pState_Manager->ChangeState(new CNormal_VillageM1::Hit_State(), this);
        }
    }


}

void CNormal_VillageM1::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("MONSTER", _pOther->Get_Name()) || !strcmp("PLAYER", _pOther->Get_Name()))
    {
        m_bMove = false;
        m_pTransformCom->Sliding_Move(m_fTimeDelta, m_pNavigationCom, _pOther->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));
    }
}

void CNormal_VillageM1::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_bMove = true;
}

CNormal_VillageM1* CNormal_VillageM1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNormal_VillageM1* pInstance = new CNormal_VillageM1(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CNormal_VillageM1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNormal_VillageM1::Clone(void* pArg)
{
    CNormal_VillageM1* pInstance = new CNormal_VillageM1(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CNormal_VillageM1");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNormal_VillageM1::Free()
{
    __super::Free();

    Safe_Release(m_pState_Manager);
}

#pragma region Idle_State

void CNormal_VillageM1::Idle_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 30;
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Idle_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f && pObject->m_fDistance >= 10.f && pObject->m_bActive)
    {
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    }
    else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f && pObject->m_fDistance < 10.f && pObject->m_bActive)
    {
        pObject->m_pState_Manager->ChangeState(new Move_State(), pObject);
    }
}

void CNormal_VillageM1::Idle_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}

#pragma endregion

#pragma region Move_State
void CNormal_VillageM1::Move_State::State_Enter(CNormal_VillageM1* pObject)
{
    if (pObject->m_fDistance > 0.5f)
        m_iIndex = 43;
    else
    {
        _uint iRandom = rand() % 3;
        switch (iRandom)
        {
        case 0:
            m_iIndex = 46;
            break;
        case 1:
            m_iIndex = 47;
            break;
        case 2:
            m_iIndex = 41;
            break;
        }
    }
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_VillageM1::Move_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (pObject->m_fDistance >= 2.f)
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    else if (pObject->m_fDistance < 2.f && pObject->m_bMove)
    {
        pObject->RotateDegree_To_Player();
        if (m_iIndex == 43)
            pObject->m_pTransformCom->Go_Straight(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 41)
            pObject->m_pTransformCom->Go_Backward_With_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 46)
            pObject->m_pTransformCom->Go_Left_Navi(fTimeDelta, pObject->m_pNavigationCom);
        else if (m_iIndex == 47)
            pObject->m_pTransformCom->Go_Right_Navi(fTimeDelta, pObject->m_pNavigationCom);
    }
}

void CNormal_VillageM1::Move_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}
#pragma endregion

#pragma region Run_State
void CNormal_VillageM1::Run_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 1;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    pObject->m_iMonster_State = STATE_RUN;
    m_pPlayerNavi = static_cast<CNavigation*>(pObject->m_pPlayer->Find_Component(TEXT("Com_Navigation")));
    pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
}

void CNormal_VillageM1::Run_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (pObject->m_fDistance >= 10.f || pObject->m_pNavigationCom->Get_BestListSize() >= 1)
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

void CNormal_VillageM1::Run_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}

#pragma endregion 

#pragma region Stun_State
void CNormal_VillageM1::Stun_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 29;
    pObject->m_bCan_Move_Anim = true;
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_STUN;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);


}

void CNormal_VillageM1::Stun_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (m_iIndex == 28)
        m_fTime += fTimeDelta;

    if (m_iIndex == 29 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 28;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

    if (m_iIndex == 28 && m_fTime >= 5.f)
    {
        m_iIndex = 27;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    else if (m_iIndex == 28 && /*pObject->m_fDistance <= 1.5f &&*/ pObject->m_pGameInstance->isMouseEnter(DIM_LB))
    {
        pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
    }

    if (m_iIndex == 27 && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_fShieldHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_IsStun = false;
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
    }
}

void CNormal_VillageM1::Stun_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Attack_01_State
void CNormal_VillageM1::Attack_01_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 4;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_bCanHit = false;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Attack_01_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (m_iIndex == 4 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM1::Attack_01_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_bCanHit = true;
}
#pragma endregion

#pragma region Attack_02_State
void CNormal_VillageM1::Attack_02_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 5;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Attack_02_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (m_iIndex == 5 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 99.f)
    {
        m_iIndex = 6;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 6 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM1::Attack_02_State::State_Exit(CNormal_VillageM1* pObject)
{
}
#pragma endregion

#pragma region Attack_03_State
void CNormal_VillageM1::Attack_03_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 8;
    pObject->m_bCanHit = false;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Attack_03_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (m_iIndex == 8 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM1::Attack_03_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_bCanHit = true;
}

#pragma endregion
#pragma region Hit_State
void CNormal_VillageM1::Hit_State::State_Enter(CNormal_VillageM1* pObject)
{
    _uint iRandom = rand() % 2;
    while (true)
    {
        iRandom = rand() % 2;
        if (m_iHitNum != iRandom)
            break;
    }
    switch (iRandom)
    {
    case 0:
        m_iIndex = 25;
        break;
    case 1:
        m_iIndex = 26;
        break;
    }
    pObject->RotateDegree_To_Player();
    pObject->m_iMonster_State = STATE_HIT;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Hit_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM1::Hit_State::State_Exit(CNormal_VillageM1* pObject)
{
}
#pragma endregion

#pragma region Execution_State
void CNormal_VillageM1::Execution_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 39;
    pObject->m_bHP_Bar_Active = false;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_iMonster_State = STATE_EXECUTION;
    pObject->RotateDegree_To_Player();

    _vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
    _vector vPlayerPos = XMLoadFloat4(&pObject->m_vPlayerPos);
    vPlayerLook = XMVector3Normalize(vPlayerLook);
    _vector vResultPos = vPlayerPos + vPlayerLook;
    pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vResultPos);

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Execution_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 40;
        pObject->m_iMonster_State = STATE_DEAD;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
}

void CNormal_VillageM1::Execution_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

void CNormal_VillageM1::Parry_Attack_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 9;
    pObject->m_bCanHit = false;
    pObject->m_iMonster_State = STATE_PARRY_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Parry_Attack_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM1::Parry_Attack_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_bCanHit = true;
}

void CNormal_VillageM1::Parry_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 48;
    pObject->m_bCanHit = false;
    pObject->m_iMonster_State = STATE_PARRY;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Parry_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 150.f)
        pObject->m_pState_Manager->ChangeState(new Parry_Attack_State(), pObject);
}

void CNormal_VillageM1::Parry_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_bCanHit = true;
}

void CNormal_VillageM1::Return_To_SpawnPoint_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 43;
    pObject->m_fDelayTime = 0.f;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_bPatternProgress = true;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_VillageM1::Return_To_SpawnPoint_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
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
        pObject->m_pState_Manager->ChangeState(new Idle_State, pObject);
    }

    if (pObject->m_fSpawn_Distance <= 5.f)
    {
        pObject->m_pState_Manager->ChangeState(new Idle_State, pObject);
    }
}

void CNormal_VillageM1::Return_To_SpawnPoint_State::State_Exit(CNormal_VillageM1* pObject)
{
    pObject->m_bActive = false;
}
