#include "pch.h"
#include "LadderObject.h"

CLadderObject::CLadderObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CStaticObject{ _pDevice , _pContext }
{
}

CLadderObject::CLadderObject(const CLadderObject& _Prototype)
    :CStaticObject(_Prototype)
{
}

HRESULT CLadderObject::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLadderObject::Initialize(void* _pArg)
{
    LADDER_DESC* pDesc = static_cast<LADDER_DESC*>(_pArg);

    if (pDesc->ObjectName == "P_Ladder02_Down")
    {
        string strLadderDownName = "P_Ladder02_Down" + to_string(pDesc->iObjectNumber);
        strcpy_s(m_szName, strLadderDownName.c_str());
    }
    else if (pDesc->ObjectName == "P_Ladder02_Up")
    {
        string strLadderUpName = "P_Ladder02_Up" + to_string(pDesc->iObjectNumber);
        strcpy_s(m_szName, strLadderUpName.c_str());
    }

    m_eCurrentLevel = pDesc->eLevelID;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    m_iObjectType = OBJECT_DEFAULT;

    m_pButtonGameObject = m_pGameInstance->Get_GameObject_To_Layer(m_eCurrentLevel, TEXT("Layer_Button"), "BUTTON");
    m_pButton = static_cast<CButton*>(m_pButtonGameObject);

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.7f, 0.7f, 0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    _uint iSettingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, iSettingColliderGroup);

    m_pGameInstance->Add_Actor_Scene(m_pActor);

    return S_OK;
}

void CLadderObject::Priority_Update(_float fTimeDelta)
{

}

void CLadderObject::Update(_float fTimeDelta)
{
    if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
        m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.0f, 15.0f, 0.0f, 1.0f });

}

void CLadderObject::Late_Update(_float fTimeDelta)
{
    if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_fFrustumRadius))
    {
        m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
        //m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
        //m_pGameInstance->Add_RenderGroup(CRenderer::RG_MOTION_BLUR, this);
    }
}

HRESULT CLadderObject::Render()
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

        m_pShaderCom->Begin(m_iPassIndex);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CLadderObject::Render_Glow()
{
    return S_OK;
}

void CLadderObject::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    _vector vLadderUpPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vLadderUpPos = XMVectorSetY(vLadderUpPos, XMVectorGetY(vLadderUpPos) + 1.0f);

    _float4 vLadderUpPosition;
    XMStoreFloat4(&vLadderUpPosition, vLadderUpPos);

    m_pButton->Set_WorldPosition(vLadderUpPosition);

    if (!strncmp(m_szName, "P_Ladder02_Up", 13))
    {
        m_pButton->Set_ButtonText(TEXT("E"), TEXT("내려갈래"));
        m_pButton->Activate_Button(true);
        m_bInteractOn = true;
        m_bFadingIn = true;
        m_bFadingOut = false;
    }
    else if (!strncmp(m_szName, "P_Ladder02_Down", 15))
    {
        m_pButton->Set_ButtonText(TEXT("E"), TEXT("올라갈래"));
        m_pButton->Activate_Button(true);
        m_bInteractOn = true;
        m_bFadingIn = true;
        m_bFadingOut = false;
    }
}

void CLadderObject::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CLadderObject::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    m_pButton->Activate_Button(false);
    m_bInteractOn = false;
    m_bFadingIn = false;
    m_bFadingOut = true;
}

HRESULT CLadderObject::Ready_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    string strModelBaseName = m_szName;
    size_t pos = strModelBaseName.find_last_of('_');

    if (pos != string::npos)
        strModelBaseName = strModelBaseName.substr(0, pos);

    string strComponentName = "Prototype_Component_Model_" + strModelBaseName;

    _tchar szComponentName[MAX_PATH] = {};
    MultiByteToWideChar(CP_ACP, 0, strComponentName.c_str(), static_cast<_int>(strComponentName.length()), szComponentName, MAX_PATH);

    if (FAILED(__super::Add_Component(m_eCurrentLevel, szComponentName, TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

//HRESULT CLadderObject::Ready_Ladder_Components()
//{
//    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
//        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
//        return E_FAIL;
//
//    string strModelBaseName = m_szName;
//    size_t pos = strModelBaseName.find_last_of('_');
//
//    if (pos != string::npos)
//        strModelBaseName = strModelBaseName.substr(0, pos);
//
//    string strComponentName = "Prototype_Component_Model_" + strModelBaseName;
//
//    _tchar szComponentName[MAX_PATH] = {};
//    MultiByteToWideChar(CP_ACP, 0, strComponentName.c_str(), static_cast<_int>(strComponentName.length()), szComponentName, MAX_PATH);
//
//    if (FAILED(__super::Add_Component(m_eCurrentLevel, szComponentName, TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
//        return E_FAIL;
//
//
//    return S_OK;
//}

HRESULT CLadderObject::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

CLadderObject* CLadderObject::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CLadderObject* pInstance = new CLadderObject(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CLadderObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLadderObject::Clone(void* _pArg)
{
    CLadderObject* pInstance = new CLadderObject(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CStatCLadderObjecticObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLadderObject::Free()
{
    __super::Free();
}
