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
#include "Player.h"

CBoss_Magician::CBoss_Magician(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CBoss_Magician::CBoss_Magician(const CBoss_Magician& Prototype)
	:CMonster(Prototype)
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
	m_fRotateSpeed = 180.f;
	m_fRootDistance = 1.f;
	m_fActive_Distance = 15.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects(pArg)))
		return E_FAIL;

	m_pNavigationCom->Set_CurrentNaviIndex(XMLoadFloat4(&m_vSpawnPoint));
	m_Player_Attack = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower_Ptr();
	m_Player_State = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PhaseState_Ptr();
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));

	m_pState_Manager = CState_Machine<CBoss_Magician>::Create();
	if (m_pState_Manager == nullptr)
		return E_FAIL;


	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.5f,0.5f,0.2f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
	m_pStunActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 1.f,1.f,1.f }, _float3{ 0.f,0.f,1.f }, 90.f, this);

	_uint settingColliderGroup = GROUP_TYPE::PLAYER | GROUP_TYPE::PLAYER_WEAPON;
	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::MONSTER, settingColliderGroup);

	settingColliderGroup = GROUP_TYPE::PLAYER;
	m_pGameInstance->Set_CollisionGroup(m_pStunActor, GROUP_TYPE::MONSTER, settingColliderGroup);

	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,0.f,1.f });
	m_pGameInstance->Set_GlobalPos(m_pStunActor, _fvector{ 0.f,0.f,5.f,1.f });

	m_pGameInstance->Add_Actor_Scene(m_pActor);

	return S_OK;
}

void CBoss_Magician::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CBoss_Magician::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pState_Manager->State_Update(fTimeDelta, this);

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
		m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CBoss_Magician::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Magician::Render()
{
	return S_OK;
}

HRESULT CBoss_Magician::Ready_Components(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
		return E_FAIL;

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

	return S_OK;
}

HRESULT CBoss_Magician::Ready_PartObjects(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	CBody_Magician::BODY_MAGICIAN_DESC BodyDesc{};
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.IsDissolveOn = &m_IsDissolveOn;
	BodyDesc.IsDissolveOff = &m_IsDissolveOff;
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Magician"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Body"), &BodyDesc)))
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
	Weapon_Desc.IsDissolveOn = &m_IsDissolveOn;
	Weapon_Desc.IsDissolveOff = &m_IsDissolveOff;
	Weapon_Desc.Is_Change_Sword_Bone = &Is_Change_Sword_Bone;
	Weapon_Desc.bCane_Collider_On = &m_bCane_Collider_On;
	Weapon_Desc.bSpecial_Skill_Progress = &m_bSpecial_Skill_Progress;
	Weapon_Desc.bCatch_Special_Attack = &m_bCatch_Special_Attack;
	Weapon_Desc.iAttack = &m_iMonster_Attack_Power;
	Weapon_Desc.fSpeedPerSec = 0.f;
	Weapon_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Magician_Sword"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Sword"), &Weapon_Desc)))
		return E_FAIL;

	CWeapon_Cane::MAGICIAN_CANE_DESC		Weapon2_Desc{};
	Weapon2_Desc.pParent = this;
	Weapon2_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_Cane");
	Weapon2_Desc.pParentState = &m_iMonster_State;
	Weapon2_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon2_Desc.pParentModel = m_pModelCom;
	Weapon2_Desc.IsDissolveOn = &m_IsDissolveOn;
	Weapon2_Desc.IsDissolveOff = &m_IsDissolveOff;
	Weapon2_Desc.bCane_Collider_On = &m_bCane_Collider_On;
	Weapon2_Desc.iAttack = &m_iMonster_Attack_Power;
	Weapon2_Desc.fSpeedPerSec = 0.f;
	Weapon2_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Magician_Cane"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician_Cane"), &Weapon2_Desc)))
		return E_FAIL;

	CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
	Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("spine_02");
	Locked_On_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Locked_On_Desc.pParentState = &m_iMonster_State;
	Locked_On_Desc.bLocked_On_Active = &m_bLocked_On;
	Locked_On_Desc.fSpeedPerSec = 0.f;
	Locked_On_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Locked_On"), LEVEL_STATIC, TEXT("Prototype_GameObject_Monster_Locked_On"), &Locked_On_Desc)))
		return E_FAIL;

	CUI_Boss_HP_Bar::UI_BOSS_HP_BAR_DESC pBoss_HP_Bar = {};
	pBoss_HP_Bar.fMaxHP = &m_fMonsterMaxHP;
	pBoss_HP_Bar.fCurHP = &m_fMonsterCurHP;
	pBoss_HP_Bar.fShieldHP = &m_fShieldHP;
	pBoss_HP_Bar.bBossActive = &m_bActive;
	pBoss_HP_Bar.bBossDead = &m_bDead;
	pBoss_HP_Bar.iPhase = &m_iPhase;
	pBoss_HP_Bar.iCurLevel = iLevel;
	pBoss_HP_Bar.sBossName = TEXT("오두르");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), iLevel, TEXT("Layer_UIScene"), &pBoss_HP_Bar)))
		return E_FAIL;

	return S_OK;
}

