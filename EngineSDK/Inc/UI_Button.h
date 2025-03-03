#pragma once
#include "UIObject.h"

BEGIN(Engine)
class ENGINE_DLL CUI_Button abstract : public CUIObject
{
protected:
	CUI_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Button(const CUI_Button& Prototype);
	virtual ~CUI_Button() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	virtual _bool Mouse_Select(HWND hWnd);
	_bool On_Mouse_UI(HWND hWnd);
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

protected:
	//_bool m_bMouseClick = { false };
	_int iTestCount = { 0 };
};

END
