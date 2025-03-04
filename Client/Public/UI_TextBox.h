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

private:

	_uint m_iTextID = {};

	_float2 m_fTextPosition = {};
	_float m_fTextScale = {};
	_float4 m_fTextLayerDeth = {};
	_float4 m_fTextEffect = {};


	// 사용 폰트 이름, 텍스트 내용, 그려질 좌표(출력되는 이미지 중점기준), 컬러, 회전 , 모르겠다, 스케일, 레이어뎁스, 이펙트 

public:
	static CUI_TextBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END