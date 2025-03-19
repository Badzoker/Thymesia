#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "State_Machine.h"


BEGIN(Engine)
class CModel;
class CShader;
class CNavigation;
END

BEGIN(Client)

class CBoss_Magician final : public CContainerObject
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
	void Rotation_To_Player();
	void Recovery_HP();

private:
	_float4                          m_vPlayerPos = {};
	_float4                          m_vSpawnPoint = {};

	_bool                            m_bBossActive = {};
	_bool                            m_bCardActive = {};
	_bool                            m_bPatternProgress = {};
	_bool                            m_bCan_Move_Anim = {};
	_bool                            m_bNeed_Rotation = {};
	_bool                            m_IsStun = false;
	_bool                            m_bDead = {};
	_bool                            m_bCulling = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint                            m_iPhase = { PHASE_END };


	_float                           m_fTimeDelta = {};
	_float                           m_fDelayTime = {};
	_float                           m_fCoolTime = {};
	_float                           m_fSpecial_Skill_CoolTime = {};
	_float                           m_fDistance = {};
	_float                           m_fLookTime = {};
	_float                           m_fRotateDegree = {};
	_float                           m_fRotateSpeed = {};

	_float                           m_fBossMaxHP = {};
	_float                           m_fShieldHP = {};
	_float                           m_fBossCurHP = {};
	_float                           m_fRecoveryTime = {};
	_bool                            m_bCanRecovery = {};

private:
	const _float4x4* m_pRootMatrix = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CState_Machine<CBoss_Magician>* m_pState_Manager = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
private:
	class CGameObject* m_pPlayer = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CBoss_Magician* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


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

	class Shoot_State : public CStates<CBoss_Magician>
	{
	public:
		Shoot_State() = default;
		virtual ~Shoot_State() = default;
	public:
		// CBoss_State을(를) 통해 상속됨
		void State_Enter(CBoss_Magician* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Magician* pObject) override;
		void State_Exit(CBoss_Magician* pObject) override;
	};


};

END