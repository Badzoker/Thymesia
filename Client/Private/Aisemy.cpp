#include "pch.h"
#include "Aisemy.h"
#include "GameInstance.h"
#include "Body_Aisemy.h"

CAisemy::CAisemy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject(pDevice, pContext)
{
}

CAisemy::CAisemy(const CAisemy& Prototype)
    :CContainerObject(Prototype)
{
}

HRESULT CAisemy::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CAisemy::Initialize(void* pArg)
{
    strcpy_s(m_szName, "NPC");

    CGameObject::GAMEOBJECT_DESC        Desc{};

    Desc.fSpeedPerSec = 1.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"), "PLAYER");

    _vector vFirst_Pos = { 70.7f, 1.3f, -110.5f, 1.0f};
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vFirst_Pos);
    m_pNavigationCom->Set_CurrentNaviIndex(vFirst_Pos);
    m_pTransformCom->Scaling(_float3{ 0.002f, 0.002f, 0.002f });


    m_pState_Manager = CState_Machine<CAisemy>::Create();
    if (m_pState_Manager == nullptr)
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    return S_OK;
}

void CAisemy::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
    m_vPlayerPos = m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
    _vector pPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_fDistance = XMVectorGetX(XMVector3Length(m_vPlayerPos - pPosition));

    if (m_fDistance <= 20.f && !m_bActive)
    {
        m_pState_Manager->ChangeState(new CAisemy::Intro_State(), this);
    }

    __super::Priority_Update(fTimeDelta);
}

void CAisemy::Update(_float fTimeDelta)
{

    m_pState_Manager->State_Update(fTimeDelta, this);

    _vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

    __super::Update(fTimeDelta);
}

void CAisemy::Late_Update(_float fTimeDelta)
{
    if (m_bNeed_Rotation)
        Rotation_To_Player();

    if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.1f, FRUSTUM_TYPE::FRUSTUM_MONSTER))
    {
        __super::Late_Update(fTimeDelta);
    }
}

HRESULT CAisemy::Render()
{
    return S_OK;
}

HRESULT CAisemy::Ready_Components()
{
    /* Com_Navigation */
    CNavigation::NAVIGATION_DESC   Desc{};

    Desc.iCurrentCellIndex = 0;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CAisemy::Ready_PartObjects()
{
    CBody_Aisemy::BODY_Aisemy_DESC BodyDesc{};

    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Aisemy"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_NPC_Aisemy_Body"), &BodyDesc)))
        return E_FAIL;

    return S_OK;
}

void CAisemy::RootAnimation()
{
}

void CAisemy::RotateDegree_To_Player()
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

void CAisemy::Rotation_To_Player()
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

void CAisemy::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
}

void CAisemy::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CAisemy::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CAisemy* CAisemy::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAisemy* pInstance = new CAisemy(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CAisemy");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAisemy::Clone(void* pArg)
{
    CAisemy* pInstance = new CAisemy(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CAisemy");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAisemy::Free()
{
    __super::Free();

    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pState_Manager);
}

void CAisemy::Idle_State::State_Enter(CAisemy* pObject)
{
    m_iIndex = 0;
    pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CAisemy::Idle_State::State_Update(_float fTimeDelta, CAisemy* pObject)
{
    if (m_iIndex == 0 && pObject->m_pModelCom->GetAniFinish())
    {
        m_iIndex = 1;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
    if (m_iIndex != 0)
        pObject->RotateDegree_To_Player();

    if (pObject->m_fRotateDegree > 5.f)
    {
        m_iIndex = 2;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
    else if (pObject->m_fRotateDegree < -5.f)
    {
        m_iIndex = 3;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }
    else
    {
        m_iIndex = 1;
        pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
    }

}

void CAisemy::Idle_State::State_Exit(CAisemy* pObject)
{
}

void CAisemy::Intro_State::State_Enter(CAisemy* pObject)
{
    //m_iIndex = 0;
    //pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CAisemy::Intro_State::State_Update(_float fTimeDelta, CAisemy* pObject)
{
   /* if (pObject->m_pModelCom->GetAniFinish())
    {

    }*/

}

void CAisemy::Intro_State::State_Exit(CAisemy* pObject)
{
}
