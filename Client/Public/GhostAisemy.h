#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"
#include "State_Machine.h"
#include "Body_GhostSemy.h"
#include "Lamp.h"

BEGIN(Engine)
class CModel;
class CNavigation;
END

BEGIN(Client)
class CGhostAisemy : public CContainerObject
{
public:
	enum GHOST_SEMY_STATE
	{
		STATE_GOSEMY_APPROACH = 0x00000001,
		STATE_GOSEMY_END = 0x00000002
	};

private:
	CGhostAisemy(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CGhostAisemy(const CGhostAisemy& _Prototype);
	virtual ~CGhostAisemy() = default;
public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* _pArg) override;
	virtual void					Priority_Update(_float _fTimeDelta) override;
	virtual void					Update(_float _fTimeDelta) override;
	virtual void					Late_Update(_float _fTimeDelta) override;
	virtual HRESULT					Render() override;

	void							Spawn_Gosemy(_float4 _vPos);


public:
	HRESULT							Ready_Components();
	HRESULT							Ready_PartObjects();
public:
	void							Culling();
public:
	void							Setting_Approach(_float _fTimeDelta);

	CLamp* Get_SemyLamp()const { return m_pLamp; }
	CBody_GhostSemy* Get_SemyBody() const { return m_pBody_GhoSemy; }


private:
	_float4                         m_vPlayerPos = {};

	_bool                           m_bActive = {};
	_bool                           m_bNeed_Rotation = {};
	_bool                           m_bNeedControl = {};
	_bool                           m_bCulling = {};

	_float                          m_fRotateDegree = {};
	_float                          m_fAngle = {};
	_float                          m_fDelayTime = {};

	_float                          m_fDistance = {};
	_float                          m_fTimeDelta = {};

private:
	_uint							m_iState = {};

private:
	_float							m_fIdleTime = {};
	_float							m_fApproachTime = {};

	CBody_GhostSemy* m_pBody_GhoSemy = { nullptr };
	CLamp* m_pLamp = { nullptr };
private:
	const _float4x4* m_pRootMatrix = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

public:
	static CGhostAisemy* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;

};
END
