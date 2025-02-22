#include "pch.h"
#include "Boss_Varg.h"
#include "Body_Varg.h"
#include "VargKnife.h"
#include "GameInstance.h"

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
    strcpy_s(m_szName, "BOSS_VARG");

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
    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, this);

    _vector vFirst_Pos = { 112.6f, 1.85f, 28.8f, 1.0f };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFirst_Pos);
    m_pNavigationCom->Set_CurrentNaviIndex(vFirst_Pos);

    m_pTransformCom->Scaling(_float3{ 0.002f, 0.002f, 0.002f });

    m_pState_Manager = CBoss_State_Manager<CBoss_Varg>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pState_Manager->ChangeState(new CBoss_Varg::Not_Active_State(), this);
    return S_OK;
}

void CBoss_Varg::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

    if (m_pGameInstance->isKeyEnter(DIK_P))
    {
        //�ӽÿ����� p��ư ������ ���� Active
        //���߿� �÷��̾� ��ǥ�� ���� Active �ҵ�.
        m_iPhase = 1;
        m_pState_Manager->ChangeState(new CBoss_Varg::Intro_State(), this);
    }
    if (m_pGameInstance->isKeyEnter(DIK_I))
    {
        //�ӽÿ����� I��ư���� ������ ��ȯ
        //���߿� �÷��̾�� ó������?���� ��ȯ ����.
        m_fDelayTime = 0.f;
        m_bPatternProgress = true;
        m_pState_Manager->ChangeState(new CBoss_Varg::ExeCution_State(), this);
    }
    if (m_pGameInstance->isKeyEnter(DIK_K))
    {
        //�ӽÿ����� k��ư ������ �ǰݸ�� ����
        m_pState_Manager->ChangeState(new CBoss_Varg::Hit_State(), this);
    }

    m_vPlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
    _vector pPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_fDistance = XMVectorGetX(XMVector3Length(m_vPlayerPos - pPosition));
    if (m_fDistance <= 2.f)
        m_bCrush = true;
    else
        m_bCrush = false;


    __super::Priority_Update(fTimeDelta);
}

void CBoss_Varg::Update(_float fTimeDelta)
{
    Special_PatternCreate();
    PatternCreate();

    m_pState_Manager->State_Update(fTimeDelta, this);

    _vector      vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector test = { 0.f,0.f,0.f,1.f };
    /* ��Ʈ ��� �ִϸ޼� �ڵ� */
    m_pRootMatrix = m_pModelCom->Get_RootMotionMatrix("root");

    if (!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModelCom->Get_LerpFinished() && m_bBossActive/*&& m_iState != STATE_IDLE*/)
    {
        if (m_pNavigationCom->isMove(vCurPosition) && !m_bCrush)
            m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix);

        /* 2�� 19�� �߰� �ڵ� */
        if (!m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
        {
            _float4x4 test = {};
            XMStoreFloat4x4(&test, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pRootMatrix)));
            const _float4x4* test2 = const_cast<_float4x4*>(&test);
            m_pTransformCom->Set_MulWorldMatrix(test2);
        }
    }
    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

    _vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));
    __super::Update(fTimeDelta);

}

void CBoss_Varg::Late_Update(_float fTimeDelta)
{
#ifdef _DEBUG
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
#endif
    __super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Varg::Render()
{
#ifdef _DEBUG
    m_pColliderCom->Render();
#endif 
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

    /* Com_Collider */
    CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


    SphereDesc.fRadius = 300.f;
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius + 100.f, 0.f);


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
        TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
        return E_FAIL;


    m_pColliderCom->Set_Collider_Name("Boss_Varg");

    return S_OK;
}

HRESULT CBoss_Varg::Ready_PartObjects()
{
    CBody_Varg::BODY_VARG_DESC BodyDesc{};
    BodyDesc.pParentState = &m_iCurState;
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
    Varg_Knife_Desc.fSpeedPerSec = 0.f;
    Varg_Knife_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Varg_Knife"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg_Knife"), &Varg_Knife_Desc)))
        return E_FAIL;

    return S_OK;

}

void CBoss_Varg::PatternCreate()
{
    if (!m_bPatternProgress && m_bBossActive)
    {
        m_fDelayTime += 1 * m_fTimeDelta;
    }
    if (m_fDelayTime >= 2.f)
    {
        if (m_fDistance >= 10.f)
            Far_Pattern_Create();
        else
            Near_Pattern_Create();

        m_fDelayTime = 0.f;
        m_bPatternProgress = true;
    }
}

