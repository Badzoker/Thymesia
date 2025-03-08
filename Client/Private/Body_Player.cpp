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
    m_pParentPhsaeState = pDesc->pParentPhaseState;
    m_pParentNextStateCan = pDesc->pParentNextStateCan;
    m_pParentStateMgr = pDesc->pParentStateMgr;
    m_pParentNavigationCom = pDesc->pParentNavigationCom;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pModelCom->SetUp_Animation(0, true);




    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

    if (m_pCamera == nullptr)
        m_pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera")));
}

void CBody_Player::Update(_float fTimeDelta)
{

#pragma region  각 상황에 따른 함수 

    switch (*m_pParentState)
    {
    case STATE_IDLE:
        STATE_IDLE_Method();
        break;
    case STATE_RUN:
        STATE_RUN_Method();
        break;
    case STATE_LOCK_ON_RUN_B:
        STATE_LOCK_ON_RUN_B_Method();
        break;
    case STATE_LOCK_ON_RUN_BL:
        STATE_LOCK_ON_RUN_BL_Method();
        break;
    case STATE_LOCK_ON_RUN_BR:
        STATE_LOCK_ON_RUN_BR_Method();
        break;
    case STATE_LOCK_ON_RUN_FL:
        STATE_LOCK_ON_RUN_FL_Method();
        break;
    case STATE_LOCK_ON_RUN_FR:
        STATE_LOCK_ON_RUN_FR_Method();
        break;
    case STATE_LOCK_ON_RUN_L:
        STATE_LOCK_ON_RUN_L_Method();
        break;
    case STATE_LOCK_ON_RUN_R:
        STATE_LOCK_ON_RUN_R_Method();
        break;
    case STATE_WALK:
        STATE_WALK_Method();
        break;
    case STATE_ATTACK:
        STATE_ATTACK_Method();
        break;
    case STATE_ATTACK_L1:
        STATE_ATTACK_L1_Method();
        break;
    case STATE_ATTACK_L2:
        STATE_ATTACK_L2_Method();
        break;
    case STATE_ATTACK_L3:
        STATE_ATTACK_L3_Method();
        break;
    case STATE_ATTACK_LONG_CLAW_01:
        STATE_ATTACK_LONG_CLAW_01_Method();
        break;
    case STATE_ATTACK_LONG_CLAW_02:
        STATE_ATTACK_LONG_CLAW_02_Method();
        break;
    case STATE_LOCK_ON_EVADE_B:
        STATE_LOCK_ON_EVADE_B_Method();
        break;
    case STATE_LOCK_ON_EVADE_L:
        STATE_LOCK_ON_EVADE_L_Method();
        break;
    case STATE_LOCK_ON_EVADE_R:
        STATE_LOCK_ON_EVADE_R_Method();
        break;
    case STATE_LOCK_ON_EVADE_F:
        STATE_LOCK_ON_EVADE_F_Method();
        break;
    case STATE_PARRY_L:
        STATE_PARRY_L_Method();
        break;
    case STATE_PARRY_R:
        STATE_PARRY_R_Method();
        break;
    case STATE_HurtMFR_L:
        STATE_HurtMFR_L_Method();
        break;
    case STATE_HurtMFR_R:
        STATE_HurtMFR_R_Method();
        break;
    case STATE_HURT_LF:
        STATE_HURT_LF_Method();
        break;
    case STATE_HURT_SF:
        STATE_HURT_SF_Method();
        break;
    case STATE_HURT_SL:
        STATE_HURT_SL_Method();
        break;
    case STATE_HURT_HURXXLF:
        STATE_HURT_HURXXLF_Method();
        break;
    case STATE_HURT_KNOCKBACK:
        STATE_HURT_KNOCKBACK_Method();
        break;
    case STATE_HURT_KNOCKDOWN:
        STATE_HURT_KNOCKDOWN_Method();
        break;
    case STATE_HURT_FALLDOWN:
        STATE_HURT_FALLDOWN_Method();
        break;
    case STATE_WEAK_GETUP_F:
        STATE_WEAK_GETUP_F_Method();
        break;
    case STATE_PARRY_DEFLECT_L:
        STATE_PARRY_DEFLECT_L_Method();
        break;
    case STATE_PARRY_DEFLECT_L_UP:
        STATE_PARRY_DEFLECT_L_UP_Method();
        break;
    case STATE_PARRY_DEFLECT_R:
        STATE_PARRY_DEFLECT_R_Method();
        break;
    case STATE_PARRY_DEFLECT_R_UP:
        STATE_PARRY_DEFLECT_R_UP_Method();
        break;
    case STATE_NORMAL_EVADE_R:
        STATE_NORMAL_EVADE_R_Method();
        break;
    case STATE_NORMAL_EVADE_L:
        STATE_NORMAL_EVADE_L_Method();
        break;
    case STATE_NORMAL_EVADE_FR:
        STATE_NORMAL_EVADE_FR_Method();
        break;
    case STATE_NORMAL_EVADE_FL:
        STATE_NORMAL_EVADE_FL_Method();
        break;
    case STATE_NORMAL_EVADE_F:
        STATE_NORMAL_EVADE_F_Method();
        break;
    case STATE_NORMAL_EVADE_BR:
        STATE_NORMAL_EVADE_BR_Method();
        break;
    case STATE_NORMAL_EVADE_BL:
        STATE_NORMAL_EVADE_BL_Method();
        break;
    case STATE_NORMAL_EVADE_B:
        STATE_NORMAL_EVADE_B_Method();
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
        || *m_pParentState == CPlayer::STATE_HURT_SL)
    {
        for (auto& iter : *m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if ((iter.eType == EVENT_COLLIDER || iter.eType == EVENT_STATE)
                    && iter.isEventActivate == true) // EVENT_COLLIDER 부분       
                {
                    // 그 구간에서는 계속 진행        
                    //m_pGameInstance->Add_Actor_Scene(m_pActor); 
                    if (!strcmp(iter.szName, "Camera_Shake"))
                    {
                        // 카메라 포인터 가져오고 싶다.
                        m_pCamera->ShakeOn();
                    }
                }

                else
                {
                    //m_pGameInstance->Sub_Actor_Scene(m_pActor);
                }

                if ((iter.eType == EVENT_SOUND || iter.eType == EVENT_EFFECT)
                    && iter.isEventActivate == true
                    && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
                {
                    iter.isPlay = true;      // 한 번만 재생 되어야 하므로   

                }


            }
        }
    }
#pragma endregion  


}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
    //m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);   
}

