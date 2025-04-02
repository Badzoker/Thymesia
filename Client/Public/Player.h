#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CModel; /* 루트 모션 때매 추가 */
class CNavigation;
END


class CStateMgr;

BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum STATE
	{
		STATE_IDLE,
		STATE_RUN,
		/* LOCK_ON 상태 */
		STATE_LOCK_ON_RUN_B,
		STATE_LOCK_ON_RUN_BL,
		STATE_LOCK_ON_RUN_BR,
		STATE_LOCK_ON_RUN_FL,
		STATE_LOCK_ON_RUN_FR,
		STATE_LOCK_ON_RUN_L,
		STATE_LOCK_ON_RUN_R,

		STATE_LOCK_ON_EVADE_F,
		STATE_LOCK_ON_EVADE_B,
		STATE_LOCK_ON_EVADE_L,
		STATE_LOCK_ON_EVADE_R,
		/*---------------- */
		STATE_ATTACK,
		STATE_ATTACK_L1,
		STATE_ATTACK_L2,
		STATE_ATTACK_L3,
		STATE_ATTACK_L4,  /* 신규 추가 */
		STATE_ATTACK_L5,  /* 신규 추가 */
		STATE_ATTACK_LONG_CLAW_01,
		STATE_ATTACK_LONG_CLAW_02,

		/* 패링 관련 */
		STATE_PARRY_L,
		STATE_PARRY_R,

		STATE_PARRY_DEFLECT_LARGE,
		STATE_PARRY_DEFLECT_L_UP,
		STATE_PARRY_DEFLECT_L,

		STATE_PARRY_DEFLECT_R_UP,
		STATE_PARRY_DEFLECT_R,
		/*-----------------*/

		/* 피격 관련 */
		STATE_HurtMFR_L,
		STATE_HurtMFR_R,
		STATE_HURT_LF,
		STATE_HURT_SF,
		STATE_HURT_SL,
		STATE_HURT_HURXXLF,
		STATE_HURT_KNOCKBACK,
		STATE_HURT_KNOCKDOWN,
		STATE_HURT_FALLDOWN,
		STATE_HURT_FALLDOWN_END,
		STATE_WEAK_GETUP_F,
		STATE_REBOUND_R,
		STATE_STUNNED_START,
		STATE_STUNNED_LOOP,
		/*-----------------*/

		/* 회피 관련 */
		STATE_NORMAL_EVADE_R,
		STATE_NORMAL_EVADE_L,
		STATE_NORMAL_EVADE_FR,
		STATE_NORMAL_EVADE_FL,
		STATE_NORMAL_EVADE_F,
		STATE_NORMAL_EVADE_BR,
		STATE_NORMAL_EVADE_BL,
		STATE_NORMAL_EVADE_B,
		/* ------------------------- */

		/* 처형 모션 */
		STATE_STUN_EXECUTE,  // 처형 시작 모션	
		STATE_HARMOR_EXECUTION,
		STATE_LV1Villager_M_Execution,
		STATE_Joker_Execution,
		STATE_Varg_Execution,
		STATE_LIGHT_EXECUTION_L,	
		STATE_LIGHT_EXECUTION_R,	

		/*앉기 및 의자 상호작용 */
		STATE_ARCHIVE_SIT_START,
		STATE_ARCHIVE_SIT_LOOP,
		STATE_ARCHIVE_SIT_GETUP,
		STATE_ARCHIVE_SIT_LIGHT_UP,

		/*힐링 모션 관련 */
		STATE_HEAL,

		/*플레이어 죽음 모션 */
		STATE_DEAD,

		/*플레이어 걸어나오는 연출 WALK*/
		STATE_START_WALK,


		/* 플레이어 우클릭 차지 공격 */
		STATE_CLAW_CHARGE_START,
		STATE_CLAW_CHARGE_LOOP,
		STATE_CLAW_CHARGE_FULL_ATTACK,
		STATE_CLAW_LONG_PLUNDER_ATTACK2,	

		/* 플레이어 스킬 */
		STATE_HALBERDS_B,	
		STATE_SCYTHE_B,	
		STATE_AXE,	

		/* 플레이어 잡히는 모션 */
		STATE_CATCHED,

		/* 플레이어 잡히고 나서 다시 일어서는 모션 */
		STATE_GET_UP,


		/* 바그 처형 달려가기전 모션 */
		STATE_VARG_STUN_EXECUTE_START_R, // 291번 모션		
		/* 바그에게 달려가면서  처형 모션 */
		STATE_VARG_RUN_EXECUTION, // 297번 애니메이션	

		/* 사다리 관련 모션 */
		STATE_LADDER_CLIMB_START,

		STATE_LADDER_CLIMB_L_DOWN,
		STATE_LADDER_CLIMB_L_DOWN_END,
		STATE_LADDER_CLIMB_L_UP,
		STATE_LADDER_CLIMB_L_UP_END,

		STATE_LADDER_CLIMB_L_IDLE,

		STATE_LADDER_CLIMB_R_DOWN,
		STATE_LADDER_CLIMB_R_DOWN_END,
		STATE_LADDER_CLIMB_R_UP,
		STATE_LADDER_CLIMB_R_UP_END,
		STATE_LADDER_CLIMB_R_UP_REVERSE_END,


		STATE_LADDER_CLIMB_R_IDLE,
	};


	enum PLAYER_PHASE
	{
		PHASE_IDLE = 1,
		PHASE_FIGHT = 1 << 1,
		PHASE_LOCKON = 1 << 2,
		PHASE_HITTED = 1 << 3,
		PHASE_PARRY = 1 << 4,
		PHASE_DASH = 1 << 5,
		PHASE_EXECUTION = 1 << 6,
		PHASE_INTERACTION = 1 << 7,
		PHASE_HEAL = 1 << 8,
		PHASE_DEAD = 1 << 9,	
		PHASE_START = 1 << 10,	
		PHASE_BOSS_INTRO = 1 << 11,	
		PHASE_LADDER = 1 << 12,	
	};



