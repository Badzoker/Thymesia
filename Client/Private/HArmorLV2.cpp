#include "pch.h"
#include "HArmorLv2.h"
#include "GameInstance.h"
#include "Body_HArmorLv2.h"
#include "Weapon_GreatSword.h"
#include "Monster_HP_Bar.h"
#include "Animation.h"

CHArmorLV2::CHArmorLV2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject(pDevice, pContext)
{
}

CHArmorLV2::CHArmorLV2(const CHArmorLV2& Prototype)
    :CContainerObject(Prototype)
{
}

HRESULT CHArmorLV2::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CHArmorLV2::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER");

    m_fMonsterMaxHP = 100.f;
    m_fMonsterCurHP = m_fMonsterMaxHP;
    m_fShieldHP = m_fMonsterMaxHP;

    CGameObject::GAMEOBJECT_DESC* Desc = static_cast<GAMEOBJECT_DESC*>(pArg);

    Desc->fSpeedPerSec = 1.f;
    Desc->fScaling = _float3{ 0.002f,0.002f,0.002f };
    Desc->fRotationPerSec = XMConvertToRadians(90.f);
    m_vSpawnPoint = XMLoadFloat4(&Desc->fPosition);

    if (FAILED(__super::Initialize(Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;


    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"), "PLAYER");
    m_pNavigationCom->Set_CurrentNaviIndex(m_vSpawnPoint);


    m_pState_Manager = CState_Machine<CHArmorLV2>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    m_pGameObjectModel = m_pModelCom;
    return S_OK;
}

void CHArmorLV2::Priority_Update(_float fTimeDelta)
{
    //플레이어와의 거리 계산
    m_fTimeDelta = fTimeDelta;
    m_vPlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
    _vector pPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_fDistance = XMVectorGetX(XMVector3Length(m_vPlayerPos - pPosition));
 
    
    //거리에따른 Active 활성화
    if (m_fDistance <= 15.f && !m_bActive)
    {
        m_bActive = true;
        m_pState_Manager->ChangeState(new CHArmorLV2::Intro_State(), this);
    }

    if (m_fMonsterCurHP <= 0.f && !m_IsStun)
    {
        m_IsStun = true;
        m_bPatternProgress = true;
        m_fDelayTime = 0.f;
        m_pState_Manager->ChangeState(new CHArmorLV2::Stun_State(), this);
    }
    if (m_bHP_Bar_Active)
    {
        m_fHP_Bar_Active_Timer += fTimeDelta;
        if (m_fHP_Bar_Active_Timer >= 10.f)
        {
            m_fHP_Bar_Active_Timer = 0.f;
            m_bHP_Bar_Active = false;
        }
    }
    __super::Priority_Update(fTimeDelta);

}

void CHArmorLV2::Update(_float fTimeDelta)
{
    PatternCreate();
    RootAnimation();

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });


    _vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));
    m_pState_Manager->State_Update(fTimeDelta, this);
    __super::Update(fTimeDelta);
}

void CHArmorLV2::Late_Update(_float fTimeDelta)
{
    Recovery_HP();
    if (m_bNeed_Rotation)
        Rotation_To_Player();

    if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.1f, FRUSTUM_TYPE::FRUSTUM_MONSTER))
    {
        __super::Late_Update(fTimeDelta);
    }
}

HRESULT CHArmorLV2::Render()
{
    return S_OK;
}

HRESULT CHArmorLV2::Ready_Components()
{
    /* Com_Navigation */
    CNavigation::NAVIGATION_DESC   Desc{};

    Desc.iCurrentCellIndex = 0;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CHArmorLV2::Ready_PartObjects()
{
    CBody_HArmorLV2::BODY_HArmorLV2_DESC BodyDesc = {};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_HArmorLV2"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Elite_HArmorLV2_Body"), &BodyDesc)))
        return E_FAIL;

    CWeapon_GreatSword::WEAPON_GreatSword_DESC		Weapon_GreatSword_Desc = {};

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_HArmorLV2"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
        return E_FAIL;
    Weapon_GreatSword_Desc.pParent = this;
    Weapon_GreatSword_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
    Weapon_GreatSword_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Weapon_GreatSword_Desc.pParentModel = m_pModelCom;
    Weapon_GreatSword_Desc.fSpeedPerSec = 0.f;
    Weapon_GreatSword_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Weapon_GreatSword"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon_GreatSword"), &Weapon_GreatSword_Desc)))
        return E_FAIL;

    CMonster_HP_Bar::Monster_HP_Bar_DESC Monster_HP_Bar_Desc = {};
    Monster_HP_Bar_Desc.pMonsterMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Monster_HP_Bar_Desc.fMaxHP = &m_fMonsterMaxHP;
    Monster_HP_Bar_Desc.fCurHP = &m_fMonsterCurHP;
    Monster_HP_Bar_Desc.fShieldHP = &m_fShieldHP;
    Monster_HP_Bar_Desc.bHP_Bar_Active = &m_bHP_Bar_Active;
    Monster_HP_Bar_Desc.fSpeedPerSec = 0.f;
    Monster_HP_Bar_Desc.fRotationPerSec = 0.f;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_HP_Bar"), LEVEL_GAMEPLAY, TEXT("Layer_MonsterHP"), &Monster_HP_Bar_Desc)))
        return E_FAIL;

    return S_OK;
}