HRESULT CBody_Player::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;


    switch (m_iRenderState)
    {
    case STATE_NORMAL:
        STATE_NORMAL_Render();
        break;
    case STATE_CLAW:
        STATE_ATTACK_LONG_CLAW_Render();
        break;
    default:
        break;
    }

    return S_OK;
}

HRESULT CBody_Player::Render_Shadow()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_ViewMatrix", "g_ProjMatrix")))
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


void CBody_Player::STATE_IDLE_Method()
{
    m_pModelCom->SetUp_Animation(2, true);
    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_RUN_Method()
{
    m_pModelCom->SetUp_Animation(9, true);
    m_iRenderState = STATE_NORMAL;

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
    m_pModelCom->SetUp_Animation(3, false);

    if (*m_pParentState == STATE_ATTACK_L1 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 60.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;

    }

    if (*m_pParentState == STATE_ATTACK_L1 && m_pModelCom->Get_VecAnimation().at(3)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == STATE_ATTACK_L1
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == STATE_ATTACK_L1
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 


    m_iRenderState = STATE_NORMAL;
    //*m_pParentNextStateCan = true;
}

void CBody_Player::STATE_ATTACK_L2_Method()
{
    m_pModelCom->SetUp_Animation(4, false);

    if (*m_pParentState == STATE_ATTACK_L2 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_L2 && m_pModelCom->Get_VecAnimation().at(4)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }


#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == STATE_ATTACK_L2
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == STATE_ATTACK_L2
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_ATTACK_L3_Method()
{
    m_pModelCom->SetUp_Animation(5, false);

    if (*m_pParentState == STATE_ATTACK_L3 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_L3 && m_pModelCom->Get_VecAnimation().at(5)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }

#pragma region 락온 상태  O 타격 중  회피 
    if (*m_pParentState == STATE_ATTACK_L3
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == STATE_ATTACK_L3
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_ATTACK_LONG_CLAW_01_Method()
{
    m_pModelCom->SetUp_Animation(0, false);

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)
    {

        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_VecAnimation().at(0)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }


#pragma region 락온 상태  O 타격 중  회피    
    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_01
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_01
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_CLAW;
    //*m_pParentNextStateCan = true;
}
void CBody_Player::STATE_ATTACK_LONG_CLAW_02_Method()
{
    m_pModelCom->SetUp_Animation(1, false);

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_VecAnimation().at(1)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }


#pragma region 락온 상태  O 타격 중  회피    
    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_02
        && (*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(18)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);

            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(19)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(20)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_LOCK_ON_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(17)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            *m_pParentNextStateCan = false;
        }
    }
#pragma endregion 
#pragma region 락온 상태 X 타격 중 노말 회피 
    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_02
        && !(*m_pParentPhsaeState & CPlayer::PHASE_LOCKON)
        && m_pModelCom->Get_CurrentAnmationTrackPosition() > 10.f)
    {

        if (m_pGameInstance->isKeyEnter(DIK_SPACE))
        {
            *m_pParentPhsaeState |= CPlayer::PHASE_DASH;
            *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;

            if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FL;
                m_pParentStateMgr->Get_VecState().at(31)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_FR;
                m_pParentStateMgr->Get_VecState().at(30)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BL;
                m_pParentStateMgr->Get_VecState().at(34)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_S) && m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_BR;
                m_pParentStateMgr->Get_VecState().at(33)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }


            else if (m_pGameInstance->isKeyPressed(DIK_S))
            {
                *m_pParentState = STATE_NORMAL_EVADE_B;
                m_pParentStateMgr->Get_VecState().at(35)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_A))
            {
                *m_pParentState = STATE_NORMAL_EVADE_L;
                m_pParentStateMgr->Get_VecState().at(29)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else if (m_pGameInstance->isKeyPressed(DIK_D))
            {
                *m_pParentState = STATE_NORMAL_EVADE_R;
                m_pParentStateMgr->Get_VecState().at(28)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }

            else
            {
                *m_pParentState = STATE_NORMAL_EVADE_F;
                m_pParentStateMgr->Get_VecState().at(32)->Priority_Update(m_pParent, m_pParentNavigationCom, m_fTimeDelta);
            }
        }
    }
