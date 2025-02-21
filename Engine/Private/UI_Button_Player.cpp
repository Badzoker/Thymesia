#include "UI_Button_Player.h"

CUI_Button_Player::CUI_Button_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Button_Player::CUI_Button_Player(const CUI_Button_Player& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Button_Player::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Button_Player::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUI_Button_Player::Priority_Update(_float fTimeDelta)
{
}

void CUI_Button_Player::Update(_float fTimeDelta)
{
}

void CUI_Button_Player::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Button_Player::Render()
{
	return S_OK;
}

_bool CUI_Button_Player::Mouse_Select(HWND hWnd, _float fX, _float fY)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	if (ptMouse.x >= m_fPos.x - m_fSize.x / 2 &&
		ptMouse.x <= m_fPos.x + m_fSize.x / 2 &&
		ptMouse.y >= m_fPos.y - m_fSize.y / 2 &&
		ptMouse.y <= m_fPos.y + m_fSize.y / 2)
	{
		return true;
	}

	return false;
}

HRESULT CUI_Button_Player::On_Mouse_UI(HWND hWnd, _float fX, _float fY,_uint iTexNumber)
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);

	_float3 vMousePos = {};
	
	if (ptMouse.x >= m_fPos.x - m_fSize.x / 2 &&
		ptMouse.x <= m_fPos.x + m_fSize.x / 2 &&
		ptMouse.y >= m_fPos.y - m_fSize.y / 2 &&
		ptMouse.y <= m_fPos.y + m_fSize.y / 2)
	{
		Set_TexNumber(0);
	}
	else
	{
		Set_TexNumber(2);

	}

	return S_OK;
}
void CUI_Button_Player::Free()
{
	__super::Free();
}
