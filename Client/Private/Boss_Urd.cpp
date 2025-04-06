#include "pch.h"
#include "Boss_Urd.h"
#include "Locked_On.h"
#include "GameInstance.h"
#include "Player.h"
#include "Body_Urd.h"
#include "Weapon_Urd_Sword.h"
#include "UI_Boss_HP_Bar.h"
#include "Boss_Urd_Camera.h"		

CBoss_Urd::CBoss_Urd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CMonster(pDevice, pContext)
{
}

CBoss_Urd::CBoss_Urd(const CBoss_Urd& Prototype)
	:CMonster(Prototype)
{
}

HRESULT CBoss_Urd::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Urd::Initialize(void* pArg)
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

	m_pNavigationCom->Set_CurCellIndex(m_pNavigationCom->Find_Closest_Cell(m_pTransformCom->Get_State(CTransform::STATE_POSITION)));
	m_Player_Attack = dynamic_cast<CPlayer*>(m_pPlayer)->Get_AttackPower_Ptr();
	m_Player_Phase = dynamic_cast<CPlayer*>(m_pPlayer)->Get_PhaseState_Ptr();
	m_Player_State = dynamic_cast<CPlayer*>(m_pPlayer)->Get_State_Ptr();
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));

	m_pState_Manager = CState_Machine<CBoss_Urd>::Create();
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

void CBoss_Urd::Priority_Update(_float fTimeDelta)
{
	if (m_bNeed_Decide_Step_Num)
	{
		m_bNeed_Decide_Step_Num = false;
		m_iCheck_Step_Num = (rand() % 3) + 1;
	}

	if(m_iMonster_State != STATE_INTRO)
		RotateDegree_To_Player();
	
	__super::Priority_Update(fTimeDelta);
}

void CBoss_Urd::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pStunActor)))
		m_pGameInstance->Update_Collider(m_pStunActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });
}

void CBoss_Urd::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBoss_Urd::Render()
{
	return S_OK;
}

void CBoss_Urd::State_Update(_float fTimeDelta)
{
	m_pState_Manager->State_Update(fTimeDelta, this);
}

void CBoss_Urd::PatternCreate()
{
	if (!m_bPatternProgress && !m_bSpecial_Skill_Progress && m_bActive)
	{
		if (m_iStep_Count < m_iCheck_Step_Num)
		{
			if (m_fDistance > 3.f)
			{
				m_pState_Manager->ChangeState(new CBoss_Urd::Step_Front_State(), this);
			}
			else
			{
				_uint iRandom = rand() % 2;
				switch (iRandom)
				{
				case 0:
					m_pState_Manager->ChangeState(new CBoss_Urd::Step_Right_State(), this);
					break;
				case 1:
					m_pState_Manager->ChangeState(new CBoss_Urd::Step_Left_State(), this);
					break;
				}
			}
		}
		else
		{
			/*m_fDelayTime += 1 * m_fTimeDelta;
			if (m_fDelayTime >= m_fCoolTime)
			{*/
			/*if (m_fSpecial_Skill_CoolTime >= 60.f)
				m_pState_Manager->ChangeState(new CBoss_Magician2::Attack_Special(), this);*/
				/*else*/ /*if (m_fDistance >= 5.f)
					Far_Pattern_Create();
				else*/
			Near_Pattern_Create();

			m_fDelayTime = 0.f;
			m_iStep_Count = 0;
			m_bPatternProgress = true;
			//}
		}
	}
}

void CBoss_Urd::Active()
{
	m_iPhase = PHASE_ONE;
	m_pState_Manager->ChangeState(new CBoss_Urd::Intro_State(), this);
}

void CBoss_Urd::Stun()
{
	m_IsStun = true;
	m_bPatternProgress = true;
	m_fDelayTime = 0.f;
	m_pState_Manager->ChangeState(new CBoss_Urd::Stun_State(), this);
}

