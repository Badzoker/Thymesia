#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBody_VillageF1 final : public CPartObject
{
public:
	struct BODY_VillageF1_DESC : public CPartObject::PARTOBJECT_DESC
	{
	};

private:
	CBody_VillageF1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_VillageF1(const CBody_VillageF1& Prototype);
	virtual ~CBody_VillageF1() = default;

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
public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBody_VillageF1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END