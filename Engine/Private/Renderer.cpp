#include "..\Public\Renderer.h"

#include "GameObject.h"
#include "GameInstance.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderer::Initialize()
{
	_uint   iNumViewports = { 1 };
	D3D11_VIEWPORT  ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);	

	m_iOriginalViewportWidth  = static_cast<_uint>(ViewportDesc.Width);		
	m_iOriginalViewportHeight = static_cast<_uint>(ViewportDesc.Height);

	/* Target_Diffuse */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Normal */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Depth */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1000.f, 0.f, 0.f))))
		return E_FAIL;	

	/*Target Occulusion*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Occulusion"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Shade */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Specular */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/*Target LightShaft*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_LightShaftX"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/*Target LightShaft*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_LightShaftY"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Shadow */
	//if (FAILED(m_pGameInstance->Add_Shadow_RenderTarget(TEXT("Target_Shadow"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R32_FLOAT, _float4(1.f, 1000.f, 1.f, 0.f), 3)))
	//	return E_FAIL;

	//if (FAILED(Ready_Depth_Stencil_Buffer(m_iOriginalViewportWidth, m_iOriginalViewportHeight, &m_pShadowDSV)))
	//	return E_FAIL;	

	/* Target_Final */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Final"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;	

	/* Target_GlowBegin */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_GlowBegin"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_GlowX */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_GlowX"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_GlowY */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_GlowY"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Distortion */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Distortion"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_MotionBlur */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_MotionBlur"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_MotionBlur_By_Velocity */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_MotionBlur_By_Velocity"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_HighLight */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_HighLight"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;	

	/* Target_HighLightX */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_HighLightX"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;	

	/* Target_HighLightY */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_HighLightY"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_Shadow */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shadow_Final"), m_iOriginalViewportWidth, m_iOriginalViewportHeight, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* MRT_GameObjects */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))	
		return E_FAIL;	
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;

	//Occulusion Texture
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Occulusion"), TEXT("Target_Occulusion"))))
		return E_FAIL;

	//LightShaft Blur Texture
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightShaftX"), TEXT("Target_LightShaftX"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightShaftY"), TEXT("Target_LightShaftY"))))
		return E_FAIL;

	/* MRT_LightAcc */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))	
		return E_FAIL;	
	/* MRT_Shadow */
	//if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_Shadow"))))	
	//	return E_FAIL;
	/* MRT_Shadow_FInal*/
	//if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Shadow_Final"), TEXT("Target_Shadow_Final"))))
	//	return E_FAIL;
	/* MRT_Final */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Final"), TEXT("Target_Final"))))	
		return E_FAIL;	
	/* MRT_GlowBegin */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GlowBegin"), TEXT("Target_GlowBegin"))))
		return E_FAIL;
	/* MRT_GlowX */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GlowX"), TEXT("Target_GlowX"))))
		return E_FAIL;
	/* MRT_GlowY */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GlowY"), TEXT("Target_GlowY"))))
		return E_FAIL;
	/* MRT_Distortion */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		return E_FAIL;
	/* MRT_MotionBlur_By_Velocity */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_MotionBlur_By_Velocity"), TEXT("Target_MotionBlur_By_Velocity"))))
		return E_FAIL;
	/* MRT_MotionBlur */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_MotionBlur"), TEXT("Target_MotionBlur"))))
		return E_FAIL;
	/* MRT_HighLight */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_HighLight"), TEXT("Target_HighLight"))))
		return E_FAIL;
	/* MRT_HighLightX */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_HighLightX"), TEXT("Target_HighLightX"))))
		return E_FAIL;
	/* MRT_HighLightY */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_HighLightY"), TEXT("Target_HighLightY"))))
		return E_FAIL;

    m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL; 

	m_pShadowShader = CShader::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Shadow.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;


	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL; 
	
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));	

	_float fWidth{}, fHeight{};

	fWidth = ViewportDesc.Width;
	fHeight = ViewportDesc.Height;

	if (FAILED(m_pShader->Bind_RawValue("g_fViewPortWidth", &fWidth, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_fViewPortHeight", &fHeight, sizeof(_float))))
		return E_FAIL;

#ifdef _DEBUG
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Diffuse"), 300.f, 100.f, 200.f, 200.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Normal"), 300.f, 300.f, 200.f, 200.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Depth"), 300.f, 500.f, 200.f, 200.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shade"), 350.f, 150.f, 300.f, 300.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Specular"), 350.f, 450.f, 300.f, 300.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shadow"), ViewportDesc.Width - 150.f, 150.f, 300.f, 300.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Final"), 100.f, 100.f, 200.f, 200.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_HighLightX"), 100.f, 300.f, 200.f, 200.f)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_HighLightY"), 100.f, 500.f, 200.f, 200.f)))
	//	return E_FAIL;
	/*if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_MotionBlur_By_Velocity"), 100.f, 500.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_MotionBlur"), 100.f, 300.f, 200.f, 200.f)))
		return E_FAIL;*/
	/*if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Occulusion"), ViewportDesc.Width - 300.f, 150.f, 300.f, 300.f)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_LightShaftX"), ViewportDesc.Width - 300.f, 450.f, 300.f, 300.f)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_LightShaftY"), ViewportDesc.Width - 300.f, 750.f, 300.f, 300.f)))
		return E_FAIL;*/
