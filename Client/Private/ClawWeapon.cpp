#include "pch.h" 
#include "ClawWeapon.h"     
#include "GameInstance.h"   
#include "Player.h"
#include "Animation.h"

CClawWeapon::CClawWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CClawWeapon::CClawWeapon(const CClawWeapon& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CClawWeapon::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CClawWeapon::Initialize(void* pArg)
{

    strcpy_s(m_szName, "PLAYER_CLAW_WEAPON");

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 0.01f,0.1f,0.01f }, _float3{ 0.f,0.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 30.f,0.f,0.f,1.f });



    return S_OK;
}

void CClawWeapon::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

}

void CClawWeapon::Update(_float fTimeDelta)
{

    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);


    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* ���� ���� */
        SocketMatrix *  /* ���� �����̽� ���� */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* ���� ���� */
    );

    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
    {
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 0.f, 0.f,0.f,1.f });
    }


#pragma region �̺�Ʈ ���� �۾�

    /* 3�� 6�� �߰� �۾� ��  �� �������� ���̵�� ������ */
    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01 || *m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02)
    {

        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true) // EVENT_COLLIDER �κ�      
                {
                    // �� ���������� ��� ����        
                    m_pGameInstance->Add_Actor_Scene(m_pActor);
                }

                else
                {
                    m_pGameInstance->Sub_Actor_Scene(m_pActor);
                }

                if (iter.eType != EVENT_COLLIDER && iter.isEventActivate == true && iter.isPlay == false)  // ���Ⱑ EVENT_EFFECT, EVENT_SOUND, EVENT_STATE �κ�    
                {
                    iter.isPlay = true;      // �� ���� ��� �Ǿ�� �ϹǷ�         
                }


            }
        }
    }
#pragma endregion  

}

void CClawWeapon::Late_Update(_float fTimeDelta)
{
}

HRESULT CClawWeapon::Render()
{

    return S_OK;
}

HRESULT CClawWeapon::Ready_Components()
{

    return S_OK;
}

HRESULT CClawWeapon::Bind_ShaderResources()
{

    return S_OK;
}

void CClawWeapon::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{

}

void CClawWeapon::OnCollision(CGameObject* _pOther, PxContactPair _information)
{

}

void CClawWeapon::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{

}

CClawWeapon* CClawWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CClawWeapon* pInstance = new CClawWeapon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CClawWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CClawWeapon::Clone(void* pArg)
{
    CClawWeapon* pInstance = new CClawWeapon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CClawWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CClawWeapon::Free()
{
    __super::Free();
}
