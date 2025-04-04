#pragma once

#include "Client_Defines.h"
#include "Monster.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CShader;
class CNavigation;
END

BEGIN(Client)

class CBoss_Urd final : public CMonster
{
public:
	enum PHASE { PHASE_ONE, PHASE_TWO, PHASE_END };
private:
	CBoss_Urd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Urd(const CBoss_Urd& Prototype);
	virtual ~CBoss_Urd() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	virtual void State_Update(_float fTimeDelta) override;
	virtual void PatternCreate() override;
	virtual void Active() override;
	virtual void Stun() override;
public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);

public:
	void Near_Pattern_Create();
	void Far_Pattern_Create();

private:
	_bool						     m_bCan_Hit_Motion = {};
	_bool							 m_bChange_Socket = {};
	_bool                            m_bExecution_Progress = {};
	_bool                            m_bSpecial_Skill_Progress = {};

	_bool                            m_bCatch_Special_Attack = {};
	_bool							 m_bNeed_Decide_Step_Num = { true };

	_float                           m_fSpecial_Skill_CoolTime = {};
	_float                           m_fSlash_Skill_CoolTime = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint							 m_iHit_Motion_Index = -1;
	_uint							 m_iStep_Count = {};
	_uint                            m_iCheck_Step_Num = {};

	_uint                            m_iPhase = { PHASE_END };


private:
	CState_Machine<CBoss_Urd>* m_pState_Manager = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBoss_Urd* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	class Intro_State : public CStates<CBoss_Urd>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Idle_State : public CStates<CBoss_Urd>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Move_State : public CStates<CBoss_Urd>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	private:
		_uint iRandomMove = {};
	};

	class Step_Front_State : public CStates<CBoss_Urd>
	{
	public:
		Step_Front_State() = default;
		virtual ~Step_Front_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	private:
		_uint iRandomMove = {};
	};

	class Step_Back_State : public CStates<CBoss_Urd>
	{
	public:
		Step_Back_State() = default;
		virtual ~Step_Back_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Step_Right_State : public CStates<CBoss_Urd>
	{
	public:
		Step_Right_State() = default;
		virtual ~Step_Right_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Step_Left_State : public CStates<CBoss_Urd>
	{
	public:
		Step_Left_State() = default;
		virtual ~Step_Left_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};


	class Dissappear_Move_State : public CStates<CBoss_Urd>
	{
	public:
		Dissappear_Move_State(_uint iDissappear_Index, _bool bShootCard = false);
		virtual ~Dissappear_Move_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	private:
		_bool m_bShootCard = {};
		_uint m_iDissappear_Index = {};
		_uint iRandomMove = {};
	};

	class Stun_State : public CStates<CBoss_Urd>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class ExeCution_State : public CStates<CBoss_Urd>
	{
	public:
		ExeCution_State() = default;
		virtual ~ExeCution_State() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Attack_Combo_A : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_A() = default;
		virtual ~Attack_Combo_A() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Attack_Combo_B : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_B() = default;
		virtual ~Attack_Combo_B() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Attack_Combo_C : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_C() = default;
		virtual ~Attack_Combo_C() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

	class Attack_Combo_D : public CStates<CBoss_Urd>
	{
	public:
		Attack_Combo_D() = default;
		virtual ~Attack_Combo_D() = default;
	public:
		void State_Enter(CBoss_Urd* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Urd* pObject) override;
		void State_Exit(CBoss_Urd* pObject) override;
	};

};

END