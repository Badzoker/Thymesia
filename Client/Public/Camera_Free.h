#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CPlayer;

class CCamera_Free final : public CCamera
{
public:
	struct Camera_Event
	{
		_char Cam_Name[MAX_PATH];
		_float4x4 Cam_WorldMatrix;
		_float4 Cam_RelativePos;
		_float  Cam_LerpTime;
		_float  Cam_LerpSpeed;
		_float4x4  Cam_ConvertedWorldMatrix;
		_uint   Cam_LerpMethod;
	};

public:
	struct FREE_CAMERA_DESC : public CCamera::CAMERA_DESC
	{
		_float			fMouseSensor = {};
	};

private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& Prototype);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	CGameObject* Find_LockOnTarget();
	void				LockOnCameraTurn(_float fTimeDelta);
	void				ShakeOn(_float _fXaxisShakeSpeed, _float _fZaxisShakeSpeed, _float _fXaxisMoveAmount, _float _fZaxisMoveAmount);
	void				ZoomIn() { m_bZoomIn = true; }
	void				ZoomOut() { m_bZoomOut = true; }
	void				ResetZoomInCameraPos();
	bool				ResetZoomOutCameraPos();
	_vector			    Camera_Shake(float deltaTime, XMVECTOR& cameraPosition);

	bool                Camera_Cut_Scene_Activate(_wstring _CutSceneName);


private:
	_vector CatmullRom_Position_Lerp(vector<Camera_Event> CameraEvent, _float _fRatio);
	void   CutSceneEndLerp(_matrix pWorldMatrix);
	void   FirstLerpFinish();

private:
	_float* m_fPlayerPosition = { nullptr };
	_float				m_fMouseSensor = {};
	_float				m_fLerpTime = { 0.06f };
	_float				m_fCamCloseLimitDistance = { 2.5f };
	_float				m_fCamDistance = { 3.5f };
	_float				m_fCamFarLimitDistance = { 4.5f };
	_float				m_fCurCamDistance = { 3.5f };
	_float				m_fTimeDelta = {};


	/* 카메라 셰이킹 관련 변수 */
	_float				m_fXaxisShakeSpeed = {};
	_float				m_fZaxisShakeSpeed = {};
	_float			    m_fXaxisMoveAmount = {};
	_float			    m_fZaxisMoveAmount = {};
	/* ===================== */

	CPlayer* m_pPlayer = { nullptr };
	CTransform* m_pPlayerTransformCom = { false };
	CCollider* m_pPlayerColliderTransformCom = { false };
	CGameObject* m_pTargetMonster = { nullptr };


	_vector				m_vPlayerHeadPos = {};
	_vector			    m_vLerpPlayerHeadPos = {};


	_bool				m_bStop = { false };
	_bool				m_bCamLockOnOff = { false };
	_bool			    m_bFirst = { false };

	/* 카메라 이벤트 관련 */
	_bool				m_bShakeOnOff = { false };
	_bool				m_bZoomIn = { false };
	_bool				m_bZoomOut = { false };
	/*  ---------------  */

	/* 카메라 툴 이벤트 관련*/
	_bool               m_bCamera_Cut_Scene_OnOff = { false };
	_bool				m_bCutSceneFirst = { true };
	_bool				m_bCutSceneFristLerpEnd = { false };
	_bool				m_bGetBackCamPos = { false };

	_uint				m_iPlayCamera_Index = { 1 };

	_float				m_fCutScene_CurTime = {};
	_float				m_fRadian = {};
	_float				m_fTest = {};
	_float				m_fEndLerpTime = {};
	_float4x4			m_pAfterCutSceneWorldMatrix = {};
	_float4x4           m_pCurWorldMatrix = {};

	/* ----------------  */



	list<class CGameObject*>* m_plistMonster;
	map<_float, CGameObject*>							m_maptMonsterDistance;

	vector<Camera_Event>								m_vecCamera_Event;
	vector<Camera_Event>								m_vecPlayingCamera_Event;
	map<wstring, vector<Camera_Event>>					m_mapCamera_Event;


public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END