void CBoss_Magician::PatternCreate()
{
	if (m_iPhase == PHASE_TWO && m_bActive)
	{
		m_fSpecial_Skill_CoolTime += m_fTimeDelta;
		m_fSlash_Skill_CoolTime += m_fTimeDelta;
	}
	if (!m_bPatternProgress && m_bActive)
	{
		m_fDelayTime += 1 * m_fTimeDelta;
		if (m_fDelayTime >= m_fCoolTime)
		{
			m_bCan_Hit_Motion = false;

			//if (m_fSpecial_Skill_CoolTime >= 30.f)
			//	m_pState_Manager->ChangeState(new CBoss_Magician::Attack_Special(), this);
			//else if (m_fSlash_Skill_CoolTime >= 40.f)
			//	m_pState_Manager->ChangeState(new CBoss_Magician::Attack_Slash(), this);
			//else if (m_fDistance >= 5.f)
			Far_Pattern_Create();
			/*else
				Near_Pattern_Create();*/

			m_fDelayTime = 0.f;
			m_bPatternProgress = true;
		}
	}

}

void CBoss_Magician::Active()
{
	m_iPhase = PHASE_ONE;
	m_pState_Manager->ChangeState(new CBoss_Magician::Intro_State(), this);
}

void CBoss_Magician::Stun()
{
	m_IsStun = true;
	m_bPatternProgress = true;
	m_fDelayTime = 0.f;
	m_pState_Manager->ChangeState(new CBoss_Magician::Stun_State(), this);
}

_vector CBoss_Magician::Bezier_Move(_vector _vStartPos, _vector vCurvePos, _vector vEndPos, _float fTime)
{
	_vector vResult = (1 - fTime) * (1 - fTime) * _vStartPos +
		2 * (1 - fTime) * fTime * vCurvePos +
		fTime * fTime * vEndPos;

	vResult = XMVectorSetY(vResult, XMVectorGetY(_vStartPos));

	return vResult;
}


void CBoss_Magician::Near_Pattern_Create()
{
	_uint iRandomPattern = {};

	if (m_iPhase == PHASE_ONE)
		iRandomPattern = rand() % 4;
	else
		iRandomPattern = rand() % 5;

	while (true)
	{
		if ((iRandomPattern == m_iNearPatternIndex) || (!m_bCanDissapear && iRandomPattern == 3))
		{
			if (m_iPhase == PHASE_ONE)
				iRandomPattern = rand() % 4;
			else
				iRandomPattern = rand() % 5;
		}
		else
		{
			m_iNearPatternIndex = iRandomPattern;
			m_bCanDissapear = true;
			break;
		}
	}
	switch (m_iNearPatternIndex)
	{
	case 0:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboA(), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboB(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboC(), this);
		break;
	case 3:
		m_bCanDissapear = false;
		m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Move_State(1, true), this);
		break;
	case 4:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboD(), this);
		break;
	}
}

void CBoss_Magician::Far_Pattern_Create()
{
	_uint iRandomPattern = {};
	if (m_iPhase == PHASE_ONE)
		iRandomPattern = rand() % 3;
	else
		iRandomPattern = rand() % 4;

	while (true)
	{
		if ((iRandomPattern == m_iFarPatternIndex) || (!m_bCanDissapear && iRandomPattern == 0))
		{
			if (m_iPhase == PHASE_ONE)
				iRandomPattern = rand() % 3;
			else
				iRandomPattern = rand() % 4;
		}
		else
		{
			m_iFarPatternIndex = iRandomPattern;
			m_bCanDissapear = true;
			break;
		}
	}
	m_iFarPatternIndex = 2;

	switch (m_iFarPatternIndex)
	{
	case 0:
		m_bCanDissapear = false;
		m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Move_State(0), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Magician::Shoot_ComboA(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Magician::Shoot_ComboB(), this);
		break;
	case 3:
		m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboD(), this);
		break;
	}
}



