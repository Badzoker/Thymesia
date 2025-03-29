#pragma once

#include "Projectile.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CProjectile_Card final : public CProjectile
{
private:
	CProjectile_Card(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProjectile_Card(const CProjectile_Card& Prototype);
	virtual ~CProjectile_Card() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	//virtual HRESULT Render() override;
	virtual HRESULT Render_Glow() override;
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CProjectile_Card* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END