#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "Boss_State_Manager.h"


BEGIN(Engine)
class CModel;
class CCollider;
class CNavigation;

END

BEGIN(Client)

class CBoss_Varg final : public CContainerObject
{
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
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
public:
	void PatternCreate();
	void Near_Pattern_Create();
	void Far_Pattern_Create();
	void RotateDegree_To_Player();
	void Rotation_To_Player();

private:
	_vector                          m_vPlayerPos = {};
	_bool                            m_bBossActive = {};
	_bool                            m_bPatternProgress = {};
	_bool                            m_bCan_Move_Anim = {};
	_bool                            m_bNeed_Rotation = {};

	_uint                            m_iNearPatternIndex = -1;
	_uint                            m_iFarPatternIndex = -1;
	_uint                            m_iPhase = {};

	_float                           m_fDelayTime = {};
	_float                           m_fSpecial_Skill_CoolTime = {};
	_float                           m_fTimeDelta = {};
	_float                           m_fDistance = {};
	_float                           m_fAngle = {};
	_float                           m_fLookTime = {};
	_float                           m_fRotateDegree = {};
private:
	const _float4x4* m_pRootMatrix = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };
	CBoss_State_Manager<CBoss_Varg>* m_pState_Manager = { nullptr };
	class CGameObject* m_pPlayer = { nullptr };
public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CBoss_Varg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;


	//상태 클래스 추가부분
	class Stun_State : public CBoss_State<CBoss_Varg>
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

	class Intro_State : public CBoss_State<CBoss_Varg>
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

	class Idle_State : public CBoss_State<CBoss_Varg>
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

	class Attack_Combo_A : public CBoss_State<CBoss_Varg>
	{
	public:
		Attack_Combo_A() = default;
		virtual ~Attack_Combo_A() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_B : public CBoss_State<CBoss_Varg>
	{
	public:
		Attack_Combo_B() = default;
		virtual ~Attack_Combo_B() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_C : public CBoss_State<CBoss_Varg>
	{
	public:
		Attack_Combo_C() = default;
		virtual ~Attack_Combo_C() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_D : public CBoss_State<CBoss_Varg>
	{
	public:
		Attack_Combo_D() = default;
		virtual ~Attack_Combo_D() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Attack_Combo_E : public CBoss_State<CBoss_Varg>
	{
	public:
		Attack_Combo_E() = default;
		virtual ~Attack_Combo_E() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Avoid_State : public CBoss_State<CBoss_Varg>
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

	class Hit_State : public CBoss_State<CBoss_Varg>
	{
	public:
		Hit_State() = default;
		virtual ~Hit_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Walk_State : public CBoss_State<CBoss_Varg>
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

	class Run_State : public CBoss_State<CBoss_Varg>
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

	class Jump_Attack : public CBoss_State<CBoss_Varg>
	{
	public:
		Jump_Attack() = default;
		virtual ~Jump_Attack() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class ExeCution_State : public CBoss_State<CBoss_Varg>
	{
	public:
		ExeCution_State() = default;
		virtual ~ExeCution_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Roar_State : public CBoss_State<CBoss_Varg>
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

	class Catch_State : public CBoss_State<CBoss_Varg>
	{
	public:
		Catch_State() = default;
		virtual ~Catch_State() = default;
	public:
		void State_Enter(CBoss_Varg* pObject) override;
		void State_Update(_float fTimeDelta, CBoss_Varg* pObject) override;
		void State_Exit(CBoss_Varg* pObject) override;
	};

	class Dead_State : public CBoss_State<CBoss_Varg>
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