void CBoss_Magician::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	/* 플레이어 무기와의 충돌 */
	if (!strcmp("PLAYER_WEAPON", _pOther->Get_Name()))
	{
		_uint iNoDamage = 1;
		if (m_iHitCount >= 3 && !m_bPatternProgress)
		{
			m_iHitCount = 0;
			m_bPatternProgress = true;
			m_bCan_Hit_Motion = false;
			m_fDelayTime = 0.f;
			iNoDamage = 0;

			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> dis(0, 3);

			_uint iRandom = dis(gen);

			switch (iRandom)
			{
			case 0:
				m_pState_Manager->ChangeState(new CBoss_Magician::Parry_Attack_A(), this);
				break;
			case 1:
				m_pState_Manager->ChangeState(new CBoss_Magician::Parry_Attack_B(), this);
				break;
			case 2:
				m_pState_Manager->ChangeState(new CBoss_Magician::Parry_Attack_C(), this);
				break;
			}
		}
		m_fRecoveryTime = 0.f;
		m_bCanRecovery = false;
		m_fMonsterCurHP -= *m_Player_Attack / 10.f * iNoDamage;
		m_fShieldHP -= (*m_Player_Attack / 10.f) * 1.5f * iNoDamage;
		if (m_bCan_Hit_Motion)
		{
			_uint iRandom = rand() % 2;
			while (true)
			{
				if (iRandom == m_iHit_Motion_Index)
					iRandom = rand() % 2;
				else
				{
					m_iHit_Motion_Index = iRandom;
					break;
				}
			}
			m_pState_Manager->ChangeState(new CBoss_Magician::Hit_State(m_iHit_Motion_Index), this);
			m_iHitCount += 1;
		}
	}

	if (!strcmp("PLAYER_PLAGUE_WEAPON", _pOther->Get_Name()))
	{
		_uint iNoDamage = 1;
		if (m_iHitCount >= 3 && !m_bPatternProgress)
		{
			m_iHitCount = 0;
			m_bPatternProgress = true;
			m_bCan_Hit_Motion = false;
			m_fDelayTime = 0.f;
			iNoDamage = 0;

			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> dis(0, 3);

			_uint iRandom = dis(gen);

			switch (iRandom)
			{
			case 0:
				m_pState_Manager->ChangeState(new CBoss_Magician::Parry_Attack_A(), this);
				break;
			case 1:
				m_pState_Manager->ChangeState(new CBoss_Magician::Parry_Attack_B(), this);
				break;
			case 2:
				m_pState_Manager->ChangeState(new CBoss_Magician::Parry_Attack_C(), this);
				break;
			}
		}
		m_fRecoveryTime = 0.f;
		m_bCanRecovery = false;
		m_fMonsterCurHP -= *m_Player_Attack / 10.f * 1.5f * iNoDamage;
		m_fShieldHP -= (*m_Player_Attack / 10.f) * iNoDamage;
		if (m_bCan_Hit_Motion)
		{
			_uint iRandom = rand() % 2;
			while (true)
			{
				if (iRandom == m_iHit_Motion_Index)
					iRandom = rand() % 2;
				else
				{
					m_iHit_Motion_Index = iRandom;
					break;
				}
			}
			m_pState_Manager->ChangeState(new CBoss_Magician::Hit_State(m_iHit_Motion_Index), this);
			m_iHitCount += 1;
		}
	}
}

void CBoss_Magician::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp("PLAYER", _pOther->Get_Name()) && /*(*m_Player_State & CPlayer::PHASE_EXECUTION) &&*/ !m_bExecution_Progress && m_iMonster_State == STATE_STUN)
	{
		m_bExecution_Progress = true;
		m_pState_Manager->ChangeState(new ExeCution_State(), this);
	}
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

	Safe_Release(m_pState_Manager);
}

#pragma region Intro_State
void CBoss_Magician::Intro_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 75;
	pObject->m_iMonster_State = STATE_INTRO;
	pObject->m_bActive = true;
	pObject->m_bCardActive = true;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Intro_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
		pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CBoss_Magician::Intro_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCardActive = false;
}
#pragma endregion

#pragma region Idle_State
void CBoss_Magician::Idle_State::State_Enter(CBoss_Magician* pObject)
{
	// 23도 Idle2임
	m_iIndex = 22;
	pObject->m_iMonster_State = STATE_IDLE;
	pObject->m_bPatternProgress = false;
	pObject->m_IsStun = false;
	pObject->m_fDelayTime = 0.f;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Idle_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new Move_State(), pObject);
	}

}

