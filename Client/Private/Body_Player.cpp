#include "pch.h" 
#include "Player.h"
#include "Body_Player.h" 
#include "GameInstance.h"
#include "Animation.h"
#include "Camera_Free.h"
#include "StateMgr.h"

CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype)
    : CPartObject(Prototype)
{
}

HRESULT CBody_Player::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
    CBody_Player::BODY_PLAYER_DESC* pDesc = static_cast<CBody_Player::BODY_PLAYER_DESC*>(pArg);

    m_pParent = pDesc->pParent;
    m_pParentState = pDesc->pParentState;
    m_pPreParentState = pDesc->pPreParentState;
    m_pParentPhsaeState = pDesc->pParentPhaseState;
    m_pParentNextStateCan = pDesc->pParentNextStateCan;
    m_pParentStateMgr = pDesc->pParentStateMgr;
    m_pParentNavigationCom = pDesc->pParentNavigationCom;
    m_pParentActor = pDesc->pParentActor;
    m_pParentMonsterExecute = pDesc->pParentExectueMonsterState;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pModelCom->SetUp_Animation(0, true);

    m_iCurrentLevel = static_cast<LEVELID>(pDesc->iCurLevel); //종한 추가 Level 전환때문에


    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

    m_fDissolveAmount = 0.3f;   

    if (m_pCamera == nullptr)
        m_pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"));

    if (*m_pParentState == CPlayer::STATE_DEAD) 
        m_fDeadStartTimer += fTimeDelta;    



    if (*m_pParentState == CPlayer::STATE_DEAD && m_fDeadStartTimer > 1.f)
    {
        /* 플레이어 죽음 알림*/
        if (0 == m_fDeadTimer)
        {
            m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_Landing"), true);
            m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_1Dead")), true);
        }

        m_fDeadTimer += fTimeDelta * 0.5f;
        m_fFinishTime += fTimeDelta * 0.5f;


        m_pCamera->Target_Reset();

        if (m_fDeadTimer >= 4.0f)
        {
            m_pGameInstance->Pop_Item(ITEM_TYPE::ITEM_DEADBRANCH, m_pParent->Get_Transfrom()->Get_State(CTransform::STATE_POSITION), this, dynamic_cast<CPlayer*>(m_pParent)->Get_MemoryFragment());
            dynamic_cast<CPlayer*>(m_pParent)->Increase_MemoryFragment(-(dynamic_cast<CPlayer*>(m_pParent)->Get_MemoryFragment()));


            *m_pParentState = CPlayer::STATE_START_WALK;
            *m_pParentPhsaeState = CPlayer::PHASE_START;
            *m_pPreParentState = CPlayer::STATE_DEAD;
            dynamic_cast<CPlayer*>(m_pParent)->Increase_PlayerHp(dynamic_cast<CPlayer*>(m_pParent)->Get_FullHp());

            *m_pParentNextStateCan = true;

            m_fDeadTimer = 0.f;
            m_fFinishTime = 0.f;
            m_fDeadStartTimer = 0.f;

            // 시작 위치 
            m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, _fvector{ 83.19f, 6.3f, -117.26f, 1.0f });
            m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&m_pCamera->Get_FirstCamDir()));

        }
    }

}

