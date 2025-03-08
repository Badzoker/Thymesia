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
		STATE_WALK,
		STATE_ATTACK,
		STATE_ATTACK_L1,
		STATE_ATTACK_L2,
		STATE_ATTACK_L3,
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
		STATE_WEAK_GETUP_F,
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
	};


	enum PLAYER_PHASE
	{
		PHASE_IDLE = 1,
		PHASE_FIGHT = 1 << 1,
		PHASE_LOCKON = 1 << 2,
		PHASE_HITTED = 1 << 3,
		PHASE_PARRY = 1 << 4,
		PHASE_DASH = 1 << 5,

	};

	enum Player_Hitted_State
	{
		PLAYER_HURT_KNOCKDOWN,			// 캐릭터 넉다운	
		PLAYER_HURT_HURTMFL,			// 조금 뒤로 이동하면서 휘청	
		PLAYER_HURT_HURTSF,				// 아주 조금 뒤로 이동하면서 휘청		
		PLAYER_HURT_HURTSL,				// 아주 조금 뒤로 이동하면서 휘청		
		PLAYER_HURT_HURTLF,				// 보통 길게 뒤로 이동하면서 휘청 하면서 무릎 꿇음		
		PLAYER_HURT_HURXXLF,			// 보통 길게 뒤로 이동하면서 휘청 하면서 무릎 꿇는 시간 조금 김	
		PLAYER_HURT_KnockBackF,			// 길게 뒤로 밀리면서 한손으로 땅짚고 일어남
		PLAYER_HURT_FallDown,			// 공중에 띄워지면서 날라감
		PLAYER_HURT_END
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
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	void Mouse_section(_float fTimeDelta);
	void Keyboard_section(_float fTimeDelta);

public:
	void Set_ParentPhaseState(_uint _PhaseState) { m_iPhaseState = _PhaseState; }
	void Set_Lockon(_bool _bLockOn) { m_bLockOn = _bLockOn; }
	void Can_Move();
	void Slide_Move(CGameObject* pGameObject);

private:
	_uint								m_iState = { STATE_IDLE };
	_uint								m_iPreState = { STATE_IDLE };

	_uint								m_iPhaseState = { PHASE_IDLE };
	_uint								m_iPrePhaseState = { PHASE_IDLE };

	_bool								m_bLockOn = { false };
	_bool								m_bMove = { false };
	_bool								m_bNextStateCanPlay = { true };


	CNavigation* m_pNavigationCom = { nullptr };
	CModel* m_pModel = { nullptr };
	const _float4x4* m_pRootMatrix = { nullptr };
	const _float4x4* m_CombinedMatrix = { nullptr };


	PxRigidDynamic* m_pActor = { nullptr };


private:
	_float								m_fTimeDelta = { 0.f };
	CStateMgr* m_pStateMgr = { nullptr };


#pragma region UI 관련 함수 
private:
	/* UI 관련 맴버 변수들 선언 */
	_uint								m_iLevel = { 1 };

	_uint								m_iFullHp = { 300 };
	_uint								m_iCurrentHp = { 300 };

	_uint								m_iFullMp = { 150 };
	_uint								m_iCurrentMp = { 150 };


	_uint								m_iAttackPower = { 25 };
	_uint								m_iClawAttackPower = { 200 };

	_uint								m_iFeathers = { 3 };

	_uint								m_iMemoryFragment = { 0 };

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



	_uint    Get_Level() { return m_iLevel; }

	_uint    Get_FullHp() { return m_iFullHp; }
	_uint    Get_CurrentHp() { return m_iCurrentHp; }

	_uint    Get_FullMp() { return m_iFullMp; }
	_uint    Get_CurrentMp() { return m_iCurrentMp; }

	_uint    Get_AttackPower() { return m_iAttackPower; }
	_uint    Get_ClawAttacPower() { return m_iClawAttackPower; }

	_uint    Get_MemoryFragment() { return m_iMemoryFragment; }
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