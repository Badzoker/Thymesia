#include "pch.h"
#include "Object.h"



CObject::CObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CObject::CObject(const CObject& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CObject::Initialize(void* pArg)
{
	CObject::OBJECT_DESC* pDesc = static_cast<CObject::OBJECT_DESC*>(pArg);

	m_fFrustumRadius = pDesc->fFrustumRadius;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	strcpy_s(m_MeshName, pDesc->ObjectName.c_str());

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(XMConvertToRadians(pDesc->fRotation.x), XMConvertToRadians(pDesc->fRotation.y), XMConvertToRadians(pDesc->fRotation.z));

	return S_OK;
}

HRESULT CObject::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CObject::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
