#include "Shadow.h"
#include "Shader.h"
#include "GameObject.h"
#include "GameInstance.h"

CShadow::CShadow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice{ pDevice }
    , m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);

}

_float4 CShadow::Get_LightPos()
{
    _vector vLightPos = XMVector3TransformCoord(XMLoadFloat4(&m_LightPos), XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_LightViewMatrix)));
    vLightPos = XMVector3TransformCoord(vLightPos, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_LightProjMatrix)));

    _float4 fLightPos = {};


    XMStoreFloat4(&fLightPos, vLightPos);

    return fLightPos;
}

HRESULT CShadow::Initialize()
{
    return S_OK;
}

HRESULT CShadow::SetUp_ShadowLight(_fvector vEye, _fvector vAt, _float fLightAngle, _float fAspect, _float fNear, _float fFar, _matrix matInvCam, _fvector vCamInfo)
{
    XMStoreFloat4(&m_LightPos, vEye);

    XMStoreFloat4x4(&m_LightViewMatrix, XMMatrixLookAtLH(vEye, vAt, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
    XMStoreFloat4x4(&m_LightProjMatrix, XMMatrixPerspectiveFovLH(fLightAngle, fAspect, fNear, fFar));

    _matrix camInv = matInvCam;

    float fov = XMVectorGetX(vCamInfo);
    float ar = XMVectorGetY(vCamInfo);
    float nearZ = XMVectorGetZ(vCamInfo);
    float farZ = XMVectorGetW(vCamInfo);

    float tanHalfVFov = tanf(XMConvertToRadians(fov / 2.f));

    float tanHalfHFov = tanHalfVFov * ar;

    m_cascadeEnd[0] = nearZ;
    m_cascadeEnd[1] = 6.f;
    m_cascadeEnd[2] = 18.f;
    m_cascadeEnd[3] = farZ;

    for (size_t i = 0; i < 3; ++i)
    {
        float xn = m_cascadeEnd[i] * tanHalfHFov;
        float xf = m_cascadeEnd[i + 1] * tanHalfHFov;
        float yn = m_cascadeEnd[i] * tanHalfVFov;
        float yf = m_cascadeEnd[i + 1] * tanHalfVFov;

        _vector frustumCorners[8] =
        {
            //near Face
            {xn,yn,m_cascadeEnd[i],1.0f},
            {-xn,yn,m_cascadeEnd[i],1.0f},
            {xn,-yn,m_cascadeEnd[i],1.0f},
            {-xn,-yn,m_cascadeEnd[i],1.0f},
            //far Face
            {xf,yf,m_cascadeEnd[i + 1],1.0f},
            {-xf,yf,m_cascadeEnd[i + 1],1.0f},
            {xf,-yf,m_cascadeEnd[i + 1],1.0f},
            {-xf,-yf,m_cascadeEnd[i + 1],1.0f}
        };

        _vector centerPos = XMVectorSet(0.f, 0.f, 0.f, 0.f);

        for (uint32_t j = 0; j < 8; ++j)
        {
            frustumCorners[j] = XMVector3TransformCoord(frustumCorners[j], camInv);
            centerPos += frustumCorners[j];
        }

        centerPos /= 8.0f;

        float radius = 0.0f;
        for (uint32_t j = 0; j < 8; ++j)
        {
            float distance = XMVectorGetX(XMVector3Length(frustumCorners[j] - centerPos));
            radius = max(radius, distance);
        }

        radius = std::ceil(radius * 16.0f) / 16.0f;

        // using radius ,  we made aabb box
        _vector maxExtents = XMVectorSet(radius, radius, radius, 1.f);
        _vector minExtents = -maxExtents;

        _float3 shadowCamPos;
        XMStoreFloat3(&shadowCamPos, (centerPos + (XMVector3Normalize(vAt) * (XMVectorGetZ(minExtents)))));

        _vector vEyePos = XMVectorSetW(XMLoadFloat3(&shadowCamPos), 1.f);

        _matrix lightMatrix = XMMatrixLookAtLH(vEyePos, centerPos, XMVectorSet(0.f, 1.f, 0.f, 0.f));
        _vector cascadeExtents = maxExtents - minExtents;

        m_shadowOrthoProj[i] = XMMatrixOrthographicOffCenterLH(XMVectorGetX(minExtents), XMVectorGetX(maxExtents), XMVectorGetY(minExtents), XMVectorGetY(maxExtents), 0.0f, XMVectorGetZ(cascadeExtents)) * lightMatrix;
    }

    return S_OK;
}

void CShadow::Update()
{
}

HRESULT CShadow::Bind_ViewMatrix(CShader* pShader, const _char* pConstantName)
{
    if (FAILED(pShader->Bind_Matrix(pConstantName, &m_LightViewMatrix)))
        return E_FAIL;

    return S_OK;
}

HRESULT CShadow::Bind_ProjMatrix(CShader* pShader, const _char* pConstantName)
{
    if (FAILED(pShader->Bind_Matrix(pConstantName, &m_LightProjMatrix)))
        return E_FAIL;

    return S_OK;
}


CShadow* CShadow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CShadow* pInstance = new CShadow(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed To Created : CShadow");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CShadow::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
