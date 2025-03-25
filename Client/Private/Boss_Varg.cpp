#include "pch.h"
#include "Boss_Varg.h"
#include "Body_Varg.h"
#include "VargKnife.h"
#include "UI_Boss_HP_Bar.h"
#include "Player.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Locked_On.h"

CBoss_Varg::CBoss_Varg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CMonster(pDevice, pContext)
{
}

CBoss_Varg::CBoss_Varg(const CBoss_Varg& Prototype)
    : CMonster(Prototype)
{
}

HRESULT CBoss_Varg::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBoss_Varg::Initialize(void* pArg)
{

    m_fRootDistance = 1.5f;
    m_fSpawn_Distance_Max = 15.f;
    m_fActive_Distance = 15.f;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    if (FAILED(Ready_PartObjects(pArg)))
        return E_FAIL;

    m_pNavigationCom->Set_CurrentNaviIndex(XMLoadFloat4(&m_vSpawnPoint));
    m_Player_Attack = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower_Ptr();
    m_Player_State = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PhaseState_Ptr();
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));

    m_pState_Manager = CState_Machine<CBoss_Varg>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;


    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.5f,0.5f,0.2f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    m_pStunActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    settingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_CollisionGroup(m_pStunActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,0.f,1.f });
    m_pGameInstance->Set_GlobalPos(m_pStunActor, _fvector{ 0.f,0.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    return S_OK;
}

void CBoss_Varg::Priority_Update(_float fTimeDelta)
{
    if (*m_Player_State == CPlayer::PHASE_DEAD)
        m_Is_Player_Dead = true;

    __super::Priority_Update(fTimeDelta);
}

void CBoss_Varg::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    m_pState_Manager->State_Update(fTimeDelta, this);

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
        m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CBoss_Varg::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Varg::Render()
{
    return S_OK;
}

HRESULT CBoss_Varg::Ready_Components(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
        return E_FAIL;
    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

    return S_OK;
}

HRESULT CBoss_Varg::Ready_PartObjects(void* pArg)
{
    CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);


    CBody_Varg::BODY_VARG_DESC BodyDesc{};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Varg"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg_Body"), &BodyDesc)))
        return E_FAIL;


    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Varg"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
        return E_FAIL;

    CVargKnife::VARGKNIFE_DESC		Varg_Knife_Desc{};
    Varg_Knife_Desc.pParent = this;
    Varg_Knife_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
    Varg_Knife_Desc.pParentState = &m_iMonster_State;
    Varg_Knife_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Varg_Knife_Desc.pParentModel = m_pModelCom;
    Varg_Knife_Desc.fSpeedPerSec = 0.f;
    Varg_Knife_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Varg_Knife"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg_Knife"), &Varg_Knife_Desc)))
        return E_FAIL;

    CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
    Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("Bip001-Spine2");
    Locked_On_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Locked_On_Desc.pParentState = &m_iMonster_State;
    Locked_On_Desc.bLocked_On_Active = &m_bLocked_On;
    Locked_On_Desc.fSpeedPerSec = 0.f;
    Locked_On_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Locked_On"), LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_Locked_On"), &Locked_On_Desc)))
        return E_FAIL;

    CUI_Boss_HP_Bar::UI_BOSS_HP_BAR_DESC pBoss_HP_Bar = {};
    pBoss_HP_Bar.fMaxHP = &m_fMonsterMaxHP;
    pBoss_HP_Bar.fCurHP = &m_fMonsterCurHP;
    pBoss_HP_Bar.fShieldHP = &m_fShieldHP;
    pBoss_HP_Bar.bBossActive = &m_bActive;
    pBoss_HP_Bar.bBossDead = &m_bDead;
    pBoss_HP_Bar.iPhase = &m_iPhase;
    pBoss_HP_Bar.iCurLevel = iLevel;

    //if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), LEVEL_TUTORIAL, TEXT("Layer_UIScene"), UI_IMAGE, &pBoss_HP_Bar)))
    //    return E_FAIL;
    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), iLevel, TEXT("Layer_UIScene"), &pBoss_HP_Bar)))
        return E_FAIL;


    return S_OK;

}


