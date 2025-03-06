#include "..\Public\GameInstance.h"


#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Light_Manager.h"
#include "Object_Manager.h"
#include "Input_Device.h"
#include "Graphic_Device.h"
#include "Prototype_Manager.h"
#include "Target_Manager.h"
#include "CollisionMgr.h"	
#include "EventMgr.h"
#include "EffectMgr.h"
#include "SoundMgr.h"	
#include "Font_Manager.h"
#include "Frustum.h"
#include "ItemMgr.h"
#include "Shadow.h"
#include "UI_Manager.h"
#include "GameObject.h"
#include "PhysX_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC & EngineDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	m_iViewportWidth = EngineDesc.iViewportWidth;
	m_iViewportHeight = EngineDesc.iViewportHeight;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iViewportWidth, EngineDesc.iViewportHeight, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	m_pLight_Manager = CLight_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);	
	if (nullptr == m_pTarget_Manager)	
		return E_FAIL;	

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;


	m_pEvent_Manager = CEventMgr::Create(*ppDevice, *ppContext);
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	m_pEffect_Manager = CEffectMgr::Create(*ppDevice, *ppContext);		
	if (nullptr == m_pEffect_Manager)	
		return E_FAIL;	

	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	m_pSound_Manager = CSoundMgr::Create();
	if (nullptr == m_pSound_Manager)
		return E_FAIL; 
	

	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	m_pItemMgr = CItemMgr::Create();	
	if (nullptr == m_pItemMgr)	
		return E_FAIL;	

	m_pShadow = CShadow::Create(*ppDevice, *ppContext);	
	if (nullptr == m_pShadow)	
		return E_FAIL;	

	m_pUI_Manager = CUI_Manager::Create(EngineDesc.iNumUIScenes);
	if (nullptr == m_pUI_Manager)
		return E_FAIL;

	m_pPhysX_Manager = CPhysX_Manager::Create(*ppDevice, *ppContext);	
	if (nullptr == m_pPhysX_Manager)	
		return E_FAIL;	


	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	if (nullptr == m_pLevel_Manager)
		return;
	/* 내 게임내에 필요한  다수 객체의 갱신작업을 모두 모아서 수행을 할거다. */

	m_pInput_Device->Update_InputDev();

	m_pLevel_Manager->Update(fTimeDelta);

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pEffect_Manager->Priority_Update(fTimeDelta);
	m_pUI_Manager->Priority_Update(fTimeDelta);

	m_pPipeLine->Update();

	m_pFrustum->Update();

	m_pObject_Manager->Update(fTimeDelta);
	m_pEffect_Manager->Update(fTimeDelta);
	m_pUI_Manager->Update(fTimeDelta);

	m_pObject_Manager->Late_Update(fTimeDelta);
	m_pEffect_Manager->Late_Update(fTimeDelta);	
	m_pUI_Manager->Late_Update(fTimeDelta);


	m_pPhysX_Manager->Update(fTimeDelta);	// PhysX 충돌 매니저 

	m_pEvent_Manager->Update();   //  객체의 삭제와 생성에 대한 매니저. 
	m_pShadow->Update();	      //  그림자를 비출 광원의 위치 업데이트를 위한 것.  ( 해당 기능은 거의 필요 없을거로 보임 ) 
}

HRESULT CGameInstance::Render_Begin(const _float4& vClearColor)
{
	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
 	m_pRenderer->Render();

	m_pPhysX_Manager->Render_PhysXDebugger();	

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::Render_End()
{
	m_pGraphic_Device->Present();

	return S_OK;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	/* 삭제된 레벨용 원형객체를 클리어한다. */
	m_pPrototype_Manager->Clear(iLevelIndex);

	/* 삭제된 레벨용 사봔객체를 들고있는 레이어들을 클리어한다. */
	m_pObject_Manager->Clear(iLevelIndex);
}

#pragma region TIMER_MANAGER
HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphic_Device->Clear_DepthStencil_View();	
}
_float CGameInstance::Get_TimeDelta(const _wstring & strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring & strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring & strTimerTag)
{
	m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Open_Level(_uint iNewLevelIndex, CLevel * pNewLevel)
{
	return m_pLevel_Manager->Open_Level(iNewLevelIndex, pNewLevel);	
}

#pragma endregion

#pragma region PROTOTYPE_MANAGER
HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring & strPrototypeTag, CBase * pPrototype)
{

	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}
CBase * CGameInstance::Clone_Prototype(PROTOTYPE eType, _uint iLevelIndex, const _wstring & strPrototypeTag, void * pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(eType, iLevelIndex, strPrototypeTag, pArg);
}

