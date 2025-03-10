#pragma once
#include "Base.h"
#include "Trigger.h"

BEGIN(Engine)
class CTriggerManager final : public CBase
{
private:
    CTriggerManager(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual ~CTriggerManager() = default;

public:
    HRESULT Initialize();
    HRESULT Set_BlackScreen(CUIObject* _pBlackScreen);
    HRESULT Add_Trigger(TRIGGER_TYPE _eTriggerType, CGameObject* _pTarget = nullptr);
    HRESULT Activate_Fade(TRIGGER_TYPE _eTriggerType, _float _Duration);
    HRESULT Activate_Awake(TRIGGER_TYPE _eTirggerType, CGameObject* _pTarget, _bool _bAwake);
    void Update(_float _fTimeDelta);

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    CUIObject* m_pBlackScreen = { nullptr };

private:
    vector<CTrigger*>           m_vecTriggers;
    map<TRIGGER_TYPE, vector<CTrigger*>>        m_mapTriggers;
    TRIGGER_TYPE                m_eTriggerType = { TRIGGER_TYPE::TT_END };
public:
    static CTriggerManager* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual void Free() override;
};
END