void CBoss_Magician::Idle_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_pModelCom->Set_Continuous_Ani(true);
}
#pragma endregion

#pragma region Move_State
void CBoss_Magician::Move_State::State_Enter(CBoss_Magician* pObject)
{
	//나중에 플레이어 위치받아올듯
	if (pObject->m_fDistance > 1.5f)
		m_iIndex = 69;
	else if (pObject->m_fDistance <= 1.5f)
	{
		iRandomMove = rand() % 3;
		iRandomMove = 1;
		switch (iRandomMove)
		{
		case 0:
			m_iIndex = 67;
			break;
		case 1:
			m_iIndex = 71;
			break;
		case 2:
			m_iIndex = 73;
			break;
		}
	}
	pObject->m_bPatternProgress = false;
	pObject->m_fDelayTime = 0.f;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_END;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
}

void CBoss_Magician::Move_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	pObject->RotateDegree_To_Player();
	if (m_iIndex == 69)
	{
		pObject->m_pTransformCom->Go_Straight(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
	}
	else if (m_iIndex == 67)
	{
		pObject->m_pTransformCom->Go_Backward_With_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
	}
	else if (m_iIndex == 73)
	{
		pObject->m_pTransformCom->LookAt(XMLoadFloat4(&pObject->m_vPlayerPos));
		pObject->m_pTransformCom->Go_Right_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
	}
	else
	{
		pObject->m_pTransformCom->LookAt(XMLoadFloat4(&pObject->m_vPlayerPos));
		pObject->m_pTransformCom->Go_Left_Navi(pObject->m_fTimeDelta, pObject->m_pNavigationCom);
	}
}

void CBoss_Magician::Move_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_pModelCom->Set_Continuous_Ani(true);
}
#pragma endregion

#pragma region Stun_State
void CBoss_Magician::Stun_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 56;
	pObject->m_iMonster_State = STATE_STUN;
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
	pObject->m_bMove = true;
	pObject->m_bCan_Move_Anim = true;
	pObject->RotateDegree_To_Player();

	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
	pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

	pObject->m_IsDissolveOff = false;
	pObject->m_IsDissolveOn = false;

	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Magician::Stun_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 54;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
	}
}

void CBoss_Magician::Stun_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}
#pragma endregion

#pragma region Shoot_ComboA
void CBoss_Magician::Shoot_ComboA::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 34;
	pObject->RotateDegree_To_Player();
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Shoot_ComboA::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	pObject->RotateDegree_To_Player();
	if (m_iIndex == 34 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 52.f)
		{
			m_iIndex = 35;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		}
		else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 40.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 41.f)
		{
			_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_float fPosY = XMVectorGetY(vPos);
			fPosY += 10.f;
			vPos = XMVectorSetY(vPos, fPosY);
			_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, XMLoadFloat4(&pObject->m_vPlayerPos));
		}
	}

	if (m_iIndex == 35 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
				break;
			}
		}
		else if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 20.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 21.f)
		{
			_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector vLook = pObject->m_pTransformCom->Get_State(CTransform::STATE_LOOK);
			pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, XMLoadFloat4(&pObject->m_vPlayerPos));
		}
	}
}
void CBoss_Magician::Shoot_ComboA::State_Exit(CBoss_Magician* pObject)
{
}
#pragma endregion

#pragma region Shoot_ComboB

void CBoss_Magician::Shoot_ComboB::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 33;
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Shoot_ComboB::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	_vector vPos = pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vPlayerPos = XMLoadFloat4(&pObject->m_vPlayerPos);
	_float fPosY = XMVectorGetY(vPos);
	_float fPlayerPosY = XMVectorGetY(vPlayerPos);
	fPosY += 0.5f;
	fPlayerPosY += 0.5f;
	vPos = XMVectorSetY(vPos, fPosY);
	vPlayerPos = XMVectorSetY(vPlayerPos, fPlayerPosY);


	pObject->RotateDegree_To_Player();

	if (m_iIndex == 33 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 130.f)
		{
			m_iIndex = 32;
			IsFired = false;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
			pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(10.f);
		}
		else if (!IsFired && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 109.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 110.f)
		{
			IsFired = true;
			pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_CARD, vPos, vPlayerPos, 5, false);
		}
	}
	if (m_iIndex == 32 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 52.f)
		{
			m_iIndex = 31;
			IsFired = false;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
			pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(10.f);
		}
		else if (!IsFired && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 40.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 41.f)
		{
			IsFired = true;
			pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_CARD, vPos, vPlayerPos, 5, true);
		}
	}

	if (m_iIndex == 31 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			m_iIndex = 30;
			IsFired = false;
			pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		}
		else if (!IsFired && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 19.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 20.f)
		{
			IsFired = true;
			pObject->m_pGameInstance->Fire_Multi_Projectile(PROJECTILE_CARD, vPos, vPlayerPos, 5, false);
		}
	}
	if (m_iIndex == 30 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->GetAniFinish())
		{
			_uint iRandom = rand() % 2;
			switch (iRandom)
			{
			case 0:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
				break;
			case 1:
				pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
				break;
			}
		}
		else if (!IsFired && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 40.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 41.f)
		{
			//아마 필살기 날릴 부분.
			IsFired = true;
			pObject->m_pGameInstance->Fire_Projectile(PROJECTILE_CARD, vPos, vPlayerPos);
		}
	}
}

