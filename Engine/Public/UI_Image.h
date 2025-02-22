#pragma once
#include "UIObject.h"
BEGIN(Engine)
class ENGINE_DLL CUI_Image abstract : public CUIObject
{
protected:
	CUI_Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Image(const CUI_Image& Prototype);
	virtual ~CUI_Image() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	virtual _bool Mouse_Select(HWND hWnd, _float fX, _float fY)override;
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END