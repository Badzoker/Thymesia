#include "pch.h"
#include "Weapon_Dagger.h"
#include "GameInstance.h"
#include "Normal_VillageF1.h"
#include "Animation.h"

CWeapon_Dagger::CWeapon_Dagger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject(pDevice, pContext)
{
}

CWeapon_Dagger::CWeapon_Dagger(const CWeapon_Dagger& Prototype)
	:CPartObject(Prototype)
{
}

HRESULT CWeapon_Dagger::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CWeapon_Dagger::Initialize(void* pArg)
{
	strcpy_s(m_szName, "MONSTER_WEAPON");

	WEAPON_DAGGER_DESC* pDesc = static_cast<WEAPON_DAGGER_DESC*>(pArg);

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

void CWeapon_Dagger::Priority_Update(_float fTimeDelta)
{
	if (*m_pParentState == STATE_DEAD)
	{
		m_fDeadTimer += fTimeDelta * 0.5f;
		m_fFinishTime += fTimeDelta * 0.5f;
	}
}

void CWeapon_Dagger::Update(_float fTimeDelta)
{
	_matrix			SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *    /* 월드 영역 */
		SocketMatrix *  /* 로컬 스페이스 영역 */
		XMLoadFloat4x4(m_pParentWorldMatrix)   /* 월드 영역 */
	);
	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(&m_CombinedWorldMatrix), _vector{ 10, 0.f,0.f,1.f });
	if (*m_pParentState != STATE_STUN && *m_pParentState != STATE_DEAD)
	{
		for (auto& iter : *m_pParentModelCom->Get_VecAnimation().at(m_pParentModelCom->Get_Current_Animation_Index())->Get_vecEvent())
		{
			if (iter.isPlay == false)
			{
				if (iter.eType == EVENT_COLLIDER && iter.isEventActivate == true && *m_pParentState != STATE_STUN)
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
	else
		m_pGameInstance->Sub_Actor_Scene(m_pActor);
}

void CWeapon_Dagger::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_SHADOW, this);
}

HRESULT CWeapon_Dagger::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

		if (*m_pParentState == STATE_DEAD)
		{
			m_iPassNum = 9;
			if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_Time", &m_fDeadTimer, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Bind_RawValue("g_DissolveAmount", &m_fFinishTime, sizeof(_float))))
				return E_FAIL;
		}
		else
			m_iPassNum = 0;


		m_pShaderCom->Begin(m_iPassNum);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CWeapon_Dagger::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Dagger"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster_Noise"),
		TEXT("Com_Noise"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Dagger::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CWeapon_Dagger::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
}

void CWeapon_Dagger::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CWeapon_Dagger::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CWeapon_Dagger* CWeapon_Dagger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Dagger* pInstance = new CWeapon_Dagger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CWeapon_Dagger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Dagger::Clone(void* pArg)
{
	CWeapon_Dagger* pInstance = new CWeapon_Dagger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CWeapon_Dagger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWeapon_Dagger::Free()
{
	__super::Free();


	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);
}