void CBoss_Varg::Special_PatternCreate()
{
    if (m_iPhase == 2 && m_bBossActive)
        m_fSpecial_Skill_CoolTime += 1 * m_fTimeDelta;
    if (m_fSpecial_Skill_CoolTime >= 30.f)
    {
        m_pState_Manager->ChangeState(new CBoss_Varg::Roar_State(false), this);
        m_fSpecial_Skill_CoolTime = 0.f;
        m_fDelayTime = 0.f;
        m_bPatternProgress = true;
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
        m_pState_Manager->ChangeState(new CBoss_Varg::Jump_Attack(), this);
        break;
    default:
        break;
    }
}

void CBoss_Varg::OnCollisionEnter(CGameObject* _pOther)
{
}

void CBoss_Varg::OnCollision(CGameObject* _pOther)
{
}

void CBoss_Varg::OnCollisionExit(CGameObject* _pOther)
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

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pState_Manager);
}


#pragma region Not_Active_State

void CBoss_Varg::Not_Active_State::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(20, true);
}

void CBoss_Varg::Not_Active_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
}

void CBoss_Varg::Not_Active_State::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Intro_State
void CBoss_Varg::Intro_State::State_Enter(CBoss_Varg* pObject)
{
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
    pObject->m_bBossActive = true;
}

#pragma endregion

#pragma region Idle_State
void CBoss_Varg::Idle_State::State_Enter(CBoss_Varg* pObject)
{
    //�ִϸ��̼� ����
    pObject->m_pModelCom->SetUp_Animation(19, false);
}

void CBoss_Varg::Idle_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //Input handle�̶�°� �ı⵵ �� 
    //�Է�ó���� ���� ���� 
    //�Ұ����ϰ� ���⼭ �ִϸ��̼� �����ؾ��Ҷ��� ����

    if (pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Walk_State(), pObject);
    }
}

void CBoss_Varg::Idle_State::State_Exit(CBoss_Varg* pObject)
{
    //���� ������ ����������
    //���࿡ Enter���� �߰��Ѱ͵� �� ����´���
    //ex) ����, ũ�� ������  
}

#pragma endregion

#pragma region Avoid_State
void CBoss_Varg::Avoid_State::State_Enter(CBoss_Varg* pObject)
{
    _uint iRandom_Avoid = (rand() % 3) + 4;
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
    pObject->m_pModelCom->SetUp_Animation(iRandom_Avoid, false);
}

void CBoss_Varg::Avoid_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        if (m_bBonusAttack)
        {
            pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
            pObject->m_pModelCom->SetUp_Animation(3, false);
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
    pObject->m_bPatternProgress = false;
}

#pragma endregion

#pragma region Hit_State
void CBoss_Varg::Hit_State::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(18, false);
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
    //���߿� �÷��̾� ��ġ�޾ƿõ�
    pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
    if (pObject->m_fDistance > 2.f)
    {
        m_iWalkIndex = 47;
        pObject->m_pModelCom->SetUp_Animation(m_iWalkIndex, true);
    }
    else if (pObject->m_fDistance < 2.f)
    {
        m_iWalkIndex = 46;
        pObject->m_pModelCom->SetUp_Animation(m_iWalkIndex, true);
    }
    else
    {
        m_iWalkIndex = 48;
        pObject->m_pModelCom->SetUp_Animation(m_iWalkIndex, true);
    }
}

void CBoss_Varg::Walk_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iWalkIndex == 47)
    {
        pObject->m_pTransformCom->Go_Straight(pObject->m_fTimeDelta);
    }
    else if (m_iWalkIndex == 46)
    {
        pObject->m_pTransformCom->Go_Backward(pObject->m_fTimeDelta);
    }
    else if (m_iWalkIndex == 48)
    {
        pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
        pObject->m_pTransformCom->Go_Right(pObject->m_fTimeDelta);
    }
}

void CBoss_Varg::Walk_State::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region ATTACK_COMBO_A

void CBoss_Varg::Attack_Combo_A::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
}

void CBoss_Varg::Attack_Combo_A::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
    {
        m_iComboIndex += 1;
        pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
        pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
    }
    if (m_iComboIndex > 9)
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State(), pObject);
}

void CBoss_Varg::Attack_Combo_A::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bPatternProgress = false;
}

#pragma endregion

#pragma region Attack_Combo_B
void CBoss_Varg::Attack_Combo_B::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
}

void CBoss_Varg::Attack_Combo_B::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iComboIndex == 10 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iComboIndex = 11;
        pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
        pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
    }

    if (m_iComboIndex == 11 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);

}

