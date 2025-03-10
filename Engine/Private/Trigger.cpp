#include "Trigger.h"

CTrigger::CTrigger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
}

HRESULT CTrigger::Initialize()
{
    return S_OK;
}

void CTrigger::Execute()
{
    if (m_bFadeActivated)
        return;
    m_bFadeActivated = true;

    switch (m_eTriggerType)
    {
    case TRIGGER_TYPE::TT_FADE_IN:
        m_fAlpha = 0.0f;
        break;
    case TRIGGER_TYPE::TT_FADE_OUT:
        m_fAlpha = 1.0f;
        break;
    case TRIGGER_TYPE::TT_INTERACTION:
        break;
    }
}

void CTrigger::Update(_float _fTimeDelta)
{
    if (!m_pTarget || !m_bFadeActivated)
        return;

    if (m_eTriggerType == TRIGGER_TYPE::TT_FADE_IN)
    {
        if (m_fAlpha > 0.0f)
        {
            m_fAlpha -= _fTimeDelta / m_fDuration;
            if (m_fAlpha < 0.0f)
            {
                m_fAlpha = 0.0f;
                m_bFadeActivated = false;
            }
        }
    }
    else if (m_eTriggerType == TRIGGER_TYPE::TT_FADE_OUT)
    {
        if (m_fAlpha < 1.0f)
        {
            m_fAlpha += _fTimeDelta / m_fDuration;
            if (m_fAlpha > 1.0f)
            {
                m_fAlpha = 1.0f;
                m_bFadeActivated = false;
            }
        }
    }

    if (m_pUITarget)
        m_pUITarget->Set_Alpha(m_fAlpha);

}

HRESULT CTrigger::Fade_Out(_float _fDuration)
{
    if (nullptr == m_pTarget)
        return E_FAIL;

    m_fAlpha = 0.0f;
    m_fDuration = _fDuration;
    m_bFadeActivated = true;

    m_pUITarget = dynamic_cast<CUIObject*>(m_pTarget);

    return S_OK;
}

HRESULT CTrigger::Fade_In(_float _fDuration)
{
    if (nullptr == m_pTarget)
        return E_FAIL;

    m_fAlpha = 1.0f;
    m_fDuration = _fDuration;
    m_bFadeActivated = true;

    m_pUITarget = dynamic_cast<CUIObject*>(m_pTarget);

    return S_OK;
}

CTrigger* CTrigger::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext/*, CGameObject* _pTarget*/)
{
    CTrigger* pInstance = new CTrigger(_pDevice, _pContext/*, _pTarget*/);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CTrigger");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTrigger::Free()
{
    __super::Free();
}
