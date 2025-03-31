#include "pch.h" 
#include "Weapon_Halberd.h"
#include "GameInstance.h"
#include "Player.h"
#include "Animation.h"
#include "Camera_Free.h"

CWeapon_Halberd::CWeapon_Halberd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CWeapon_Halberd::CWeapon_Halberd(const CWeapon_Halberd& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CWeapon_Halberd::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CWeapon_Halberd::Initialize(void* pArg)
{

    strcpy_s(m_szName, "PLAYER_WEAPON");

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_pParentPhaseState = pDesc->pParentPhaseState;
    m_pPreParentState = pDesc->pPreParentState;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Rotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(-90.f));

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.06f,0.6f,0.f }, _float3{ 0.f,0.f,0.f }, 0.f, this);

    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 2.f,0.f,0.f,1.f });

    _uint settingColliderGroup = GROUP_TYPE::MONSTER;

    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::PLAYER_WEAPON, settingColliderGroup);

    m_iCurrentLevel = static_cast<LEVELID>(pDesc->iCurLevel); //종한 추가 Level 전환때문에


    m_fDissolveAmount = 0.3f;

    return S_OK;

}

void CWeapon_Halberd::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;
    m_fTime += fTimeDelta;

    if (m_pCamera == nullptr)
        m_pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"));
}

void CWeapon_Halberd::Update(_float fTimeDelta)
{

    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);


    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );


    if (*m_pParentState == CPlayer::STATE_HALBERDS_B)
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true) // EVENT_COLLIDER, STATE 부분        
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fStartTime
                        && m_pParentModelCom->Get_CurrentAnmationTrackPosition() <= iter.fEndTime)
                        m_pGameInstance->Add_Actor_Scene(m_pActor);
                }

                else if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == false)
                {
                    m_pGameInstance->Sub_Actor_Scene(m_pActor);
                }


                if (iter.eType == EVENT_STATE && iter.isEventActivate == true)
                {
                    if (!strcmp(iter.szName, "Dissolve_Weapon"))
                    {
                        m_bDeadOn = true;
                    }

                    if (!strcmp(iter.szName, "Reverse_Dissolve_Weapon"))
                    {
                        m_bAppear = true;
                    }

                }

                if (iter.eType != EVENT_COLLIDER && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
                {
                    iter.isPlay = true;      // 한 번만 재생 되어야 하므로         

#pragma region Effect
                    if (!strcmp(iter.szName, "Effect_Start"))
                        m_pGameInstance->Play_Effect_Speed_Matrix(EFFECT_NAME::EFFECT_PLAYER_HALBERD, m_pParentWorldMatrix, &m_pParentModelCom->Get_CurAnimation_FinalSpeed());
                    else if (!strcmp(iter.szName, "Particle1_Start"))
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_PLAYER_HALBERD_1, *m_pParentWorldMatrix);
                    else if (!strcmp(iter.szName, "Particle2_Start"))
                        m_pGameInstance->Play_Effect_Matrix_OneMoment(EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_PLAYER_HALBERD_2, *m_pParentWorldMatrix);
#pragma endregion
                }
            }
        }
    }

    else
    {
        m_pGameInstance->Sub_Actor_Scene(m_pActor);
    }

    if (m_bDeadOn)
        m_fFinishTime += fTimeDelta * 4.f;

    if (m_bAppear)
        m_fAppearTimer += fTimeDelta * 8.f;

    if (*m_pParentState != *m_pPreParentState)
    {
        m_bAppear = false;
        m_bDeadOn = false;
        m_fAppearTimer = 0.f;
        m_fDeadTimer = 0.f;
        m_fFinishTime = 0.f;
    } // 이전하고 현재 비교 해야함 


    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 70.f, 0.f,0.f,1.f });

}

void CWeapon_Halberd::Late_Update(_float fTimeDelta)
{

    if (*m_pParentState == CPlayer::STATE_HALBERDS_B)
    {
        //m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);     
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
    }

    //m_iPreParentState = *m_pParentState;
}

HRESULT CWeapon_Halberd::Render()
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

    return S_OK;
}

HRESULT CWeapon_Halberd::Render_Glow()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 9)))
            return E_FAIL;

        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveNoiseTexture", 32)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fTime, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_ReverseDissolveTime", &m_fAppearTimer, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_Dead", &m_bDeadOn, sizeof(_bool))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_Appear", &m_bAppear, sizeof(_bool))))
            return E_FAIL;

        m_pShaderCom->Begin(14);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CWeapon_Halberd::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corvus_Halberd"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mesh_Noise"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CWeapon_Halberd::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Halberd::Hit_Slow()
{
    m_fHitStopTime += m_fTimeDelta;


    if (m_fHitStopTime < 0.15f)
    {
        m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(m_fTimeDelta);
        m_pCamera->ShakeOn(400.f, 400.f, 4.f, 4.f);
    }
    else
    {
        m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Set_HitStopTime(1.f);
        m_bHitStopOnOff = false;
    }
    m_fHitStopTime += m_fTimeDelta;//1.f / 80.f; //         

    return S_OK;
}

void CWeapon_Halberd::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{

}

void CWeapon_Halberd::OnCollision(CGameObject* _pOther, PxContactPair _information)
{

}

void CWeapon_Halberd::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{

}

CWeapon_Halberd* CWeapon_Halberd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Halberd* pInstance = new CWeapon_Halberd(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CWeapon_Halberd");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon_Halberd::Clone(void* pArg)
{
    CWeapon_Halberd* pInstance = new CWeapon_Halberd(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CWeapon_Halberd");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon_Halberd::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
