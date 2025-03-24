#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CNavigation;
class CTexture; 
END

class CStateMgr;

BEGIN(Client)


class CCamera_Free;

class CBody_Player final : public CPartObject
{

	enum RENDER_STATE
	{
		STATE_NORMAL_RENDER,  /*   평상시 공격 상태       */
		STATE_CLAW_RENDER,    /*   발톱 공격시의 상태     */
		STATE_DEAD_RENDER,    /*   죽음 상태 일시         */

	};

public:
	struct BODY_PLAYER_DESC : public CPartObject::PARTOBJECT_DESC
	{
		_uint* pParentState = { nullptr };
		_uint* pPreParentState = { nullptr };
		_uint* pParentPhaseState = { nullptr };
		_uint* pParentExectueMonsterState = { nullptr };

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
	HRESULT    STATE_DEAD_Render();	
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
	void	STATE_ATTACK_L4_Method();
	void	STATE_ATTACK_L5_Method();
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
	void   STATE_HURT_FALLDOWN_END_Method();
	void   STATE_WEAK_GETUP_F_Method();
	void   STATE_REBOUND_R_Method();
	void   STATE_STUNNED_START_Method();
	void   STATE_STUNNED_LOOP_Method();
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

	/* 처형 모션 */
	void STATE_HARMOR_EXECUTION_Method();
	void STATE_STUN_EXECUTE_Method();
	void STATE_LV1Villager_M_Execution_Method();
	void STATE_Joker_Execution_Method();
	void STATE_Varg_Execution_Method();

	/* ------------------------------- */

	/* 의자 관련 모션 */
	void STATE_ARCHIVE_SIT_START_Method();
	void STATE_ARCHIVE_SIT_LOOP_Method();
	void STATE_ARCHIVE_SIT_GETUP_Method();
	void STATE_ARCHIVE_SIT_LIGHT_UP_Method();

	/* 힐 관련 모션 */
	void STATE_HEAL_Method();

	/* 죽음 관련 모션 */
	void STATE_DEAD_Method();	

	/* 게임 시작 및 리스폰 시작 모션*/
	void STATE_START_WALK_Method();	

	/* 플레이어 우클릭 차지 공격 */
	void STATE_CLAW_CHARGE_START_Method();	
	void STATE_CLAW_CHARGE_LOOP_Method();	
	void STATE_CLAW_CHARGE_FULL_ATTACK_Method();	

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };	
	CCamera_Free* m_pCamera = { nullptr };
	CGameObject* m_pParent = { nullptr };

	_float							m_fAnimSpeed = 1.f;
	_float						    m_fTimeDelta = {};

	_float							m_fHitStopTime = {};
private:

	_uint* m_pParentMonsterExecute = { nullptr };
	_uint* m_pParentState = { nullptr };
	_uint* m_pPreParentState = { nullptr };
	_uint* m_pParentPhsaeState = { nullptr };
	_bool* m_pParentNextStateCan = { nullptr };
	_uint  m_iRenderState = {};

	CStateMgr* m_pParentStateMgr = { nullptr };
	CNavigation* m_pParentNavigationCom = { nullptr };
	PxRigidDynamic* m_pParentActor = { nullptr };


	_bool  m_bParryStopOnOff = { true };

	_float m_fDissolveAmount = {};	
	_float m_fDeadTimer      = {};		
	_float m_fFinishTime     = {};		
	_float m_fDeadStartTimer = {};		

	_uint m_iCurrentLevel = {}; //종한 추가 Level전환때문에

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();


public:
	static CBody_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END