void CBody_Player::Update(_float fTimeDelta)
{

#pragma region  각 상황에 따른 함수 

    switch (*m_pParentState)
    {
    case CPlayer::STATE_IDLE:
        STATE_IDLE_Method();
        break;
    case CPlayer::STATE_RUN:
        STATE_RUN_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_B:
        STATE_LOCK_ON_RUN_B_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_BL:
        STATE_LOCK_ON_RUN_BL_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_BR:
        STATE_LOCK_ON_RUN_BR_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_FL:
        STATE_LOCK_ON_RUN_FL_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_FR:
        STATE_LOCK_ON_RUN_FR_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_L:
        STATE_LOCK_ON_RUN_L_Method();
        break;
    case CPlayer::STATE_LOCK_ON_RUN_R:
        STATE_LOCK_ON_RUN_R_Method();
        break;
    case CPlayer::STATE_ATTACK:
        STATE_ATTACK_Method();
        break;
    case CPlayer::STATE_ATTACK_L1:
        STATE_ATTACK_L1_Method();
        break;
    case CPlayer::STATE_ATTACK_L2:
        STATE_ATTACK_L2_Method();
        break;
    case CPlayer::STATE_ATTACK_L3:
        STATE_ATTACK_L3_Method();
        break;
    case CPlayer::STATE_ATTACK_L4:
        STATE_ATTACK_L4_Method();
        break;
    case CPlayer::STATE_ATTACK_L5:
        STATE_ATTACK_L5_Method();
        break;
    case CPlayer::STATE_ATTACK_LONG_CLAW_01:
        STATE_ATTACK_LONG_CLAW_01_Method();
        break;
    case CPlayer::STATE_ATTACK_LONG_CLAW_02:
        STATE_ATTACK_LONG_CLAW_02_Method();
        break;
    case CPlayer::STATE_LOCK_ON_EVADE_B:
        STATE_LOCK_ON_EVADE_B_Method();
        break;
    case CPlayer::STATE_LOCK_ON_EVADE_L:
        STATE_LOCK_ON_EVADE_L_Method();
        break;
    case CPlayer::STATE_LOCK_ON_EVADE_R:
        STATE_LOCK_ON_EVADE_R_Method();
        break;
    case CPlayer::STATE_LOCK_ON_EVADE_F:
        STATE_LOCK_ON_EVADE_F_Method();
        break;
    case CPlayer::STATE_PARRY_L:
        STATE_PARRY_L_Method();
        break;
    case CPlayer::STATE_PARRY_R:
        STATE_PARRY_R_Method();
        break;
    case CPlayer::STATE_HurtMFR_L:
        STATE_HurtMFR_L_Method();
        break;
    case CPlayer::STATE_HurtMFR_R:
        STATE_HurtMFR_R_Method();
        break;
    case CPlayer::STATE_HURT_LF:
        STATE_HURT_LF_Method();
        break;
    case CPlayer::STATE_HURT_SF:
        STATE_HURT_SF_Method();
        break;
    case CPlayer::STATE_HURT_SL:
        STATE_HURT_SL_Method();
        break;
    case CPlayer::STATE_HURT_HURXXLF:
        STATE_HURT_HURXXLF_Method();
        break;
    case CPlayer::STATE_HURT_KNOCKBACK:
        STATE_HURT_KNOCKBACK_Method();
        break;
    case CPlayer::STATE_HURT_KNOCKDOWN:
        STATE_HURT_KNOCKDOWN_Method();
        break;
    case CPlayer::STATE_HURT_FALLDOWN:
        STATE_HURT_FALLDOWN_Method();
        break;
    case CPlayer::STATE_HURT_FALLDOWN_END:
        STATE_HURT_FALLDOWN_END_Method();
        break;
    case CPlayer::STATE_WEAK_GETUP_F:
        STATE_WEAK_GETUP_F_Method();
        break;
    case CPlayer::STATE_REBOUND_R:
        STATE_REBOUND_R_Method();
        break;
    case CPlayer::STATE_STUNNED_START:
        STATE_STUNNED_START_Method();
        break;
    case CPlayer::STATE_STUNNED_LOOP:
        STATE_STUNNED_LOOP_Method();
        break;
    case CPlayer::STATE_PARRY_DEFLECT_L:
        STATE_PARRY_DEFLECT_L_Method();
        break;
    case CPlayer::STATE_PARRY_DEFLECT_L_UP:
        STATE_PARRY_DEFLECT_L_UP_Method();
        break;
    case CPlayer::STATE_PARRY_DEFLECT_R:
        STATE_PARRY_DEFLECT_R_Method();
        break;
    case CPlayer::STATE_PARRY_DEFLECT_R_UP:
        STATE_PARRY_DEFLECT_R_UP_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_R:
        STATE_NORMAL_EVADE_R_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_L:
        STATE_NORMAL_EVADE_L_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_FR:
        STATE_NORMAL_EVADE_FR_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_FL:
        STATE_NORMAL_EVADE_FL_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_F:
        STATE_NORMAL_EVADE_F_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_BR:
        STATE_NORMAL_EVADE_BR_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_BL:
        STATE_NORMAL_EVADE_BL_Method();
        break;
    case CPlayer::STATE_NORMAL_EVADE_B:
        STATE_NORMAL_EVADE_B_Method();
        break;
    case CPlayer::STATE_HARMOR_EXECUTION:
        STATE_HARMOR_EXECUTION_Method();
        break;
    case CPlayer::STATE_STUN_EXECUTE:
        STATE_STUN_EXECUTE_Method();
        break;
    case CPlayer::STATE_LV1Villager_M_Execution:
        STATE_LV1Villager_M_Execution_Method();
        break;
    case CPlayer::STATE_Joker_Execution:
        STATE_Joker_Execution_Method();
        break;
    case CPlayer::STATE_Varg_Execution:
        STATE_Varg_Execution_Method();
        break;  
    case CPlayer::STATE_ARCHIVE_SIT_START:
        STATE_ARCHIVE_SIT_START_Method();
        break;
    case CPlayer::STATE_ARCHIVE_SIT_LOOP:
        STATE_ARCHIVE_SIT_LOOP_Method();
        break;
    case CPlayer::STATE_ARCHIVE_SIT_GETUP:
        STATE_ARCHIVE_SIT_GETUP_Method();
        break;
    case CPlayer::STATE_ARCHIVE_SIT_LIGHT_UP:
        STATE_ARCHIVE_SIT_LIGHT_UP_Method();
        break;
    case CPlayer::STATE_HEAL:
        STATE_HEAL_Method();
        break;
    case CPlayer::STATE_DEAD:   
        STATE_DEAD_Method();    
        break;
    case CPlayer::STATE_START_WALK:
        STATE_START_WALK_Method();
        break;
    case CPlayer::STATE_CLAW_CHARGE_START:
        STATE_CLAW_CHARGE_START_Method();
        break;
    case CPlayer::STATE_CLAW_CHARGE_LOOP:
        STATE_CLAW_CHARGE_LOOP_Method();
        break;
    case CPlayer::STATE_CLAW_CHARGE_FULL_ATTACK:
        STATE_CLAW_CHARGE_FULL_ATTACK_Method();
        break;
    case CPlayer::STATE_CLAW_LONG_PLUNDER_ATTACK2:  
        STATE_CLAW_LONG_PLUNDER_ATTACK2_Method();   
        break;
    case CPlayer::STATE_HALBERDS_B: 
        STATE_HALBERDS_B_Method();  
        break;
    case CPlayer::STATE_SCYTHE_B:   
        STATE_SCYTHE_B_Method();    
        break;
    case CPlayer::STATE_CATCHED:
        STATE_CATCHED_Method();
        break;
    case CPlayer::STATE_GET_UP:
        STATE_GET_UP_Method();
        break;
    case CPlayer::STATE_VARG_RUN_EXECUTION: 
        STATE_VARG_RUN_EXECUTION_Method();  
        break;
    case CPlayer::STATE_VARG_STUN_EXECUTE_START_R:  
        STATE_VARG_STUN_EXECUTE_START_R_Method();   
        break;
    default:
        break;
    }
#pragma endregion 

    m_pModelCom->Play_Animation(fTimeDelta);

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));




