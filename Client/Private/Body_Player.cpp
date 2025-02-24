#include "pch.h" 
#include "Player.h"
#include "Body_Player.h" 
#include "GameInstance.h"
#include "Animation.h"

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

    m_pParentState = pDesc->pParentState;
    m_pParentPhsaeState = pDesc->pParentPhaseState;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pModelCom->SetUp_Animation(0, true);

    /* 여기서 보간속도 설정하기 */

    ///* 칼 1번째 휘두를때 애니메이션 보간 시간 조정*/
    //m_pModelCom->Get_VecAnimation().at(1)->Set_LerpTime(0.1f);
    ///* 칼 2번째 휘두를때 애니메이션 보간 시간 조정*/
    //m_pModelCom->Get_VecAnimation().at(2)->Set_LerpTime(0.1f);
    ///* 칼 3번째 휘두를때 애니메이션 보간 시간 조정*/
    //m_pModelCom->Get_VecAnimation().at(3)->Set_LerpTime(0.1f);


    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{

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
    case STATE_ROCK_ON_RUN_B:
        STATE_ROCK_ON_RUN_B_Method();
        break;
    case STATE_ROCK_ON_RUN_BL:
        STATE_ROCK_ON_RUN_BL_Method();
        break;
    case STATE_ROCK_ON_RUN_BR:
        STATE_ROCK_ON_RUN_BR_Method();
        break;
    case STATE_ROCK_ON_RUN_FL:
        STATE_ROCK_ON_RUN_FL_Method();
        break;
    case STATE_ROCK_ON_RUN_FR:
        STATE_ROCK_ON_RUN_FR_Method();
        break;
    case STATE_ROCK_ON_RUN_L:
        STATE_ROCK_ON_RUN_L_Method();
        break;
    case STATE_ROCK_ON_RUN_R:
        STATE_ROCK_ON_RUN_R_Method();
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
    default:
        break;
    }
#pragma endregion 

    m_pModelCom->Play_Animation(fTimeDelta);

    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));


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
        *m_pParentPhsaeState ^= CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_L1 && m_pModelCom->Get_VecAnimation().at(3)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }

    m_iRenderState = STATE_NORMAL;
}

void CBody_Player::STATE_ATTACK_L2_Method()
{
    m_pModelCom->SetUp_Animation(4, false);

    if (*m_pParentState == STATE_ATTACK_L2 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        *m_pParentPhsaeState ^= CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_L2 && m_pModelCom->Get_VecAnimation().at(4)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }

    m_iRenderState = STATE_NORMAL;
}

void CBody_Player::STATE_ATTACK_L3_Method()
{
    m_pModelCom->SetUp_Animation(5, false);

    if (*m_pParentState == STATE_ATTACK_L3 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 125.f)
    {
        *m_pParentPhsaeState ^= CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_L3 && m_pModelCom->Get_VecAnimation().at(5)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }

    m_iRenderState = STATE_NORMAL;
}

void CBody_Player::STATE_ATTACK_LONG_CLAW_01_Method()
{
    m_pModelCom->SetUp_Animation(0, false);

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)
    {

        *m_pParentPhsaeState ^= CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_VecAnimation().at(0)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }

    m_iRenderState = STATE_CLAW;
}

void CBody_Player::STATE_ATTACK_LONG_CLAW_02_Method()
{
    m_pModelCom->SetUp_Animation(1, false);

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)
    {
        *m_pParentPhsaeState ^= CPlayer::PHASE_FIGHT;
        *m_pParentPhsaeState |= CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_VecAnimation().at(1)->isAniMationFinish())
    {
        *m_pParentPhsaeState &= ~CPlayer::PHASE_FIGHT;
        *m_pParentState = STATE_IDLE;
    }

    m_iRenderState = STATE_CLAW;
}

void CBody_Player::STATE_ROCK_ON_RUN_B_Method()
{
    m_pModelCom->SetUp_Animation(6, true);
    m_iRenderState = STATE_NORMAL;
}

void CBody_Player::STATE_ROCK_ON_RUN_BL_Method()
{
    m_pModelCom->SetUp_Animation(7, true);
    m_iRenderState = STATE_NORMAL;
}

void CBody_Player::STATE_ROCK_ON_RUN_BR_Method()
{
    m_pModelCom->SetUp_Animation(8, true);
    m_iRenderState = STATE_NORMAL;
}

void CBody_Player::STATE_ROCK_ON_RUN_FL_Method()
{
    m_pModelCom->SetUp_Animation(10, true);
    m_iRenderState = STATE_NORMAL;
}

void CBody_Player::STATE_ROCK_ON_RUN_FR_Method()
{
    m_pModelCom->SetUp_Animation(11, true);
    m_iRenderState = STATE_NORMAL;
}

void CBody_Player::STATE_ROCK_ON_RUN_L_Method()
{
    m_pModelCom->SetUp_Animation(12, true);
    m_iRenderState = STATE_NORMAL;
}

void CBody_Player::STATE_ROCK_ON_RUN_R_Method()
{
    m_pModelCom->SetUp_Animation(13, true);
    m_iRenderState = STATE_NORMAL;
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
