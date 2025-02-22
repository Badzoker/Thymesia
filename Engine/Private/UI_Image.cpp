#include "UI_Image.h"

CUI_Image::CUI_Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUI_Image::CUI_Image(const CUI_Image& Prototype)
	: CUIObject(Prototype)
{
}

HRESULT CUI_Image::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Image::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

void CUI_Image::Priority_Update(_float fTimeDelta)
{
}

void CUI_Image::Update(_float fTimeDelta)
{
}

void CUI_Image::Late_Update(_float fTimeDelta)
{
}

HRESULT CUI_Image::Render()
{
	return S_OK;
}

_bool CUI_Image::Mouse_Select(HWND hWnd, _float fX, _float fY)
{
	return _bool();
}

void CUI_Image::Free()
{
	__super::Free();
}
