#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CEffect abstract : public CGameObject
{
public:
	struct EFFECT_DESC : public CGameObject::GAMEOBJECT_DESC
	{
		//EFFECT_TYPE eType = { EFFECT_TYPE::EFFECT_TYPE_END };
		_uint iDiffuse = {};
		_float fMaxTimer = {};
	};
protected:
	CEffect(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CEffect(const CEffect& _Prototype);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* _pArg);
	virtual void Priority_Update(_float _fTimeDelta);
	virtual void Update(_float _fTimeDelta);
	virtual void Late_Update(_float _fTimeDelta);
	virtual HRESULT Render();

	virtual void Set_IsPlaying(_bool _bIsPlaying);

public:

	_bool Get_IsPlaying() { return m_bIsPlaying; }
	_bool Get_IsHave_SettinMatrix() {
		if (nullptr != m_pSettingMatrix)
			return true;
		return false;
	}
	void Set_MaxTimer(_float _fMaxTimer) { m_fMaxTimer = _fMaxTimer; }
	void Set_SettingMatrix(const _float4x4* _pSettingMatrix) { m_pSettingMatrix = _pSettingMatrix; }
	void Clear_SettingMatrix() { m_pSettingMatrix = nullptr; }


protected:
	//EFFECT_TYPE m_eEffectType = { EFFECT_TYPE::EFFECT_TYPE_END };
	const _float4x4* m_pSettingMatrix = { nullptr };
	_float4x4 m_matCombined = {};

	_bool m_bIsPlaying = { false };

	_float m_fMaxTimer = {};
	_float m_fDissolve = {}; //Dissolve Amount юс

	_float m_fTimerX = {};
	_float m_fTimerY = {};

	_float m_fTimer_SpeedX = { 1.f };
	_float m_fTimer_SpeedY = { 1.f };
	_float m_fDissolve_Speed = { 1.f };

protected:
	void Timer_Check(_float _fTimeDelta);

public:
	virtual CGameObject* Clone(void* _pArg) = 0;
	virtual void Free() override;
};

END