void CBoss_Magician::Shoot_ComboB::State_Exit(CBoss_Magician* pObject)
{
}

#pragma endregion

#pragma region Attack_ComboA(한번 베고 지팡이 돌린뒤 쿡 찌르기)
void CBoss_Magician::Attack_ComboA::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 6;
	pObject->m_bCane_Collider_On = true;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTSF;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_ComboA::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (m_iIndex == 6 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		m_iIndex = 5;
		pObject->m_iMonster_State = STATE_ATTACK;
		pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTSL;
		pObject->RotateDegree_To_Player();
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}

	if (m_iIndex == 5 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 100.f)
	{
		pObject->m_iPlayer_Hitted_State = PLAYER_HURT_KnockBackF;
	}

	if (m_iIndex == 5 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 250.f)
		pObject->m_bCan_Hit_Motion = true;

	if (m_iIndex == 5 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
			break;
		}
	}

}

void CBoss_Magician::Attack_ComboA::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCane_Collider_On = false;
}
#pragma endregion

#pragma region Attack_ComboB(발차기 두번하고 베는 애니메이션(끝나면 뒤로 슉 하고 사라짐))
void CBoss_Magician::Attack_ComboB::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 25;
	pObject->m_iMonster_State = STATE_KICK_ATTACK;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTSF;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_ComboB::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (m_iIndex == 25 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 100.f)
	{
		pObject->m_iMonster_State = STATE_ATTACK;
		pObject->RotateDegree_To_Player();
		pObject->Is_Change_Sword_Bone = true;
		pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTMFL;
	}

	if (m_iIndex == 25 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Move_State(1), pObject);
	}
}

void CBoss_Magician::Attack_ComboB::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Hit_Motion = true;
	pObject->Is_Change_Sword_Bone = false;
}
#pragma endregion

#pragma region Attack_ComboC(준비자세후 한번베고 발차기)
void CBoss_Magician::Attack_ComboC::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 4;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTMFL;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_ComboC::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 150.f)
	{
		pObject->RotateDegree_To_Player();
		pObject->m_iMonster_State = STATE_KICK_ATTACK;
		pObject->m_iPlayer_Hitted_State = PLAYER_HURT_KnockBackF;
	}
	if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 220.f)
		pObject->m_bCan_Hit_Motion = true;

	if (m_iIndex == 4 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Magician::Attack_ComboC::State_Exit(CBoss_Magician* pObject)
{
}
#pragma endregion

#pragma region Attack_ComboD((2페이지 패턴),달려가면서 플레이어쪽으로 여러 베기동작 실행)

void CBoss_Magician::Attack_ComboD::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 60;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iPlayer_Hitted_State = PLAYER_HURT_HURTMFL;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(0.f);

}

