#pragma once

#include "PipeLine.h"
#include "Renderer.h"
#include "Prototype_Manager.h"

/* 0. 엔진에서 클라이언트 프로젝트에 직접 보여주는 클래스 : CGameInstance, 부모클래스(CBase, CLevel, CGameObject, CComponent..) */
/* 1. 클라이언트 프로젝트에서 엔진프로젝트에 있는 기능을 호출하고자한다라면 무조건 GameInstance를 통해서 호출할 수 있도록 하겠다.  */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	_uint2 Get_ViewportSize() const {
		return _uint2(m_iViewportWidth, m_iViewportHeight);
	}

public:
	/* 클라에서 호출할 만한 엔진의 기능을 위해 필요한 초기화를 수행한다. */
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Begin(const _float4 & vClearColor);
	HRESULT Draw();
	HRESULT Render_End();
	void Clear(_uint iLevelIndex);

	_float Compute_Random_Normal() 
	{
		return static_cast<_float>(rand()) / RAND_MAX;	
	}

	_float Compute_Random(_float fMin, _float fMax)
	{
		return fMin + Compute_Random_Normal() * (fMax - fMin);
	}

	HRESULT Clear_DepthStencil_View();	

#pragma region TIMER_MANAGER
public: /* For.Timer_Manager */
	_float			Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT			Add_Timer(const _wstring& strTimerTag);
	void			Compute_TimeDelta(const _wstring& strTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT Open_Level(_uint iNewLevelIndex, class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject_To_Layer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr, _char* pName = nullptr);	
	HRESULT Sub_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* _pGameObject);
	CGameObject* Get_Player_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag);	
	list<class CGameObject*>* Get_LayerGameObject(_uint iLevelIndex, const _wstring& strLayerTag);	
	CComponent* Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentName);
	map<const _wstring, class CLayer*>* Get_Layers();	
	_uint	Get_NumLevel();
	HRESULT UIScene_Render_OnOff(_uint iLevelIndex, const _wstring& strLayerTag, _bool bCheck);
	_char* Get_ColliderName(CGameObject* pGameObejct);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroupID, class CGameObject* pGameObject);
#pragma endregion

#pragma region PIPELINE
	_float4x4 Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4x4 Get_Transform_Float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_matrix Get_Transform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const;
	_float4 Get_CamPosition() const;
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);
#pragma endregion


#pragma region INPUT_DEVICE
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);	
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

	_bool   isKeyEnter(_int _iKey);
	_bool	isKeyReleased(_int _iKey);	
	_bool   isKeyPressed(_int _iKey);

	_bool   isMouseEnter(_uint _iKey); 
	void    Push_MouseState(MOUSEKEYSTATE eMouseKeyState);
	_bool   isMouseRelease();
#pragma endregion

#pragma region LIGHT_MANAGER
	const LIGHT_DESC* Get_LightDesc(_uint iIndex) const;	
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);	
	void Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);	
#pragma endregion

#pragma region TARGET_MANAGER
	HRESULT Add_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);	
	HRESULT Bind_RT_ShaderResource(const _wstring& strRenderTargetTag, CShader* pShader, const _char* pConstantName);	
	HRESULT Add_MRT(const _wstring& strMRTTag, const _wstring& strRenderTargetTag);	
	HRESULT Begin_MRT(const _wstring& strMRTTag, _bool isClear = true, ID3D11DepthStencilView* pDSV = nullptr);
	HRESULT End_MRT(ID3D11DepthStencilView* _pDSV = nullptr);
	HRESULT Add_Shadow_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor, _uint iArraySize);