#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject_To_Layer(_uint iPrototypeLevelIndex, const _wstring & strPrototypeTag, _uint iLevelIndex, const _wstring & strLayerTag, void * pArg, _char* pName)
{
	return m_pObject_Manager->Add_GameObject_To_Layer(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg, pName);
}

CGameObject* CGameInstance::Add_GameObject_To_Layer_Take(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg, _char* pName)
{
	return m_pObject_Manager->Add_GameObject_To_Layer_Take(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg, pName);
}

HRESULT CGameInstance::Sub_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, CGameObject* _pGameObject)
{
	return m_pObject_Manager->Sub_GameObject_To_Layer(iLevelIndex, strLayerTag, _pGameObject);
}

CGameObject* CGameInstance::Get_Player_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Get_Player_GameObject_To_Layer(iLevelIndex, strLayerTag);
}

list<class CGameObject*>* CGameInstance::Get_LayerGameObject(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Get_LayerGameObject(iLevelIndex, strLayerTag);
}

CComponent* CGameInstance::Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentName)
{
	return m_pObject_Manager->Find_Component(iLevelIndex, strLayerTag, strComponentName);
}

map<const _wstring, class CLayer*>* CGameInstance::Get_Layers()
{
	return m_pObject_Manager->Get_Layers();
}

_uint CGameInstance::Get_NumLevel()
{
	return m_pObject_Manager->Get_NumLevel();	
}

HRESULT CGameInstance::UIScene_Render_OnOff(_uint iLevelIndex, const _wstring& strLayerTag, _bool bCheck)
{
	return m_pObject_Manager->UIScene_Render_OnOff(iLevelIndex, strLayerTag, bCheck);
}

_char* CGameInstance::Get_ColliderName(CGameObject* pGameObejct)
{
	return dynamic_cast<CCollider*>(pGameObejct->Get_Components()->find(TEXT("Com_Collider"))->second)->Get_CollierName();
}

#pragma endregion


#pragma region LEVEL_MANAGER
HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroupID, CGameObject * pGameObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroupID, pGameObject);
}

_float4x4 CGameInstance::Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

_float4x4 CGameInstance::Get_Transform_Float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4_Inverse(eState);
}

_matrix CGameInstance::Get_Transform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Matrix_Inverse(eState);
}

_float4 CGameInstance::Get_CamPosition() const
{
	return m_pPipeLine->Get_CamPosition();
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	return m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

#pragma endregion

#pragma region INPUT_DEVICE

_byte	CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}
_byte	CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}
_long	CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

_bool CGameInstance::isKeyEnter(_int _iKey)
{
	return m_pInput_Device->isKeyEnter(_iKey);
}

_bool CGameInstance::isKeyPressed(_int _iKey)
{
	return m_pInput_Device->isKeyPressed(_iKey);	
}

_bool CGameInstance::isKeyReleased(_int _iKey)
{
	return m_pInput_Device->isKeyReleased(_iKey);
}


_bool CGameInstance::isMouseEnter(_uint _iKey)
{
	return m_pInput_Device->isMouseEnter(_iKey);	
}	

void CGameInstance::Push_MouseState(MOUSEKEYSTATE eMouseKeyState)
{
	m_pInput_Device->Push_MouseState(eMouseKeyState);	
}
_bool CGameInstance::isMouseRelease()
{
	return m_pInput_Device->isMouseRelease();	
}
#pragma endregion

#pragma region LIGHT_MANAGER

const LIGHT_DESC * CGameInstance::Get_LightDesc(_uint iIndex) const
{
	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(const LIGHT_DESC & LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}
void CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLight_Manager->Render_Lights(pShader, pVIBuffer);
}
#pragma endregion


#pragma region TARGET_MANAGER
HRESULT CGameInstance::Add_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strRenderTargetTag,iWidth,iHeight,ePixelFormat,vClearColor);
}

HRESULT CGameInstance::Bind_RT_ShaderResource(const _wstring& strRenderTargetTag, CShader* pShader, const _char* pConstantName)
{
	return m_pTarget_Manager->Bind_RT_ShaderResource(strRenderTargetTag,pShader,pConstantName);	
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strRenderTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strRenderTargetTag);	
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag, _bool isClear, ID3D11DepthStencilView* pDSV)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag,isClear,pDSV);	
}

HRESULT CGameInstance::End_MRT(ID3D11DepthStencilView* _pDSV)
{
	return m_pTarget_Manager->End_MRT(_pDSV);
}

HRESULT CGameInstance::Add_Shadow_RenderTarget(const _wstring& strRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT ePixelFormat, const _float4& vClearColor, _uint iArraySize)
{
	return m_pTarget_Manager->Add_Shadow_RenderTarget(strRenderTargetTag, iWidth, iHeight, ePixelFormat , vClearColor, iArraySize);
}

HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_RT_Debug(strRenderTargetTag, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_RT_Debug(strMRTTag, pShader, pVIBuffer);	
}




#pragma region EventMgr

HRESULT CGameInstance::Event_Activate(_wstring _EventName)
{
	return m_pEvent_Manager->Event_Activate(_EventName);
}
HRESULT CGameInstance::Add_EventObject(_wstring _EventName, CGameObject* pGaemObject)
{
	return m_pEvent_Manager->Add_EventObject(_EventName,pGaemObject);	
}
HRESULT CGameInstance::Sub_EventObject(_wstring _EventName, CGameObject* pGaemObject)
{
	return m_pEvent_Manager->Sub_EventObject(_EventName, pGaemObject);		
}
HRESULT CGameInstance::Add_DeadObject(_wstring _LayerName, CGameObject* pGaemObject)	
{
	return m_pEvent_Manager->Add_DeadObject(_LayerName, pGaemObject);
}

HRESULT CGameInstance::Add_DeadEffect(CGameObject* pGameObject)
{
	return m_pEvent_Manager->Add_DeadEffect(pGameObject);	
}

#pragma endregion

#pragma region Effect Manager

HRESULT CGameInstance::Add_Effect(CGameObject* pEffect)
{
	return m_pEffect_Manager->Add_Effect(pEffect);	
}

HRESULT CGameInstance::Sub_Effect(CGameObject* pEffect)
{
	return m_pEffect_Manager->Sub_Effect(pEffect);	
}

#pragma endregion 

#pragma region Sound Manager 

void CGameInstance::Play_Sound(const _tchar* pSoundKey, CHANNELID eID, float fVolume)
{
	m_pSound_Manager->Play_Sound(pSoundKey, eID, fVolume);
}

void CGameInstance::PlayBGM(const _tchar* pSoundKey, float fVolume)
{
	m_pSound_Manager->PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::StopSound(CHANNELID eID)
{
	m_pSound_Manager->StopSound(eID);
}

void CGameInstance::StopAll()
{
	m_pSound_Manager->StopAll();
}

void CGameInstance::SetChannelVolume(CHANNELID eID, float fVolume)
{
	m_pSound_Manager->SetChannelVolume(eID, fVolume);
}
bool CGameInstance::CheckSoundPlaying(CHANNELID eID)
{
	return m_pSound_Manager->CheckSoundPlaying(eID);
}
#pragma endregion 
#pragma region FONT_MANAGER
HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}
HRESULT CGameInstance::Render_Font(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
	return m_pFont_Manager->Render(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale, layerDepth, effects);
}
HRESULT CGameInstance::Render_Shadow(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
	return m_pFont_Manager->Render_Shadow(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale, layerDepth, effects);
}
HRESULT CGameInstance::Render_Outline(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _float4 vColor, _float fRotation, const _float2& vOrigin, const _float fScale, float layerDepth, SpriteEffects effects)
{
	return m_pFont_Manager->Render_Outline(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale, layerDepth, effects);
}
_float2 CGameInstance::Get_TextSize(const _wstring& strFontTag, const _tchar* pText)
{
	return m_pFont_Manager->Get_TextSize(strFontTag, pText);
}
_bool CGameInstance::isIn_Frustum_WorldSpace(_fvector vWorldPoint, _float fRange)
{
	return m_pFrustum->isIn_WorldSpace(vWorldPoint, fRange);
}
#pragma endregion

#pragma region ITEM_MANAGER
_uint CGameInstance::Count_Item(_wstring _ItemName)
{
	return m_pItemMgr->Count_Item(_ItemName);	
}
HRESULT CGameInstance::Add_Item(_wstring _ItemName)
{
	return m_pItemMgr->Add_Item(_ItemName);	
}
HRESULT CGameInstance::Sub_Item(_wstring _ItemName)
{
	return m_pItemMgr->Sub_Item(_ItemName);	
}
#pragma endregion

#pragma region SHADOW
HRESULT CGameInstance::SetUp_ShadowLight(_fvector vEye, _fvector vAt, _float fLightAngle, _float fAspect, _float fNear, _float fFar, _matrix matInvCam, _fvector vCamInfo, CTransform* pTransform)
{
	return m_pShadow->SetUp_ShadowLight(vEye, vAt, fLightAngle, fAspect, fNear, fFar, matInvCam, vCamInfo, pTransform);

}
HRESULT CGameInstance::Bind_Shadow_Matrices(CShader* pShader, const _char* pViewConstantName, const _char* pProjConstantName)
{
	if (FAILED(m_pShadow->Bind_ViewMatrix(pShader, pViewConstantName)))
		return E_FAIL;
	if (FAILED(m_pShadow->Bind_ProjMatrix(pShader, pProjConstantName)))
		return E_FAIL;

	return S_OK;
}
HRESULT CGameInstance::Bind_LightZ(CShader* pShader)
{
	return m_pShadow->Bind_LightZ(pShader);
}
_float4 CGameInstance::Get_LightPos()
{
	return m_pShadow->Get_LightPos();
}
#pragma endregion