#pragma region 이벤트 관련 작업
    /* 3월 6일 추가 작업 및  이 방향으로 아이디어 나가기 */
    if (*m_pParentState == CPlayer::STATE_PARRY_DEFLECT_L
        || *m_pParentState == CPlayer::STATE_PARRY_L
        || *m_pParentState == CPlayer::STATE_PARRY_R
        || *m_pParentState == CPlayer::STATE_PARRY_DEFLECT_L_UP
        || *m_pParentState == CPlayer::STATE_PARRY_DEFLECT_R
        || *m_pParentState == CPlayer::STATE_PARRY_DEFLECT_R_UP
        || *m_pParentState == CPlayer::STATE_HurtMFR_L
        || *m_pParentState == CPlayer::STATE_HurtMFR_R
        || *m_pParentState == CPlayer::STATE_HURT_FALLDOWN
        || *m_pParentState == CPlayer::STATE_HURT_HURXXLF
        || *m_pParentState == CPlayer::STATE_HURT_KNOCKBACK
        || *m_pParentState == CPlayer::STATE_HURT_KNOCKDOWN
        || *m_pParentState == CPlayer::STATE_HURT_LF
        || *m_pParentState == CPlayer::STATE_HURT_SF
        || *m_pParentState == CPlayer::STATE_HURT_SL
        || *m_pParentState == CPlayer::STATE_NORMAL_EVADE_B
        || *m_pParentState == CPlayer::STATE_NORMAL_EVADE_BL
        || *m_pParentState == CPlayer::STATE_NORMAL_EVADE_BR
        || *m_pParentState == CPlayer::STATE_NORMAL_EVADE_F
        || *m_pParentState == CPlayer::STATE_NORMAL_EVADE_FL
        || *m_pParentState == CPlayer::STATE_NORMAL_EVADE_FR
        || *m_pParentState == CPlayer::STATE_NORMAL_EVADE_L
        || *m_pParentState == CPlayer::STATE_NORMAL_EVADE_R
        || *m_pParentState == CPlayer::STATE_LOCK_ON_EVADE_B
        || *m_pParentState == CPlayer::STATE_LOCK_ON_EVADE_F
        || *m_pParentState == CPlayer::STATE_LOCK_ON_EVADE_R
        || *m_pParentState == CPlayer::STATE_LOCK_ON_EVADE_L)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if ((iter.eType == EVENT_COLLIDER || iter.eType == EVENT_STATE)
                    && iter.isEventActivate == true) // EVENT_COLLIDER 부분       
                {

                    if (!strcmp(iter.szName, "Camera_Shake"))
                    {
                        // 카메라 포인터 가져오고 싶다.

                        m_pCamera->ShakeOn(400.f, 400.f, 4.f, 4.f);
                    }

                    if (!strcmp(iter.szName, "Camera_Parry_Zoom_In"))
                    {
                        m_pCamera->Set_Camera_ZoomInSpeed(10.f);    
                        m_pCamera->ZoomIn();
                    }

                    if (!strcmp(iter.szName, "Zoom_In_Blur"))
                    {
                        m_fZoomBlurDeltaTime += fTimeDelta;
                        m_pCamera->ShakeOn(400.f, 400.f, 2.f, 2.f); 
                        m_pGameInstance->Set_Zoom_Blur_Center(m_pParent->Get_Object_UV_Pos());  
                        m_pGameInstance->Set_ZoomBlur_Option(true, m_fZoomBlurDeltaTime * 0.3f);
                    }

                    if (!strcmp(iter.szName, "Evade"))
                    {
                        if (*m_pParentState != CPlayer::STATE_NORMAL_EVADE_F)
                            m_pGameInstance->Sub_Actor_Scene(m_pParentActor);
                    }
                }

                else
                {
                    if (!strcmp(iter.szName, "Camera_Parry_Zoom_In"))
                    {
                        // 카메라 포인터 가져오고 싶다. 
                        m_pCamera->ResetZoomInCameraPos(10.f);  
                    }

                    if (!strcmp(iter.szName, "Zoom_In_Blur"))
                    {
                        m_fZoomBlurDeltaTime = 0.f;
                        m_pGameInstance->Set_ZoomBlur_Option(false, 0.f);
                    }
                }

                if ((iter.eType == EVENT_SOUND || iter.eType == EVENT_EFFECT)
                    && iter.isEventActivate == true
                    && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
                {
                    iter.isPlay = true;      // 한 번만 재생 되어야 하므로   
                }
            }

            else
            {
                m_pCamera->ResetZoomInCameraPos(1.f);
            }
        }
    }

    else
    {
        if (*m_pParentPhsaeState != CPlayer::PHASE_EXECUTION)
            m_pGameInstance->Add_Actor_Scene(m_pParentActor);

        if (*m_pParentPhsaeState != CPlayer::PHASE_EXECUTION
            && *m_pParentState != CPlayer::STATE_CLAW_CHARGE_START
            && *m_pParentState != CPlayer::STATE_CLAW_CHARGE_LOOP
            && *m_pParentState != CPlayer::STATE_CLAW_CHARGE_FULL_ATTACK
            && *m_pParentState != CPlayer::STATE_ATTACK_LONG_CLAW_01
            && *m_pParentState != CPlayer::STATE_ATTACK_LONG_CLAW_02)
        {
            m_pCamera->ResetZoomInCameraPos(1.f);
            m_fZoomBlurDeltaTime = 0.f; 
        }
    }