void CBoss_Magician::Attack_ComboD::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	//아마 이쪽에서 베지어 무빙 해야할듯?
	if (m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		m_fLinearTime += fTimeDelta;
		_vector vNextPos = pObject->Bezier_Move(XMLoadFloat4(&m_vStartPos), XMLoadFloat4(&m_vCurvePos), XMLoadFloat4(&m_vEndPos), m_fLinearTime);
		_vector vLook = XMVector3Normalize(vNextPos - XMLoadFloat4(&m_vStartPos));
		pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNextPos);
		pObject->m_pTransformCom->Look(vLook);
	}
	//베지어 아닐땐 플레이어 바라보게하기
	else
		pObject->RotateDegree_To_Player();

	//특정 
	if (!m_bFirst && m_iIndex == 60 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 83.f)
	{
		pObject->m_IsDissolveOn = true;
	}

	// 달려가는 부분 끝나면 이동하기위해 디절브 먹이고 베지어로 이동하기 구분 
	if (!m_bFirst && m_iIndex == 60 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 93.f)
	{
		m_bFirst = true;
		m_iIndex = 10;
		pObject->m_bMove = false;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);

		//베지어 이동시키기
		XMStoreFloat4(&m_vStartPos, pObject->m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		XMStoreFloat4(&m_vEndPos, XMLoadFloat4(&pObject->m_vPlayerPos));
		_vector vDir = XMVector3Normalize(XMLoadFloat4(&m_vEndPos) - XMLoadFloat4(&m_vStartPos));
		_vector vRight = XMVector3Cross(vDir, XMVectorSet(0.f, 1.f, 0.f, 0.f));
		vRight *= 4.f;
		if (rand() % 2 == 0)
			vRight *= -1;

		_vector vMidPoint = (XMLoadFloat4(&m_vStartPos) + XMLoadFloat4(&m_vEndPos)) * 0.5f;
		XMStoreFloat4(&m_vCurvePos, vMidPoint + vRight);
		XMStoreFloat4(&m_vCurvePos, XMVectorSetY(XMLoadFloat4(&m_vCurvePos), XMVectorGetY(XMLoadFloat4(&m_vStartPos))));

	}
	//거리가 1.f이하로 가까워지거나 Linear타임 다 끝나면 다시 등장해서 공격하기 진행
	if ((m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance <= 1.f && pObject->m_pModelCom->Get_Current_Animation_Index() == 10) || m_fLinearTime >= 1.f)
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		m_iIndex = 60;
		m_fLinearTime = 0.f;
		pObject->m_bMove = true;
		pObject->m_pModelCom->Set_Continuous_Ani(true);
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
		pObject->m_pModelCom->Get_NextAnimation()->Set_StartOffSetTrackPosition(93.f);
	}

	//특정 프레임에 맞게 칼에 뼈 위치 바꿔주기
	if (m_iIndex == 60 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 110.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 430.f)
		pObject->Is_Change_Sword_Bone = true;
	else if (m_iIndex == 60 && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 430.f)
	{
		pObject->m_bCan_Hit_Motion = true;
		pObject->Is_Change_Sword_Bone = false;
	}

	//특정 프레임에 케인 콜라이더로 변경 했다가 다시 검 콜라이더로 돌아가기
	if (m_iIndex == 60 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 121.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 156.f)
		pObject->m_bCane_Collider_On = true;
	else
		pObject->m_bCane_Collider_On = false;

	if (m_iIndex == 60 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
			break;
		}
	}

}

void CBoss_Magician::Attack_ComboD::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_IsDissolveOn = false;
	pObject->m_IsDissolveOff = false;
}

#pragma endregion

#pragma region Excution_State
void CBoss_Magician::ExeCution_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 63;
	pObject->m_iMonster_State = STATE_EXECUTION;
	pObject->RotateDegree_To_Player();
	pObject->m_bCan_Move_Anim = true;
	pObject->m_pModelCom->Set_Continuous_Ani(true);

	_vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerPos = XMLoadFloat4(&pObject->m_vPlayerPos);
	vPlayerLook = XMVector3Normalize(vPlayerLook);
	vPlayerLook *= 2.f;
	_vector vResultPos = vPlayerPos + vPlayerLook;
	pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vResultPos);

	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Magician::ExeCution_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{

	if (m_iIndex == 63 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_iPhase == PHASE_ONE && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);
		pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pActor);
		pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
	}
	if (m_iIndex == 63 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_iPhase == PHASE_TWO && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Jump_State, pObject);
	}

}

void CBoss_Magician::ExeCution_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Move_Anim = false;
	pObject->m_bExecution_Progress = false;
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
	if (pObject->m_iPhase == PHASE_ONE)
	{
		pObject->m_iPhase = PHASE_TWO;
		pObject->m_fMonsterCurHP = pObject->m_fMonsterMaxHP;
		pObject->m_bCanRecovery = true;
	}
}

#pragma endregion

#pragma region Hit_State

CBoss_Magician::Hit_State::Hit_State(_uint iHit_Index)
{
	m_iHit_Index = iHit_Index;
}

