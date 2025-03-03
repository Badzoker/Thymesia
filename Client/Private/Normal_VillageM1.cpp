#include "pch.h"
#include "Normal_VillageM1.h"
#include "VillageM_Weapon.h"
#include "Body_VillageM1.h"
#include "GameInstance.h"

CNormal_VillageM1::CNormal_VillageM1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject(pDevice, pContext)
{
}

CNormal_VillageM1::CNormal_VillageM1(const CNormal_VillageM1& Prototype)
    :CContainerObject(Prototype)
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
    strcpy_s(m_szName, "VILLAGEM1");

    CGameObject::GAMEOBJECT_DESC        Desc{};

    Desc.fSpeedPerSec = 1.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_pGameInstance->Add_ObjCollider(GROUP_TYPE::MONSTER, this);
    m_pPlayer = m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));

    _vector vFirst_Pos = { 105.6f, 1.85f, 28.8f, 1.0f };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFirst_Pos);
    m_pNavigationCom->Set_CurrentNaviIndex(vFirst_Pos);
    m_pTransformCom->Scaling(_float3{ 0.002f, 0.002f, 0.002f });


    m_pState_Manager = CState_Machine<CNormal_VillageM1>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;


    return S_OK;
}

void CNormal_VillageM1::Priority_Update(_float fTimeDelta)
{

    //�÷��̾���� �Ÿ� ���
    m_fTimeDelta = fTimeDelta;
    m_vPlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
    _vector pPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_fDistance = XMVectorGetX(XMVector3Length(m_vPlayerPos - pPosition));

    //�Ÿ������� Active Ȱ��ȭ
    if (m_fDistance <= 15.f && !m_bActive)
    {
        m_bActive = true;
        m_pState_Manager->ChangeState(new CNormal_VillageM1::Idle_State(), this);
    }

    __super::Priority_Update(fTimeDelta);
}

void CNormal_VillageM1::Update(_float fTimeDelta)
{
   PatternCreate();
   RootAnimation();

   m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
   _vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
   m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));
   m_pState_Manager->State_Update(fTimeDelta, this);
    __super::Update(fTimeDelta);
}

void CNormal_VillageM1::Late_Update(_float fTimeDelta)
{
    if (m_bNeed_Rotation)
        Rotation_To_Player();
#ifdef _DEBUG
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
#endif 
    if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.1f))
    {
        __super::Late_Update(fTimeDelta);
    }
}

HRESULT CNormal_VillageM1::Render()
{
#ifdef _DEBUG
    m_pColliderCom->Render();
#endif 
    return S_OK;
}

HRESULT CNormal_VillageM1::Ready_Components()
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


    m_pColliderCom->Set_Collider_Name("Monster");

    return S_OK;
}

HRESULT CNormal_VillageM1::Ready_PartObjects()
{
    CBody_VillageM1::BODY_VillageM1_DESC BodyDesc = {};
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_VillageM1"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageM1_Body"), &BodyDesc)))
        return E_FAIL;

    CVillageM_Weapon::VillageM_WEAPON_DESC		VillageM_Weapon_Desc = {};

    m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_VillageM1"), TEXT("Com_Model")));
    if (nullptr == m_pModelCom)
        return E_FAIL;

    VillageM_Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
    VillageM_Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    VillageM_Weapon_Desc.fSpeedPerSec = 0.f;
    VillageM_Weapon_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_VillageM_Weapon"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageM_Weapon"), &VillageM_Weapon_Desc)))
        return E_FAIL;

    return S_OK;
}

void CNormal_VillageM1::RootAnimation()
{
    /* ��Ʈ ��� �ִϸ޼� �ڵ� */
    _vector      vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector test = { 0.f,0.f,0.f,1.f };
    m_pRootMatrix = m_pModelCom->Get_RootMotionMatrix("root");
    _uint iTest = m_pModelCom->Get_Current_Animation_Index();
    if ((!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModelCom->Get_LerpFinished()))
    {
        if ((m_pNavigationCom->isMove(vCurPosition) && m_fDistance > 1.5f) && !m_bNeedControl)
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

void CNormal_VillageM1::RotateDegree_To_Player()
{
    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
    _vector vLook2 = m_vPlayerPos - vPos;

    vLook = XMVector3Normalize(vLook);
    vLook2 = XMVector3Normalize(vLook2);

    //ȸ���ؾ� �ϴ� ����
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

void CNormal_VillageM1::Rotation_To_Player()
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

void CNormal_VillageM1::OnCollisionEnter(CGameObject* _pOther)
{
}

void CNormal_VillageM1::OnCollision(CGameObject* _pOther)
{
}

void CNormal_VillageM1::OnCollisionExit(CGameObject* _pOther)
{
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

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pState_Manager);
}

#pragma region Idle_State

void CNormal_VillageM1::Idle_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 30;
    pObject->m_bPatternProgress = false;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Idle_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f && pObject->m_fDistance >= 10.f)
    {
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
    }
    else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f && pObject->m_fDistance < 10.f)
    {
        pObject->m_pState_Manager->ChangeState(new Move_State(), pObject);
    }
}

