#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CNavigation;
END

class CStateMgr;

BEGIN(Client)


class CCamera_Free;

class CBody_Player final : public CPartObject
{
	enum STATE
	{
		STATE_IDLE,
		STATE_RUN,
		/* 락온 상태일때의 모션 */
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
		/* ==================== */
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

	enum RENDER_STATE
	{
		STATE_NORMAL,  /* 평상시 공격 상태 */
		STATE_CLAW,   /*  발톱 공격시의 상태 */
	};

public:
	struct BODY_PLAYER_DESC : public CPartObject::PARTOBJECT_DESC
	{
		_uint* pParentState = { nullptr };
		_uint* pParentPhaseState = { nullptr };
		_bool* pParentNextStateCan = { nullptr };
		CStateMgr* pParentStateMgr = { nullptr };
		CNavigation* pParentNavigationCom = { nullptr };
		CGameObject* pParent = { nullptr };
		PxRigidDynamic* pParentActor = { nullptr };
	};

private:
	CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Player(const CBody_Player& Prototype);
	virtual ~CBody_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;


public:
	/* 각 모션에 따른 렌더링 관련 */
	HRESULT    STATE_NORMAL_Render();
	HRESULT    STATE_ATTACK_LONG_CLAW_Render();
	/* ============================ */

public:
	/* 각 모션에 따른 애니메이션 관련 */
	void	STATE_IDLE_Method();
	void	STATE_RUN_Method();
	void	STATE_WALK_Method();
	void	STATE_ATTACK_Method();
	void	STATE_ATTACK_L1_Method();
	void	STATE_ATTACK_L2_Method();
	void	STATE_ATTACK_L3_Method();
	void	STATE_ATTACK_LONG_CLAW_01_Method();
	void	STATE_ATTACK_LONG_CLAW_02_Method();
	/*락온 관련 기능 */
	void    STATE_LOCK_ON_RUN_B_Method();
	void    STATE_LOCK_ON_RUN_BL_Method();
	void    STATE_LOCK_ON_RUN_BR_Method();
	void    STATE_LOCK_ON_RUN_FL_Method();
	void    STATE_LOCK_ON_RUN_FR_Method();
	void    STATE_LOCK_ON_RUN_L_Method();
	void    STATE_LOCK_ON_RUN_R_Method();

	void    STATE_LOCK_ON_EVADE_F_Method();
	void    STATE_LOCK_ON_EVADE_B_Method();
	void    STATE_LOCK_ON_EVADE_L_Method();
	void    STATE_LOCK_ON_EVADE_R_Method();
	/* =========================================== */

	/* 패링 관련 기능 */
	void    STATE_PARRY_L_Method();
	void    STATE_PARRY_R_Method();


	void    STATE_PARRY_DEFLECT_LARGE_Method();
	void	STATE_PARRY_DEFLECT_L_UP_Method();
	void	STATE_PARRY_DEFLECT_L_Method();

	void 	STATE_PARRY_DEFLECT_R_UP_Method();
	void 	STATE_PARRY_DEFLECT_R_Method();
	/* ============================================ */



	/* 피격 관련 */
	void   STATE_HurtMFR_L_Method();
	void   STATE_HurtMFR_R_Method();
	void   STATE_HURT_LF_Method();
	void   STATE_HURT_SF_Method();
	void   STATE_HURT_SL_Method();
	void   STATE_HURT_HURXXLF_Method();
	void   STATE_HURT_KNOCKBACK_Method();
	void   STATE_HURT_KNOCKDOWN_Method();
	void   STATE_HURT_FALLDOWN_Method();
	void   STATE_WEAK_GETUP_F_Method();
	/*-----------------*/


	/* 회피 관련 */
	void STATE_NORMAL_EVADE_R_Method();
	void STATE_NORMAL_EVADE_L_Method();
	void STATE_NORMAL_EVADE_FR_Method();
	void STATE_NORMAL_EVADE_FL_Method();
	void STATE_NORMAL_EVADE_F_Method();
	void STATE_NORMAL_EVADE_BR_Method();
	void STATE_NORMAL_EVADE_BL_Method();
	void STATE_NORMAL_EVADE_B_Method();
	/* ------------------------- */


private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCamera_Free* m_pCamera = { nullptr };
	CGameObject* m_pParent = { nullptr };

	_float							m_fAnimSpeed = 1.f;
	_float						    m_fTimeDelta = {};

	_float							m_fHitStopTime = {};
private:

	_uint* m_pParentState = { nullptr };
	_uint* m_pParentPhsaeState = { nullptr };
	_bool* m_pParentNextStateCan = { nullptr };
	_uint  m_iRenderState = {};
	CStateMgr* m_pParentStateMgr = { nullptr };
	CNavigation* m_pParentNavigationCom = { nullptr };
	PxRigidDynamic* m_pParentActor = { nullptr };


public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CBody_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END