void CBoss_Magician::Hit_State::State_Enter(CBoss_Magician* pObject)
{
	switch (m_iHit_Index)
	{
	case 0:
		m_iIndex = 18;
		break;
	case 1:
		m_iIndex = 19;
		break;
	}
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_bCan_Move_Anim = true;
	pObject->RotateDegree_To_Player();
	pObject->Is_Change_Sword_Bone = false;
	pObject->m_bPatternProgress = false;
	pObject->m_bMove = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Hit_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
		pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
}

void CBoss_Magician::Hit_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Move_Anim = false;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
}
#pragma endregion

#pragma region Dissappear_Move_State

CBoss_Magician::Dissappear_Move_State::Dissappear_Move_State(_uint iDissappear_Index, _bool bShootCard)
{
	m_iDissappear_Index = iDissappear_Index;
	m_bShootCard = bShootCard;
}

void CBoss_Magician::Dissappear_Move_State::State_Enter(CBoss_Magician* pObject)
{
	switch (m_iDissappear_Index)
	{
	case 0:
		//앞으로 이동
		m_iIndex = 70;
		break;
	case 1:
		//뒤로 이동
		m_iIndex = 68;
		break;
	case 2:
		//왼쪽 이동
		m_iIndex = 72;
		break;
	case 3:
		//오른쪽으로 이동
		m_iIndex = 74;
		break;
	default:
		break;
	}
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_IsDissolveOn = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	pObject->m_bCan_Move_Anim = true;
}

void CBoss_Magician::Dissappear_Move_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	//앞
	if (m_iIndex == 70 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 10;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
	}
	if (m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
		pObject->RotateDegree_To_Player();
	if (m_iIndex == 10 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_fDistance <= 1.5f)
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		pObject->Near_Pattern_Create();
	}
	//뒤
	if (m_iIndex == 68 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 8;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}
	if (m_iIndex == 8 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		//if (!m_bShootCard)
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Idle_State(), pObject);
	}

	//왼
	if (m_iIndex == 72 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 11;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}
	if (m_iIndex == 11 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Idle_State(), pObject);
	}

	if (m_iIndex == 74 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 13;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}
	if (m_iIndex == 13 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_IsDissolveOn = false;
		pObject->m_IsDissolveOff = true;
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Dissappear_Idle_State(), pObject);
	}


}

void CBoss_Magician::Dissappear_Move_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Move_Anim = false;
	pObject->m_bCan_Hit_Motion = true;
}
#pragma endregion

#pragma region Dissappear_Idle_State

void CBoss_Magician::Dissappear_Idle_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 15;
	pObject->m_bPatternProgress = false;
	pObject->m_fDelayTime = 0.f;
	pObject->m_iMonster_State = STATE_IDLE;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Dissappear_Idle_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new Move_State(), pObject);
	}
}

void CBoss_Magician::Dissappear_Idle_State::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_IsDissolveOff = false;
}
#pragma endregion

#pragma region Attack_Special
void CBoss_Magician::Attack_Special::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 40;
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_bSpecial_Skill_Progress = true;
	pObject->m_bCan_Hit_Motion = false;
	pObject->RotateDegree_To_Player();
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_Special::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (m_iIndex == 39 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 15.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 200.f)
		pObject->Is_Change_Sword_Bone = true;
	else
	{
		pObject->RotateDegree_To_Player();
		pObject->Is_Change_Sword_Bone = false;
	}
	if (m_iIndex == 40 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 39;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
	}

	if (m_iIndex == 39 &&
		pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex &&
		pObject->m_pModelCom->Get_Current_Animation_Index() == 39 &&
		pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 30.f &&
		pObject->m_bCatch_Special_Attack)
	{
		pObject->m_pState_Manager->ChangeState(new Attack_Special_Catch(), pObject);
		return;
	}


	if (m_iIndex == 39 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 35.f)
	{
		pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
		pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
	}
	if (m_iIndex == 39 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 190.f)
	{
		pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_FallDown;
		pObject->m_iMonster_State = STATE_SPECIAL_ATTACK;
	}
	//특별 컷씬 실패했을때 그냥 Idle이나 Move로 돌아감
	if (m_iIndex == 39 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_Current_Animation_Index() == 39 && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Magician::Attack_Special::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_bCatch_Special_Attack = false;
	pObject->m_bSpecial_Skill_Progress = false;
	pObject->m_fSpecial_Skill_CoolTime = 0.f;
}
#pragma endregion

#pragma region Attack_Special_Catch
void CBoss_Magician::Attack_Special_Catch::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 41;
	pObject->Is_Change_Sword_Bone = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}


