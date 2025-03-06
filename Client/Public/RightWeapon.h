#pragma once

#include "Client_Defines.h"
#include "PartObject.h"


BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)



class CRightWeapon final : public CPartObject
{
public:
	struct WEAPON_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
	};
private:
	CRightWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRightWeapon(const CRightWeapon& Prototype);
	virtual ~CRightWeapon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };
	float			   m_AccColliderLifeAttack1 = { 0.f };
	float			   m_AccColliderLifeAttack2 = { 0.f };


private:
	_float			   m_fTimeDelta = { 0.f };

private:
	const _uint* m_pParentState = { nullptr };

	PxRigidDynamic* m_pActor = { nullptr };
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static  CRightWeapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END