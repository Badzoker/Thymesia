#include "pch.h" 
#include "PlayerCamera.h"
#include "GameInstance.h"
#include "Player.h"
#include "Animation.h"
#include "Camera_Free.h"

CPlayerCamera::CPlayerCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice, pContext }
{
}

CPlayerCamera::CPlayerCamera(const CPlayerCamera& Prototype)
    :CPartObject(Prototype)
{
}

HRESULT CPlayerCamera::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CPlayerCamera::Initialize(void* pArg)
{

    strcpy_s(m_szName, "PLAYER_CAMERA");

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pParentState = pDesc->pParentState;
    m_pParentModelCom = pDesc->pParentModel;
    m_pParentActor = pDesc->pParentActor;


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

    m_iCurrentLevel = static_cast<LEVELID>(pDesc->iCurLevel); //종한 추가 Level 전환때문에


    return S_OK;

}

void CPlayerCamera::Priority_Update(_float fTimeDelta)
{
    m_fTimeDelta = fTimeDelta;

    if (m_pCamera == nullptr)
        m_pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"));
}

void CPlayerCamera::Update(_float fTimeDelta)
{
    _matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);


    _vector vScale, vRotation, vTranslation;
    XMMatrixDecompose(&vScale, &vRotation, &vTranslation, SocketMatrix);

    _vector vParentScale, vParentRotation, vParentTranslation;
    XMMatrixDecompose(&vParentScale, &vParentRotation, &vParentTranslation, XMLoadFloat4x4(m_pParentWorldMatrix));

    _vector qResult = XMQuaternionMultiply(vRotation, vParentRotation);


    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
        SocketMatrix *  /* 로컬 스페이스 영역 */
        XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
    );

#pragma region 이벤트 관련 작업
       /* 3월 6일 추가 작업 및  이 방향으로 아이디어 나가기 */
    if (*m_pParentState == CPlayer::STATE_HARMOR_EXECUTION
        || *m_pParentState == CPlayer::STATE_LV1Villager_M_Execution
        || *m_pParentState == CPlayer::STATE_Joker_Execution
        || *m_pParentState == CPlayer::STATE_Varg_Execution
        || *m_pParentState == CPlayer::STATE_STUN_EXECUTE    // 이게 처형 시작 모션 
        || *m_pParentState == CPlayer::STATE_CATCHED
        || *m_pParentState == CPlayer::STATE_VARG_RUN_EXECUTION)    
    {
        for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
        {
            if (iter.isPlay == false)
            {
                if (iter.isEventActivate == true) // EVENT_STATE 부분           
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() > iter.fStartTime     
                        && m_pParentModelCom->Get_CurrentAnmationTrackPosition() < iter.fEndTime)

                    {
                        _vector pos = { m_CombinedWorldMatrix._41,m_CombinedWorldMatrix._42,m_CombinedWorldMatrix._43,1.f };
                        _vector vRight = { m_CombinedWorldMatrix._11,m_CombinedWorldMatrix._12,m_CombinedWorldMatrix._13,0.f };
                        _vector vUp = { m_CombinedWorldMatrix._21,m_CombinedWorldMatrix._22,m_CombinedWorldMatrix._23,0.f };
                        _vector vLook = { m_CombinedWorldMatrix._31,m_CombinedWorldMatrix._32,m_CombinedWorldMatrix._33,0.f };


                        _matrix Rotation = XMMatrixRotationAxis(XMVectorSetW(XMVector3Normalize(vLook), 0.f), XMConvertToRadians(-90.f));
                        vLook = XMVector3TransformNormal(vLook, Rotation);
                        vLook = XMVectorSetW(XMVector3Normalize(vLook), 0.f);



                        vRight = XMVector3TransformNormal(vRight, Rotation);
                        vRight = XMVectorSetW(XMVector3Normalize(vRight), 0.f); // 여기서 LOOK 축으로 90도 회전  ( 0.f, 0.f,1.f,0.f)     



                        vUp = XMVector3TransformNormal(vUp, Rotation);
                        vUp = XMVectorSetW(XMVector3Normalize(vUp), 0.f);    // 여기서 LOOK 축으로 90도 회전  ( 0.f, 0.f,1.f,0.f)  



                        m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_LOOK, vLook);
                        m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_RIGHT, vRight);
                        m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_UP, vUp);

                        m_pCamera->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, pos);


                        m_pCamera->Set_Camera_Cut_Scene_OnOff(true);
                        m_pCamera->Set_Camera_GetBackCamPos(true);

                        m_pGameInstance->Sub_Actor_Scene(m_pParentActor);

                    }
                }

                else
                {
                    if (m_pParentModelCom->Get_CurrentAnmationTrackPosition() >= iter.fEndTime)
                    {
                        if (m_pCamera->Get_Execute_CamereScene() == 0)  
                            m_pCamera->Set_Camera_Cut_Scene_OnOff(false);    // 여기가 문제구나    

                        m_pGameInstance->Add_Actor_Scene(m_pParentActor);   
                    }

                }
            }
        }
    }

    else
    {
        //m_pCamera->ResetZoomInCameraPos();    
    }

}

void CPlayerCamera::Late_Update(_float fTimeDelta)
{

    m_iPreParentState = *m_pParentState;
}

HRESULT CPlayerCamera::Render()
{


    return S_OK;
}

HRESULT CPlayerCamera::Ready_Components()
{


    return S_OK;
}

HRESULT CPlayerCamera::Bind_ShaderResources()
{

    return S_OK;
}

void CPlayerCamera::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{

}

void CPlayerCamera::OnCollision(CGameObject* _pOther, PxContactPair _information)
{

}

void CPlayerCamera::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{

}

CPlayerCamera* CPlayerCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerCamera* pInstance = new CPlayerCamera(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CPlayer_Camera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayerCamera::Clone(void* pArg)
{
    CPlayerCamera* pInstance = new CPlayerCamera(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CPlayerCamera");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayerCamera::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