private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components(void* _pArg);
	HRESULT Ready_PartObjects(void* _pArg);

public:
	void Mouse_section(_float fTimeDelta);
	void Keyboard_section(_float fTimeDelta);

public:
	_uint Get_PhaseState() { return m_iPhaseState; }	
	void Set_ParentPhaseState(_uint _PhaseState) { m_iPhaseState = _PhaseState; }
	void Sub_PhaseState(_uint _PhaseState) { m_iPhaseState &= ~_PhaseState; }		
	void Set_Lockon(_bool _bLockOn) { m_bLockOn = _bLockOn; }
	_bool Get_Lockon()const { return m_bLockOn; }
	void Can_Move();
	void Slide_Move(CGameObject* pGameObject);
	void Set_LockOnTargetMonsterPtr(CGameObject* pGameObject) { m_pTargetMonsterPtr = pGameObject; }	
	void Set_MonsterEvent(_bool _OnOff) { m_bMonsterEvent = _OnOff; }
	void Set_PlayerState(_uint _iState) { m_iState = _iState; }		

	_bool Get_MonsterEvent() { return m_bMonsterEvent; }
	CGameObject* Get_TargetObjectPtr() const { return m_pTargetMonsterPtr; }

	unordered_set<STATE>* Get_Body_State() { return &m_set_Body_States; }	
	unordered_set<STATE>* Get_Claw_Weapon_State() { return &m_set_Claw_Weapon_States; }	
	unordered_set<STATE>* Get_Right_Weapon_State() { return &m_set_Right_Weapon_States; }
	unordered_set<STATE>* Get_Player_Camera_State() { return &m_set_Player_Camera_States; }	
	unordered_set<STATE>* Get_Halberd_State() { return &m_set_Halberd_Weapon_States; }
	unordered_set<STATE>* Get_Scythe_State() { return &m_set_Scythe_Weapon_States; }
	unordered_set<STATE>* Get_Axe_State() { return &m_set_Axe_Weapon_States; }

private:
	void Player_Interaction(CGameObject* _pOther);
	void Player_Setting_PartAni();

private:
	_uint								m_iState = { STATE_IDLE };
	_uint								m_iPreState = { STATE_IDLE };

	_uint								m_iPhaseState = { PHASE_IDLE };
	_uint								m_iPrePhaseState = { PHASE_IDLE };

	_uint								m_iMonsterExectue = {};		 //  몬스터  처형 상태		

	_uint								m_iNormalExecute_Motion = {};

	_bool								m_bLockOn = { false };
	_bool								m_bMove = { false };
	_bool								m_bNextStateCanPlay = { true };
	_bool								m_bMonsterEvent = { false };


	_float								m_fChrageTime = {};

	CGameObject*						m_pTargetMonsterPtr = { nullptr };


	CNavigation* m_pNavigationCom = { nullptr };
	CModel* m_pModel = { nullptr };
	const _float4x4* m_pRootMatrix = { nullptr };
	const _float4x4* m_CombinedMatrix = { nullptr };


	PxRigidDynamic* m_pActor = { nullptr };

	unordered_set<STATE> m_set_Body_States = {};	
	unordered_set<STATE> m_set_Claw_Weapon_States = {};	
	unordered_set<STATE> m_set_Halberd_Weapon_States = {};	
	unordered_set<STATE> m_set_Right_Weapon_States = {};	
	unordered_set<STATE> m_set_Scythe_Weapon_States = {};	
	unordered_set<STATE> m_set_Axe_Weapon_States = {};	
	unordered_set<STATE> m_set_Player_Camera_States = {};	