#pragma endregion  


}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    if (m_pParentState != m_pPreParentState)
        m_pModelCom->Get_CurAnimation()->Set_HitStopTime(1.f);

    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
}

HRESULT CBody_Player::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;


    switch (m_iRenderState)
    {
    case STATE_NORMAL_RENDER:
        STATE_NORMAL_Render();
        break;
    case STATE_CLAW_RENDER:
        STATE_ATTACK_LONG_CLAW_Render();
        break;
    case STATE_DEAD_RENDER: 
        STATE_DEAD_Render();    
        break;  
    default:
        break;
    }

    return S_OK;
}

HRESULT CBody_Player::Render_Shadow()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;/*
    if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_ViewMatrix", "g_ProjMatrix")))
        return E_FAIL;*/


    if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_LightViewMatrix", "g_LightProjMatrix")))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;

        m_pShaderCom->Begin(2);
        m_pModelCom->Render(i);
    }

    return S_OK;
}



HRESULT CBody_Player::STATE_NORMAL_Render()
{
    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        /* 평상시 모드
        (i == 11  깃털 ),
        (i == 12(왼) , i == 5(오), 발톱, ),
        (i == 10(왼) , i == 9(오),  팔목 장식)
        (i == 4(왼) ,   i == 8(오),  어깨 장식)*/
        if (i == 11
            || i == 12
            || i == 5
            || i == 10
            || i == 9
            || i == 4
            || i == 8)
            continue;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;


        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }
    return S_OK;
}

