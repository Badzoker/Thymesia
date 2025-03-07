#include "pch.h"
#include "Boss_Varg.h"
#include "Body_Varg.h"
#include "VargKnife.h"
#include "UI_Boss_HP_Bar.h"
#include "GameInstance.h"
#include "Animation.h"

CBoss_Varg::CBoss_Varg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CContainerObject(pDevice, pContext)
{
}

CBoss_Varg::CBoss_Varg(const CBoss_Varg& Prototype)
    : CContainerObject(Prototype)
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
    strcpy_s(m_szName, "MONSTER");
    m_fBossMaxHP = 100.f;
    m_fBossCurHP = m_fBossMaxHP;
    m_fShieldHP = m_fBossMaxHP;

    CGameObject::GAMEOBJECT_DESC        Desc{};

    Desc.fSpeedPerSec = 1.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;


    m_pPlayer = m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));

    _vector vFirst_Pos = { 112.6f, 1.85f, 28.8f, 1.0f };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFirst_Pos);
    m_pNavigationCom->Set_CurrentNaviIndex(vFirst_Pos);
    m_pTransformCom->Scaling(_float3{ 0.002f, 0.002f, 0.002f });


    m_pState_Manager = CState_Machine<CBoss_Varg>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;


    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    return S_OK;
}

void CBoss_Varg::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
    m_vPlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
    _vector pPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_fDistance = XMVectorGetX(XMVector3Length(m_vPlayerPos - pPosition));

    if (m_fDistance <= 20.f && !m_bBossActive)
    {
        m_iPhase = 1;
        m_pState_Manager->ChangeState(new CBoss_Varg::Intro_State(), this);
    }

    if (m_fBossCurHP <= 0.f && !m_IsStun)
    {
        m_IsStun = true;
        m_bPatternProgress = true;
        m_fDelayTime = 0.f;
        m_pState_Manager->ChangeState(new CBoss_Varg::Stun_State(), this);
    }
    if (m_pGameInstance->isKeyEnter(DIK_K) && m_fBossCurHP > 0.f)
    {
        //임시용으로 k버튼 누르면 피격모션 진행
        m_fRecoveryTime = 0.f;
        m_fBossCurHP -= 5.f;  //나중에 플레이어의 공격력 받아오기
        m_fShieldHP -= 10.f;
        m_pState_Manager->ChangeState(new CBoss_Varg::Hit_State(), this);
    }
    m_fLookTime += fTimeDelta;

    if (!m_bPatternProgress)
    {
        m_fLookTime = 0.f;
        RotateDegree_To_Player();
    }
    __super::Priority_Update(fTimeDelta);
}

void CBoss_Varg::Update(_float fTimeDelta)
{

    if (m_bNeed_Rotation)
        Rotation_To_Player();

    PatternCreate();
    RootAnimation();


    m_pState_Manager->State_Update(fTimeDelta, this);

    _vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));

    m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

    __super::Update(fTimeDelta);

}

void CBoss_Varg::Late_Update(_float fTimeDelta)
{
    Recovery_HP();

    if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.1f))
    {
        __super::Late_Update(fTimeDelta);
    }
}

HRESULT CBoss_Varg::Render()
{
    return S_OK;
}

HRESULT CBoss_Varg::Ready_Components()
{
    /* Com_Navigation */
    CNavigation::NAVIGATION_DESC   Desc{};

    Desc.iCurrentCellIndex = 0;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBoss_Varg::Ready_PartObjects()
{
    CBody_Varg::BODY_VARG_DESC BodyDesc{};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Varg"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg_Body"), &BodyDesc)))
        return E_FAIL;

    CVargKnife::VARGKNIFE_DESC		Varg_Knife_Desc{};

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Varg"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
        return E_FAIL;

    Varg_Knife_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
    Varg_Knife_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Varg_Knife_Desc.pParentModel = m_pModelCom;
    Varg_Knife_Desc.fSpeedPerSec = 0.f;
    Varg_Knife_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Varg_Knife"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg_Knife"), &Varg_Knife_Desc)))
        return E_FAIL;

    CUI_Boss_HP_Bar::UI_BOSS_HP_BAR_DESC pBoss_HP_Bar = {};
    pBoss_HP_Bar.fMaxHP = &m_fBossMaxHP;
    pBoss_HP_Bar.fCurHP = &m_fBossCurHP;
    pBoss_HP_Bar.fShieldHP = &m_fShieldHP;
    pBoss_HP_Bar.bBossActive = &m_bBossActive;


    //if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), LEVEL_GAMEPLAY, TEXT("Layer_UIScene"), UI_IMAGE, &pBoss_HP_Bar)))
    //    return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), LEVEL_GAMEPLAY, TEXT("Layer_UIScene"), &pBoss_HP_Bar)))
        return E_FAIL;


    return S_OK;

}

