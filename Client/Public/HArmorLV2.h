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
	void CalCulate_Distance();
	void Culling();

public:
	void PatternCreate();
	void Near_Pattern_Create();
	void Far_Pattern_Create();
	void RotateDegree_To_Player();
	_bool Is_Need_Turn_Attack();
	void Rotation_To_Player();
	void Recovery_HP();
private:
	_float4                          m_vPlayerPos = {};
	_float4                          m_vSpawnPoint = {};

	_bool                            m_bActive = {};
	_bool                            m_bExecution_Progress = {};
	_bool                            m_bFirst_Active = true;
	_bool                            m_bCan_Move_Anim = {};
	_bool                            m_bPatternProgress = {};
	_bool                            m_bNeed_Rotation = {};
	_bool                            m_IsStun = {};
	_bool                            m_bHP_Bar_Active = {};
	_bool							 m_bMove = true;
	_bool                            m_bDead = {};
	_bool                            m_bCulling = {};

	_uint                            m_iNearPatternIndex = {};
	_uint							 m_iSpawn_Cell_Index = {};
	_uint							 m_iHit_Motion_Index = {};
	_uint                            m_iHitCount = {};
	_uint* m_Player_State = {};
	const _uint* m_Player_Attack = {};


	_float                           m_fRotateDegree = {};
	_float                           m_fRotateSpeed = {};
	_float                           m_fDelayTime = {};
	_float                           m_fDistance = {};
	_float                           m_fSpawn_Distance = {};
	_float                           m_fTimeDelta = {};
	_float                           m_fHP_Bar_Active_Timer = {};
	_float                           m_fHP_Bar_Height = {};

	_float                           m_fMonsterMaxHP = {};
	_float                           m_fMonsterCurHP = {};
	_float                           m_fShieldHP = {};
	_float                           m_fRecoveryTime = {};
	_bool                            m_bCanRecovery = {};
private:
	const _float4x4* m_pRootMatrix = { nullptr };
	CModel* m_pModelCom = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
	PxRigidDynamic* m_pStunActor = { nullptr };
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

	//상태들
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

	class Move_State : public CStates<CHArmorLV2>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
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
	private:
		_float4 m_vOriginalLook = {};
		_float4 m_vChangeLook = {};
		_float m_fLinear = {};
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
		Hit_State(_uint pHitNum);
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	private:
		_uint m_iHitNum = {};
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

	class Return_To_SpawnPoint_State : public CStates<CHArmorLV2>
	{
	public:
		Return_To_SpawnPoint_State() = default;
		virtual ~Return_To_SpawnPoint_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	private:
		_bool bCheck = {};
	};

	class NotActive_Idle : public CStates<CHArmorLV2>
	{
	public:
		NotActive_Idle() = default;
		virtual ~NotActive_Idle() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;

	};

	class Parry_State : public CStates<CHArmorLV2>
	{
	public:
		Parry_State() = default;
		virtual ~Parry_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};
	class Parry_Attack_State : public CStates<CHArmorLV2>
	{
	public:
		Parry_Attack_State() = default;
		virtual ~Parry_Attack_State() = default;
	public:
		void State_Enter(CHArmorLV2* pObject) override;
		void State_Update(_float fTimeDelta, CHArmorLV2* pObject) override;
		void State_Exit(CHArmorLV2* pObject) override;
	};
};

END