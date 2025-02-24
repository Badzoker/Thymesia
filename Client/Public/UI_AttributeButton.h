#pragma once

#include "Client_Defines.h"
#include "UI_Button_Player.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_AttributeButton final : public CUI_Button_Player
{
public:
	struct BUTTON_DESC : CUIObject::UIOBJECT_DESC
	{

	};
private:
	CUI_AttributeButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_AttributeButton(const CUI_AttributeButton& Prototype);
	virtual ~CUI_AttributeButton() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	_tchar* m_pUIObjName = {};

public:
	HRESULT Ready_Components();

public:
	static CUI_AttributeButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END