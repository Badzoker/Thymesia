#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CCollider;
class CNavigation;

END

BEGIN(Client)

class CNormal_VillageM1 final : public CContainerObject
{


private:
	CNormal_VillageM1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNormal_VillageM1(const CNormal_VillageM1& Prototype);
	virtual ~CNormal_VillageM1() = default;
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
private:
	_vector                          m_vPlayerPos = {};

	_bool                            m_bActive = {};
	_bool                            m_bNeed_Rotation = {};
	_bool                            m_bPatternProgress = {};
	_bool                            m_bNeedControl = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;

	_float                           m_fRotateDegree = {};
	_float                           m_fAngle = {};
	_float                           m_fDelayTime = {};
	_float                           m_fDistance = {};
	_float                           m_fTimeDelta = {};

private:
	const _float4x4* m_pRootMatrix = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CState_Machine<CNormal_VillageM1>* m_pState_Manager = { nullptr };
	class CGameObject* m_pPlayer = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CNormal_VillageM1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	//상태
	class Idle_State : public CStates<CNormal_VillageM1>
	{
	public:
		Idle_State() = default;
		virtual ~Idle_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};

	class Move_State : public CStates<CNormal_VillageM1>
	{
	public:
		Move_State() = default;
		virtual ~Move_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	private:
		_bool bCheck = {};
	};

	class Run_State : public CStates<CNormal_VillageM1>
	{
	public:
		Run_State() = default;
		virtual ~Run_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	private:
		_bool bCheck = {};
	};

	class Stun_State : public CStates<CNormal_VillageM1>
	{
	public:
		Stun_State() = default;
		virtual ~Stun_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	private:
		_float m_fTime;
	};

	class Attack_01_State : public CStates<CNormal_VillageM1>
	{
	public:
		Attack_01_State() = default;
		virtual ~Attack_01_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};
	class Attack_02_State : public CStates<CNormal_VillageM1>
	{
	public:
		Attack_02_State() = default;
		virtual ~Attack_02_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};

	class Attack_03_State : public CStates<CNormal_VillageM1>
	{
	public:
		Attack_03_State() = default;
		virtual ~Attack_03_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};


	class Hit_State : public CStates<CNormal_VillageM1>
	{
	public:
		Hit_State() = default;
		virtual ~Hit_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};

	class Execution_State : public CStates<CNormal_VillageM1>
	{
	public:
		Execution_State() = default;
		virtual ~Execution_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CNormal_VillageM1* pObject) override;
		void State_Update(_float fTimeDelta, CNormal_VillageM1* pObject) override;
		void State_Exit(CNormal_VillageM1* pObject) override;
	};


};

END