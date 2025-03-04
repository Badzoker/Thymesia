#pragma once

#include "Base.h"

BEGIN(Engine)

class CCustomFont final : public CBase
{
private:
	CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCustomFont() = default;

public:
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Render(const _tchar* pText, const _float2& vPosition, _float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);
	HRESULT Render_Shadow(const _tchar* pText, const _float2& vPosition, _float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);
	HRESULT Render_Outline(const _tchar* pText, const _float2& vPosition, _float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	_float2 Get_TextSize(const _tchar* pText);
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	SpriteBatch* m_pBatch = { nullptr };
	SpriteFont* m_pFont = { nullptr };


public:
	static CCustomFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath);
	virtual void Free() override;

};

END