#endif // _DEBUG


 	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroupID, CGameObject * pGameObject)
{
	if (nullptr == pGameObject || 
		eRenderGroupID >= RG_END )
		return E_FAIL;

	m_RenderObjects[eRenderGroupID].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Render()
{
 	if (FAILED(Render_Priority()))
		return E_FAIL;

	/*if (FAILED(Render_Shadow()))	
		return E_FAIL;	*/

	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_Occulusion()))
		return E_FAIL;

	if (FAILED(Render_LightAcc()))	
		return E_FAIL;	

	if (FAILED(Render_GlowBegin()))
		return E_FAIL;

	if (FAILED(Render_GlowX()))
		return E_FAIL;

	if (FAILED(Render_GlowY()))
		return E_FAIL;

	if (FAILED(Render_LightShaftX()))
		return E_FAIL;

	if (FAILED(Render_LightShaftY()))
		return E_FAIL;

	if (FAILED(Render_Distortion()))
		return E_FAIL;

	if (FAILED(Render_Deferred()))	
		return E_FAIL;	

	//if (FAILED(Render_Shadow_Final()))
	//	return E_FAIL;

	if (FAILED(Render_MotionBlurBegin()))
		return E_FAIL;

	if (FAILED(Render_MotionBlur_By_Velocity()))
		return E_FAIL;

	if (FAILED(Render_HighLightBegin()))
		return E_FAIL;

	if (FAILED(Render_HighLightX()))
		return E_FAIL;

	if (FAILED(Render_HighLightY()))
		return E_FAIL;

	if (FAILED(Render_Final()))	
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;	

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_Font()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Render_Debug()))	
		return E_FAIL;	
#endif

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Final"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_PRIORITY])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_PRIORITY].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Shadow()			
{
	/* Shadow */
  	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Shadow"), true, m_pShadowDSV)))	
		return E_FAIL;

	//if (FAILED(SetUp_ViewportDesc(g_iMaxWidth, g_iMaxHeight)))	
	//	return E_FAIL;	

	for (auto& pRenderObject : m_RenderObjects[RG_SHADOW])	
	{
		if (FAILED(pRenderObject->Render_Shadow()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_SHADOW].clear();

	if (FAILED(m_pGameInstance->End_MRT(m_pShadowDSV)))		
		return E_FAIL;	/*

	if (FAILED(SetUp_ViewportDesc(m_iOriginalViewportWidth, m_iOriginalViewportHeight)))			
		return E_FAIL;	*/


	return S_OK;	
}

HRESULT CRenderer::Render_NonBlend()
{
	/* Diffuse + Normal */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))		
		return E_FAIL;	

	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONBLEND].clear();

	if (FAILED(m_pGameInstance->End_MRT()))	
		return E_FAIL;	

	return S_OK;
}


