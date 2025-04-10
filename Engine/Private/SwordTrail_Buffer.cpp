#include "SwordTrail_Buffer.h"

CSwordTrail_Buffer::CSwordTrail_Buffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{

}

CSwordTrail_Buffer::CSwordTrail_Buffer(const CSwordTrail_Buffer& Prototype)
	: CVIBuffer(Prototype)	
{

}

HRESULT CSwordTrail_Buffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSwordTrail_Buffer::Initialize(void* pArg)
{
	m_iNumVertices = (64 * 4) + 4; // 네모가 128개가 필요한 vtx -> 시작점때문에 2개를 더해준다.
	m_iNumIndices = 64 * 12; // 세모는 2개 이므로 6개의 인덱스, 네모가 128개이다.
	m_iVertexStride = sizeof(VTXST);
	m_iIndexStride = 4;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iIndexStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	_uint* pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	for (_uint i = 0; i < 64; i++)
	{
		pIndices[(12 * i)] = 4 * i + 4;
		pIndices[(12 * i) + 1] = 4 * i;
		pIndices[(12 * i) + 2] = 4 * i + 1;

		pIndices[(12 * i) + 3] = 4 * i + 4;
		pIndices[(12 * i) + 4] = 4 * i + 1;
		pIndices[(12 * i) + 5] = 4 * i + 5;

		pIndices[(12 * i) + 6] = 4 * i + 2;
		pIndices[(12 * i) + 7] = 4 * i + 3;
		pIndices[(12 * i) + 8] = 4 * i + 7;

		pIndices[(12 * i) + 9] = 4 * i + 2;
		pIndices[(12 * i) + 10] = 4 * i + 7;
		pIndices[(12 * i) + 11] = 4 * i + 6;
	}



	ZeroMemory(&m_InitialData, sizeof(m_InitialData));
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion
#pragma region VERTEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	VTXST* pVertices = new VTXST[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXST) * m_iNumVertices);


	ZeroMemory(&m_InitialData, sizeof(m_InitialData));
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion

	return S_OK;
}

HRESULT CSwordTrail_Buffer::Set_Trail_Local(deque<_float3>& _dequeCenterPos, _uint _idequeCount, const _float3& _vRight, const _float3& _vUp)
{
	D3D11_MAPPED_SUBRESOURCE SubSource{};
	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubSource);

	VTXST* pVertices = static_cast<VTXST*>(SubSource.pData);

	_float3 vPos = {};
	_vector vDir{}, vStartPos{}, vEndPos{};
	_float fTexcoordX = {};

	for (_uint i = 0; i < 65; i++)
	{
		if (i < _idequeCount)
		{
			vDir = XMLoadFloat3(&_vRight);
			vStartPos = XMLoadFloat3(&_dequeCenterPos[i]) - (vDir);

			XMStoreFloat3(&vPos, vStartPos);
			pVertices[4 * i].vPosition = vPos; // 시작점

			vEndPos = XMLoadFloat3(&_dequeCenterPos[i]) + (vDir);

			XMStoreFloat3(&vPos, vEndPos);
			pVertices[(4 * i) + 1].vPosition = vPos; // 끝점

			vDir = XMLoadFloat3(&_vUp);
			vStartPos = XMLoadFloat3(&_dequeCenterPos[i]) - (vDir);

			XMStoreFloat3(&vPos, vStartPos);
			pVertices[4 * i + 2].vPosition = vPos; // 시작점

			vEndPos = XMLoadFloat3(&_dequeCenterPos[i]) + (vDir);

			XMStoreFloat3(&vPos, vEndPos);
			pVertices[(4 * i) + 3].vPosition = vPos; // 끝점

			fTexcoordX = 1.f - static_cast<_float>(i) / (_idequeCount - 1);

			pVertices[4 * i].vTexcoord = _float2(fTexcoordX, 1.f);
			pVertices[(4 * i) + 1].vTexcoord = _float2(fTexcoordX, 0.f);
			pVertices[(4 * i) + 2].vTexcoord = _float2(fTexcoordX, 1.f);
			pVertices[(4 * i) + 3].vTexcoord = _float2(fTexcoordX, 0.f);

		}
		else
		{
			pVertices[4 * i].vTexcoord = _float2(fTexcoordX, 1.f);
			pVertices[4 * i].vPosition = vPos;
			pVertices[(4 * i) + 1].vTexcoord = _float2(fTexcoordX, 0.f);
			pVertices[(4 * i) + 1].vPosition = vPos;
			pVertices[(4 * i) + 2].vTexcoord = _float2(fTexcoordX, 1.f);
			pVertices[(4 * i) + 2].vPosition = vPos;
			pVertices[(4 * i) + 3].vTexcoord = _float2(fTexcoordX, 0.f);
			pVertices[(4 * i) + 3].vPosition = vPos;
		}
	}

	m_pContext->Unmap(m_pVB, 0);

	return S_OK;
}

HRESULT CSwordTrail_Buffer::Set_Trail_Reset()
{
	D3D11_MAPPED_SUBRESOURCE SubSource{};
	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubSource);

	VTXST* pVertices = static_cast<VTXST*>(SubSource.pData);

	for (_uint i = 0; i < 65; i++)
	{
		pVertices[4 * i].vTexcoord = _float2(0.f, 0.f); // 시작점
		pVertices[(4 * i) + 1].vTexcoord = _float2(0.f, 0.f); // 끝점
		pVertices[(4 * i) + 2].vTexcoord = _float2(0.f, 0.f); // 끝점
		pVertices[(4 * i) + 3].vTexcoord = _float2(0.f, 0.f); // 끝점
	}

	m_pContext->Unmap(m_pVB, 0);

	return S_OK;
}

CSwordTrail_Buffer* CSwordTrail_Buffer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSwordTrail_Buffer* pInstance = new CSwordTrail_Buffer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CSwordTrail_Buffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent* CSwordTrail_Buffer::Clone(void* pArg)
{
	CSwordTrail_Buffer* pInstance = new CSwordTrail_Buffer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CSwordTrail_Buffer");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CSwordTrail_Buffer::Free()
{
	__super::Free();


}
