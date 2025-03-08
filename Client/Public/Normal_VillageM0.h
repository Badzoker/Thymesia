#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;

class CNavigation;

END

BEGIN(Client)

class CNormal_VillageM0 final : public CContainerObject
{
private:
	CNormal_VillageM0(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNormal_VillageM0(const CNormal_VillageM0& Prototype);
	virtual ~CNormal_VillageM0() = default;
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
	void RotateDegree_To_Player();
	void Rotation_To_Player();
	void Recovery_HP();
private:
	_vector                          m_vPlayerPos = {};

	_bool                            m_bActive = {};
	_bool                            m_bNeed_Rotation = {};
	_bool                            m_bCan_Move_Anim = {};
	_bool                            m_bPatternProgress = {};
	_bool                            m_IsStun = {};
	_bool                            m_bHP_Bar_Active = {};


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
	CState_Machine<CNormal_VillageM0>* m_pState_Manager = { nullptr };
	class CGameObject* m_pPlayer = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CNormal_VillageM0* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	//상태
	class Intro_State : public CStates<CNormal_VillageM0>
	{
	public:
		Intro_State() = default;
		virtual ~Intro_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM0* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM0* pObject) override;
		void State_Exit(CNormal_VillageM0* pObject) override;
	};

	class Idle_State : public CStates<CNormal_VillageM0>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM0* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM0* pObject) override;
		void State_Exit(CNormal_VillageM0* pObject) override;
	};

	class Move_State : public CStates<CNormal_VillageM0>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM0* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM0* pObject) override;
		void State_Exit(CNormal_VillageM0* pObject) override;
	private:
		_bool bCheck = {};
	};

	class Run_State : public CStates<CNormal_VillageM0>
	{
	public:
		Run_State() = default;
		virtual ~Run_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM0* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM0* pObject) override;
		void State_Exit(CNormal_VillageM0* pObject) override;
	private:
		_bool bCheck = {};
		_bool bTest = {};
		CNavigation* m_pPlayerNavi = { nullptr };
	};



	class Stun_State : public CStates<CNormal_VillageM0>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM0* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM0* pObject) override;
		void State_Exit(CNormal_VillageM0* pObject) override;
	private:
		_float m_fTime;
	};

	class Attack_01_State : public CStates<CNormal_VillageM0>
	{
	public:
		Attack_01_State() = default;
		virtual ~Attack_01_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM0* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM0* pObject) override;
		void State_Exit(CNormal_VillageM0* pObject) override;
	};

	class Attack_02_State : public CStates<CNormal_VillageM0>
	{
	public:
		Attack_02_State() = default;
		virtual ~Attack_02_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM0* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM0* pObject) override;
		void State_Exit(CNormal_VillageM0* pObject) override;
	};

	class Hit_State : public CStates<CNormal_VillageM0>
	{
	public:
		Hit_State() = default;
		virtual ~Hit_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM0* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM0* pObject) override;
		void State_Exit(CNormal_VillageM0* pObject) override;
	};

	class Dead_State : public CStates<CNormal_VillageM0>
	{
	public:
		Dead_State() = default;
		virtual ~Dead_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM0* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM0* pObject) override;
		void State_Exit(CNormal_VillageM0* pObject) override;
	};
};

END