#ifdef _DEBUG
public:
	HRESULT Ready_RT_Debug(const _wstring& strRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RT_Debug(const _wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	
#endif
#pragma endregion


#pragma region COLLIDER_MANAGER
	HRESULT Add_ObjCollider(GROUP_TYPE _GroupType, CGameObject* _pGameObject);
	HRESULT Sub_ObjCollider(GROUP_TYPE _GroupType, CGameObject* _pGameObject);	
	HRESULT CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
#pragma endregion 


#pragma region EVENT_MANAGER
	HRESULT  Event_Activate(_wstring _EventName);	
	HRESULT  Add_EventObject(_wstring _EventName, CGameObject* pGaemObject);		
	HRESULT  Sub_EventObject(_wstring _EventName, CGameObject* pGaemObject);	
	HRESULT  Add_DeadObject(_wstring _LayerName, CGameObject* pGaemObject);	
	HRESULT  Add_DeadEffect(CGameObject* pGameObject);	
#pragma endregion 


#pragma region EFFECT MANAGER

	HRESULT Add_Effect(CGameObject* pEffect);
	HRESULT Sub_Effect(CGameObject* pEffect);	

#pragma endregion


#pragma region SOUND MANAGER
	void Play_Sound(const _tchar* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(const _tchar* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);
	bool CheckSoundPlaying(CHANNELID eID);
#pragma endregion  



#pragma region Font_Manager
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	HRESULT Render_Font(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	HRESULT Render_Shadow(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);

	HRESULT Render_Outline(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_float4 vColor = { 255.f / 255.f, 255.f / 255.f, 255.f / 255.f, 1.0f }, _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f, float layerDepth = 0, SpriteEffects effects = SpriteEffects_None);
	_float2 Get_TextSize(const _wstring& strFontTag, const _tchar* pText);
#pragma endregion


#pragma region FRUSTUM
	_bool isIn_Frustum_WorldSpace(_fvector vWorldPoint, _float fRange = 0.f);
#pragma endregion


#pragma region ItemMgr 
	_uint   Count_Item(_wstring _ItemName);	
	HRESULT Add_Item(_wstring _ItemName);	
	HRESULT Sub_Item(_wstring _ItemName);	
#pragma endregion 

#pragma region SHADOW
	HRESULT SetUp_ShadowLight(_fvector vEye, _fvector vAt, _float fLightAngle, _float fAspect, _float fNear, _float fFar, _matrix matInvCam, _fvector vCamInfo, CTransform* pTransform);
	HRESULT Bind_Shadow_Matrices(class CShader* pShader, const _char* pViewConstantName, const _char* pProjConstantName);
	HRESULT Bind_LightZ(CShader* pShader);
	_float4	Get_LightPos();
#pragma endregion

#pragma region UI_Manager
	HRESULT Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag, _uint iUIType, void* pArg = nullptr);
	class CUI_Scene* Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag);
	map<const _wstring, class CUI_Scene*>* Find_UIScene_ALL();
	HRESULT UIScene_UIObject_Render_OnOff(CUI_Scene* pScene, _bool bOpen);
	void Clear_Choice(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag, class CUIObject* pUIObj); // 선택 obj 지우기
	void Clear_Last(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag);; // 마지막꺼 지우기
	void Clear_ALL(); // UI 모든 씬 지우기

	HRESULT LoadDataFile_UIObj_Info(HWND hWnd, _uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadDataFile_UIText_Info(HWND hWnd, const _tchar* szSceneName, vector<UI_TextInfo>& pOut);

#pragma endregion UI_Manager


#pragma region PhysX_Manager
	PxRigidDynamic* Add_Actor(COLLIDER_TYPE _eType, _float3 _Scale, _float3 _Axis,
		_float _degree, CGameObject* _pGameObject);

	HRESULT Sub_Actor(PxRigidDynamic* pActor);
	HRESULT Update_Collider(PxRigidDynamic* Actor, _matrix _WorldMatrix, _vector _vOffSet);
	HRESULT Set_GlobalPos(PxRigidDynamic* Actor, _vector _fPosition);
	HRESULT Set_CollisionGroup(PxRigidDynamic* pActor, GROUP_TYPE _eMeType, PxU32 _ColliderGroup);
	HRESULT Clear_Scene();
#pragma endregion 

private:
	_uint								m_iViewportWidth{}, m_iViewportHeight{};

private:
	class CTimer_Manager*				m_pTimer_Manager      = { nullptr };
	class CGraphic_Device*				m_pGraphic_Device     = { nullptr };
	class CInput_Device*				m_pInput_Device       = { nullptr };
	class CLevel_Manager*				m_pLevel_Manager      = { nullptr };	
	class CObject_Manager*				m_pObject_Manager     = { nullptr };
	class CPrototype_Manager*			m_pPrototype_Manager  = { nullptr };
	class CRenderer*					m_pRenderer           = { nullptr };
	class CPipeLine*					m_pPipeLine           = { nullptr };
	class CLight_Manager*				m_pLight_Manager      = { nullptr };
	class CTarget_Manager*				m_pTarget_Manager     = { nullptr };
	class CCollisionMgr*			    m_pCollider_Manager   = { nullptr };
	class CEventMgr*					m_pEvent_Manager      = { nullptr };
	class CEffectMgr*					m_pEffect_Manager     = { nullptr };
	class CSoundMgr*					m_pSound_Manager      = { nullptr };
	class CFont_Manager*				m_pFont_Manager       = { nullptr };
	class CFrustum*						m_pFrustum            = { nullptr };
	class CItemMgr*						m_pItemMgr            = { nullptr };
	class CShadow*						m_pShadow             = { nullptr };
	class CUI_Manager*					m_pUI_Manager		  = { nullptr };
	class CPhysX_Manager*				m_pPhysX_Manager	  = { nullptr };	

public:
	void Release_Engine();
	virtual void Free() override;
	
};

END