void CNormal_VillageM1::Idle_State::State_Exit(CNormal_VillageM1* pObject)
{
}

#pragma endregion

#pragma region Move_State
void CNormal_VillageM1::Move_State::State_Enter(CNormal_VillageM1* pObject)
{
    if (pObject->m_fDistance > 1.5f)
        m_iIndex = 43;
    else if (pObject->m_fDistance < 1.5f)
        m_iIndex = 41;
    else
    {
        _uint iRandom = rand() % 2;
        switch (iRandom)
        {
        case 0:
            m_iIndex = 46;
            break;
        case 1:
            m_iIndex = 47;
            break;
        }
    }
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_VillageM1::Move_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
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

    if (pObject->m_pNavigationCom->Get_BestListSize() >= 2.f && pObject->m_fDistance >= 10.f)
        pObject->m_pState_Manager->ChangeState(new Run_State(), pObject);
}

void CNormal_VillageM1::Move_State::State_Exit(CNormal_VillageM1* pObject)
{
}
#pragma endregion

#pragma region Run_State
void CNormal_VillageM1::Run_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 1;
    CNavigation* pPlayerNavi = static_cast<CNavigation*>(pObject->m_pPlayer->Find_Component(TEXT("Com_Navigation")));
    pObject->m_pNavigationCom->Start_Astar(pPlayerNavi->Get_CurCellIndex());
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CNormal_VillageM1::Run_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    _vector vDir = XMVectorSetY(pObject->m_pNavigationCom->MoveAstar(pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION), bCheck), 0.f);
    if (bCheck)
    {
        pObject->m_pTransformCom->LookAt_Astar(vDir);
        pObject->m_pTransformCom->Go_Straight(fTimeDelta * 2.f, pObject->m_pNavigationCom);
    }
    else
    {
        pObject->RotateDegree_To_Player();
        pObject->m_pTransformCom->Go_Straight(fTimeDelta * 2.f, pObject->m_pNavigationCom);
    }
}

void CNormal_VillageM1::Run_State::State_Exit(CNormal_VillageM1* pObject)
{
}

#pragma endregion 

#pragma region Stun_State
void CNormal_VillageM1::Stun_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 29;
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
    else if (m_iIndex == 28 && pObject->m_fDistance <= 1.5f && pObject->m_pGameInstance->isMouseEnter(DIM_LB))
    {
        pObject->m_pState_Manager->ChangeState(new Execution_State(), pObject);
    }

    if (m_iIndex == 27 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM1::Stun_State::State_Exit(CNormal_VillageM1* pObject)
{
}
#pragma endregion

#pragma region Attack_01_State
void CNormal_VillageM1::Attack_01_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 4;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Attack_01_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (m_iIndex == 4 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM1::Attack_01_State::State_Exit(CNormal_VillageM1* pObject)
{
}
#pragma endregion

#pragma region Attack_02_State
void CNormal_VillageM1::Attack_02_State::State_Enter(CNormal_VillageM1* pObject)
{
    m_iIndex = 5;
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
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CNormal_VillageM1::Attack_03_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
    if (m_iIndex == 8 && pObject->m_pModelCom->GetAniFinish())
        pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CNormal_VillageM1::Attack_03_State::State_Exit(CNormal_VillageM1* pObject)
{
}

#pragma endregion
#pragma region Hit_State
void CNormal_VillageM1::Hit_State::State_Enter(CNormal_VillageM1* pObject)
{
    _uint iRandomHit = rand() % 2;
    switch (iRandomHit)
    {
    case 0:
        m_iIndex = 25;
        break;
    case 1:
        m_iIndex = 26;
        break;
    }
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
}

void CNormal_VillageM1::Execution_State::State_Update(_float fTimeDelta, CNormal_VillageM1* pObject)
{
}

void CNormal_VillageM1::Execution_State::State_Exit(CNormal_VillageM1* pObject)
{
}
#pragma endregion