void CBoss_Varg::PatternCreate()
{
    //2페이즈 들어온 후
    if (m_iPhase == PHASE_TWO && m_bActive)
    {
        m_fSpecial_Skill_CoolTime += 1 * m_fTimeDelta;
    }

    if (!m_bPatternProgress && m_bActive)
    {
        m_fDelayTime += 1 * m_fTimeDelta;
        if (m_fDelayTime >= m_fCoolTime)
        {
            if (m_fSpecial_Skill_CoolTime >= 30.f)
            {
                m_pState_Manager->ChangeState(new CBoss_Varg::Roar_State(false), this);
            }
            else if (m_fDistance >= 5.f)
                Far_Pattern_Create();
            else
                Near_Pattern_Create();

            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
        }
    }
}

void CBoss_Varg::Active()
{
    m_iPhase = PHASE_ONE;
    m_pState_Manager->ChangeState(new CBoss_Varg::Intro_State(), this);
}

void CBoss_Varg::Return_To_Spawn()
{
}

void CBoss_Varg::Stun()
{
    m_IsStun = true;
    m_bPatternProgress = true;
    m_fDelayTime = 0.f;
    m_pState_Manager->ChangeState(new CBoss_Varg::Stun_State(), this);
}

void CBoss_Varg::Near_Pattern_Create()
{
    _uint iRandomPattern = rand() % 6;
    while (true)
    {
        if (iRandomPattern == m_iNearPatternIndex)
        {
            iRandomPattern = rand() % 6;
        }
        else
        {
            m_iNearPatternIndex = iRandomPattern;
            break;
        }
    }
    switch (m_iNearPatternIndex)
    {
    case 0:
        m_pState_Manager->ChangeState(new CBoss_Varg::Attack_Combo_A(), this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CBoss_Varg::Attack_Combo_B(), this);
        break;
    case 2:
        m_pState_Manager->ChangeState(new CBoss_Varg::Attack_Combo_C(), this);
        break;
    case 3:
        m_pState_Manager->ChangeState(new CBoss_Varg::Attack_Combo_D(), this);
        break;
    case 4:
        m_pState_Manager->ChangeState(new CBoss_Varg::Attack_Combo_E(), this);
        break;
    case 5:
        m_pState_Manager->ChangeState(new CBoss_Varg::Avoid_State(), this);
        break;
    }
}

void CBoss_Varg::Far_Pattern_Create()
{
    _uint iRandomPattern = rand() % 3;
    while (true)
    {
        if (iRandomPattern == m_iFarPatternIndex)
        {
            iRandomPattern = rand() % 3;
        }
        else
        {
            m_iFarPatternIndex = iRandomPattern;
            break;
        }
    }
    m_iFarPatternIndex = 3;
    switch (m_iFarPatternIndex)
    {
    case 0:
        m_pState_Manager->ChangeState(new CBoss_Varg::Run_State(), this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CBoss_Varg::Avoid_State(), this);
        break;
    case 2:
        m_pState_Manager->ChangeState(new CBoss_Varg::Raid_Attack_01(), this);
        break;
    case 3:
        m_pState_Manager->ChangeState(new CBoss_Varg::Raid_Attack_02(), this);
        break;
    default:
        break;
    }
}

void CBoss_Varg::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    /* 몬스터 무기와의 충돌 */
    if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
    {
        m_fRecoveryTime = 0.f;
        m_bCanRecovery = false;
        m_fMonsterCurHP -= *m_Player_Attack / 5.f;
        m_fShieldHP -= (*m_Player_Attack / 5.f) * 1.5f;
        if (!m_bPatternProgress)
        {
            m_pState_Manager->ChangeState(new CBoss_Varg::Hit_State(), this);
        }
    }
}

