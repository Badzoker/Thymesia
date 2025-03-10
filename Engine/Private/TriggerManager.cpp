#include "TriggerManager.h"

CTriggerManager::CTriggerManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :m_pDevice(_pDevice)
    , m_pContext(_pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CTriggerManager::Initialize()
{
    return S_OK;
}

HRESULT CTriggerManager::Set_BlackScreen(CUIObject* _pBlackScreen)
{
    if (nullptr == _pBlackScreen)
        return E_FAIL;

    m_pBlackScreen = _pBlackScreen;
}

HRESULT CTriggerManager::Add_Trigger(TRIGGER_TYPE _eTriggerType, CGameObject* _pTarget)
{
    if (nullptr == _pTarget)
    {
        _pTarget = m_pBlackScreen;
    }

    for (auto& pTrigger : m_mapTriggers[_eTriggerType])
    {
        if (pTrigger->Get_TargetObject() == _pTarget)
        {
            return S_OK;
        }
    }

    CTrigger* pTrigger = CTrigger::Create(m_pDevice, m_pContext);
    if (nullptr == pTrigger)
        return E_FAIL;

    pTrigger->Set_Type(_eTriggerType);
    pTrigger->Set_TargetObject(_pTarget);

    m_mapTriggers[_eTriggerType].push_back(pTrigger);

    return S_OK;
}

void CTriggerManager::Update(_float _fTimeDelta)
{
    for (auto iter = m_mapTriggers.begin(); iter != m_mapTriggers.end(); ++iter)
    {
        TRIGGER_TYPE eTriggerType = iter->first;
        vector<CTrigger*>& vecTriggers = iter->second;

        for (auto& pTrigger : vecTriggers)
        {
            pTrigger->Update(_fTimeDelta);
        }
    }
}

HRESULT CTriggerManager::Activate_Fade(TRIGGER_TYPE _eTriggerType, _float _fDuration)
{
    if (nullptr == m_pBlackScreen)
        return E_FAIL;

    auto& iter = m_mapTriggers.find(_eTriggerType);
    if (iter == m_mapTriggers.end())
    {
        return E_FAIL;
    }

    for (auto& pTrigger : iter->second)
    {
        if (pTrigger->Get_TargetObject() == m_pBlackScreen)
        {
            if (_eTriggerType == TRIGGER_TYPE::TT_FADE_OUT)
            {
                pTrigger->Fade_Out(_fDuration);
            }
            else if (_eTriggerType == TRIGGER_TYPE::TT_FADE_IN)
            {
                pTrigger->Fade_In(_fDuration);
            }
            return S_OK;
        }
    }

    return E_FAIL;
}

HRESULT CTriggerManager::Activate_Awake(TRIGGER_TYPE _eTirggerType, CGameObject* _pTarget, _bool _bAwake)
{
    for (auto& pTrigger : m_vecTriggers)
    {
        if (pTrigger->Get_TargetObject() == _pTarget)
        {
            if (_eTirggerType == TRIGGER_TYPE::TT_INTERACTION && pTrigger->GetType() == TRIGGER_TYPE::TT_INTERACTION)
            {
                //pTrigger->
            }
        }
    }
    return S_OK;
}

CTriggerManager* CTriggerManager::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
    CTriggerManager* pInstance = new CTriggerManager(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CTriggerManager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTriggerManager::Free()
{
    __super::Free();

    for (auto& Pair : m_mapTriggers)
    {
        for (auto& pTrigger : Pair.second)
            Safe_Release(pTrigger);

        Pair.second.clear();
    }

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
