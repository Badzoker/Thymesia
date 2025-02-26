#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CModel; /* 루트 모션 때매 추가 */
class CCollider;
class CNavigation;
END


class CStateMgr;

BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum STATE
	{
		STATE_IDLE,
		STATE_RUN,
		STATE_ROCK_ON_RUN_B,
		STATE_ROCK_ON_RUN_BL,
		STATE_ROCK_ON_RUN_BR,
		STATE_ROCK_ON_RUN_FL,
		STATE_ROCK_ON_RUN_FR,
		STATE_ROCK_ON_RUN_L,
		STATE_ROCK_ON_RUN_R,
		STATE_WALK,
		STATE_ATTACK,
		STATE_ATTACK_L1,
		STATE_ATTACK_L2,
		STATE_ATTACK_L3,
		STATE_ATTACK_LONG_CLAW_01,
		STATE_ATTACK_LONG_CLAW_02,
	};


	enum PLAYER_PHASE
	{
		PHASE_IDLE = 1,
		PHASE_FIGHT = 1 << 1,
		PHASE_ROCKON = 1 << 2,
	};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

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
	void Mouse_section(_float fTimeDelta);
	void Keyboard_section(_float fTimeDelta);

public:
	void Set_ParentPhaseState(_uint _PhaseState) { m_iPhaseState = _PhaseState; }
	void Set_Lockon(_bool _bLockOn) { m_bLockOn = _bLockOn; }

private:
	_uint								m_iState = { STATE_IDLE };
	_uint								m_iPreState = { STATE_IDLE };

	_uint								m_iPhaseState = { PHASE_IDLE };
	_uint								m_iPrePhaseState = { PHASE_IDLE };

	_bool								m_bLockOn = { false };


	CNavigation* m_pNavigationCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CModel* m_pModel = { nullptr };
	const _float4x4* m_pRootMatrix = { nullptr };
	const _float4x4* m_CombinedMatrix = { nullptr };



private:
	_float								m_fTimeDelta = { 0.f };
	CStateMgr* m_pStateMgr = { nullptr };


public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END