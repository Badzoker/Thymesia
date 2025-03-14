#include "pch.h"

#include "StaticObject.h"

#include "GameInstance.h"

CStaticObject::CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CObject(pDevice, pContext)
{
}

CStaticObject::CStaticObject(const CStaticObject& Prototype)
	: CObject(Prototype)
{
}

HRESULT CStaticObject::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pButtonGameObject = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Button"), "BUTTON");
	m_pButton = static_cast<CButton*>(m_pButtonGameObject);

	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_SPHERE, _float3{ 0.5f, 0.5f, 0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
	_uint iSettingColliderGroup = GROUP_TYPE::PLAYER;
	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,20.f,0.f,1.f });
	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::OBJECT, iSettingColliderGroup);
	m_pGameInstance->Add_Actor_Scene(m_pActor);

	return S_OK;
}

void CStaticObject::Priority_Update(_float fTimeDelta)
{
}

void CStaticObject::Update(_float fTimeDelta)
{
	if (!strcmp(m_MeshName, ("P_Archive_Chair01")) || !strcmp(m_MeshName, ("Ladder")))
	{
		if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
			m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 0.f,0.f,1.f });
	}
}

void CStaticObject::Late_Update(_float fTimeDelta)
{
	if(m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), m_fFrustumRadius))
		m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_OCCULUSION, this);

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);

	m_pGameInstance->Add_RenderGroup(CRenderer::RG_MOTION_BLUR, this);	
}

HRESULT CStaticObject::Render()
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

HRESULT CStaticObject::Render_Occulusion()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

		m_pShaderCom->Begin(6);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CStaticObject::Render_Shadow()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShaderCom, "g_LightViewMatrix", "g_LightProjMatrix")))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{

		m_pShaderCom->Begin(2);
		m_pModelCom->Render(i);
	}

	return S_OK;
}
HRESULT CStaticObject::Render_Motion_Blur()
{
	if (FAILED(Bind_Motion_Blur_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		m_pShaderCom->Begin(5);
		m_pModelCom->Render(i);
	}

	return S_OK;
}
void CStaticObject::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp(m_MeshName, ("P_Archive_Chair01")))
	{
		_vector vChairPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vChairPos = XMVectorSetY(vChairPos, XMVectorGetY(vChairPos) + 1.0f);

		_float4 vChairPosition;
		XMStoreFloat4(&vChairPosition, vChairPos);

		m_pButton->Set_WorldPosition(vChairPosition);
		m_pButton->Set_ButtonText(TEXT("E"), TEXT("앉기"));
		m_pButton->Activate_Button(true);
	}
}
void CStaticObject::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}
void CStaticObject::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
	m_pButton->Activate_Button(false);
}
HRESULT CStaticObject::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	string strComponentName = "Prototype_Component_Model_";
	strComponentName += m_MeshName;

	_tchar		szComponentName[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strComponentName.c_str(), static_cast<_int>(strlen(strComponentName.c_str())), szComponentName, MAX_PATH);

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, szComponentName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticObject::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticObject::Bind_Motion_Blur_ShaderResources()
{
	/* 물체는 멈춰있기 때문에 월드 매트릭스를 바인딩 안해도 되겠다. */
	if (FAILED(m_pShaderCom->Bind_Matrix("g_PreViewMatrix", &m_pGameInstance->Get_PreTransform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CStaticObject* CStaticObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStaticObject* pInstance = new CStaticObject(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CStaticObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStaticObject::Clone(void* pArg)
{
	CStaticObject* pInstance = new CStaticObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CStaticObject");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticObject::Free()
{
	__super::Free();
}
