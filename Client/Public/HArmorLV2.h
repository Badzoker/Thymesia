#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CNavigation;

END

BEGIN(Client)

class CHArmorLV2 final : public CContainerObject
{
private:
	CHArmorLV2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHArmorLV2(const CHArmorLV2& Prototype);
	virtual ~CHArmorLV2() = default;
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
	_vector                          m_vSpawnPoint = {};

	_bool                            m_bActive = {};
	_bool                            m_bCan_Move_Anim = {};
	_bool                            m_bPatternProgress = {};
	_bool                            m_bNeed_Rotation = {};
	_bool                            m_IsStun = {};
	_bool                            m_bHP_Bar_Active = {};

	_uint                            m_iNearPatternIndex = {};

	_float                           m_fRotateDegree = {};
	_float                           m_fAngle = {};
	_float                           m_fDelayTime = {};
	_float                           m_fDistance = {};
	_float                           m_fTimeDelta = {};
	_float                           m_fHP_Bar_Active_Timer = {};

	_float                           m_fMonsterMaxHP = {};
	_float                           m_fMonsterCurHP = {};
	_float                           m_fShieldHP = {};
	_float                           m_fRecoveryTime = {};
	_bool                            m_bCanRecovery = {};
private:
	const _float4x4* m_pRootMatrix = { nullptr };
	CModel* m_pModelCom = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CState_Machine<CHArmorLV2>* m_pState_Manager = { nullptr };
	class CGameObject* m_pPlayer = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CHArmorLV2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	//���µ�
	class Intro_State : public CStates<CHArmorLV2>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Idle_State : public CStates<CHArmorLV2>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Walk_State : public CStates<CHArmorLV2>
	{
	public:
		Walk_State() = default;
		virtual ~Walk_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Run_State : public CStates<CHArmorLV2>
	{
	public:
		Run_State() = default;
		virtual ~Run_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	private:
		_bool bCheck = {};
		CNavigation* m_pPlayerNavi = { nullptr };
	};

	class Stun_State : public CStates<CHArmorLV2>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	private:
		_float m_fTime = {};
	};

	class Attack_Pattern_01 : public CStates<CHArmorLV2>
	{
	public:
		Attack_Pattern_01() = default;
		virtual ~Attack_Pattern_01() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Attack_Pattern_02 : public CStates<CHArmorLV2>
	{
	public:
		Attack_Pattern_02() = default;
		virtual ~Attack_Pattern_02() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Attack_Pattern_03 : public CStates<CHArmorLV2>
	{
	public:
		Attack_Pattern_03() = default;
		virtual ~Attack_Pattern_03() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Attack_Pattern_04 : public CStates<CHArmorLV2>
	{
	public:
		Attack_Pattern_04() = default;
		virtual ~Attack_Pattern_04() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Attack_Pattern_05 : public CStates<CHArmorLV2>
	{
	public:
		Attack_Pattern_05() = default;
		virtual ~Attack_Pattern_05() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Attack_Pattern_06 : public CStates<CHArmorLV2>
	{
	public:
		Attack_Pattern_06() = default;
		virtual ~Attack_Pattern_06() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Attack_Pattern_07 : public CStates<CHArmorLV2>
	{
	public:
		Attack_Pattern_07() = default;
		virtual ~Attack_Pattern_07() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Hit_State : public CStates<CHArmorLV2>
	{
	public:
		Hit_State() = default;
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

	class Execution_State : public CStates<CHArmorLV2>
	{
	public:
		Execution_State() = default;
		virtual ~Execution_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};

};

END