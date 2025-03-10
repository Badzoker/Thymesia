#pragma once
#include "Base.h"
//#include "GameObject.h"
#include "UIObject.h"

BEGIN(Engine)
class CTrigger final : public CBase
{

private:
    CTrigger(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual ~CTrigger() = default;

public:
    HRESULT Initialize();
    void Execute();
    void Update(_float _fTimeDelta);
    TRIGGER_TYPE GetType() const { return m_eTriggerType; }
    _float GetAlpha() const { return m_fAlpha; }

    void Set_BlackScreen(CUIObject* _pGameObject) { m_pTarget = _pGameObject; }
    CGameObject* Get_TargetObject() const { return m_pTarget; }
    CUIObject* m_pUITarget = { nullptr };


    HRESULT Fade_Out(_float _fDuration);
    HRESULT Fade_In(_float _fDuration);

    void Set_Type(TRIGGER_TYPE _eType) { m_eTriggerType = _eType; }
    void Set_TargetObject(CGameObject* _pTarget) { m_pTarget = _pTarget; }
private:
    TRIGGER_TYPE m_eTriggerType = { TRIGGER_TYPE::TT_END };
    _float m_fAlpha = { 0.0f };
    _bool m_bFadeActivated = { false };
    CGameObject* m_pTarget = { nullptr };
    _float m_fDuration = {};
public:
    static CTrigger* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    virtual void Free() override;
};
END
