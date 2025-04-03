#include "pch.h"
#include "GhostAisemy.h"
#include "GameInstance.h"

CGhostAisemy::CGhostAisemy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    : CContainerObject{ _pDevice , _pContext }
{
}

CGhostAisemy::CGhostAisemy(const CGhostAisemy& _Prototype)
    :CContainerObject(_Prototype)
{
}

HRESULT CGhostAisemy::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CGhostAisemy::Initialize(void* pArg)
{
    strcpy_s(m_szName, "GHOSEMY");

    GHOST_SEMY_DESC* pDesc = static_cast<GHOST_SEMY_DESC*>(pArg);

    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

    if (FAILED(__super::Initialize(&pDesc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_iState = STATE_GOSEMY_APPROACH;

    m_pTransformCom->Scaling(_float3{ 0.002f, 0.002f, 0.002f });

    //m_fApproachTime = 2.0f;
    //m_fLightOnTime = 2.0f;

    m_pButtonGameObject = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Button"), "BUTTON");
    m_pButton = static_cast<CButton*>(m_pButtonGameObject);

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.3f,0.3f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

    _uint settingColliderGroup = GROUP_TYPE::PLAYER;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, settingColliderGroup);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });

    return S_OK;
}

void CGhostAisemy::Priority_Update(_float _fTimeDelta)
{
    Culling();
    if (m_bCulling)
        return;

    __super::Priority_Update(_fTimeDelta);
}

void CGhostAisemy::Update(_float _fTimeDelta)
{
    if (m_pBody_GhoSemy->IsActivate())
    {
        _vector vGhoSemyPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

        if (m_bColliderOn)
        {
            if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
                m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ -150.f, 250.f,200.f,1.f });
        }

        switch (m_iState)
        {
        case STATE_GOSEMY_APPROACH:
            Setting_Approach(_fTimeDelta);
            break;
        case STATE_GOSEMY_LIGHT_OFF:
            Setting_LightOff(_fTimeDelta);
            break;
        case STATE_GOSEMY_LIGHT_ON:
        {
            if (m_pBody_GhoSemy->IsAnimationFinish())
                m_iState = STATE_GOSEMY_LIGHT_LOOP;
        }
        break;
        case STATE_GOSEMY_LIGHT_LOOP:
            Setting_LightLoop(_fTimeDelta);
            break;
        }
    }


    // Test
    _vector vGhoSemyPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    _float fX = XMVectorGetX(vGhoSemyPosition);
    vGhoSemyPosition = XMVectorSetX(vGhoSemyPosition, fX + 3.0f);

    _float4 vPos;
    XMStoreFloat4(&vPos, vGhoSemyPosition);
    if (m_pGameInstance->isKeyEnter(DIK_8))
        Spawn_Conversation_Gosemy(vPos, true);

    __super::Update(_fTimeDelta);
}

void CGhostAisemy::Late_Update(_float _fTimeDelta)
{
    if (m_bCulling)
        return;

    __super::Late_Update(_fTimeDelta);
}

HRESULT CGhostAisemy::Render()
{
    return S_OK;
}

void CGhostAisemy::Spawn_Gosemy(_float4 _vPos, _bool _bFirstAppear)
{
    if (_bFirstAppear)
        m_iState = STATE_GOSEMY_APPROACH;
    else
        m_iState = STATE_GOSEMY_LIGHT_LOOP;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_vPos));
}

void CGhostAisemy::Spawn_Conversation_Gosemy(_float4 _vPos, _bool _bColliderOn)
{
    m_iState = STATE_GOSEMY_LIGHT_OFF;
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_vPos));
}

HRESULT CGhostAisemy::Ready_Components()
{
    return S_OK;
}