#pragma region UI_Manager

HRESULT CGameInstance::Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag, _uint iUIType, void* pArg)
{
	return m_pUI_Manager->Add_UIObject_To_UIScene(iPrototypeLevelIndex, strPrototypeTag, iSceneIndex, strSceneTag, iUIType, pArg);
}

CUI_Scene* CGameInstance::Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag)
{
	return m_pUI_Manager->Find_UIScene(iSceneIndex, strSceneTag);
}

map<const _wstring, class CUI_Scene*>* CGameInstance::Find_UIScene_ALL()
{
	return m_pUI_Manager->Find_UIScene_ALL();
}

HRESULT CGameInstance::UIScene_UIObject_Render_OnOff(CUI_Scene* pScene, _bool bOpen)
{
	return m_pUI_Manager->UIScene_UIObject_Render_OnOff(pScene, bOpen);
}

void CGameInstance::Clear_Choice(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag, CUIObject* pUIObj)
{
	return m_pUI_Manager->Clear_Choice(iUIType, iScenelIndex, strSceneTag, pUIObj);
}

void CGameInstance::Clear_Last(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag)
{
	return m_pUI_Manager->Clear_Last(iUIType, iScenelIndex, strSceneTag);
}

void CGameInstance::Clear_ALL()
{
	return m_pUI_Manager->Clear_ALL();
}

HRESULT CGameInstance::LoadDataFile_UIObj_Info(HWND hWnd, _uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName)
{
	return m_pUI_Manager->LoadDataFile_UIObj_Info(hWnd, iLevelIndex, iSceneIndex, szSceneName);
}

HRESULT CGameInstance::LoadDataFile_UIText_Info(HWND hWnd, const _tchar* szSceneName, vector<UI_TextInfo>& pOut)
{
	return m_pUI_Manager->LoadDataFile_UIText_Info(hWnd, szSceneName, pOut);
}
#pragma endregion UI_Manager

#pragma region PhysX_Manager
PxRigidDynamic* CGameInstance::Create_Actor(COLLIDER_TYPE _eType, _float3 _Scale, _float3 _Axis, _float _degree, CGameObject* _pGameObject)
{
	return m_pPhysX_Manager->Create_Actor(_eType, _Scale, _Axis, _degree, _pGameObject);
}

HRESULT CGameInstance::Add_Actor_Scene(PxRigidDynamic* pActor)
{
	return m_pPhysX_Manager->Add_Actor_Scene(pActor);	
}

HRESULT CGameInstance::Sub_Actor_Scene(PxRigidDynamic* pActor)
{
	return m_pPhysX_Manager->Sub_Actor_Scene(pActor);	
}



HRESULT CGameInstance::Update_Collider(PxRigidDynamic* Actor, _matrix _WorldMatrix, _vector _vOffSet)
{
	return m_pPhysX_Manager->Update_Collider(Actor, _WorldMatrix, _vOffSet);
}

HRESULT CGameInstance::Set_GlobalPos(PxRigidDynamic* Actor, _vector _fPosition)
{
	return m_pPhysX_Manager->Set_GlobalPos(Actor, _fPosition);
}

HRESULT CGameInstance::Set_CollisionGroup(PxRigidDynamic* pActor, GROUP_TYPE _eMeType, PxU32 _ColliderGroup)
{
	return m_pPhysX_Manager->Set_CollisionGroup(pActor, _eMeType, _ColliderGroup);
}

HRESULT CGameInstance::Clear_Scene()
{
	return m_pPhysX_Manager->Clear_Scene();
}
#pragma endregion PhysX_Manager



void CGameInstance::Release_Engine()
{
	Safe_Release(m_pGraphic_Device);	
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);	
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pEvent_Manager);	
	Safe_Release(m_pEffect_Manager);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pFrustum);
	Safe_Release(m_pItemMgr);	
	Safe_Release(m_pShadow);	
	Safe_Release(m_pUI_Manager);
	Safe_Release(m_pPhysX_Manager);	
	m_pSound_Manager->Release();
	m_pSound_Manager->Free();

	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	__super::Free();


}
