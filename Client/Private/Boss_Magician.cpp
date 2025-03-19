#include "pch.h"
#include "Boss_Magician.h"
#include "Body_Magician.h"
#include "Weapon_Magician_Sword.h"
#include "Weapon_Cane.h"
//#include "Weapon_Card.h"
#include "UI_Boss_HP_Bar.h"
#include "GameInstance.h"
#include "Animation.h"
#include "Locked_On.h"

CBoss_Magician::CBoss_Magician(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{
}

CBoss_Magician::CBoss_Magician(const CBoss_Magician& Prototype)
	:CContainerObject(Prototype)
{
}

HRESULT CBoss_Magician::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Magician::Initialize(void* pArg)
{
	strcpy_s(m_szName, "MONSTER");

	m_fBossMaxHP = 100.f;
	m_fBossCurHP = m_fBossMaxHP;
	m_fShieldHP = m_fBossMaxHP;
	m_fRotateSpeed = 180.f;
	m_fCoolTime = 1.f;
	CGameObject::GAMEOBJECT_DESC* Desc = static_cast<GAMEOBJECT_DESC*>(pArg);

	Desc->fSpeedPerSec = 1.f;
	Desc->fScaling = _float3{ 0.0025f,0.0025f,0.0025f };
	Desc->fRotationPerSec = XMConvertToRadians(90.f);
	XMStoreFloat4(&m_vSpawnPoint, XMLoadFloat4(&Desc->fPosition));

	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;



	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"), "PLAYER");
	m_pNavigationCom->Set_CurrentNaviIndex(XMLoadFloat4(&m_vSpawnPoint));



	m_pState_Manager = CState_Machine<CBoss_Magician>::Create();
	if (m_pState_Manager == nullptr)
		return E_FAIL;


	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.5f,0.5f,0.2f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

	_uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;

	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,0.f,1.f });

	m_pGameInstance->Add_Actor_Scene(m_pActor);


	/* 3월 9일 추가 */
	m_pGameObjectModel = m_pModelCom;

	return S_OK;
}

void CBoss_Magician::Priority_Update(_float fTimeDelta)
{
	Culling();
	if (m_bCulling)
		return;

	m_fTimeDelta = fTimeDelta;
	CalCulate_Distance();

	if (m_fDistance <= 15.f && !m_bBossActive && !m_bDead)
	{
		m_iPhase = PHASE_ONE;
		m_pState_Manager->ChangeState(new CBoss_Magician::Intro_State(), this);
	}
	__super::Priority_Update(fTimeDelta);
}

void CBoss_Magician::Update(_float fTimeDelta)
{
	if (m_bCulling)
		return;

	m_pState_Manager->State_Update(fTimeDelta, this);

	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));

	__super::Update(fTimeDelta);
	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CBoss_Magician::Late_Update(_float fTimeDelta)
{
	if (m_bCulling)
		return;

	__super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Magician::Render()
{
	return S_OK;
}

HRESULT CBoss_Magician::Ready_Components()
{
	/* Com_Navigation */
	CNavigation::NAVIGATION_DESC   Desc{};

	Desc.iCurrentCellIndex = 11;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Magician::Ready_PartObjects()
{
	CBody_Magician::BODY_MAGICIAN_DESC BodyDesc{};
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Magician"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Magician_Body"), &BodyDesc)))
		return E_FAIL;


	m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Magician"), TEXT("Com_Model")));
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CWeapon_Magician_Sword::MAGICIAN_SWORD_DESC		Weapon_Desc{};
	Weapon_Desc.pParent = this;
	Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r_Sword");
	Weapon_Desc.pParentState = &m_iMonster_State;
	Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_Desc.pParentModel = m_pModelCom;
	Weapon_Desc.fSpeedPerSec = 0.f;
	Weapon_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Magician_Sword"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Magician_Sword"), &Weapon_Desc)))
		return E_FAIL;

	CWeapon_Cane::MAGICIAN_CANE_DESC		Weapon2_Desc{};
	Weapon2_Desc.pParent = this;
	Weapon2_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_Cane");
	Weapon2_Desc.pParentState = &m_iMonster_State;
	Weapon2_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon2_Desc.pParentModel = m_pModelCom;
	Weapon2_Desc.fSpeedPerSec = 0.f;
	Weapon2_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Magician_Cane"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Magician_Cane"), &Weapon2_Desc)))
		return E_FAIL;

	//CWeapon_Card::MAGICIAN_CARD_DESC		Weapon3_Desc{};
	//Weapon3_Desc.pParent = this;
	//Weapon3_Desc.pParentState = &m_iMonster_State;
	//Weapon3_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	//Weapon3_Desc.pParentModel = m_pModelCom;
	//Weapon3_Desc.bActive = &m_bCardActive;
	//Weapon3_Desc.vPlayerPos = &m_vPlayerPos;
	//Weapon3_Desc.fSpeedPerSec = 0.f;
	//Weapon3_Desc.fRotationPerSec = 0.f;

	//if (FAILED(__super::Add_PartObject(TEXT("Part_Magician_Card"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Magician_Card"), &Weapon3_Desc)))
	//	return E_FAIL;

	CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
	Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("spine_02");
	Locked_On_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Locked_On_Desc.pParentState = &m_iMonster_State;
	Locked_On_Desc.bLocked_On_Active = &m_bLocked_On;
	Locked_On_Desc.fSpeedPerSec = 0.f;
	Locked_On_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Locked_On"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_Locked_On"), &Locked_On_Desc)))
		return E_FAIL;


	return S_OK;
}

