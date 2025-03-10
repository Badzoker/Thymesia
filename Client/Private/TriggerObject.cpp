#include "pch.h"
#include "TriggerObject.h"
#include "GameInstance.h"

CTriggerObject::CTriggerObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CGameObject{ _pDevice, _pContext }
{
}

CTriggerObject::CTriggerObject(const CTriggerObject& _Prototype)
    :CGameObject(_Prototype)
{
}

HRESULT CTriggerObject::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTriggerObject::Initialize(void* _pArg)
{
    TC_DESC* pDesc = static_cast<TC_DESC*>(_pArg);

    m_tagInfoTempCollider.fPosition = pDesc->fPosition;
    m_tagInfoTempCollider.fRotation = pDesc->fRotation;
    m_tagInfoTempCollider.fScale = pDesc->fScale;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_tagInfoTempCollider.fPosition.x, m_tagInfoTempCollider.fPosition.y, m_tagInfoTempCollider.fPosition.z, m_tagInfoTempCollider.fPosition.w));

    return S_OK;
}

void CTriggerObject::Priority_Update(_float _fTimeDelta)
{
}

void CTriggerObject::Update(_float _fTimeDelta)
{
    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));
}

void CTriggerObject::Late_Update(_float _fTimeDelta)
{
   /* if (Check_Collision_With_Player())
    {
        if (!m_bFade)
        {
            m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 2.0f);
            m_bFade = true;
        }
    }
    else
    {
        if (m_bFade)
        {
            m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 2.0f);
            m_bFade = false;
        }
    }*/
    if (m_pGameInstance->Get_DIKeyState(DIK_1) & 0x80)
    {
        m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_OUT, 0.5f);
    }
    else if (m_pGameInstance->Get_DIKeyState(DIK_2) & 0x80)
    {
        m_pGameInstance->Activate_Fade(TRIGGER_TYPE::TT_FADE_IN, 2.0f);
    }

    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CTriggerObject::Render()
{
    m_pColliderCom->Render();
    return S_OK;
}

HRESULT CTriggerObject::Ready_Components()
{
    CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc{};
    ColliderDesc.vExtents = _float3(0.3f, -0.3f, 0.3f);
    ColliderDesc.vCenter = _float3(0.f, 0.0f, 0.f);

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"), TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

_bool CTriggerObject::Check_Collision_With_Player()
{
    if (nullptr == m_pColliderCom)
        return false;

    CCollider* pTargetCollider = static_cast<CCollider*>(m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider")));

    if (nullptr == pTargetCollider)
        return false;

    return m_pColliderCom->Intersect(pTargetCollider);
}

CTriggerObject* CTriggerObject::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CTriggerObject* pInstance = new CTriggerObject(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CTriggerObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTriggerObject::Clone(void* _pArg)
{
    CTriggerObject* pInstance = new CTriggerObject(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CTriggerObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTriggerObject::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
}