HRESULT CRenderer::Render_Occulusion()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Occulusion"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_OCCULUSION])
	{
		if (FAILED(pRenderObject->Render_Occulusion()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}
	_float4 fLight4Pos = m_pGameInstance->Get_LightPos();

	_float2 fLight2Pos = { fLight4Pos.x, fLight4Pos.y };
	_float	fFogRange = 0.03f;
	_float4 fLightShaft = _float4(0.4f, 0.97f, 0.8f, 1.f);

	if (FAILED(m_pShader->Bind_RawValue("g_FogRange", &fFogRange, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_ScreenLightPos", &fLight2Pos, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_LightShaftValue", &fLightShaft, sizeof(_float4))))
		return E_FAIL;

	m_RenderObjects[RG_OCCULUSION].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}


HRESULT CRenderer::Render_Distortion()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Distortion"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_DISTORTION])
	{
		if (FAILED(pRenderObject->Render_Distortion()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_DISTORTION].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_GlowBegin()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GlowBegin"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_GLOW])
	{
		if (FAILED(pRenderObject->Render_Glow()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_GLOW].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_GlowX()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GlowX"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_GlowBegin"), m_pShader, "g_GlowBeginTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(7);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_GlowY()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GlowY"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_GlowX"), m_pShader, "g_GlowXTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(8);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}



HRESULT CRenderer::Render_LightShaftX()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightShaftX"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Occulusion"), m_pShader, "g_OccusionTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(11);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_LightShaftY()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightShaftY"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_LightShaftX"), m_pShader, "g_LightShaftXTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(12);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_MotionBlur_By_Velocity()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_MotionBlur_By_Velocity"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_MotionBlur"), m_pShader, "g_MotionBlurTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Final"), m_pShader, "g_FinalTexture")))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	m_pShader->Begin(9);

	m_pVIBuffer->Bind_InputAssembler();

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_MotionBlurBegin()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_MotionBlur"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_MOTION_BLUR])
	{
		if (FAILED(pRenderObject->Render_Motion_Blur()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_MOTION_BLUR].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pRenderObject : m_RenderObjects[RG_NONLIGHT])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONLIGHT].clear();
	
	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	/* Shade */

	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))	
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))	
		return E_FAIL;	
	
	m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);
	m_pShader->Bind_Matrix("g_ViewMatrix",  &m_ViewMatrix);
	m_pShader->Bind_Matrix("g_ProjMatrix",  &m_ProjMatrix); 
	
	m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ));

	m_pShader->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4));

 	m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Deferred() //원래 Final에 있었음
{                  
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Final"), false)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_LightShaftY"), m_pShader, "g_LightShaftYTexture")))
		return E_FAIL;


	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ));

	m_pShader->Begin(6);
	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Shadow_Final()
{
	//Target_Shadow_Final
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Shadow_Final"), false)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Final"), m_pShadowShader, "g_FinalTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShadowShader, "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Shadow"), m_pShadowShader, "g_CascadeShadowMapTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_Shadow_Matrices(m_pShadowShader, "g_lightviewmatrix", "g_lightprojmatrix")))
		return E_FAIL;

	m_pShadowShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	m_pShadowShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_PROJ));


	if (FAILED(m_pGameInstance->Bind_LightZ(m_pShadowShader)))
		return E_FAIL;

	if (FAILED(m_pShadowShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShadowShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShadowShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShadowShader->Begin(0);
	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_HighLightBegin()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_HighLight"))))
		return E_FAIL;


	for (auto& pRenderObject : m_RenderObjects[RG_HIGHLIGHT])
	{
		if (FAILED(pRenderObject->Render_HighLight()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_HIGHLIGHT].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_HighLightX()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_HighLightX"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_HighLight"), m_pShader, "g_HighLightTexture")))
		return E_FAIL;


	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShader->Begin(4);

	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_HighLightY()
{
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_HighLightY"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_HighLightX"), m_pShader, "g_HighLightXTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_HighLight"), m_pShader, "g_HighLightTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	m_pShader->Begin(5);

	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Final() //원래 Deferred 에 있었음
{
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Final"), m_pShader, "g_FinalTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_HighLightY"), m_pShader, "g_HighLightYTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_MotionBlur_By_Velocity"), m_pShader, "g_MotionBlurTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_GlowY"), m_pShader, "g_GlowYTexture")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Distortion"), m_pShader, "g_DistortionTexture")))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	m_pShader->Begin(3);
	m_pVIBuffer->Bind_InputAssembler();
	m_pVIBuffer->Render();

	return S_OK;	
}