void CBoss_Magician::RootAnimation()
{
	_vector      vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector test = { 0.f,0.f,0.f,1.f };
	/* 루트 모션 애니메션 코드 */
	m_pRootMatrix = m_pModelCom->Get_RootMotionMatrix("root");

	if ((!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModelCom->Get_LerpFinished() && m_bBossActive))
	{
		if ((m_pNavigationCom->isMove(vCurPosition) && m_fDistance > 1.5f) || m_bCan_Move_Anim)
			m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix);

		/* 2월 19일 추가 코드 */
		if (!m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
		{
			_float4x4 test = {};
			XMStoreFloat4x4(&test, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pRootMatrix)));
			const _float4x4* test2 = const_cast<_float4x4*>(&test);
			m_pTransformCom->Set_MulWorldMatrix(test2);
		}
	}
}

void CBoss_Magician::CalCulate_Distance()
{
	XMStoreFloat4(&m_vPlayerPos, m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION));
	_vector pPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	m_fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat4(&m_vPlayerPos) - pPosition));
}

void CBoss_Magician::Culling()
{
	//절두체 안에있을때
	if (!m_bBossActive)
	{
		if (m_pGameInstance->isIn_Frustum_WorldSpace(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 0.1f, FRUSTUM_TYPE::FRUSTUM_MONSTER) && !m_bDead)
		{
			m_bCulling = false;
		}
		//절두체 안에 없을때
		else
		{
			m_bCulling = true;
		}
	}
}

void CBoss_Magician::PatternCreate()
{
}

void CBoss_Magician::Near_Pattern_Create()
{
}

void CBoss_Magician::Far_Pattern_Create()
{
}

void CBoss_Magician::RotateDegree_To_Player()
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	_vector vLook2 = XMLoadFloat4(&m_vPlayerPos) - vPos;

	vLook = XMVector3Normalize(vLook);
	vLook2 = XMVector3Normalize(vLook2);

	//회전해야 하는 각도
	_float fAngle = acos(XMVectorGetX(XMVector3Dot(vLook, vLook2)));
	fAngle = XMConvertToDegrees(fAngle);
	_vector fCrossResult = XMVector3Cross(vLook, vLook2);

	if (XMVectorGetY(fCrossResult) < 0)
	{
		fAngle *= -1;
	}
	m_fRotateDegree = fAngle;

	if (fabs(m_fRotateDegree) > 1.f)
		m_bNeed_Rotation = true;
}

void CBoss_Magician::Rotation_To_Player()
{
	_float fRadians = m_fRotateSpeed * m_fTimeDelta;
	if (m_fRotateDegree < 0.f)
		fRadians *= -1;

	if (fabs(m_fRotateDegree) < fabs(fRadians))
		fRadians = m_fRotateDegree;

	m_pTransformCom->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fRadians));

	m_fRotateDegree -= fRadians;

	if (fabs(m_fRotateDegree) <= 1.f)
	{
		m_fRotateDegree = 0.f;
		m_bNeed_Rotation = false;
	}
}

void CBoss_Magician::Recovery_HP()
{
	if (m_fBossCurHP != m_fShieldHP)
	{
		m_fRecoveryTime += m_fTimeDelta;
		if (m_fRecoveryTime >= 5.f)
			m_bCanRecovery = true;
	}

	if (m_bCanRecovery)
	{
		m_fShieldHP += 0.1f;
		if (m_fShieldHP >= m_fBossCurHP)
		{
			m_fShieldHP = m_fBossCurHP;
			m_bCanRecovery = false;
			m_fRecoveryTime = 0.f;
		}
	}
}

void CBoss_Magician::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
}

void CBoss_Magician::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBoss_Magician::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBoss_Magician* CBoss_Magician::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Magician* pInstance = new CBoss_Magician(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Magician");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Magician::Clone(void* pArg)
{
	CBoss_Magician* pInstance = new CBoss_Magician(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_Magician");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Magician::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pState_Manager);
}

void CBoss_Magician::Idle_State::State_Enter(CBoss_Magician* pObject)
{
	// 23도 Idle2임
	m_iIndex = 22;
	pObject->m_iMonster_State = STATE_IDLE;
	_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	//pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, XMLoadFloat4(&pObject->m_vPlayerPos), vLook);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Idle_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new Shoot_State(), pObject);
	}

}

void CBoss_Magician::Idle_State::State_Exit(CBoss_Magician* pObject)
{
}

void CBoss_Magician::Intro_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 75;
	pObject->m_iMonster_State = STATE_INTRO;
	pObject->m_bBossActive = true;
	pObject->m_bCardActive = true;
	//pObject->m_pGameInstance->Fire_Projectile()
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Magician::Intro_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->GetAniFinish())
		pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CBoss_Magician::Intro_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCardActive = false;
}

void CBoss_Magician::Shoot_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 30;
	_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
	//pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, XMLoadFloat4(&pObject->m_vPlayerPos), vLook);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Shoot_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (m_iIndex == 30 && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 31;
		_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		//pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, XMLoadFloat4(&pObject->m_vPlayerPos), vLook);
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}
	else if (m_iIndex == 31 && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 30;
		_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
		//pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, XMLoadFloat4(&pObject->m_vPlayerPos), vLook);
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}

}

void CBoss_Magician::Shoot_State::State_Exit(CBoss_Magician* pObject)
{
}