void CBoss_Varg::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER", _pOther->Get_Name()) && (*m_Player_State & CPlayer::PHASE_EXECUTION) && !m_bExecution_Progress)
    {
        m_bExecution_Progress = true;
        m_pState_Manager->ChangeState(new ExeCution_State(), this);
    }
}

void CBoss_Varg::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBoss_Varg* CBoss_Varg::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBoss_Varg* pInstance = new CBoss_Varg(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBoss_Varg");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBoss_Varg::Clone(void* pArg)
{
    CBoss_Varg* pInstance = new CBoss_Varg(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBoss_Varg");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBoss_Varg::Free()
{
    __super::Free();

    Safe_Release(m_pState_Manager);
}

#pragma region Stun_State

void CBoss_Varg::Stun_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 36;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_bCan_Move_Anim = true;
    pObject->m_iMonster_State = STATE_STUN;

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_VARG;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Stun_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 35;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
}

void CBoss_Varg::Stun_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

#pragma region Intro_State
void CBoss_Varg::Intro_State::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_bActive = true;
    pObject->m_bPatternProgress = true;
    pObject->m_iMonster_State = STATE_INTRO;
    pObject->m_pModelCom->SetUp_Animation(26, false);
}

void CBoss_Varg::Intro_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
    }
}

void CBoss_Varg::Intro_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bPatternProgress = false;
}

#pragma endregion

#pragma region Idle_State
void CBoss_Varg::Idle_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 19;
    //Idle로 돌아오면 패턴 진행할수 있게 진행중이라는 불값 해제 
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_IDLE;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    //애니메이션 실행
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Idle_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    pObject->RotateDegree_To_Player();

    if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
    }
}

void CBoss_Varg::Idle_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}

#pragma endregion

#pragma region Avoid_State
void CBoss_Varg::Avoid_State::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_bCan_Move_Anim = true;
    pObject->m_iMonster_State = STATE_MOVE;
    m_iIndex = (rand() % 3) + 4;
    _uint iBonus_Attack = rand() % 2;
    switch (iBonus_Attack)
    {
    case 0:
        m_bBonusAttack = true;
        break;
    case 1:
        m_bBonusAttack = false;
        break;
    }
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Avoid_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //if (pObject->m_fDistance <= 1.5)
    //    pObject->m_bMove = false;
    if (pObject->m_pModelCom->GetAniFinish())
    {
        if (m_bBonusAttack)
        {
            m_iIndex = 3;
            pObject->m_bCan_Move_Anim = false;
            pObject->RotateDegree_To_Player();
            pObject->m_iMonster_State = STATE_ATTACK;
            pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
            pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
            m_bBonusAttack = false;
        }
        else
        {
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
        }
    }
}

void CBoss_Varg::Avoid_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bCan_Move_Anim = false;
    pObject->m_bMove = true;
}

#pragma endregion

#pragma region Hit_State
void CBoss_Varg::Hit_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 18;
    pObject->m_iMonster_State = STATE_HIT;
    pObject->m_pModelCom->Set_Continuous_Ani(true);
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Hit_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
    }
}

void CBoss_Varg::Hit_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}
#pragma endregion

#pragma region Walk_State
void CBoss_Varg::Walk_State::State_Enter(CBoss_Varg* pObject)
{
    //나중에 플레이어 위치받아올듯
    if (pObject->m_fDistance > 1.5f)
        m_iIndex = 47;
    else if (pObject->m_fDistance <= 1.5f)
    {
        iRandomMove = rand() % 3;
        iRandomMove = 1;
        switch (iRandomMove)
        {
        case 0:
            m_iIndex = 46;
            break;
        case 1:
            m_iIndex = 48;
            break;
        case 2:
            m_iIndex = 49;
            break;
        }
    }
    pObject->m_bPatternProgress = false;
    pObject->m_iMonster_State = STATE_MOVE;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CBoss_Varg::Walk_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    pObject->RotateDegree_To_Player();
    if (m_iIndex == 47)
    {
        pObject->m_pTransformCom->Go_Straight(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
    }
    else if (m_iIndex == 46)
    {
        pObject->m_pTransformCom->Go_Backward_With_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
    }
    else if (m_iIndex == 48)
    {
        pObject->m_pTransformCom->LookAt(XMLoadFloat4(&pObject->m_vPlayerPos));
        pObject->m_pTransformCom->Go_Right_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
    }
    else
    {
        pObject->m_pTransformCom->LookAt(XMLoadFloat4(&pObject->m_vPlayerPos));
        pObject->m_pTransformCom->Go_Left_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
    }
}

void CBoss_Varg::Walk_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->Set_Continuous_Ani(true);
}