void CHArmorLV2::RootAnimation()
{
    /* 루트 모션 애니메션 코드 */
    _vector      vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector test = { 0.f,0.f,0.f,1.f };
    m_pRootMatrix = m_pModelCom->Get_RootMotionMatrix("root");
    _uint iTest = m_pModelCom->Get_Current_Animation_Index();
    if ((!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModelCom->Get_LerpFinished()))
    {
        if ((m_pNavigationCom->isMove(vCurPosition) && m_fDistance > 0.5f) || m_bCan_Move_Anim)
            m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix);

        if (!m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
        {
            _float4x4 test = {};
            XMStoreFloat4x4(&test, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pRootMatrix)));
            const _float4x4* test2 = const_cast<_float4x4*>(&test);
            m_pTransformCom->Set_MulWorldMatrix(test2);
        }
    }
}

void CHArmorLV2::PatternCreate()
{
    if (!m_bPatternProgress && m_bActive)
    {
        m_fDelayTime += m_fTimeDelta;
        if (m_fDelayTime >= 2.f && m_fDistance <= 1.5f)
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

void CHArmorLV2::Near_Pattern_Create()
{
    _uint iRandomPattern = rand() % 5;
    while (true)
    {
        if (iRandomPattern == m_iNearPatternIndex)
        {
            iRandomPattern = rand() % 5;
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
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_01, this);
        break;
    case 1:
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_02, this);
        break;
    case 2:
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_03, this);
        break;
    case 3:
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_06, this);
        break;
    case 4:
        m_pState_Manager->ChangeState(new CHArmorLV2::Attack_Pattern_07, this);
        break;
    default:
        break;
    }
}

void CHArmorLV2::Far_Pattern_Create()
{
    m_pState_Manager->ChangeState(new Attack_Pattern_05, this);
}

void CHArmorLV2::RotateDegree_To_Player()
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

void CHArmorLV2::Rotation_To_Player()
{
    _float fRadians = 3.f;
    if (m_fRotateDegree < 0.f)
    {
        fRadians *= -1;
        m_fAngle -= 3.f;
    }
    else
    {
        m_fAngle += 3.f;
    }

    m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fRadians));
    if (fabs(m_fAngle) >= fabs(m_fRotateDegree))
    {
        m_fAngle = 0.f;
        m_fRotateDegree = 0.f;
        m_bNeed_Rotation = false;
    }
}

void CHArmorLV2::Recovery_HP()
{
    if (m_fMonsterCurHP != m_fShieldHP)
    {
        m_fRecoveryTime += m_fTimeDelta;
        if (m_fRecoveryTime >= 5.f)
            m_bCanRecovery = true;
    }
    if (m_bCanRecovery)
    {
        m_fShieldHP += 0.1f;
        if (m_fShieldHP >= m_fMonsterCurHP)
        {
            m_fShieldHP = m_fMonsterCurHP;
            m_bCanRecovery = false;
            m_fRecoveryTime = 0.f;
        }
    }
}

void CHArmorLV2::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()) && m_fMonsterCurHP > 0.f)
    {
        m_fRecoveryTime = 0.f;
        m_bHP_Bar_Active = true;
        m_fHP_Bar_Active_Timer = 0.f;
        m_fMonsterCurHP -= 5.f;  //나중에 플레이어의 공격력 받아오기
        m_fShieldHP -= 10.f;
        if (!m_bPatternProgress)
        {
            m_pModelCom->Get_CurAnimation()->Set_LerpTime(0.f);
            //m_pState_Manager->ChangeState(new C::Hit_State(), this);
        }
    }
}

void CHArmorLV2::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CHArmorLV2::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CHArmorLV2* CHArmorLV2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHArmorLV2* pInstance = new CHArmorLV2(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CHArmorLv2");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CHArmorLV2::Clone(void* pArg)
{
    CHArmorLV2* pInstance = new CHArmorLV2(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CHArmorLv2");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CHArmorLV2::Free()
{
    __super::Free();

    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pState_Manager);
}

#pragma region Intro_State

void CHArmorLV2::Intro_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 34;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Intro_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Idle_State(), pObject);
}

void CHArmorLV2::Intro_State::State_Exit(CHArmorLV2* pObject)
{
}

#pragma endregion

#pragma region Idle_State

void CHArmorLV2::Idle_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 26;
    pObject->m_bPatternProgress = false;
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Idle_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CHArmorLV2::Walk_State(), pObject);
}

void CHArmorLV2::Idle_State::State_Exit(CHArmorLV2* pObject)
{
}

#pragma endregion


#pragma region Walk_State


void CHArmorLV2::Walk_State::State_Enter(CHArmorLV2* pObject)
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
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CHArmorLV2::Walk_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (pObject->m_fDistance >= 2.f)
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    else
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

void CHArmorLV2::Walk_State::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_pModelCom->Set_LerpFinished(true);
}

