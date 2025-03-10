#pragma once

#include "Transform.h"

BEGIN(Engine)

class CModel;

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	struct GAMEOBJECT_DESC : public CTransform::TRANSFORM_DESC
	{
		_char* szName = {};
		_float4x4 _ParentMatrix = {};
		_float4  _fPosition = {};
	};

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Shadow() { return S_OK; }
	virtual HRESULT Render_Blur() { return S_OK; }
	virtual HRESULT Render_HighLight() { return S_OK; }
	virtual HRESULT Render_Distortion() { return S_OK; }
	virtual HRESULT Render_Glow() { return S_OK; }
	virtual HRESULT Render_Motion_Blur() { return S_OK; }
	virtual HRESULT Render_Gaussian_Blur() { return S_OK; }
	virtual HRESULT Render_Occulusion() { return S_OK; }

	/* 11월 11일 추가*/
	map<const wstring, class CComponent*>* Get_Components() { return &m_Components; }

	const _char* Get_Name()
	{
		return m_szName;
	}

	void   Set_Name(_char* _szName)
	{
		strcpy_s(m_szName, _szName);
	}


public:
	class CComponent* Find_Component(const _wstring& strComponetTag);
	_uint Get_Player_Hitted_State() const { return m_iPlayer_Hitted_State; }
	CModel* Get_GameObject_Model() { return m_pGameObjectModel; }

protected:
	_char				        m_szName[MAX_PATH] = {};
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CModel* m_pGameObjectModel = { nullptr };
	class CGameInstance* m_pGameInstance = { nullptr };
	class CTransform* m_pTransformCom = { nullptr };	

	_uint                       m_iPlayer_Hitted_State = { Player_Hitted_State::PLAYER_HURT_END };	

protected:
	map<const _wstring, class CComponent*>		m_Components;	

protected:
	_bool  m_bTriggerOnOff = { false };
	_bool  m_bTriggerEvent2OnOff = { false };

protected:

	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);


public:
	class CTransform* Get_Transfrom() { return m_pTransformCom; }

public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information) {};
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information) {};
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information) {};


public:
	virtual void IsTriggerEnter() {};
	virtual void IsTriggerOn(_wstring _EventName) {};
	virtual void IsTriggerExit() {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END