HRESULT CBody_Player::STATE_ATTACK_LONG_CLAW_Render()
{
    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {

        /* 평상시 모드
        (i == 11  깃털 ),
        (i == 12(왼) , i == 5(오), 발톱, ),
        (i == 10(왼) , i == 9(오),  팔목 장식)
        (i == 4(왼) ,   i == 8(오),  어깨 장식)*/
        if (i == 11
            || i == 12
            || i == 10
            || i == 4)
            continue;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;


        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네 
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }
    return S_OK;
}

HRESULT CBody_Player::STATE_DEAD_Render()
{
    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {

        /* 디졸브 텍스처 넣기 */
        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
            return E_FAIL;

        /* 디졸브 상수들 넣기 */
        if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDeadTimer, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_TimeStart", &m_fDeadStartTimer, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
            return E_FAIL;

        /* 평상시 모드
        (i == 11  깃털 ),
        (i == 12(왼) , i == 5(오), 발톱, ),
        (i == 10(왼) , i == 9(오),  팔목 장식)
        (i == 4(왼) ,   i == 8(오),  어깨 장식)*/
        if (i == 11
            || i == 12
            || i == 10
            || i == 4)
            continue;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0);

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))   // 여기서 이동값을 없애줘야겟네 
            return E_FAIL;

        m_pShaderCom->Begin(6);
        m_pModelCom->Render(i);
    }
    return S_OK;    
}


