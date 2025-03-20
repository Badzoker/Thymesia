#include "Item.h"

CItem::CItem(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CGameObject{ _pDevice, _pContext }
{
}

CItem::CItem(const CItem& _Prototype)
    :CGameObject(_Prototype)
{
}

HRESULT CItem::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem::Initialize(void* _pArg)
{
    if (FAILED(__super::Initialize(_pArg)))
        return E_FAIL;

    return S_OK;
}

void CItem::Priority_Update(_float _fTimeDelta)
{
}

void CItem::Update(_float _fTimeDelta)
{
    if (m_bDropping)
    {
        Bezier(m_vInitialPos, m_vCurvePos, m_vEndPos, _fTimeDelta);
        m_fElapsedTime += _fTimeDelta * 0.5f;

        if (m_fElapsedTime >= 5.0f)
        {
            m_bDropping = false;
        }
    }
}

void CItem::Late_Update(_float fTimeDelta)
{
}

HRESULT CItem::Render()
{
    return S_OK;
}

void CItem::Set_Position(_fvector _vWorldPos)
{
    _float fPosX = XMVectorGetX(_vWorldPos);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vWorldPos);
}

void CItem::Set_BezierPosition(const _float4& _vStartPos, CGameObject* _pGameObject)
{
    m_vInitialPos = _vStartPos;
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&_vStartPos));

    m_fElapsedTime = 0.0f;

    _vector vTargetPos = _pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
    const _float fRadius = 2.0f;
    _float fRandomX = (rand() % 100 / 100.f - 0.5f) * 2.0f * fRadius;
    _float fRandomZ = (rand() % 100 / 100.f - 0.5f) * 2.0f * fRadius;
    vTargetPos = XMVectorSetX(vTargetPos, XMVectorGetX(vTargetPos) + fRandomX);
    vTargetPos = XMVectorSetZ(vTargetPos, XMVectorGetZ(vTargetPos) + fRandomZ);

    _vector vTargetDir = XMVector3Normalize(vTargetPos - XMLoadFloat4(&_vStartPos));

    m_pTransformCom->LookAt(XMLoadFloat4(&_vStartPos) + vTargetDir);

    _vector vMiddlePoint = (XMLoadFloat4(&_vStartPos) + vTargetPos) * 0.5f;
    vMiddlePoint = XMVectorSetY(vMiddlePoint, XMVectorGetY(vMiddlePoint) + 2.0f);

    vTargetPos = XMVectorSetY(vTargetPos, XMVectorGetY(vTargetPos) + 0.5f);

    XMStoreFloat4(&m_vEndPos, vTargetPos);
    XMStoreFloat4(&m_vCurvePos, vMiddlePoint);

    m_vLastPos = _vStartPos;
}

_float4 CItem::Bezier(_float4 _vStartPos, _float4 _vCurvePos, _float4 _vEndPos, _float _fTimeDelta)
{
    _float4 vResult;
    XMStoreFloat4(&vResult, ((1 - _fTimeDelta) * (1 - _fTimeDelta) * XMLoadFloat4(&_vStartPos) + 2 * (1 - _fTimeDelta) * _fTimeDelta * XMLoadFloat4(&_vCurvePos) + _fTimeDelta * _fTimeDelta * XMLoadFloat4(&_vEndPos)));

    return  vResult;
}

void CItem::Free()
{
    __super::Free();
}
