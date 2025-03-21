#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CProjectile abstract : public CGameObject
{
public:
	struct PROJECTILE_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		PROJECTILE_CATEGORY eProjectile_Type = { PROJECTILE_CATEGORY::PROJECTILE_END };
		_float fDamage = {};
		_float fDelete_Time = {};
	};

protected:
	CProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProjectile(const CProjectile& Prototype);
	virtual ~CProjectile() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	_bool Get_IsFire() { return m_bIsFire; }
	void Set_Info(_fvector vStartPos, _fvector vEndPos, _fvector vDir)
	{
		XMStoreFloat4(&m_vDir, vDir);
		XMStoreFloat4(&m_vEndPos, vEndPos);
		XMStoreFloat4(&m_vStartPos, vStartPos);
	}
	void Set_IsFire(_bool bCheck) { m_bIsFire = bCheck; }
	void Set_IsMultiFire(_bool bCheck) { m_bMultiFire = bCheck; }

	void Reset_Projectile();
public:
	void Set_Target(_fvector vStartPos, _fvector vEndPos);
	void Fire_OneShoot(_fvector vStartPos, _fvector vEndPos, _float _fTimeDelta);
	void Fire_MultiShoot(_fvector vStartPos, _fvector vEndPos, _float _fTimeDelta);

protected:
	_bool m_bIsFire = { false };
	_bool m_bMultiFire = { false };

	_uint  m_iCategory = { PROJECTILE_CATEGORY::PROJECTILE_END };

	_float m_fDamage = {};
	_float m_fSpeed = {};
	_float m_fDelete_Time = {};

	_float4 m_vStartPos = {};
	_float4 m_vEndPos = {};
	_float4 m_vDir = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END