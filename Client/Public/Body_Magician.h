#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_Magician final : public CPartObject
{
public:
	struct BODY_MAGICIAN_DESC : public CPartObject::PARTOBJECT_DESC
	{
	};

private:
	CBody_Magician(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Magician(const CBody_Magician& Prototype);
	virtual ~CBody_Magician() = default;

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
private:
	_uint   m_iPassNum = {};

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBody_Magician* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END