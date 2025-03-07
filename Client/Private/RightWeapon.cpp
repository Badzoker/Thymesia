#include "pch.h" 
#include "RightWeapon.h"
#include "GameInstance.h"
#include "Player.h"
#include "Animation.h"

CRightWeapon::CRightWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CRightWeapon::CRightWeapon(const CRightWeapon& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CRightWeapon::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CRightWeapon::Initialize(void* pArg)
{

    strcpy_s(m_szName, "PLAYER_RIGHT_WEAPON");

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.01f,0.2f,0.f }, _float3{ 0.f,0.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 2.f,0.f,0.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::MONSTER | GROUP_TYPE::MONSTER_WEAPON;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::PLAYER_WEAPON, settingColliderGroup);


    return S_OK;

}

void CRightWeapon::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

}

void CRightWeapon::Update(_float fTimeDelta)
{

    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    //for (size_t i = 0; i < 3; i++)  
    //    SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);  

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );



    /* 콜라이더 위치 세부 조정 */

    /*
    _matrix CombinedMatrix = XMLoadFloat4x4(&m_CombinedWorldMatrix);
    CombinedMatrix.r[3] = { m_CombinedWorldMatrix._41 + 0.5f,m_CombinedWorldMatrix._42 ,m_CombinedWorldMatrix._43,1.f };*/

    m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 50.f, 0.f,0.f,1.f });


#pragma region 이벤트 관련 작업

    /* 3월 6일 추가 작업 및  이 방향으로 아이디어 나가기 */
    for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
    {
        if (iter.isPlay == false)
        {
            if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true) // EVENT_COLLIDER 부분      
            {
                // 그 구간에서는 계속 진행        
                m_pGameInstance->Add_Actor_Scene(m_pActor);
            }

            else
            {
                m_pGameInstance->Sub_Actor_Scene(m_pActor);
            }

            if (iter.eType != EVENT_COLLIDER && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
            {
                iter.isPlay = true;      // 한 번만 재생 되어야 하므로         
            }


        }
    }
#pragma endregion  
}

void CRightWeapon::Late_Update(_float fTimeDelta)
{

    if (*m_pParentState != CPlayer::STATE_ATTACK_LONG_CLAW_01
        && *m_pParentState != CPlayer::STATE_ATTACK_LONG_CLAW_02)
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    }

}

HRESULT CRightWeapon::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
    //if(*m_pParentState & (CPlayer::STATE_ATTACK | CPlayer::STATE_ATTACK2) && m_AccColliderLife < 0.4f)
    //    m_pColliderCom->Render();
    //if ((*m_pParentState & CPlayer::STATE_ATTACK) && m_AccColliderLifeAttack1 < 0.4f || (*m_pParentState & CPlayer::STATE_ATTACK2) && m_AccColliderLifeAttack2 < 0.2f)  
    //    m_pColliderCom->Render();       
#endif // DEBUG 


    return S_OK;
}

HRESULT CRightWeapon::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Right_Weapon"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CRightWeapon::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CRightWeapon::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{


}

void CRightWeapon::OnCollision(CGameObject* _pOther, PxContactPair _information)
{

}

void CRightWeapon::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{

}

CRightWeapon* CRightWeapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRightWeapon* pInstance = new CRightWeapon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CRightWeapon::Clone(void* pArg)
{
    CRightWeapon* pInstance = new CRightWeapon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CRightWeapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRightWeapon::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
