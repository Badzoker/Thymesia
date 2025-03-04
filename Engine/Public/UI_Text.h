#pragma once
#include "UIObject.h"

BEGIN(Engine)
class ENGINE_DLL CUI_Text abstract : public CUIObject
{
public:
	enum TextRender
	{
		FONT_DEFALUT, FONT_SHADOW, FONT_OUTLINE, FONT_END
};
protected:
	CUI_Text(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CUI_Text(const CUI_Text& Prototype);
	virtual ~CUI_Text() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	void Set_Change_TextColor(_float4 fColor) { m_fTextColor = { fColor.x / 255.f, fColor.y / 255.f,fColor.z / 255.f,fColor.w / 255.f }; }
	void Set_TextRenderType(TextRender eType) { m_eRenderType = eType; }

protected:
	_float4		m_fTextColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f };
	TextRender	m_eRenderType = { FONT_DEFALUT };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END