#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CNavigation;

END

BEGIN(Client)

class CBoss_Varg final : public CContainerObject
{
public:
	enum Attack_Power
	{
		Varg_Intro_State,
		Varg_Idle_State,
		Varg_Walk_State,
		Varg_Run_State,
		Varg_Stun_State,
		Varg_Hit_State,
		Varg_Dead_State,
		Varg_Avoid_State,
		Varg_Avoid_Attack_State,        // KnockDown 시키기
		Varg_Raid_Attack_01_State,          // 날리기
		Varg_Raid_Attack_02_State,          // KnockDown 시키기
		Varg_Attack_Combo_A_01_State,   // 약공 찍기
		Varg_Attack_Combo_A_02_State,   // 약공 찍기
		Varg_Attack_Combo_A_03_State,   // 강공 찌르기
		Varg_Attack_Combo_B_01_State,   // 약공 돌면서 베기
		Varg_Attack_Combo_B_02_State,   // 강공 돌아서 찍기
		Varg_Attack_Combo_B_03_State,   // knockbackF
		Varg_Attack_Combo_B_04_State,   // knockbackF
		Varg_Attack_Combo_C_01_State,   // 강공 돌면서 베기
		Varg_Attack_Roar_State,         // 기절 걸기
		Varg_Attack_Catch_State,        // 붙잡기 패턴
		Varg_Execution_First_State,
		Varg_Execution_Second_State,
		Varg_State_END
	};

private:
	CBoss_Varg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Varg(const CBoss_Varg& Prototype);
	virtual ~CBoss_Varg() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	_uint Get_Varg_State() const { return m_iState; }
public:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
public:
	void RootAnimation();
public:
	void PatternCreate();
	void Near_Pattern_Create();
	void Far_Pattern_Create();
	void RotateDegree_To_Player();
	void Rotation_To_Player();
	void Recovery_HP();

private:
	_vector                          m_vPlayerPos = {};
	_bool                            m_bBossActive = {};
	_bool                            m_bPatternProgress = {};
	_bool                            m_bCan_Move_Anim = {};
	_bool                            m_bNeed_Rotation = {};
	_bool                            m_IsStun = false;
	_bool                            m_bWeapon_Collider_On_Off = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint                            m_iPhase = {};
	_uint                            m_iState = { CBoss_Varg::Varg_State_END };

	_float                           m_fTimeDelta = {};
	_float                           m_fDelayTime = {};
	_float                           m_fSpecial_Skill_CoolTime = {};
	_float                           m_fDistance = {};
	_float                           m_fAngle = {};
	_float                           m_fLookTime = {};
	_float                           m_fRotateDegree = {};

	_float                           m_fBossMaxHP = {};
	_float                           m_fShieldHP = {};
	_float                           m_fBossCurHP = {};
	_float                           m_fRecoveryTime = {};
	_bool                            m_bCanRecovery = {};

private:
	const _float4x4* m_pRootMatrix = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CState_Machine<CBoss_Varg>* m_pState_Manager = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
private:
	class CGameObject* m_pPlayer = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBoss_Varg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


	//상태 클래스 추가부분
	class Stun_State : public CStates<CBoss_Varg>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Intro_State : public CStates<CBoss_Varg>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Idle_State : public CStates<CBoss_Varg>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_A : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_A() = default;
		virtual ~Attack_Combo_A() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_B : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_B() = default;
		virtual ~Attack_Combo_B() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_C : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_C() = default;
		virtual ~Attack_Combo_C() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_D : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_D() = default;
		virtual ~Attack_Combo_D() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_E : public CStates<CBoss_Varg>
	{
	public:
		Attack_Combo_E() = default;
		virtual ~Attack_Combo_E() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Avoid_State : public CStates<CBoss_Varg>
	{
	public:
		Avoid_State() = default;
		virtual ~Avoid_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_bool m_bBonusAttack = false;
	};

	class Hit_State : public CStates<CBoss_Varg>
	{
	public:
		Hit_State() = default;
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Walk_State : public CStates<CBoss_Varg>
	{
	public:
		Walk_State() = default;
		virtual ~Walk_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_uint iRandomMove = {};
	};

	class Run_State : public CStates<CBoss_Varg>
	{
	public:
		Run_State() = default;
		virtual ~Run_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_float m_fTimer = {};
	};

	class Raid_Attack_01 : public CStates<CBoss_Varg>
	{
	public:
		Raid_Attack_01() = default;
		virtual ~Raid_Attack_01() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Raid_Attack_02 : public CStates<CBoss_Varg>
	{
	public:
		Raid_Attack_02() = default;
		virtual ~Raid_Attack_02() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class ExeCution_State : public CStates<CBoss_Varg>
	{
	public:
		ExeCution_State() = default;
		virtual ~ExeCution_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Roar_State : public CStates<CBoss_Varg>
	{
	public:
		Roar_State(_bool m_bCheck);
		virtual ~Roar_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	private:
		_bool   m_bFirst = {};
	};

	class Catch_State : public CStates<CBoss_Varg>
	{
	public:
		Catch_State() = default;
		virtual ~Catch_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Dead_State : public CStates<CBoss_Varg>
	{
	public:
		Dead_State() = default;
		virtual ~Dead_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

};

END