#include "pch.h"
#include "EnvironmentObject.h"

CEnvironmentObject::CEnvironmentObject(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CGameObject{ _pDevice, _pContext }
{
}

CEnvironmentObject::CEnvironmentObject(const CEnvironmentObject& _Prototype)
    :CGameObject(_Prototype)
{
}

HRESULT CEnvironmentObject::Initialize(void* _pArg)
{
    CEnvironmentObject::ENVIRONMENT_OBJECT_DESC* pDesc = static_cast<CEnvironmentObject::ENVIRONMENT_OBJECT_DESC*>(_pArg);

    m_fFrustumRadius = pDesc->fFrustumRadius;

    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    strcpy_s(m_EnvironmentMeshName, pDesc->ObjectName.c_str());

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Rotation(XMConvertToRadians(pDesc->fRotation.x), XMConvertToRadians(pDesc->fRotation.y), XMConvertToRadians(pDesc->fRotation.z));

    return S_OK;
}

HRESULT CEnvironmentObject::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxInstanceMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	return S_OK;
}

void CEnvironmentObject::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
