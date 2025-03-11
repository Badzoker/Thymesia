#include "pch.h"
#include "VargKnife.h"
#include "GameInstance.h"
#include "Animation.h"

CVargKnife::CVargKnife(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject(pDevice, pContext)
{
}

CVargKnife::CVargKnife(const CVargKnife& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CVargKnife::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CVargKnife::Initialize(void* pArg)
{
    strcpy_s(m_szName, "MONSTER_WEAPON");

    VARGKNIFE_DESC* pDesc = static_cast<VARGKNIFE_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;



    m_pTransformCom->Scaling(_float3{ 0.5f, 0.5f, 0.5f });


    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.8f,0.15f }, _float3{ 0.f,1.f,0.f }, 90.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,100.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

    return S_OK;
}

void CVargKnife::Priority_Update(_float fTimeDelta)
{
}

void CVargKnife::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 100.f, 0.f,-350.f,1.f });

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

void CVargKnife::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CVargKnife::Render()
{

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CVargKnife::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Varg_Knife"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CVargKnife::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CVargKnife::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
}

void CVargKnife::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CVargKnife::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CVargKnife* CVargKnife::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVargKnife* pInstance = new CVargKnife(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CVargKnife");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVargKnife::Clone(void* pArg)
{
    CVargKnife* pInstance = new CVargKnife(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CVargKnife");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVargKnife::Free()
{
    __super::Free();


    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
