#pragma once

#include "Client_Defines.h"
#include "PartObject.h"


BEGIN(Engine)
class CShader;
class CModel;
class CTexture;
END


BEGIN(Client)

class CWeapon_Cane final : public CPartObject
{
public:
	struct MAGICIAN_CANE_DESC : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_bool* IsDissolveOn = {nullptr};
		_bool* IsDissolveOff = {nullptr};
		_bool* bRender = { nullptr };
		_bool* bCane_Collider_On = { nullptr };
		_uint* iAttack = {nullptr};
	};
private:
	CWeapon_Cane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Cane(const CWeapon_Cane& Prototype);
	virtual ~CWeapon_Cane() = default;

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
	CTexture* m_pTextureCom = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };

private:
	_bool* m_IsDissolveOn = {nullptr};
	_bool* m_IsDissolveOff = {nullptr};
	_bool* m_bRender = { nullptr };
	_bool* m_bCane_Collider_On = { nullptr };
	_bool  m_bColliderOff = {};

	_uint m_iPassNum = {};

	_float			   m_fTimeDelta = { 0.f };
	_float m_fDissolveOn_Timer = {};
	_float m_fDissolveOff_Timer = {};
	_float m_fDissolveOn_FinishTime = 1.f;
	_float m_fDissolveOff_FinishTime = 1.f;
private:
	const _uint* m_pParentState = { nullptr };
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CWeapon_Cane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END