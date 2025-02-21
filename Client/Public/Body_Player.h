#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Player final : public CPartObject
{
	enum STATE
	{
		STATE_IDLE,
		STATE_RUN,
		STATE_WALK,
		STATE_ATTACK,
		STATE_ATTACK_L1,
		STATE_ATTACK_L2,
		STATE_ATTACK_L3,
	};

public:
	struct BODY_PLAYER_DESC : public CPartObject::PARTOBJECT_DESC
	{
		_uint* pParentState = { nullptr };
		_uint* pParentPhaseState = { nullptr };
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

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	_float							m_fAnimSpeed = 1.f;


private:
	_uint* m_pParentState = { nullptr };
	_uint* m_pParentPhsaeState = { nullptr };

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBody_Player* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END