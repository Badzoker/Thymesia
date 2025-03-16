#pragma once
#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CShader_Compute;
class CVIBuffer_Point_Compute;
class CTexture;
END

BEGIN(Client)

class CEffect_Particle final : public CEffect
{
public:
	enum SHADERPASS { SHADERPASS_DEFAULT, SHADERPASS_WEIGHTBLEND, SHADERPASS_GLOW, SHADERPASS_END };
public:
	struct EFFECT_PARTICLE_DESC : public CEffect::EFFECT_DESC
	{
		wstring szBufferName;
		wstring szShaderName;
		_uint iParticle_Count = {};
		_uint iShaderPass = {};
		_float3 vRGB = { 1.f, 1.f, 1.f };
		_float3 vScale = {};
		_float3 vRot = {};
		_float3 vTranslation = {};
		
	};

private:
	CEffect_Particle(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect_Particle(const CEffect_Particle& _Prototype);
	virtual ~CEffect_Particle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* _pArg) override;
	virtual void Priority_Update(_float _fTimeDelta) override;
	virtual void Update(_float _fTimeDelta) override;
	virtual void Late_Update(_float _fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_WeightBlend() override;

	virtual void Set_IsPlaying(_bool _bIsPlaying) override;

private:
	CShader_Compute* m_pShaderCom = { nullptr };
	CVIBuffer_Point_Compute* m_pBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_uint m_iParticle_Count = {};
	_uint m_iDiffuse = {}; //Texture »ö±ò ¹ºÁö

	SHADERPASS m_eShaderPass = {};

	_float3 m_vRGB = {};

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CEffect_Particle* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* _pArg) override;
	virtual void Free() override;
};

END