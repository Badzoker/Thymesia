#include "UI_Text_PlayerInfo.h"

CUI_Text_PlayerInfo::CUI_Text_PlayerInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Text_PlayerInfo::CUI_Text_PlayerInfo(const CUI_Text_PlayerInfo& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Text_PlayerInfo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Text_PlayerInfo::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CUI_Text_PlayerInfo::Priority_Update(_float fTimeDelta)
{
}

void CUI_Text_PlayerInfo::Update(_float fTimeDelta)
{
}

void CUI_Text_PlayerInfo::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Text_PlayerInfo::Render()
{
	return S_OK;
}

_bool CUI_Text_PlayerInfo::Mouse_Select(HWND hWnd, _float fX, _float fY)
{
	return _bool();
}

void CUI_Text_PlayerInfo::Free()
{
	__super::Free();
}
