#pragma once
#include "Client_Defines.h"
#include "UI_Text.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CUI_TextBox final : public CUI_Text
{
private:
	CUI_TextBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_TextBox(const CUI_TextBox& Prototype);
	virtual ~CUI_TextBox() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_FontName(_wstring pFontName) {pUseFontName = pFontName;	}
	void Set_ContentText(_wstring pText) { pUseText = pText;	}


private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	
	_uint m_iTextID = {};
	_wstring pUseFontName = {};
	_wstring pUseText = {};

	_float2 m_fTextPosition = {};
	_float4 m_fTextCloro = {1.0f,1.0f,1.0f,1.0f};
	_float m_fTextScale = {};
	_float4 m_fTextLayerDeth = {};
	_float4 m_fTextEffect = {};




	// 사용 폰트 이름, 텍스트 내용, 그려질 좌표(출력되는 이미지 중점기준), 컬러, 회전 , 모르겠다, 스케일, 레이어뎁스, 이펙트 


public:
	HRESULT Ready_Components();

public:
	static CUI_TextBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END