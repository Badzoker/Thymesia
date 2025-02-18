
#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Varg final : public CPartObject
{
	enum STATE
	{
	};

public:
	struct BODY_VARG_DESC : public CPartObject::PARTOBJECT_DESC
	{
		_uint* pParentState = { nullptr };
	};

private:
	CBody_Varg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Varg(const CBody_Varg& Prototype);
	virtual ~CBody_Varg() = default;

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

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBody_Varg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END