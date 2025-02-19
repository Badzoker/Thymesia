#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CLobTrap final : public CGameObject
{
private:
	CLobTrap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLobTrap(const CLobTrap& Prototype);
	virtual ~CLobTrap() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;
	virtual HRESULT Render_Distortion() override;
	virtual HRESULT Render_Glow() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

	_float m_fTimer = {};//이건 Distortion Test용 지워야할것임

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CLobTrap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END