private:
	_float								m_fTimeDelta = { 0.f };
	CStateMgr* m_pStateMgr = { nullptr };


#pragma region UI 관련 함수 
private:
	/* UI 관련 맴버 변수들 선언 */
	_int								m_iLevel = { 1 };

	_int								m_iFullHp = { 300 };
	_int								m_iCurrentHp = { 300 };

	_int								m_iFullMp = { 150 };
	_int								m_iCurrentMp = { 150 };


	_uint								m_iAttackPower = { 25 };
	_int								m_iClawAttackPower = { 200 };

	_int								m_iPotionCount = { 3 };
	_int								m_iFeathers = { 3 };

	_int								m_iMemoryFragment = { 0 };

	/* 플레이어 장착할 스킬 2칸 */
	_uint								m_iSkill_Eqip_1st = {};
	_uint								m_iSkill_Eqip_2st = {};

	/*플레이어가 약탈한 스킬 */
	_uint								m_iTake_Away_Skill = { PLAYER_SKILL_SCYTHE }; // ui 테스트용으로 임의 enum값 세팅 - 유빈

public:
	void	 Set_Level(_uint _iLevel) { m_iLevel = _iLevel; }

	void	 Set_PlayerFullHp(_int _iSetHp) { m_iFullHp = _iSetHp; }
	void	 Increase_PlayerHp(_int _iSetHp) { m_iCurrentHp += _iSetHp; }

	void	 Set_PlayerFullMp(_int _iSetMp) { m_iFullMp = _iSetMp; }
	void	 Increase_PlayerMp(_int _iSetMp) { m_iCurrentMp += _iSetMp; }

	void	 Increase_AttackPower(_int _iAttackPower) { m_iAttackPower = _iAttackPower; }
	void	 Increase_ClawAttackPower(_int _iClawAttackPower) { m_iClawAttackPower = _iClawAttackPower; }

	void     Increase_FeatherNumber(_int _iPlusFeather) { m_iFeathers += _iPlusFeather; }
	void     Increase_MemoryFragment(_int _iMemoryFragment) { m_iMemoryFragment += _iMemoryFragment; }

	void     Set_Potion_Number(_uint _iPotionCount) { m_iPotionCount = _iPotionCount; }
	void     Increase_PotionNumber(_uint _iCount) { m_iPotionCount += _iCount; }
		
	void     Set_Player_Skill_1st(_uint _iSkill) { m_iSkill_Eqip_1st = _iSkill; }	
	void     Set_Player_Skill_2st(_uint _iSkill) { m_iSkill_Eqip_2st = _iSkill; }	

	void     Set_Player_Take_Away_Skill(_uint _iSkill) { m_iTake_Away_Skill = _iSkill; }	

	_uint    Get_Player_Skill_1st() { return m_iSkill_Eqip_1st; }	
	_uint    Get_Player_Skill_2st() { return m_iSkill_Eqip_2st; }	
	_uint    Get_Player_Take_Away_Skill() { return m_iTake_Away_Skill; }	

	_int    Get_Level() { return m_iLevel; }

	_int    Get_FullHp() { return m_iFullHp; }
	_int    Get_CurrentHp() { return m_iCurrentHp; }	

	_int    Get_FullMp() { return m_iFullMp; }	
	_int    Get_CurrentMp() { return m_iCurrentMp; }	

	_int    Get_AttackPower() { return m_iAttackPower; }

	_uint* Get_AttackPower_Ptr() { return &m_iAttackPower; }	
	_uint* Get_PhaseState_Ptr() { return &m_iPhaseState; }	
	_uint* Get_State_Ptr() { return &m_iState; }
	_int    Get_ClawAttacPower() { return m_iClawAttackPower; }

	_int    Get_MemoryFragment() { return m_iMemoryFragment; }

	_int    Get_Potion_Count() { return m_iPotionCount; }
	/* ============================== */
#pragma endregion 


public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END