void CBoss_Varg::Attack_Combo_B::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bPatternProgress = false;
}

#pragma endregion

#pragma region Attack_Combo_C
void CBoss_Varg::Attack_Combo_C::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
}

void CBoss_Varg::Attack_Combo_C::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iComboIndex == 10 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iComboIndex = 12;
        pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
        pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
    }

    if (m_iComboIndex == 12 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
}

void CBoss_Varg::Attack_Combo_C::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bPatternProgress = false;
}

#pragma endregion

#pragma region Attack_Combo_D
void CBoss_Varg::Attack_Combo_D::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
}

void CBoss_Varg::Attack_Combo_D::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iComboIndex == 7 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iComboIndex = 10;
        pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
        pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
    }
    if (m_iComboIndex == 10 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iComboIndex = 12;
        pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
        pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
    }

    if (m_iComboIndex == 12 && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
    }
}

void CBoss_Varg::Attack_Combo_D::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bPatternProgress = false;
}

#pragma endregion

#pragma region Attack_Combo_E
void CBoss_Varg::Attack_Combo_E::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
}

void CBoss_Varg::Attack_Combo_E::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (m_iComboIndex == 10 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iComboIndex = 9;
        pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
        pObject->m_pModelCom->SetUp_Animation(m_iComboIndex, false);
    }
    if (m_iComboIndex == 9 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
}

void CBoss_Varg::Attack_Combo_E::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bPatternProgress = false;
}
#pragma endregion

#pragma region Run_State
void CBoss_Varg::Run_State::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(m_iRunIndex, false);
}

void CBoss_Varg::Run_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);
    if (m_iRunIndex == 25 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iRunIndex = 24;
        pObject->m_pModelCom->SetUp_Animation(m_iRunIndex, true);
    }
    if (pObject->m_bCrush)
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Attack_Combo_B(), pObject);

}

void CBoss_Varg::Run_State::State_Exit(CBoss_Varg* pObject)
{
}

#pragma endregion

#pragma region Jump_Attack
void CBoss_Varg::Jump_Attack::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(22, false);
}

void CBoss_Varg::Jump_Attack::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
}

void CBoss_Varg::Jump_Attack::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bPatternProgress = false;
}

#pragma endregion

void CBoss_Varg::ExeCution_State::State_Enter(CBoss_Varg* pObject)
{
    if (pObject->m_iPhase == 1)
    {
        m_iExeCutionIndex = 50;
        pObject->m_pModelCom->SetUp_Animation(m_iExeCutionIndex, false);
    }

}

void CBoss_Varg::ExeCution_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //���߿� ������ ���� ������ҵ�
    //1�������̰� �ִ� �������� ��ȯ��Ű��
    if (m_iExeCutionIndex == 50 && pObject->m_pModelCom->GetAniFinish())
    {
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Roar_State(true), pObject);
    }
}

void CBoss_Varg::ExeCution_State::State_Exit(CBoss_Varg* pObject)
{
    if (pObject->m_iPhase == 1)
        pObject->m_iPhase = 2;
}

CBoss_Varg::Roar_State::Roar_State(_bool m_bCheck)
    : m_bFirst(m_bCheck)
{
}

void CBoss_Varg::Roar_State::State_Enter(CBoss_Varg* pObject)
{
    if (m_bFirst)
        m_AnimIndex = 32;
    else
        m_AnimIndex = 31;

    pObject->m_pModelCom->SetUp_Animation(m_AnimIndex, false);
}

void CBoss_Varg::Roar_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Catch_State(), pObject);
}

void CBoss_Varg::Roar_State::State_Exit(CBoss_Varg* pObject)
{
}

void CBoss_Varg::Catch_State::State_Enter(CBoss_Varg* pObject)
{
    pObject->m_pModelCom->SetUp_Animation(30, false);
}

void CBoss_Varg::Catch_State::State_Update(_float fTimeDelta, CBoss_Varg* pObject)
{
    //�޷������߿� �÷��̾� �ε������
    pObject->m_pTransformCom->LookAt(pObject->m_vPlayerPos);

    if (pObject->m_bCrush)
        pObject->m_pModelCom->SetUp_Animation(28, false);
    //�Ⱥε����� ������ ����� ���
    if (pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new CBoss_Varg::Idle_State, pObject);
}

void CBoss_Varg::Catch_State::State_Exit(CBoss_Varg* pObject)
{
    pObject->m_bPatternProgress = false;
}