HRESULT CBoss_Urd::Ready_Components(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	if (FAILED(__super::Add_Component(iLevel, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), nullptr)))
		return E_FAIL;

	m_pPlayer = m_pGameInstance->Get_GameObject_To_Layer(iLevel, TEXT("Layer_Player"), "PLAYER");

	return S_OK;
}

HRESULT CBoss_Urd::Ready_PartObjects(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	LEVELID iLevel = static_cast<LEVELID>(pDesc->iCurLevel);

	CBody_Urd::BODY_URD_DESC BodyDesc{};
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.fSpeedPerSec = 0.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body_Urd"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Body"), &BodyDesc)))
		return E_FAIL;


	m_pModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body_Urd"), TEXT("Com_Model")));
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CWeapon_Urd_Sword::URD_SWORD_DESC		Weapon_Desc{};
	Weapon_Desc.pParent = this;
	Weapon_Desc.pIntroSocketMatrix = m_pModelCom->Get_BoneMatrix("AnimTargetPoint");
	Weapon_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("weapon_r");
	Weapon_Desc.pParentState = &m_iMonster_State;
	Weapon_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Weapon_Desc.pParentModel = m_pModelCom;
	Weapon_Desc.bChange_Socket = &m_bChange_Socket;
	//Weapon_Desc.bSpecial_Skill_Progress = &m_bSpecial_Skill_Progress;
	//Weapon_Desc.bCatch_Special_Attack = &m_bCatch_Special_Attack;
	Weapon_Desc.iAttack = &m_iMonster_Attack_Power;
	Weapon_Desc.fSpeedPerSec = 0.f;
	Weapon_Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Urd_Sword"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Sword"), &Weapon_Desc)))
		return E_FAIL;

	CLocked_On::LOCKED_ON_DESC Locked_On_Desc = {};
	Locked_On_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("pelvis-Spine2");
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
	pBoss_HP_Bar.sBossName = TEXT("우르드");

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"), iLevel, TEXT("Layer_UIScene"), &pBoss_HP_Bar)))
		return E_FAIL;


	CBoss_Urd_Camera::CAMERA_DESC Boss_Urd_Camera_Desc = {};

	Boss_Urd_Camera_Desc.pParent = this;
	Boss_Urd_Camera_Desc.pSocketMatrix = m_pModelCom->Get_BoneMatrix("camera");
	Boss_Urd_Camera_Desc.pParentState = &m_iMonster_State;
	Boss_Urd_Camera_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	Boss_Urd_Camera_Desc.pParentModel = m_pModelCom;
	Boss_Urd_Camera_Desc.fSpeedPerSec = 0.f;
	Boss_Urd_Camera_Desc.fRotationPerSec = 0.f;
	Boss_Urd_Camera_Desc.iCurLevel = pDesc->iCurLevel;
	Boss_Urd_Camera_Desc.pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);

	if (FAILED(__super::Add_PartObject(TEXT("Part_Urd_Camera"), LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd_Camera"), &Boss_Urd_Camera_Desc)))
		return E_FAIL;



	return S_OK;
}

void CBoss_Urd::Near_Pattern_Create()
{

	_uint iRandomPattern = {};

	iRandomPattern = rand() % 4;

	while (true)
	{
		if ((iRandomPattern == m_iNearPatternIndex))
		{
			iRandomPattern = rand() % 4;
		}
		else
		{
			m_iNearPatternIndex = iRandomPattern;
			break;
		}
	}

	switch (m_iNearPatternIndex)
	{
	case 0:
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_A(), this);
		break;
	case 1:
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_B(), this);
		break;
	case 2:
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_C(), this);
		break;
	case 3:
		m_pState_Manager->ChangeState(new CBoss_Urd::Attack_Combo_D(), this);
		break;
	}
}

void CBoss_Urd::Far_Pattern_Create()
{
}

