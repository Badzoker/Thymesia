#include "pch.h"
#include "DeadBranch.h"
#include "GameInstance.h"

CDeadBranch::CDeadBranch(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CItem{ _pDevice, _pContext }
{
}

CDeadBranch::CDeadBranch(const CDeadBranch& _Prototype)
    :CItem(_Prototype)
{
}

HRESULT CDeadBranch::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CDeadBranch::Initialize(void* _pArg)
{
    DEADBRANCH_DESC* pDesc = static_cast<DEADBRANCH_DESC*>(_pArg);

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    strcpy_s(m_szName, pDesc->GameItemName.c_str());
    LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);
    m_pButtonGameObject = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Button"), "BUTTON");
    m_pButton = static_cast<CButton*>(m_pButtonGameObject);

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Scaling(_float3(0.01f, 0.01f, 0.01f));

    m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.5f, 0.5f, 0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
    _uint iSettingColliderGroup = GROUP_TYPE::PLAYER;
    m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
    m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::ITEM, iSettingColliderGroup);
    m_pGameInstance->Add_Actor_Scene(m_pActor);

    _vector vTestPosition = { 83.19f, 5.3f, -117.27f, 1.f };
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vTestPosition);

    return S_OK;
}

void CDeadBranch::Priority_Update(_float _fTimeDelta)
{
    


}

void CDeadBranch::Update(_float _fTimeDelta)
{
    if (m_bDeActivate)
        return;

    if (!m_bDeActivate)
    {
        if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
            m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 0.f,0.f,1.f });
    }

    static _float fTimeFlow = 0.0f;
    fTimeFlow += _fTimeDelta;

    m_fBranchAlpha = (sinf(fTimeFlow * 2.0f) * 0.5f) + 0.75f;

    if (m_bDissolving)
    {
        m_fDissolveTime += _fTimeDelta * 0.4f;
        //m_fDissolveTime = min(m_fDissolveTime, 1.0f);

        if (m_fDissolveTime >= 1.0f)
        {
            m_fDissolveTime = 1.0f;
            m_bDeActivate = true;
        }
    }

}

void CDeadBranch::Late_Update(_float fTimeDelta)
{
    //m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND,this);
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_GLOW, this);
}

HRESULT CDeadBranch::Render()
{
    //if (m_bDeActivate)
    //    return S_OK;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

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

HRESULT CDeadBranch::Render_Glow()
{
    if (m_bDeActivate)
        return S_OK;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (_uint i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_fObjectAlpha", &m_fBranchAlpha, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveValue", &m_fDissolveTime, sizeof(_float))))
            return E_FAIL;

        if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 0)))
            return E_FAIL;

        m_pShaderCom->Begin(12);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CDeadBranch::Ready_Components()
{
    string strComponentName = "Prototype_Component_Model_";
    strComponentName += m_szName;

    _tchar		szComponentName[MAX_PATH] = {};

    MultiByteToWideChar(CP_ACP, 0, strComponentName.c_str(), static_cast<_int>(strlen(strComponentName.c_str())), szComponentName, MAX_PATH);

    if (FAILED(__super::Add_Component(LEVEL_STATIC, szComponentName, TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"), TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BranchDissolve"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CDeadBranch::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CDeadBranch::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
    if (m_bDeActivate || m_bDissolving)
        return;

    _vector vBranchPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vBranchPosition = XMVectorSetY(vBranchPosition, XMVectorGetY(vBranchPosition) + 1.0f);

    _float4 vBranchPos;
    XMStoreFloat4(&vBranchPos, vBranchPosition);

    m_pButton->Set_WorldPosition(vBranchPos);
    m_pButton->Set_ButtonText(TEXT("E"), TEXT("기억회수"));
    m_pButton->Activate_Button(true);
}

void CDeadBranch::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
    if (m_bDeActivate)
        return;

    if (m_pGameInstance->Get_DIKeyState(DIK_E) & 0x80)
    {
        m_bDissolving = true;
        m_fDissolveTime = 0.0f;
        m_pButton->Activate_Button(false);
    }
}

void CDeadBranch::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
    if (m_bDeActivate)
        return;

    m_pButton->Activate_Button(false);
}

CDeadBranch* CDeadBranch::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CDeadBranch* pInstance = new CDeadBranch(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CDeadBranch");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDeadBranch::Clone(void* _pArg)
{
    CDeadBranch* pInstance = new CDeadBranch(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX("Failed To Cloned : CDeadBranch");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CDeadBranch::Free()
{
    __super::Free();
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
