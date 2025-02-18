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

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 232.f,62.f,200.f,1.f });
    m_pTransformCom->Scaling(_float3{ 0.045f, 0.045f, 0.045f });


    return S_OK;
}

void CBoss_Varg::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CBoss_Varg::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

}

void CBoss_Varg::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Varg::Render()
{
    return S_OK;
}

HRESULT CBoss_Varg::Ready_Components()
{
    return S_OK;
}

HRESULT CBoss_Varg::Ready_PartObjects()
{
    CBody_Varg::BODY_VARG_DESC BodyDesc{};
    BodyDesc.pParentState = &m_iState;
    BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.fSpeedPerSec = 0.f;
    BodyDesc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Varg"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg_Body"), &BodyDesc)))
        return E_FAIL;

    CVargKnife::VARGKNIFE_DESC		Varg_Knife_Desc{};

    CModel* pBodyModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Varg"), TEXT("Com_Model")));
    if (nullptr == pBodyModelCom)
        return E_FAIL;

    Varg_Knife_Desc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r");
    Varg_Knife_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    Varg_Knife_Desc.fSpeedPerSec = 0.f;
    Varg_Knife_Desc.fRotationPerSec = 0.f;

    if (FAILED(__super::Add_PartObject(TEXT("Part_Varg_Knife"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg_Knife"), &Varg_Knife_Desc)))
        return E_FAIL;

    return S_OK;

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
}