void CBody_Player::STATE_IDLE_Method()
{
    m_pModelCom->SetUp_Animation(2, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_RUN_Method()
{
    m_pModelCom->SetUp_Animation(9, true);
    m_iRenderState = STATE_NORMAL_RENDER;

    /* 3월 8일 추가 */
    *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
}
void CBody_Player::STATE_WALK_Method()
{
}
void CBody_Player::STATE_ATTACK_Method()
{
}
void CBody_Player::STATE_ATTACK_L1_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);   
    m_pModelCom->Set_LerpFinished(true);
    m_pModelCom->SetUp_Animation(3, false);

    if (*m_pParentState == CPlayer::STATE_ATTACK_L1 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 60.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;


    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L1 && m_pModelCom->Get_VecAnimation().at(3)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L1
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L1
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 


    m_iRenderState = STATE_NORMAL_RENDER;
    //*m_pParentNextStateCan = true;  
}

void CBody_Player::STATE_ATTACK_L2_Method()
{
    m_pModelCom->SetUp_Animation(4, false);

    if (*m_pParentState == CPlayer::STATE_ATTACK_L2 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L2 && m_pModelCom->Get_VecAnimation().at(4)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }


#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L2
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L2
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL_RENDER;
    //*m_pParentNextStateCan = true;
}
void CBody_Player::STATE_ATTACK_L3_Method()
{
    m_pModelCom->SetUp_Animation(5, false);

    if (*m_pParentState == CPlayer::STATE_ATTACK_L3 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L3 && m_pModelCom->Get_VecAnimation().at(5)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L3
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L3
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL_RENDER;   

}
void CBody_Player::STATE_ATTACK_L4_Method()
{
    m_pModelCom->SetUp_Animation(276, false);

    if (*m_pParentState == CPlayer::STATE_ATTACK_L4 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 100.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L4 && m_pModelCom->Get_VecAnimation().at(276)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L4
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L4
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL_RENDER;   


}
void CBody_Player::STATE_ATTACK_L5_Method()
{

    m_pModelCom->SetUp_Animation(277, false);

    if (*m_pParentState == CPlayer::STATE_ATTACK_L5 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_L5 && m_pModelCom->Get_VecAnimation().at(277)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L5
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_L5
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL_RENDER;

}

void CBody_Player::STATE_ATTACK_LONG_CLAW_01_Method()
{
    m_pModelCom->SetUp_Animation(0, false);


    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)
    {

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_VecAnimation().at(0)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }


#pragma region 락온 상태  O 타격 중  회피    
    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_01
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_CLAW_RENDER;
    //*m_pParentNextStateCan = true;

}
void CBody_Player::STATE_ATTACK_LONG_CLAW_02_Method()
{
    m_pModelCom->SetUp_Animation(1, false);

    m_pModelCom->Get_VecAnimation().at(1)->Set_StartOffSetTrackPosition(16.f);  

    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_VecAnimation().at(1)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }


#pragma region 락온 상태  O 타격 중  회피    
    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == CPlayer::STATE_ATTACK_LONG_CLAW_02
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = CPlayer::STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_CLAW_RENDER;
    //*m_pParentNextStateCan = true;

}

void CBody_Player::STATE_LOCK_ON_RUN_B_Method()
{
    m_pModelCom->SetUp_Animation(6, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_BL_Method()
{
    m_pModelCom->SetUp_Animation(7, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_BR_Method()
{
    m_pModelCom->SetUp_Animation(8, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_FL_Method()
{
    m_pModelCom->SetUp_Animation(10, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_FR_Method()
{
    m_pModelCom->SetUp_Animation(11, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_L_Method()
{
    m_pModelCom->SetUp_Animation(12, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_RUN_R_Method()
{
    m_pModelCom->SetUp_Animation(13, true);
    m_iRenderState = STATE_NORMAL_RENDER;
}
void CBody_Player::STATE_LOCK_ON_EVADE_F_Method()
{
    m_pModelCom->SetUp_Animation(18, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(18)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }



}
void CBody_Player::STATE_LOCK_ON_EVADE_B_Method()
{
    m_pModelCom->SetUp_Animation(17, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(17)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_LOCK_ON_EVADE_L_Method()
{

    m_pModelCom->SetUp_Animation(19, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(19)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_LOCK_ON_EVADE_R_Method()
{
    m_pModelCom->SetUp_Animation(20, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(20)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_PARRY_L_Method()
{
    m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->SetLerpTime(0.f);   
    m_pModelCom->Set_LerpFinished(true);    

    m_pModelCom->SetUp_Animation(15, false);

    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(15)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }

    if (m_pModelCom->Get_CurrentAnmationTrackPosition() > 100.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }

    /* 패링 조건 */
    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 0.5f
        && m_pModelCom->Get_CurrentAnmationTrackPosition() <= 40.f)
    {

        *m_pParentPhsaeState |= CPlayer::PHASE_PARRY; // 
    }

    else
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;

    *m_pParentNextStateCan = true; // 여기서 보간이 잘 안됨 이러면 

}
void CBody_Player::STATE_PARRY_R_Method()
{
    m_pModelCom->SetUp_Animation(16, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    m_pModelCom->Get_VecAnimation().at(16)->Set_StartOffSetTrackPosition(10.f);

    if (m_pModelCom->Get_VecAnimation().at(16)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }


    if (m_pModelCom->Get_CurrentAnmationTrackPosition() > 100.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }

    /* 패링 조건 */
    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 0.5f
        && m_pModelCom->Get_CurrentAnmationTrackPosition() <= 50.f)
    {

        *m_pParentPhsaeState |= CPlayer::PHASE_PARRY;
    }

    else
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    *m_pParentNextStateCan = true; // 여기서 보간이 잘 안됨 
}
void CBody_Player::STATE_PARRY_DEFLECT_LARGE_Method()
{
    m_pModelCom->SetUp_Animation(55, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(55)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }
}
void CBody_Player::STATE_PARRY_DEFLECT_L_UP_Method()
{
    m_pModelCom->SetUp_Animation(56, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(56)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    }

    /* 여기 애니메이션 안끝났을 때 다른 동작시 문제 생김 */

}
void CBody_Player::STATE_PARRY_DEFLECT_L_Method()
{
    m_pModelCom->SetUp_Animation(54, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(54)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    }
}
void CBody_Player::STATE_PARRY_DEFLECT_R_UP_Method()
{
    m_pModelCom->SetUp_Animation(59, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(59)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }
}
void CBody_Player::STATE_PARRY_DEFLECT_R_Method()
{
    m_pModelCom->SetUp_Animation(58, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(58)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }
}
void CBody_Player::STATE_HurtMFR_L_Method()
{
    m_pModelCom->SetUp_Animation(31, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    if (m_pModelCom->Get_VecAnimation().at(31)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
    }
}
void CBody_Player::STATE_HurtMFR_R_Method()
{
    m_pModelCom->SetUp_Animation(32, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(32)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
    }
}

void CBody_Player::STATE_HURT_LF_Method()
{
    m_pModelCom->SetUp_Animation(30, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(30)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_HURT_SF_Method()
{
    m_pModelCom->SetUp_Animation(33, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(33)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_HURT_SL_Method()
{
    m_pModelCom->SetUp_Animation(34, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(34)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
    }
}


void CBody_Player::STATE_HURT_HURXXLF_Method()
{
    m_pModelCom->SetUp_Animation(37, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(37)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_HURT_KNOCKBACK_Method()
{
    m_pModelCom->SetUp_Animation(38, false);
    m_iRenderState = STATE_NORMAL_RENDER;




    if (m_pModelCom->Get_VecAnimation().at(38)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_HURT_KNOCKDOWN_Method()
{
    m_pModelCom->SetUp_Animation(29, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(29)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_WEAK_GETUP_F;

    }
}

void CBody_Player::STATE_HURT_FALLDOWN_Method()
{
    m_pModelCom->SetUp_Animation(27, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(27)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_HURT_FALLDOWN_END;
    }
}

void CBody_Player::STATE_HURT_FALLDOWN_END_Method()
{
    m_pModelCom->SetUp_Animation(28, false);
    m_iRenderState = STATE_NORMAL_RENDER;


    if (m_pModelCom->Get_VecAnimation().at(28)->isAniMationFinish())
    {

        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_WEAK_GETUP_F_Method()
{
    m_pModelCom->SetUp_Animation(282, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(282)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }
}

void CBody_Player::STATE_REBOUND_R_Method()
{

    m_pModelCom->SetUp_Animation(62, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(62)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;

    }
}

void CBody_Player::STATE_STUNNED_START_Method()
{
    m_pModelCom->SetUp_Animation(25, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(25)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_STUNNED_LOOP;
    }
}

void CBody_Player::STATE_STUNNED_LOOP_Method()
{
    m_pModelCom->SetUp_Animation(24, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(24)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;
    }
}



void CBody_Player::STATE_NORMAL_EVADE_R_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }
}

void CBody_Player::STATE_NORMAL_EVADE_L_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }
}

void CBody_Player::STATE_NORMAL_EVADE_FR_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_FL_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_F_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_BR_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_BL_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_B_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }
}

void CBody_Player::STATE_HARMOR_EXECUTION_Method()
{
    m_pModelCom->SetUp_Animation(222, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(222)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
    }
}

void CBody_Player::STATE_STUN_EXECUTE_Method()
{
    m_pModelCom->SetUp_Animation(291, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 20.f)
    {
        switch (*m_pParentMonsterExecute)
        {
        case MONSTER_EXECUTION_CATEGORY::MONSTER_HARMOR:
            *m_pParentState = CPlayer::STATE_HARMOR_EXECUTION;
            break;
        case MONSTER_EXECUTION_CATEGORY::MONSTER_JOKER:
            *m_pParentState = CPlayer::STATE_Joker_Execution;
            break;
        case MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1: 
            *m_pParentState = CPlayer::STATE_LV1Villager_M_Execution;   
            m_pCamera->Set_Execute_CamereScene(MONSTER_EXECUTION_CATEGORY::MONSTER_VILLAGEM1);  
            m_pCamera->Set_Camera_Cut_Scene_OnOff(true);    
            break;
        default:
            break;
        }
        // 여기를 이제 부모에서 받아온 몬스터 처형 스테이트로 switch case 문으로 하면 될듯 
    }

    /*if (m_pModelCom->Get_VecAnimation().at(291)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
    }*/
}



void CBody_Player::STATE_LV1Villager_M_Execution_Method()
{
    m_pModelCom->SetUp_Animation(52, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(52)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        m_pCamera->Set_Execute_CamereScene(MONSTER_EXECUTION_CATEGORY::MONSTER_START);  
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
    }
}

void CBody_Player::STATE_Joker_Execution_Method()
{
    m_pModelCom->SetUp_Animation(51, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(51)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
    }
}

void CBody_Player::STATE_Varg_Execution_Method()
{
    m_pModelCom->SetUp_Animation(50, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(50)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_EXECUTION;
        *m_pParentMonsterExecute = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
    }
}

void CBody_Player::STATE_VARG_STUN_EXECUTE_START_R_Method()
{
    m_pModelCom->SetUp_Animation(292, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 68.f)
    {
        m_pModelCom->Get_VecAnimation().at(298)->Set_StartOffSetTrackPosition(68.f);
        *m_pParentState = CPlayer::STATE_VARG_RUN_EXECUTION;
    }
}

void CBody_Player::STATE_VARG_RUN_EXECUTION_Method()
{
    m_pModelCom->SetUp_Animation(298, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(298)->isAniMationFinish())
    {
        //m_pModelCom->Get_VecAnimation().at(50)->Set_StartOffSetTrackPosition(15.f);
        *m_pParentState = CPlayer::STATE_Varg_Execution;
    }
}

void CBody_Player::STATE_ARCHIVE_SIT_START_Method()
{
    m_pModelCom->SetUp_Animation(66, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(66)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE_ARCHIVE_SIT_LOOP;
    }

}

void CBody_Player::STATE_ARCHIVE_SIT_LOOP_Method()
{
    m_pModelCom->SetUp_Animation(65, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    m_pModelCom->Get_VecAnimation().at(65)->Set_AnimationSpeed(0.f);
    /* if (m_pModelCom->Get_VecAnimation().at(65)->isAniMationFinish())
     {
         m_pModelCom->
     }*/
}

void CBody_Player::STATE_ARCHIVE_SIT_GETUP_Method()
{
    m_pModelCom->SetUp_Animation(63, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(63)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_CHAIR;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
    }
}

void CBody_Player::STATE_ARCHIVE_SIT_LIGHT_UP_Method()
{
    /* 의자 불켜기 관련 */
}

void CBody_Player::STATE_HEAL_Method()
{
    m_pModelCom->SetUp_Animation(237, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(237)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_HEAL;
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
    }
}

void CBody_Player::STATE_DEAD_Method()
{
    m_pModelCom->SetUp_Animation(266, false);
    m_iRenderState = STATE_DEAD_RENDER;     

    if (m_pModelCom->Get_VecAnimation().at(266)->isAniMationFinish())
    {
    }
}

void CBody_Player::STATE_START_WALK_Method()
{
    m_pModelCom->SetUp_Animation(184, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(184)->isAniMationFinish())
    {

        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_START;
    }
}

void CBody_Player::STATE_CLAW_CHARGE_START_Method()
{
    m_pModelCom->SetUp_Animation(144, false);
    m_iRenderState = STATE_CLAW_RENDER;

    m_pModelCom->Get_VecAnimation().at(2)->SetLerpTime(0.15f);  

    if (m_pModelCom->Get_VecAnimation().at(144)->isAniMationFinish() && m_pModelCom->Get_LerpFinished()) 
    {
        *m_pParentState = CPlayer::STATE::STATE_CLAW_CHARGE_LOOP;

    }
}

void CBody_Player::STATE_CLAW_CHARGE_LOOP_Method()
{
    m_pModelCom->SetUp_Animation(142, true);    
    m_iRenderState = STATE_CLAW_RENDER;
}

void CBody_Player::STATE_CLAW_CHARGE_FULL_ATTACK_Method()
{
    m_pModelCom->SetUp_Animation(145, false);   
    m_iRenderState = STATE_CLAW_RENDER; 

    m_pModelCom->Get_VecAnimation().at(145)->Set_StartOffSetTrackPosition(36.f);    

    if (m_pModelCom->Get_VecAnimation().at(145)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;

    }
}

void CBody_Player::STATE_CLAW_LONG_PLUNDER_ATTACK2_Method() 
{

    m_pModelCom->SetUp_Animation(147, false);   
    m_iRenderState = STATE_CLAW_RENDER;
        
    m_pModelCom->Get_VecAnimation().at(147)->Set_StartOffSetTrackPosition(3.f); 

    if (m_pModelCom->Get_VecAnimation().at(147)->isAniMationFinish())   
    {
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
    }
}

void CBody_Player::STATE_HALBERDS_B_Method()
{
    m_pModelCom->SetUp_Animation(107, false);
    m_iRenderState = STATE_NORMAL_RENDER;       

    if (m_pModelCom->Get_VecAnimation().at(107)->isAniMationFinish())   
    {   
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
    }
}

void CBody_Player::STATE_SCYTHE_B_Method()
{
    m_pModelCom->SetUp_Animation(122, false);   
    m_iRenderState = STATE_NORMAL_RENDER;   

    if (m_pModelCom->Get_VecAnimation().at(122)->isAniMationFinish())   
    {
        *m_pParentState = CPlayer::STATE::STATE_IDLE;   
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_FIGHT;
    }
}

void CBody_Player::STATE_CATCHED_Method()
{
    m_pModelCom->SetUp_Animation(233, false);
    m_iRenderState = STATE_NORMAL_RENDER;

    if (m_pModelCom->Get_VecAnimation().at(233)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE::STATE_GET_UP;
        m_pParent->Get_Transfrom()->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(180.f));
    }
}

void CBody_Player::STATE_GET_UP_Method()
{
    m_pModelCom->SetUp_Animation(273, false);
    m_iRenderState = STATE_NORMAL_RENDER;



    if (m_pModelCom->Get_VecAnimation().at(273)->isAniMationFinish())
    {
        *m_pParentState = CPlayer::STATE::STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PLAYER_PHASE::PHASE_HITTED;
    }
}





HRESULT CBody_Player::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"), 
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Corner"),   
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))  
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Monster_Noise"),  
        TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))    
        return E_FAIL;


    return S_OK;
}

HRESULT CBody_Player::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
    CBody_Player* pInstance = new CBody_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pTextureCom);
}