void CBoss_Varg::RootAnimation()
{
    _vector      vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector test = { 0.f,0.f,0.f,1.f };
    /* 루트 모션 애니메션 코드 */
    m_pRootMatrix = m_pModelCom->Get_RootMotionMatrix("root");

    if (!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModelCom->Get_LerpFinished() && m_bBossActive)
    {
        if ((m_pNavigationCom->isMove(vCurPosition) && m_fDistance > 1.5f) || m_bCan_Move_Anim)
            m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix);

        /* 2월 19일 추가 코드 */
        if (!m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
        {
            _float4x4 test = {};
            XMStoreFloat4x4(&test, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pRootMatrix)));
            const _float4x4* test2 = const_cast<_float4x4*>(&test);
            m_pTransformCom->Set_MulWorldMatrix(test2);
        }
    }
}

void CBoss_Varg::PatternCreate()
{
    //2페이즈 들어온 후
    if (m_iPhase == 2 && m_bBossActive)
    {
        m_fSpecial_Skill_CoolTime += 1 * m_fTimeDelta;
    }

    if (!m_bPatternProgress && m_bBossActive)
    {
        m_fDelayTime += 1 * m_fTimeDelta;
        if (m_fDelayTime >= 3.f)
        {
            if (m_fDistance >= 5.f)
                Far_Pattern_Create();
            else
                Near_Pattern_Create();

            m_fDelayTime = 0.f;
            m_bPatternProgress = true;
        }
    }
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

void CBoss_Varg::RotateDegree_To_Player()
{
    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    _vector vLook2 = m_vPlayerPos - vPos;

    vLook = XMVector3Normalize(vLook);
    vLook2 = XMVector3Normalize(vLook2);

    //회전해야 하는 각도
    _float fAngle = acos(XMVectorGetX(XMVector3Dot(vLook, vLook2)));
    fAngle = XMConvertToDegrees(fAngle);
    m_fRotateDegree = fAngle;
    if (m_fRotateDegree > 5.f)
    {
        m_bNeed_Rotation = true;
    }
    _vector fCrossResult = XMVector3Cross(vLook, vLook2);
    if (XMVectorGetY(fCrossResult) < 0)
    {
        m_fRotateDegree *= -1;
    }
}

void CBoss_Varg::Rotation_To_Player()
{
    _float fRadians = 1.f;
    if (m_fRotateDegree < 0.f)
    {
        fRadians *= -1;
        m_fAngle -= 1.f;
    }
    else
    {
        m_fAngle += 1.f;
    }

    m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fRadians));
    if (fabs(m_fAngle) >= fabs(m_fRotateDegree))
    {
        m_fAngle = 0.f;
        m_fRotateDegree = 0.f;
        m_bNeed_Rotation = false;
    }
}

void CBoss_Varg::Recovery_HP()
{
    if (m_fBossCurHP != m_fShieldHP)
    {
        m_fRecoveryTime += m_fTimeDelta;
        if (m_fRecoveryTime >= 5.f)
            m_bCanRecovery = true;
    }

    if (m_bCanRecovery)
    {
        m_fShieldHP += 0.1f;
        if (m_fShieldHP >= m_fBossCurHP)
        {
            m_fShieldHP = m_fBossCurHP;
            m_bCanRecovery = false;
            m_fRecoveryTime = 0.f;
        }
    }

}

