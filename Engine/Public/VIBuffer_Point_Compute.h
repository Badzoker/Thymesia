#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Point_Compute final : public CVIBuffer
{
public:
	struct PARTICLE_COMPUTE_DESC
	{
		_uint		iNumInstance = {};
		_float3     vRange = {};
		_float3     vCenter = {};
		_float2     vSize = {};
		_float2     vSpeed = {};
		_float2     vLifeTime = {};
	};
private:
	CVIBuffer_Point_Compute(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Compute(const CVIBuffer_Point_Compute& Prototype);
	virtual ~CVIBuffer_Point_Compute() = default;

public:
	virtual HRESULT Initialize_Prototype(const PARTICLE_COMPUTE_DESC* _pDesc);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_InputAssembler() override;
	virtual HRESULT Render() override;
	HRESULT Compute_Shader(class CShader_Compute* _pComputeShader, _uint _iThreadCountX, _uint _iThreadCountY, _uint _iThreadCountZ);

private:

	D3D11_BUFFER_DESC				m_InstanceBufferDesc = {};
	D3D11_SUBRESOURCE_DATA			m_InstanceInitialData = {};
	_uint							m_iInstanceVertexStride = {};
	_uint							m_iNumIndexPerInstance = {};
	_uint							m_iNumInstance = {};

	COMPUTE_PARTICLE_INSTANCE* m_pInstanceVertices = { nullptr };
	ID3D11Buffer* m_pVBInstance = { nullptr };


	//_float* m_pSpeeds = { nullptr };
	_bool							m_isLoop = { false };

	ID3D11UnorderedAccessView* m_pUAV = { nullptr };
	ID3D11ShaderResourceView* m_pSRV = { nullptr };

	ID3D11Buffer* m_pBuffer_UAV = { nullptr };
	ID3D11Buffer* m_pBuffer_SRV= { nullptr };
	ID3D11Buffer* m_pBuffer_Copy= { nullptr };

	HRESULT CreateStructureBuffer();
	HRESULT CreateBuffer_SRV_UAV();
	HRESULT CreateAndCopyBuffer();

	_float m_fTest = {};

public:
	static CVIBuffer_Point_Compute* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const PARTICLE_COMPUTE_DESC* pArg);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END