#pragma endregion


#pragma region Run_State

void CHArmorLV2::Run_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 33;
    m_pPlayerNavi = static_cast<CNavigation*>(pObject->m_pPlayer->Find_Component(TEXT("Com_Navigation")));
    pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CHArmorLV2::Run_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (pObject->m_fDistance >= 10.f || pObject->m_pNavigationCom->Get_BestListSize() >= 1)
    {
        pObject->m_pNavigationCom->Start_Astar(m_pPlayerNavi->Get_CurCellIndex());
    }

    _vector vDir = XMVectorSetY(pObject->m_pNavigationCom->MoveAstar(pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), bCheck), 0.f);
    if (bCheck)
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

void CHArmorLV2::Run_State::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Stun_State
void CHArmorLV2::Stun_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 24;
    pObject->m_bCan_Move_Anim = true;
    pObject->RotateDegree_To_Player();
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Stun_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (m_iIndex == 23)
        m_fTime += fTimeDelta;


    if (m_iIndex == 24 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 23;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

    if (m_iIndex == 23 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 23;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

    if (m_iIndex == 23 && m_fTime >= 5.f)
    {
        m_iIndex = 22;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }
    else if (m_iIndex == 23 && pObject->m_fDistance <= 1.5f && pObject->m_pGameInstance->isMouseEnter(DIM_LB))
    {
        pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
    }

    if (m_iIndex == 22 && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_fShieldHP = pObject->m_fMonsterMaxHP / 2.f;
        pObject->m_IsStun = false;
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
    }
}

void CHArmorLV2::Stun_State::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion


#pragma region Attack_Pattern_01 (회전베기 후 앞 대쉬 이후 찍기 패턴)
void CHArmorLV2::Attack_Pattern_01::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 4;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_01::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (m_iIndex == 4 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 61.f)
    {
        m_iIndex = 8;
        pObject->RotateDegree_To_Player();
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 8 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 85.f)
    {
        m_iIndex = 7;
        pObject->RotateDegree_To_Player();
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
        pObject->m_pModelCom->Get_CurAnimation()->Set_StartOffSetTrackPosition(8.f);
    }

    if (m_iIndex == 7 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CHArmorLV2::Attack_Pattern_01::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_02 (회전베기 (오른쪽으로끝남) 이후 날리는 콤보 패턴)
void CHArmorLV2::Attack_Pattern_02::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 5;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_02::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (m_iIndex == 5 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 57.f)
    {
        m_iIndex = 0;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 0 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CHArmorLV2::Attack_Pattern_02::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_03 (한번 찌르고 뒤로 백스탭 패턴)
void CHArmorLV2::Attack_Pattern_03::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 3;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_03::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 36;
        pObject->RotateDegree_To_Player();
        pObject->m_bCan_Move_Anim = true;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 36 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CHArmorLV2::Attack_Pattern_03::State_Exit(CHArmorLV2* pObject)
{
    pObject->m_bCan_Move_Anim = false;
}
#pragma endregion


#pragma region Attack_Pattern_04 (Turn 공격)
void CHArmorLV2::Attack_Pattern_04::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 39;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_04::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CHArmorLV2::Attack_Pattern_04::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_05 (멀리있을때 점프뛰고 날리는 콤보 패턴)
void CHArmorLV2::Attack_Pattern_05::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 32;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KNOCKDOWN;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_05::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (m_iIndex == 32 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 70.f)
    {
        m_iIndex = 0;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 0 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);

}

void CHArmorLV2::Attack_Pattern_05::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_06(대각선으로 베고 난뒤 회전베기)
void CHArmorLV2::Attack_Pattern_06::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 1;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_06::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (m_iIndex == 1 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 49.f)
    {
        m_iIndex = 4;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 4 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CHArmorLV2::Attack_Pattern_06::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion


#pragma region Attack_Pattern_07(대각선 베기 한 뒤 날리기 패턴)
void CHArmorLV2::Attack_Pattern_07::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 1;
    pObject->RotateDegree_To_Player();
    pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTMFL;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Attack_Pattern_07::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    if (m_iIndex == 1 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 20.f)
    {
        m_iIndex = 0;
        pObject->RotateDegree_To_Player();
        pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
    }

    if (m_iIndex == 0 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CHArmorLV2::Attack_Pattern_07::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion

#pragma region Execution_State
void CHArmorLV2::Execution_State::State_Enter(CHArmorLV2* pObject)
{
    m_iIndex = 53;
    pObject->RotateDegree_To_Player();
    pObject->m_bHP_Bar_Active = false;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CHArmorLV2::Execution_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
    //죽음 처리
}

void CHArmorLV2::Execution_State::State_Exit(CHArmorLV2* pObject)
{
}
#pragma endregion

void CHArmorLV2::Hit_State::State_Enter(CHArmorLV2* pObject)
{
}

void CHArmorLV2::Hit_State::State_Update(_float fTimeDelta, CHArmorLV2* pObject)
{
}

void CHArmorLV2::Hit_State::State_Exit(CHArmorLV2* pObject)
{
}