void CBoss_Varg::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    /* 몬스터 무기와의 충돌 */
    if (!strcmp("PLAYER_RIGHT_WEAPON", _pOther->Get_Name()))
    {
        m_fRecoveryTime = 0.f;
        m_fBossCurHP -= 5.f;  //나중에 플레이어의 공격력 받아오기
        m_fShieldHP -= 10.f;
        if (!m_bPatternProgress)
        {
            m_pModelCom->Get_CurAnimation()->Set_LerpTime(0.f);
            m_pState_Manager->ChangeState(new CBoss_Varg::Hit_State(), this);
        }
    }
}

void CBoss_Varg::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
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


    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pState_Manager);
}


#pragma region Stun_State

void CBoss_Varg::Stun_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 36;
    pObject->m_bCan_Move_Anim = true;
    pObject->m_iState = CBoss_Varg::Varg_Stun_State;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Stun_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 35;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

    if (m_iIndex == 35 && pObject->m_fDistance <= 1.5f && pObject->m_pGameInstance->isMouseEnter(DIM_LB))
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::ExeCution_State(), pObject);
}

void CBoss_Varg::Stun_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

#pragma region Intro_State
void CBoss_Varg::Intro_State::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_bBossActive = true;
    pObject->m_bPatternProgress = true;
    pObject->m_iState = CBoss_Varg::Varg_Intro_State;
    pObject->m_pModelCom->SetUp_Animation(17, false);
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
    pObject->m_iState = CBoss_Varg::Varg_Idle_State;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    //애니메이션 실행
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Idle_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //Input handle이라는걸 파기도 함 
    //입력처리나 상태 변경 
    //불가피하게 여기서 애니메이션 실행해야할때도 있음

    if (pObject->m_pModelCom->GetAniFinish() && pObject->m_fSpecial_Skill_CoolTime >= 30.f && !pObject->m_bPatternProgress)
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Roar_State(false), pObject);
        pObject->m_fSpecial_Skill_CoolTime = 0.f;
        pObject->m_bPatternProgress = true;
        return;
    }

    if (pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
    }
}

void CBoss_Varg::Idle_State::State_Exit(CBoss_Varg* pObject)
{
    //거의 쓸일이 없긴하지만
    //만약에 Enter에서 추가한것들 다 지우는느낌
    //ex) 사운드, 크기 돌리고  
}

#pragma endregion

#pragma region Avoid_State
void CBoss_Varg::Avoid_State::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_bCan_Move_Anim = true;
    pObject->m_iState = CBoss_Varg::Varg_Avoid_State;
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
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Avoid_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        if (m_bBonusAttack)
        {
            m_iIndex = 3;
            pObject->RotateDegree_To_Player();
            pObject->m_iState = CBoss_Varg::Varg_Avoid_Attack_State;
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
}

#pragma endregion

#pragma region Hit_State
void CBoss_Varg::Hit_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 18;
    pObject->m_iState = CBoss_Varg::Varg_Hit_State;
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
}
#pragma endregion

#pragma region Walk_State
void CBoss_Varg::Walk_State::State_Enter(CBoss_Varg* pObject)
{
    //나중에 플레이어 위치받아올듯
    pObject->RotateDegree_To_Player();
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
    pObject->m_iState = CBoss_Varg::Varg_Walk_State;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CBoss_Varg::Walk_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
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
        pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
        pObject->m_pTransformCom->Go_Right_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
    }
    else
    {
        pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
        pObject->m_pTransformCom->Go_Left_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
    }
}

void CBoss_Varg::Walk_State::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region ATTACK_COMBO_A

void CBoss_Varg::Attack_Combo_A::State_Enter(CBoss_Varg* pObject)
{
    //1단
    m_iIndex = 7;
    pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_A_01_State;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_A::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //2단
    if (m_iIndex == 7 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 45.f)
    {
        m_iIndex += 1;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
        pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_A_02_State;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    //3단
    if (m_iIndex == 8 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 45.f)
    {
        m_iIndex += 1;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_A_03_State;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    //끝
    if (m_iIndex == 9 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);

}

void CBoss_Varg::Attack_Combo_A::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_B
void CBoss_Varg::Attack_Combo_B::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 10;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_B_01_State;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_B::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 10 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 90.f)
    {
        m_iIndex = 11;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_B_02_State;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 11 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);

}

