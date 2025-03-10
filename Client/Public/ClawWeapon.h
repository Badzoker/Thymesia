#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CCamera_Free;


class CClawWeapon final : public CPartObject
{
public:
	struct WEAPON_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
	};
private:
	CClawWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CClawWeapon(const CClawWeapon& Prototype);
	virtual ~CClawWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	CCamera_Free* m_pCamera = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	float			   m_AccColliderLifeAttack1 = { 0.f };
	float			   m_AccColliderLifeAttack2 = { 0.f };

	PxRigidDynamic* m_pActor = { nullptr };


private:
	_float			   m_fTimeDelta = { 0.f };
	_float			   m_fHitStopTime = { 0.f };

	_uint		     m_iPreParentState = {};

private:
	const _uint* m_pParentState = { nullptr };


public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static  CClawWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END