void CBoss_Magician::Attack_Special_Catch::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (m_iIndex == 41 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 290.f)
	{
		pObject->Is_Change_Sword_Bone = false;
	}
	if (m_iIndex == 41 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Magician::Attack_Special_Catch::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_fSpecial_Skill_CoolTime = 0.f;
}
#pragma endregion

#pragma region Attack_Slash
void CBoss_Magician::Attack_Slash::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 57;
	pObject->m_bCan_Hit_Motion = false;
	pObject->m_iMonster_State = STATE_SPECIAL_ATTACK2;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Attack_Slash::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 246.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 340.f)
		pObject->Is_Change_Sword_Bone = true;
	else
	{
		pObject->RotateDegree_To_Player();
		pObject->Is_Change_Sword_Bone = false;
	}
	if (m_iIndex == 57 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Magician::Attack_Slash::State_Exit(CBoss_Magician* pObject)
{
	pObject->m_bCan_Hit_Motion = true;
	pObject->m_fSlash_Skill_CoolTime = 0.f;
}
#pragma endregion

#pragma region Parry_Attack_A (뒤로 살짝 빠졌다가 앞으로 칼로 두번 긁는 패링)

void CBoss_Magician::Parry_Attack_A::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 26;
	pObject->m_iMonster_State = STATE_PARRY;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Parry_Attack_A::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (m_iIndex == 26 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 140.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 161.f)
	{
		pObject->m_iMonster_Attack_Power = 66;
		pObject->m_iMonster_State = STATE_PARRY_ATTACK;
		pObject->Is_Change_Sword_Bone = true;
		pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSF;
	}
	if (m_iIndex == 26 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 165.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 181.f)
	{
		pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_HURTSL;
	}
	if (m_iIndex == 26 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 185.f && pObject->Is_Change_Sword_Bone)
	{
		pObject->Is_Change_Sword_Bone = false;
	}

	if (m_iIndex == 26 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Magician::Parry_Attack_A::State_Exit(CBoss_Magician* pObject)
{
	pObject->Is_Change_Sword_Bone = false;
}

#pragma endregion

#pragma region Parry_Attack_B (발로 차는 패링)

void CBoss_Magician::Parry_Attack_B::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 27;
	pObject->m_iMonster_State = STATE_PARRY;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Parry_Attack_B::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (m_iIndex == 27 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 140.f && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() <= 171.f)
	{
		pObject->m_iMonster_Attack_Power = 48;
		pObject->m_iMonster_State = STATE_PARRY_ATTACK;
		pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_KnockBackF;
	}
	if (m_iIndex == 27 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		_uint iRandom = rand() % 2;
		switch (iRandom)
		{
		case 0:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Idle_State(), pObject);
			break;
		case 1:
			pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Move_State(), pObject);
			break;
		}
	}
}

void CBoss_Magician::Parry_Attack_B::State_Exit(CBoss_Magician* pObject)
{
}
#pragma endregion

#pragma region Parry_Attack_C (뒤로 점프하는 패링)

void CBoss_Magician::Parry_Attack_C::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 29;
	pObject->m_iMonster_State = STATE_PARRY;
	pObject->m_iMonster_Attack_Power = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_iPlayer_Hitted_State = Player_Hitted_State::PLAYER_HURT_REBOUND;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Parry_Attack_C::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (m_iIndex == 29 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Magician::Attack_ComboA(), pObject);
	}
}

void CBoss_Magician::Parry_Attack_C::State_Exit(CBoss_Magician* pObject)
{
}

#pragma endregion

#pragma region Dissappear_Jump_State

void CBoss_Magician::Dissappear_Jump_State::State_Enter(CBoss_Magician* pObject)
{
	m_iIndex = 52;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_bCan_Move_Anim = true;
	pObject->m_iMonster_State = STATE_DEAD;

	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);

	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Magician::Dissappear_Jump_State::State_Update(_float fTimeDelta, CBoss_Magician* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 35.f)
	{
		pObject->m_IsDissolveOn = true;
	}
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		if (!m_Is_Spawn)
		{
			m_Is_Spawn = true;
			CGameObject::GAMEOBJECT_DESC pDesc = {};
			pDesc.iCurLevel = LEVEL_SEAOFTREES;
			pDesc.fPosition = pObject->m_vSpawnPoint;
			if (FAILED(pObject->m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician2"), CATEGORY_BOSS, &pDesc)))
				return;
		}
		pObject->m_bDead = true;
		pObject->m_bActive = false;
	}
}

void CBoss_Magician::Dissappear_Jump_State::State_Exit(CBoss_Magician* pObject)
{
}
#pragma endregion