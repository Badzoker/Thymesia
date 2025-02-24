#pragma once

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	struct UIOBJECT_DESC : public CGameObject::GAMEOBJECT_DESC	
	{
		_float			fX, fY, fZ;
		_float			fSizeX, fSizeY;
		_float			fNear, fFar;
		_wstring		szProtoName;
		_uint			iShaderPassNum;
		_float3			fRotation;
	};

protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;


public:
	virtual _bool Mouse_Select(HWND hWnd, _float fX, _float fY);

public:
	virtual void Set_Render_OnOff(_bool bOpen) { m_bRenderOpen = bOpen; } // UIObj 랜더 상태 제어 - 유빈
	virtual _uint Set_UI_ShaderPassNum(_uint iShadernum) { return m_iShaderPassNum = iShadernum; } // 쉐이더 패스 넘버 가져오기 -유빈

protected:
	HRESULT Add_UI_Object(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CGameObject** ppOut, void* pArg);

protected:
	_float4x4			m_ViewMatrix{}, m_ProjMatrix{};

	_bool				m_bRenderOpen = { false };// 랜더 상태 제어

	_float3				m_fPos = {};  // UI 저장용이었는데 필요 없어서 나중에 삭제 예정 - 유빈
	_float2				m_fSize = {}; // UI 저장용이었는데 필요 없어서 나중에 삭제 예정 - 유빈
	_uint				m_iShaderPassNum = { 0 };
	_uint				m_iUIType = { 0 };		// UI 생성 타입

	_wstring			m_szProtoName = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END