void CBoss_Urd::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
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
			//m_pState_Manager->ChangeState(new CBoss_Urd::Hit_State(m_iHit_Motion_Index), this);
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
			//m_pState_Manager->ChangeState(new CBoss_Urd::Hit_State(m_iHit_Motion_Index), this);
			m_iHitCount += 1;
		}
	}
}

void CBoss_Urd::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
}

void CBoss_Urd::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
}

CBoss_Urd* CBoss_Urd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Urd* pInstance = new CBoss_Urd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Urd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Urd::Clone(void* pArg)
{
	CBoss_Urd* pInstance = new CBoss_Urd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_Urd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Urd::Free()
{
	__super::Free();

	Safe_Release(m_pState_Manager);
}

void CBoss_Urd::Intro_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 24;
	pObject->m_iMonster_State = STATE_INTRO;
	pObject->m_bActive = true;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Intro_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex)
	{
		if (pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 575.f && !pObject->m_bChange_Socket)
		{
			pObject->m_bChange_Socket = true;
		}
		if (pObject->m_pModelCom->GetAniFinish())
		{
			pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
		}
	}
}

void CBoss_Urd::Intro_State::State_Exit(CBoss_Urd* pObject)
{
}

void CBoss_Urd::Idle_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 19;
	pObject->m_iMonster_State = STATE_IDLE;
	pObject->m_bPatternProgress = false;
	pObject->m_fDelayTime = 0.f;
	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Idle_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	if (m_iIndex == 19 &&
		pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex &&
		pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f &&
		pObject->m_fDistance >= 10.f)
	{
		pObject->m_pState_Manager->ChangeState(new Move_State(), pObject);
	}
}

void CBoss_Urd::Idle_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_pModelCom->Set_Continuous_Ani(true);
}

void CBoss_Urd::Move_State::State_Enter(CBoss_Urd* pObject)
{
}

void CBoss_Urd::Move_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
}

void CBoss_Urd::Move_State::State_Exit(CBoss_Urd* pObject)
{
}

void CBoss_Urd::Stun_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 40;
	pObject->m_iMonster_State = STATE_STUN;
	pObject->m_iMonster_Execution_Category = MONSTER_EXECUTION_CATEGORY::MONSTER_START;
	pObject->m_bMove = true;
	pObject->m_bCan_Move_Anim = true;
	pObject->RotateDegree_To_Player();

	pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pActor);
	pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pStunActor);

	pObject->m_pModelCom->Set_Continuous_Ani(true);
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Stun_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		m_iIndex = 39;
		pObject->m_pModelCom->SetUp_Animation(m_iIndex, true);
	}

	if (m_iIndex == 39)//&&(*pObject->m_Player_State) == CPlayer::STATE_MAGICIAN_MUTATION_Execution)
	{
		//pObject->m_pState_Manager->ChangeState(new CBoss_Urd::ExeCution_State(), pObject);
	}

}

void CBoss_Urd::Stun_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

void CBoss_Urd::ExeCution_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 41;
	pObject->m_iMonster_State = STATE_EXECUTION;
	pObject->m_bMove = true;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_pModelCom->Set_Continuous_Ani(true);

	_float teleportDistance = 1.f;
	_vector vPlayerLook = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerRight = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT);
	_vector vPlayerPos = pObject->m_pPlayer->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	vPlayerLook = XMVector3Normalize(vPlayerLook);

	_vector vNewPos = XMVectorAdd(vPlayerPos, XMVectorScale(vPlayerLook, teleportDistance));
	pObject->m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewPos);
	pObject->RotateDegree_To_Player();

	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::ExeCution_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	if (m_iIndex == 41 && pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		if (pObject->m_iPhase == PHASE_ONE)
		{
			pObject->m_pGameInstance->Sub_Actor_Scene(pObject->m_pStunActor);
			pObject->m_pGameInstance->Add_Actor_Scene(pObject->m_pActor);
			pObject->m_pState_Manager->ChangeState(new Idle_State(), pObject);
		}
		//else
			//pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Dead_State(), pObject);
	}

}

