#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader_Compute;
class CVIBuffer_Point_Compute;
class CTexture;
END

BEGIN(Client)

class CParticle_Compute_Example final : public CGameObject
{
private:
	CParticle_Compute_Example(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticle_Compute_Example(const CParticle_Compute_Example& Prototype);
	virtual ~CParticle_Compute_Example() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader_Compute* m_pShaderCom = { nullptr };
	CVIBuffer_Point_Compute* m_pBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

public:
	HRESULT Ready_Components();

public:
	static CParticle_Compute_Example* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END