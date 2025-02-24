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
	virtual void Set_Render_OnOff(_bool bOpen) { m_bRenderOpen = bOpen; } // UIObj ���� ���� ���� - ����
	virtual _uint Set_UI_ShaderPassNum(_uint iShadernum) { return m_iShaderPassNum = iShadernum; } // ���̴� �н� �ѹ� �������� -����

protected:
	HRESULT Add_UI_Object(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CGameObject** ppOut, void* pArg);

protected:
	_float4x4			m_ViewMatrix{}, m_ProjMatrix{};

	_bool				m_bRenderOpen = { false };// ���� ���� ����

	_float3				m_fPos = {};  // UI ������̾��µ� �ʿ� ��� ���߿� ���� ���� - ����
	_float2				m_fSize = {}; // UI ������̾��µ� �ʿ� ��� ���߿� ���� ���� - ����
	_uint				m_iShaderPassNum = { 0 };
	_uint				m_iUIType = { 0 };		// UI ���� Ÿ��

	_wstring			m_szProtoName = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END