#pragma endregion 

    m_iRenderState = STATE_CLAW;
}

void CBody_Player::STATE_LOCK_ON_RUN_B_Method()
{
    m_pModelCom->SetUp_Animation(6, true);
    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_LOCK_ON_RUN_BL_Method()
{
    m_pModelCom->SetUp_Animation(7, true);
    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_LOCK_ON_RUN_BR_Method()
{
    m_pModelCom->SetUp_Animation(8, true);
    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_LOCK_ON_RUN_FL_Method()
{
    m_pModelCom->SetUp_Animation(10, true);
    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_LOCK_ON_RUN_FR_Method()
{
    m_pModelCom->SetUp_Animation(11, true);
    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_LOCK_ON_RUN_L_Method()
{
    m_pModelCom->SetUp_Animation(12, true);
    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_LOCK_ON_RUN_R_Method()
{
    m_pModelCom->SetUp_Animation(13, true);
    m_iRenderState = STATE_NORMAL;
}
void CBody_Player::STATE_LOCK_ON_EVADE_F_Method()
{
    m_pModelCom->SetUp_Animation(18, false);
    m_iRenderState = STATE_NORMAL;


    if (m_pModelCom->Get_VecAnimation().at(18)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_LOCK_ON_EVADE_B_Method()
{
    m_pModelCom->SetUp_Animation(17, false);
    m_iRenderState = STATE_NORMAL;


    if (m_pModelCom->Get_VecAnimation().at(17)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_LOCK_ON_EVADE_L_Method()
{

    m_pModelCom->SetUp_Animation(19, false);
    m_iRenderState = STATE_NORMAL;


    if (m_pModelCom->Get_VecAnimation().at(19)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_LOCK_ON_EVADE_R_Method()
{
    m_pModelCom->SetUp_Animation(20, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(20)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentNextStateCan = true;
    }

}
void CBody_Player::STATE_PARRY_L_Method()
{
    m_pModelCom->SetUp_Animation(15, false);
    m_iRenderState = STATE_NORMAL;

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
    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 10.f
        && m_pModelCom->Get_CurrentAnmationTrackPosition() <= 40.f)
    {

        *m_pParentPhsaeState |= CPlayer::PHASE_PARRY;
    }

    else
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;

    *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;

    *m_pParentNextStateCan = true; // 여기서 보간이 잘 안됨 이러면 

}
void CBody_Player::STATE_PARRY_R_Method()
{
    m_pModelCom->SetUp_Animation(16, false);
    m_iRenderState = STATE_NORMAL;

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
    if (m_pModelCom->Get_CurrentAnmationTrackPosition() >= 10.f
        && m_pModelCom->Get_CurrentAnmationTrackPosition() <= 40.f)
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
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(55)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }
}
void CBody_Player::STATE_PARRY_DEFLECT_L_UP_Method()
{
    m_pModelCom->SetUp_Animation(56, false);
    m_iRenderState = STATE_NORMAL;

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
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(54)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }
}
void CBody_Player::STATE_PARRY_DEFLECT_R_UP_Method()
{
    m_pModelCom->SetUp_Animation(59, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(59)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }
}
void CBody_Player::STATE_PARRY_DEFLECT_R_Method()
{
    m_pModelCom->SetUp_Animation(58, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(58)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_PARRY;
    }
}
void CBody_Player::STATE_HurtMFR_L_Method()
{
    m_pModelCom->SetUp_Animation(31, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(31)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        //m_pModelCom->Get_VecAnimation().at(2)->Set_LerpTime(0.2f);  

        *m_pParentState = STATE_IDLE;
    }
}
void CBody_Player::STATE_HurtMFR_R_Method()
{
    m_pModelCom->SetUp_Animation(32, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(32)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        /* IDLE lerp time 없애주는거 */
        //m_pModelCom->Get_VecAnimation().at(2)->Set_LerpTime(0.2f);

        *m_pParentState = STATE_IDLE;
    }
}

void CBody_Player::STATE_HURT_LF_Method()
{
    m_pModelCom->SetUp_Animation(30, false);
    m_iRenderState = STATE_NORMAL;

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
    m_iRenderState = STATE_NORMAL;

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
    m_iRenderState = STATE_NORMAL;

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
    m_iRenderState = STATE_NORMAL;

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
    m_iRenderState = STATE_NORMAL;

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
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(29)->isAniMationFinish())
    {
        *m_pParentState = STATE_WEAK_GETUP_F;
        /**m_pParentPhsaeState &= ~CPlayer::PHASE_HITTED;
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentNextStateCan = true;

        *m_pParentState = STATE_IDLE;*/
    }
}

void CBody_Player::STATE_HURT_FALLDOWN_Method()
{
    m_pModelCom->SetUp_Animation(27, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(27)->isAniMationFinish())
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
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(282)->isAniMationFinish())
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
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }
}

void CBody_Player::STATE_NORMAL_EVADE_L_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }
}

void CBody_Player::STATE_NORMAL_EVADE_FR_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_FL_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_F_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_BR_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_BL_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }

}

void CBody_Player::STATE_NORMAL_EVADE_B_Method()
{
    m_pModelCom->SetUp_Animation(257, false);
    m_iRenderState = STATE_NORMAL;

    if (m_pModelCom->Get_VecAnimation().at(257)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_DASH;
    }
}


HRESULT CBody_Player::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corner"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
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

}