HRESULT CRenderer::Render_Blend()
{
	for (auto& pRenderObject : m_RenderObjects[RG_BLEND])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pRenderObject : m_RenderObjects[RG_UI])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Font()
{
	for (auto& pRenderObject : m_RenderObjects[RG_FONT])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_FONT].clear();

	return S_OK;
}

HRESULT CRenderer::Ready_Depth_Stencil_Buffer(_uint iWidth, _uint iHeight, ID3D11DepthStencilView** ppOut)
{
	ID3D11Texture2D* pDepthStencilTexture = { nullptr };

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	/* 깊이 버퍼의 픽셀으 ㄴ백버퍼의 픽셀과 갯수가 동일해야만 깊이 텍스트가 가능해진다. */
	/* 픽셀의 수가 다르면 아에 렌덜잉을 못함. */
	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 3;
	TextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = { DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2DARRAY, 0 };
	DSVDesc.Texture2DArray.ArraySize = 3;
	DSVDesc.Texture2DArray.MipSlice = 0;
	DSVDesc.Texture2DArray.FirstArraySlice = 0;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, &DSVDesc, &m_pShadowDSV)))
		return E_FAIL;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2DARRAY, 0, 0 };
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = 3;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	//SRVDesc.Texture2DArray.FirstArraySlice = 0;

	if (FAILED(m_pDevice->CreateShaderResourceView(pDepthStencilTexture, &srvDesc, &m_pShadowSRV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

HRESULT CRenderer::SetUp_ViewportDesc(_uint iWidth, _uint iHeight)
{
	D3D11_VIEWPORT			ViewPortDesc;	
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));	
	ViewPortDesc.TopLeftX = 0;		
	ViewPortDesc.TopLeftY = 0;	
	ViewPortDesc.Width = (_float)iWidth;	
	ViewPortDesc.Height = (_float)iHeight;	
	ViewPortDesc.MinDepth = 0.f;	
	ViewPortDesc.MaxDepth = 1.f;	

	m_pContext->RSSetViewports(1, &ViewPortDesc);	

	return S_OK;
}

HRESULT CRenderer::Render_Debug()
{
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Shadow"), m_pShader, m_pVIBuffer)))	
	//	return E_FAIL;	
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Final"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_HighLightX"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_HighLightY"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_HighLightY"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_MotionBlur_By_Velocity"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_MotionBlur"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Occulusion"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_LightShaftX"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_LightShaftY"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;


	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderer::Free()
{
	__super::Free();

	for (auto& RenderObjects : m_RenderObjects)
	{
		for (auto& pRenderObject : RenderObjects)		
			Safe_Release(pRenderObject);

		RenderObjects.clear();
	}

	Safe_Release(m_pShadowDSV);	
	Safe_Release(m_pShadowSRV);
	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);


	Safe_Release(m_pGameInstance);	
	Safe_Release(m_pContext);	
	Safe_Release(m_pDevice);
	Safe_Release(m_pShadowShader);
}	

