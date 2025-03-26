#pragma once

#include "Object.h"
#include "Button.h"

BEGIN(Client)

class CStaticObject final : public CObject
{
public:
	typedef struct StaticObjectDesc : public CObject::OBJECT_DESC
	{

	}STATIC_OBJECT_DESC;

private:
	CStaticObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStaticObject(const CStaticObject& Prototype);
	virtual ~CStaticObject() = default;

public:
	virtual HRESULT				Initialize_Prototype() override;
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

	HRESULT						Render_Shadow() override;
	virtual HRESULT				Render_Motion_Blur() override;	

private:
	virtual HRESULT				Ready_Components() override;
	virtual HRESULT				Bind_ShaderResources() override;
	virtual HRESULT				Bind_Motion_Blur_ShaderResources();

private:
	PxRigidDynamic*				m_pActor = { nullptr };
	class CGameObject*			m_pInteractButton = { nullptr };

	CGameObject*				m_pButtonGameObject = { nullptr };
	CButton*					m_pButton = { nullptr };
	_bool						m_bInteractOn = { false };
	_float						m_fAlphaValue = {};
	_bool                       m_bFadingIn = { false };
	_bool                       m_bFadingOut = { false };

public:
	static CStaticObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END