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

    CGameObject::GAMEOBJECT_DESC Desc = {};

    Desc.fSpeedPerSec = 1.0f;
    Desc.fRotationPerSec = XMConvertToRadians(90.0f);

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_iState = STATE_GOSEMY_APPROACH;

    _vector vTestPosition = { 83.19f, 5.3f, -117.27f, 1.f };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTestPosition);

    //m_pTransformCom->Get_State(CTransform::StatE_rota)

    m_pTransformCom->Scaling(_float3{ 0.002f, 0.002f, 0.002f });

    m_fApproachTime = 2.0f;

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
    _vector vGhoSemyPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    switch (m_iState)
    {
    case STATE_GOSEMY_APPROACH:
        Setting_Approach(_fTimeDelta);
        break;
    }


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

void CGhostAisemy::Spawn_Gosemy(_float4 _vPos)
{
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

    LampDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("ik_hand_gun");
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

void CGhostAisemy::Setting_Approach(_float _fTimeDelta)
{
    m_fApproachTime -= _fTimeDelta;

    if (m_fApproachTime <= 0.0f)
    {
        m_fApproachTime = 2.0f;

        /* if (m_pBody_GhoSemy->Get_AnimationStop())
         {
             m_pLamp->Activate_Lamp(true);
         }*/
    }
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