#pragma endregion

#pragma region ATTACK_COMBO_A

void CBoss_Varg::Attack_Combo_A::State_Enter(CBoss_Varg* pObject)
{
    //1단
    m_iIndex = 7;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_iMonster_Attack_Power = 145;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_A::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //2단
    if (m_iIndex == 7 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 45.f)
    {
        m_iIndex += 1;
        pObject->m_iMonster_Attack_Power = 145;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    //3단
    if (m_iIndex == 8 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 45.f)
    {
        m_iIndex += 1;
        pObject->m_iMonster_Attack_Power = 174;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    //끝
    if (m_iIndex == 9 && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
            break;
        }
    }
}

void CBoss_Varg::Attack_Combo_A::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_B
void CBoss_Varg::Attack_Combo_B::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 10;
    pObject->m_iMonster_Attack_Power = 145;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_B::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 10 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 80.f)
    {
        m_iIndex = 11;
        pObject->m_iMonster_Attack_Power = 174;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 11 && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
            break;
        }
    }
}

void CBoss_Varg::Attack_Combo_B::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_C
void CBoss_Varg::Attack_Combo_C::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 10;
    pObject->m_iMonster_Attack_Power = 145;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_C::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 10 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 80.f)
    {
        m_iIndex = 12;
        pObject->m_iMonster_Attack_Power = 174;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 12 && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
            break;
        }
    }
}

void CBoss_Varg::Attack_Combo_C::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_D
void CBoss_Varg::Attack_Combo_D::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 7;
    pObject->m_iMonster_Attack_Power = 145;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_D::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 7 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 50.f)
    {
        m_iIndex = 10;
        pObject->m_iMonster_Attack_Power = 145;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    if (m_iIndex == 10 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 80.f)
    {
        m_iIndex = 12;
        pObject->m_iMonster_Attack_Power = 174;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 12 && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
            break;
        }
    }
}

void CBoss_Varg::Attack_Combo_D::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_E
void CBoss_Varg::Attack_Combo_E::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 10;
    pObject->m_iMonster_Attack_Power = 145;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_E::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 10 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 110.f)
    {
        m_iIndex = 9;
        pObject->m_iMonster_Attack_Power = 174;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(10.f);
    }
    if (m_iIndex == 9 && pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
            break;
        }
    }
}

void CBoss_Varg::Attack_Combo_E::State_Exit(CBoss_Varg* pObject)
{
}
#pragma endregion

#pragma region Run_State
void CBoss_Varg::Run_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 25;
    pObject->m_iMonster_State = STATE_RUN;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Run_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 24)
        m_fTimer += fTimeDelta;

    pObject->m_pTransformCom->LookAt(XMLoadFloat4(&pObject->m_vPlayerPos));

    if (m_iIndex == 25 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 24;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
    if (pObject->m_fDistance <= 1.5f || m_fTimer >= 2.f)
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Attack_Combo_B(), pObject);

}

void CBoss_Varg::Run_State::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Raid_Attack_01
void CBoss_Varg::Raid_Attack_01::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 21;
    pObject->m_iMonster_Attack_Power = 174;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Raid_Attack_01::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
            break;
        }
    }
}

void CBoss_Varg::Raid_Attack_01::State_Exit(CBoss_Varg* pObject)
{
}
#pragma endregion

