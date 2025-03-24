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

class CBoss_Magician final : public CMonster
{
public:
	enum PHASE { PHASE_ONE, PHASE_TWO, PHASE_END };
private:
	CBoss_Magician(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Magician(const CBoss_Magician& Prototype);
	virtual ~CBoss_Magician() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	void PatternCreate() override;
	void Active() override;
	//void Return_To_Spawn() override;
	void Stun() override;
public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_PartObjects(void* pArg);

public:
	void Near_Pattern_Create();
	void Far_Pattern_Create();

private:
	_bool                            m_bCardActive = {};
	_bool                            m_bExecution_Progress = {};
	_bool                            m_IsDissolveOn = {};
	_bool                            m_IsDissolveOff = {};
	_bool                            m_bCanDissapear = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint                            m_iPhase = { PHASE_END };

private:
	CState_Machine<CBoss_Magician>* m_pState_Manager = { nullptr };
	PxRigidDynamic* m_pStunActor = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBoss_Magician* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	class Intro_State : public CStates<CBoss_Magician>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Idle_State : public CStates<CBoss_Magician>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Dissappear_Idle_State : public CStates<CBoss_Magician>
	{
	public:
		Dissappear_Idle_State() = default;
		virtual ~Dissappear_Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Move_State : public CStates<CBoss_Magician>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_uint iRandomMove = {};
	};

	class Dissappear_Move_State : public CStates<CBoss_Magician>
	{
	public:
		Dissappear_Move_State(_uint iDissappear_Index, _bool bShootCard = false);
		virtual ~Dissappear_Move_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_bool m_bShootCard = {};
		_uint m_iDissappear_Index = {};
		_uint iRandomMove = {};
	};

	class Stun_State : public CStates<CBoss_Magician>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};


	class Shoot_ComboA : public CStates<CBoss_Magician>
	{
	public:
		Shoot_ComboA() = default;
		virtual ~Shoot_ComboA() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Shoot_ComboB : public CStates<CBoss_Magician>
	{
	public:
		Shoot_ComboB() = default;
		virtual ~Shoot_ComboB() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	private:
		_bool IsFired = {};
	};


	class Attack_ComboA : public CStates<CBoss_Magician>
	{
	public:
		Attack_ComboA() = default;
		virtual ~Attack_ComboA() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Attack_ComboB : public CStates<CBoss_Magician>
	{
	public:
		Attack_ComboB() = default;
		virtual ~Attack_ComboB() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Attack_ComboC : public CStates<CBoss_Magician>
	{
	public:
		Attack_ComboC() = default;
		virtual ~Attack_ComboC() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class Hit_State : public CStates<CBoss_Magician>
	{
	public:
		Hit_State() = default;
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

	class ExeCution_State : public CStates<CBoss_Magician>
	{
	public:
		ExeCution_State() = default;
		virtual ~ExeCution_State() = default;
	public:
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};

};

END