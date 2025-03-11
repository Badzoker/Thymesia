#pragma once
#include "Client_Defines.h"
#include "UI_Button.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_EmptyItemBox final : public CUI_Button
{
private:
	CUI_EmptyItemBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_EmptyItemBox(const CUI_EmptyItemBox& Prototype);
	virtual ~CUI_EmptyItemBox() = default;

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

private:
	_bool m_bImageOn = { false };
	_bool m_bMouseSelected = { false };


public:
	HRESULT Ready_Components();

public:
	static CUI_EmptyItemBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END