HRESULT CGhostAisemy::Ready_PartObjects()
{
    CBody_GhostSemy::BODY_Ghost_Aisemy_DESC BodyDesc{};
    BodyDesc.pParentState = &m_iState;
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_GhoSemy"), LEVEL_STATIC, TEXT("Prototype_GameObject_GhoSemy_Body"), &BodyDesc)))
        return E_FAIL;

    m_pBody_GhoSemy = static_cast<CBody_GhostSemy*>(Find_PartObject(TEXT("Part_Body_GhoSemy")));
    if (nullptr == m_pBody_GhoSemy)
        return E_FAIL;

#pragma region LAMP
    CLamp::LAMP_DESC        LampDesc = {};
    CModel* pBodyModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_GhoSemy"), TEXT("Com_Model")));
    if (nullptr == pBodyModelCom)
        return E_FAIL;

    LampDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("ik_hand_l");
    LampDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

    if (FAILED(__super::Add_PartObject(TEXT("Part_Lamp"), LEVEL_STATIC, TEXT("Prototype_GameObject_Lamp"), &LampDesc)))
        return E_FAIL;

    m_pLamp = static_cast<CLamp*>(Find_PartObject(TEXT("Part_Lamp")));
    if (m_pLamp == nullptr)
        return E_FAIL;
#pragma endregion

    return S_OK;
}

void CGhostAisemy::Culling()
{
    if (!m_bActive)
    {
        if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.1f, FRUSTUM_TYPE::FRUSTUM_MONSTER))
        {
            m_bCulling = false;
        }
        else
        {
            m_bCulling = true;
        }
    }
}

void CGhostAisemy::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    _vector vItemPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vItemPos = XMVectorSetY(vItemPos, XMVectorGetY(vItemPos) + 1.0f);

    _float4 vItemPosition;
    XMStoreFloat4(&vItemPosition, vItemPos);

    m_pButton->Set_WorldPosition(vItemPosition);
    m_pButton->Set_ButtonText(TEXT("E"), TEXT("작업걸기"));
    m_pButton->Activate_Button(true);
}

void CGhostAisemy::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    if (m_pGameInstance->isKeyEnter(DIK_E))
    {
        m_pButton->Activate_Button(false);


        // UI 뜨면될듯



    }
}

void CGhostAisemy::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_pButton->Activate_Button(false);
}

void CGhostAisemy::Setting_Approach(_float _fTimeDelta)
{
    m_pLamp->LightUp_Lamp(false);

    //m_fApproachTime -= _fTimeDelta;

    //if (m_fApproachTime <= 0.0f)
    //{
    //    m_fApproachTime = 2.0f;

    //}

    if (m_pBody_GhoSemy->IsAnimationFinish())
        m_iState = STATE_GOSEMY_LIGHT_ON;
}

void CGhostAisemy::Setting_LightOn(_float _fTimeDelta)
{
    //m_fLightOnTime -= _fTimeDelta;

    //if (m_fLightOnTime <= 0.0f)
    //{
    //    m_fLightOnTime = 2.0f;

    //}
    m_iState = STATE_GOSEMY_LIGHT_LOOP;
}

void CGhostAisemy::Setting_LightOff(_float _fTimeDelta)
{
    //m_iState = STATE_GOSEMY_LIGHT_OFF;

    m_bColliderOn = true;
    m_pGameInstance->Add_Actor_Scene(m_pActor);

    m_pLamp->LightUp_Lamp(false);

}

void CGhostAisemy::Setting_LightLoop(_float _fTimeDelta)
{
    //m_pBody_GhoSemy->Set_AnimationStop(true);

    m_bColliderOn = false;
    m_pGameInstance->Sub_Actor_Scene(m_pActor);


    m_iState = STATE_GOSEMY_LIGHT_LOOP;


    m_pLamp->LightUp_Lamp(true);

}

CGhostAisemy* CGhostAisemy::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CGhostAisemy* pInstance = new CGhostAisemy(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CGhostAisemy");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGhostAisemy::Clone(void* _pArg)
{
    CGhostAisemy* pInstance = new CGhostAisemy(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CGhostAisemy");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGhostAisemy::Free()
{
    __super::Free();
}