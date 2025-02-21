#include "UI_Button.h"

CUI_Button::CUI_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Button::CUI_Button(const CUI_Button& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Button::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Button::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_Button::Priority_Update(_float fTimeDelta)
{
}

void CUI_Button::Update(_float fTimeDelta)
{
}

void CUI_Button::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Button::Render()
{
	return S_OK;
}

_bool CUI_Button::Mouse_Select(HWND hWnd, _float fX, _float fY)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	if (ptMouse.x >= m_fPos.x - m_fSize.x /2 &&
		ptMouse.x <= m_fPos.x + m_fSize.x /2 &&
		ptMouse.y >= m_fPos.y - m_fSize.y /2 &&
		ptMouse.y <= m_fPos.y + m_fSize.y /2)
	{
		return true;
	}
	
	return false;
}

void CUI_Button::Free()
{
	__super::Free();
}