void CBoss_Urd::ExeCution_State::State_Exit(CBoss_Urd* pObject)
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

void CBoss_Urd::Step_Front_State::State_Enter(CBoss_Urd* pObject)
{
	iRandomMove = rand() % 4;

	switch (iRandomMove)
	{
	case 0:
		m_iIndex = 32;
		break;
	case 1:
		m_iIndex = 33;
		break;
	case 2:
		m_iIndex = 34;
		break;
	case 3:
		m_iIndex = 35;
		break;
	}
	pObject->m_iStep_Count += 1;
	pObject->m_bPatternProgress = true;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Step_Front_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	pObject->RotateDegree_To_Player();
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Step_Front_State::State_Exit(CBoss_Urd* pObject)
{
}

void CBoss_Urd::Step_Back_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 31;
	pObject->m_iStep_Count += 1;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Step_Back_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	pObject->RotateDegree_To_Player();
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Step_Back_State::State_Exit(CBoss_Urd* pObject)
{
}

void CBoss_Urd::Step_Right_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 37;
	pObject->m_iStep_Count += 1;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Urd::Step_Right_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	pObject->RotateDegree_To_Player();
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Step_Right_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

void CBoss_Urd::Step_Left_State::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 36;
	pObject->m_iStep_Count += 1;
	pObject->m_bCan_Move_Anim = true;
	pObject->m_iMonster_State = STATE_MOVE;
	pObject->m_bPatternProgress = true;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Step_Left_State::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	pObject->RotateDegree_To_Player();
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->Get_CurrentAnmationTrackPosition() >= 30.f)
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Step_Left_State::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bCan_Move_Anim = false;
}

#pragma region Attack_Combo_A (3번 연속 찌르기)

void CBoss_Urd::Attack_Combo_A::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 0;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Attack_Combo_A::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Attack_Combo_A::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bNeed_Decide_Step_Num = true;
}

#pragma endregion 


#pragma region Attack_Combo_B (올려치기 이후 찌르기)

void CBoss_Urd::Attack_Combo_B::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 7;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Attack_Combo_B::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Attack_Combo_B::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bNeed_Decide_Step_Num = true;
}

#pragma endregion 


#pragma region Attack_Combo_C (대각선 이동 후에 찌르기)

void CBoss_Urd::Attack_Combo_C::State_Enter(CBoss_Urd* pObject)
{
	_uint iRandom = rand() % 2;
	switch (iRandom)
	{
	case 0:
		m_iIndex = 5;
		break;
	case 1:
		m_iIndex = 6;
		break;
	}
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);

}

void CBoss_Urd::Attack_Combo_C::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Attack_Combo_C::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bNeed_Decide_Step_Num = true;
}

#pragma endregion 

#pragma region Attack_Combo_D (기다렸다가 빠르게 한번 찌르기 (초록색공격))

void CBoss_Urd::Attack_Combo_D::State_Enter(CBoss_Urd* pObject)
{
	m_iIndex = 3;
	pObject->RotateDegree_To_Player();
	pObject->m_iMonster_State = STATE_ATTACK;
	pObject->m_pModelCom->SetUp_Animation(m_iIndex, false);
}

void CBoss_Urd::Attack_Combo_D::State_Update(_float fTimeDelta, CBoss_Urd* pObject)
{
	if (pObject->m_pModelCom->Get_Current_Animation_Index() == m_iIndex && pObject->m_pModelCom->GetAniFinish())
	{
		pObject->m_pState_Manager->ChangeState(new CBoss_Urd::Idle_State(), pObject);
	}
}

void CBoss_Urd::Attack_Combo_D::State_Exit(CBoss_Urd* pObject)
{
	pObject->m_bNeed_Decide_Step_Num = true;
}

#pragma endregion 