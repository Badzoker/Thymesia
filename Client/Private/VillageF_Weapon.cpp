#include "pch.h"
#include "VillageF_Weapon.h"
#include "GameInstance.h"
#include "Animation.h"

CVillageF_Weapon::CVillageF_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CVillageF_Weapon::CVillageF_Weapon(const CVillageF_Weapon& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CVillageF_Weapon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CVillageF_Weapon::Initialize(void* pArg)
{
	strcpy_s(m_szName, "MONSTER_WEAPON");

	VillageF_WEAPON_DESC* pDesc = static_cast<VillageF_WEAPON_DESC*>(pArg);

	m_pSocketMatrix = pDesc->pSocketMatrix;
	m_pParentState = pDesc->pParentState;
	m_pParentModelCom = pDesc->pParentModel;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	m_pTransformCom->Scaling(_float3{ 0.5f, 0.5f, 0.5f });
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(180.f));

	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.4f,0.4f,0.15f }, _float3{ 0.f,1.f,0.f }, 0.f, this);

	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,80.f,1.f });

	_uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER_WEAPON, settingColliderGroup);

	return S_OK;
}

void CVillageF_Weapon::Priority_Update(_float fTimeDelta)
{
}

void CVillageF_Weapon::Update(_float fTimeDelta)
{
	_matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
		SocketMatrix *  /* 로컬 스페이스 영역 */
		XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
	);
	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 10, 0.f,0.f,1.f });

	for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
	{
		if (iter.isPlay == false)
		{
			if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true)
				m_pGameInstance->Add_Actor_Scene(m_pActor);
			else
				m_pGameInstance->Sub_Actor_Scene(m_pActor);

			if (iter.eType != EVENT_COLLIDER && iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분    
			{
				iter.isPlay = true;
			}
		}
	}

}

void CVillageF_Weapon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CVillageF_Weapon::Render()
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

HRESULT CVillageF_Weapon::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Normal_VillageF_Weapon"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CVillageF_Weapon::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CVillageF_Weapon::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
}

void CVillageF_Weapon::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CVillageF_Weapon::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CVillageF_Weapon* CVillageF_Weapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVillageF_Weapon* pInstance = new CVillageF_Weapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVillageF_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVillageF_Weapon::Clone(void* pArg)
{
	CVillageF_Weapon* pInstance = new CVillageF_Weapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVillageF_Weapon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVillageF_Weapon::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