void CBoss_Varg::Attack_Combo_B::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_C
void CBoss_Varg::Attack_Combo_C::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 10;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_B_01_State;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_C::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 10 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 90.f)
    {
        m_iIndex = 12;
        pObject->RotateDegree_To_Player();
        pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_B_03_State;
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 12 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
}

void CBoss_Varg::Attack_Combo_C::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Attack_Combo_D
void CBoss_Varg::Attack_Combo_D::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 7;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_A_01_State;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_D::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 7 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 50.f)
    {
        m_iIndex = 10;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
        pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_B_01_State;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    if (m_iIndex == 10 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 90.f)
    {
        m_iIndex = 12;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_B_03_State;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 12 && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
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
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTLF;
    pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_B_01_State;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Attack_Combo_E::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 10 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 90.f)
    {
        m_iIndex = 14;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_iState = CBoss_Varg::Varg_Attack_Combo_C_01_State;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    if (m_iIndex == 14 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
}

void CBoss_Varg::Attack_Combo_E::State_Exit(CBoss_Varg* pObject)
{
}
#pragma endregion

#pragma region Run_State
void CBoss_Varg::Run_State::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 25;
    pObject->m_iState = CBoss_Varg::Varg_Run_State;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Run_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iIndex == 24)
        m_fTimer += fTimeDelta;

    if (m_iIndex == 25 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 24;
        pObject->RotateDegree_To_Player();
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
    if (pObject->m_fDistance <= 1.5f || m_fTimer >= 3.f)
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
    pObject->m_iState = CBoss_Varg::Varg_Raid_Attack_01_State;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Raid_Attack_01::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
}

void CBoss_Varg::Raid_Attack_01::State_Exit(CBoss_Varg* pObject)
{
}
#pragma endregion

#pragma region Raid_Attack_02
void CBoss_Varg::Raid_Attack_02::State_Enter(CBoss_Varg* pObject)
{
    m_iIndex = 22;
    pObject->m_iState = CBoss_Varg::Varg_Raid_Attack_02_State;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}
void CBoss_Varg::Raid_Attack_02::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
}

void CBoss_Varg::Raid_Attack_02::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region ExeCution_State
void CBoss_Varg::ExeCution_State::State_Enter(CBoss_Varg* pObject)
{
    if (pObject->m_iPhase == 1)
    {
        m_iIndex = 50;
        pObject->m_iState = CBoss_Varg::Varg_Execution_First_State;
    }
    else
    {
        m_iIndex = 41;
        pObject->m_iState = CBoss_Varg::Varg_Execution_Second_State;
    }
    pObject->m_bCan_Move_Anim = true;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::ExeCution_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //나중에 페이즈 구분 해줘야할듯
    //1페이즈이고 애님 끝났으면 변환시키기
    if (m_iIndex == 50 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Roar_State(true), pObject);
    else if (m_iIndex == 41 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Dead_State, pObject);
}

void CBoss_Varg::ExeCution_State::State_Exit(CBoss_Varg* pObject)
{
    if (pObject->m_iPhase == 1)
        pObject->m_iPhase = 2;


    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Roar_State
CBoss_Varg::Roar_State::Roar_State(_bool m_bCheck)
    : m_bFirst(m_bCheck)
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

    pObject->m_fBossCurHP = pObject->m_fBossMaxHP;
    pObject->m_bCanRecovery = true;
    pObject->m_IsStun = false;
    pObject->m_iState = CBoss_Varg::Varg_Attack_Roar_State;
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
    pObject->m_bCan_Move_Anim = true;
    pObject->m_iState = CBoss_Varg::Varg_Attack_Catch_State;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Catch_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //달려가는중에 플레이어 부딪힌경우
    //pObject->RotateDegree_To_Player();
    pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);

    if (pObject->m_fDistance <= 1.5f)
    {
        m_iIndex = 28;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    //안부딪혀서 끝까지 진행된 경우
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
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
    pObject->m_iState = CBoss_Varg::Varg_Dead_State;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Varg::Dead_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 39;
        pObject->m_pModelCom->SetUp_Animation(39, true);
    }
}

void CBoss_Varg::Dead_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}

#pragma endregion