#pragma region Raid_Attack_02
void CBoss_Varg::Raid_Attack_02::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 22;
    pObject->m_iMonster_Attack_Power = 174;
    pObject->m_iMonster_State = STATE_ATTACK;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}
void CBoss_Varg::Raid_Attack_02::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
            break;
        case 1:
            pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
            break;
        }
    }
}

void CBoss_Varg::Raid_Attack_02::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bMove = true;
}

#pragma endregion

#pragma region ExeCution_State
void CBoss_Varg::ExeCution_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 41;
    pObject->m_iMonster_State = STATE_EXECUTION;
    pObject->RotateDegree_To_Player();
    pObject->m_bCan_Move_Anim = true;

    _vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
    _vector vPlayerPos = XMLoadFloat4(&pObject->m_vPlayerPos);
    vPlayerLook = XMVector3Normalize(vPlayerLook);
    vPlayerLook *= 2.f;
    _vector vResultPos = vPlayerPos + vPlayerLook;
    pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vResultPos);

    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::ExeCution_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //나중에 페이즈 구분 해줘야할듯
      //1페이즈이고 애님 끝났으면 변환시키기
    if (m_iIndex == 41 && pObject->m_iPhase == PHASE_ONE && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 140.f)
    {
        m_iIndex = 40;
        pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);
        pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pActor);
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    if (m_iIndex == 41 && pObject->m_iPhase == PHASE_TWO && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Dead_State, pObject);
    }

    if (m_iIndex == 40 && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Roar_State(true), pObject);
    }
}

void CBoss_Varg::ExeCution_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
    if (pObject->m_iPhase == PHASE_ONE)
    {
        pObject->m_iPhase = PHASE_TWO;
        pObject->m_fCoolTime = 0.1f;
        pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP;
        pObject->m_bCanRecovery = true;
    }
    pObject->m_bExecution_Progress = false;
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Roar_State

CBoss_Varg::Roar_State::Roar_State(_bool pCheck)
    : m_bFirst(pCheck)
{
}

void CBoss_Varg::Roar_State::State_Enter(CBoss_Varg* pObject)
{
    if (m_bFirst)
    {
        m_iIndex = 32;
    }
    else
    {
        m_iIndex = 31;
    }

    pObject->m_fSpecial_Skill_CoolTime = 0.f;
    pObject->RotateDegree_To_Player();
    pObject->m_bPatternProgress = true;
    pObject->m_IsStun = false;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_STUN;
    pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);



}

void CBoss_Varg::Roar_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{

    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Catch_State(), pObject);
}

void CBoss_Varg::Roar_State::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Catch_State
void CBoss_Varg::Catch_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 30;
    //pObject->m_bCan_Move_Anim = true;
    m_bCanCatch = true;
    pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Catch_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    pObject->RotateDegree_To_Player();
    //첫번째 잡을려하는 모션 끝내고 달려가는거 넣기
    if (m_iIndex == 30 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 20.f && m_bFirst)
    {
        m_bFirst = false;
        m_iIndex = 33;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    //달려가는거 1초?가량 지나면 잡을려하는 모션 취함
    if (m_iIndex == 33 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 30;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(20.f);
    }
    //항상 거리가 짧으면 바로 잡히는 애니메이션 실행
    if (pObject->m_fDistance <= 1.5f && m_bCanCatch)
    {
        m_iIndex = 28;
        pObject->m_iMonster_Attack_Power = 281;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    //안부딪혀서 끝까지 진행된 경우
    if ((m_iIndex == 30 || m_iIndex == 28) && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
    }
}

void CBoss_Varg::Catch_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bCan_Move_Anim = false;

}

#pragma endregion

#pragma region Dead_State

void CBoss_Varg::Dead_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 37;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_iMonster_State = STATE_DEAD;

    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
    pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);

    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Dead_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 39;
        pObject->m_bDead = true;
        pObject->m_bActive = false;
        pObject->m_pModelCom->SetUp_Animation(39, false);
    }
}

void CBoss_Varg::Dead_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

