#include "pch.h" 
#include "Loader.h"
#include "Body_Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "LobTrap.h"

#pragma region 보스 
#include "Boss_Magician.h"
#include "Boss_Varg.h"
#include "Body_Varg.h"
#include "VargKnife.h"

#include "UI_Boss_HP_Bar.h"
#include "UI_Boss_HP_Bar_Gage.h"
#include "UI_Boss_HP_Bar_Gage_Effect.h"

#pragma endregion 

#pragma region 엘리트 몬스터
#include "Elite_Joker.h"
#include "Body_Joker.h"
#include "Joker_Weapon.h"
#pragma endregion 

#pragma region 일반 몬스터
#include "Normal_VillageM0.h"
#include "Normal_VillageM1.h"
#include "Normal_VillageF0.h"
#include "Normal_VillageF1.h"
#include "Body_VillageM0.h"
#include "Body_VillageM1.h"
#include "Body_VillageF0.h"
#include "Body_VillageF1.h"
#include "VillageM_Weapon.h"
#include "VillageF_Weapon.h"

#include "Monster_HP_Bar.h"
#include "Monster_HP_Gage.h"
#include "Monster_HP_Gage_Effect.h"
#include "Monster_HP_Bar_Effect.h"
#pragma endregion 

#pragma region 플레이어 관련 
#include "Player.h"
#include "LeftWeapon.h"
#include "RightWeapon.h"
#pragma endregion 

#pragma region 환경요소 
#include "Sky.h"
#include "Terrain.h"
#pragma endregion 

#pragma region Effect

#include "Particle_Compute_Example.h"

#pragma endregion

#pragma region 테스트용 사다리
#include "Ladder.h"
#pragma endregion

#pragma region  UIs
#include "UI_TextBox.h"
#include "UI_MouseCursor.h"

#include "UIGroup_GameIntro.h"
#include "UIGroup_PlayerMenu.h"
#include "UIGroup_PlayerLevelUP.h"
#include "UIGroup_PlayerTalent.h"
#include "UIGroup_PlayerScreen.h"

#include "UI_NewHint.h"
#include "UI_LockedIcon.h"
#include "UI_LevelImage.h"
#include "UI_KeyBox_Long.h"
#include "UI_Indicator_Selected.h"
#include "UI_HightlightBar.h"
#include "UI_DiamondIcon.h"
#include "UI_RightBackground.h"
#include "UI_LeftBackground.h"
#include "UI_ButtonHighlight.h"
#include "UI_DecorationLine.h"

#include "UI_SceneAboveBackground.h"
#include "UI_SceneBackground.h"
#include "UI_SceneUnderBackground.h"
#include "UI_UnderLine.h"
#include "UI_Arrow.h"
#include "UI_Attribute_Slot_Active.h"
#include "UI_BaseTalentIndicator.h"
#include "UI_ContentBackground.h"
#include "UI_DescBackground.h"
#include "UI_Frame.h"

#include "UI_Arrow_Long.h"
#include "UI_BlackBar.h"
#include "UI_DecorationFrame.h"
#include "UI_ButtonBackground.h"
#include "UI_TitleBackgroundBar.h"

#include "UI_Feather_Icon.h"
#include "UI_HPBar1_Border.h"
#include "UI_HPBar2_BG.h"
#include "UI_HPBar3_MainBar.h"
#include "UI_MemoryShardsIcon.h"
#include "UI_MPBar1_Border.h"
#include "UI_MPBar2_BG.h"
#include "UI_MPBar3_MainBar.h"
#include "UI_PlunderSlotFrame.h"
#include "UI_Potion_DefaultType.h"

#include "UI_GameLogoImage.h"
#pragma endregion

#pragma region 오브젝트
#include "StaticObject.h"		// (아닐 비)인스턴싱용 게임오브젝트
#include "GroundObject.h"		// 인스턴싱용 게임오브젝트
#pragma endregion

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

_uint APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	pLoader->Start_Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{


	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	// m_eNextLevelID가 필요로하는 자원을 로드해주기위한 스레드를 만들어낸다. 
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_Level_GamePlay();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

void CLoader::SetUp_WindowText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading_For_Level_Logo()
{

#pragma region Logo 
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/LoadingScreen/LoadingScreen_%d.dds"), 8))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("셰이더 원형을 생성한다."));
	//D3D11_INPUT_ELEMENT_DESC	VertexElements[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA , 0 },			
	//};

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("모델 원형을 생성한다."));
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("객체 원형을 생성한다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

	/* 로딩이 완료되었습ㄴ미다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));
	m_isFinished = true;

	return S_OK;
#pragma endregion 
}

HRESULT CLoader::Loading_For_Level_GamePlay()
{
	/* 게임플레이레벨을 위한 자원을 로드합니다.  */
	lstrcpyW(m_szLoadingText, TEXT("게임플레이레벨을 위한 자원을 로딩 중입니다."));

	// 초기 설정을 위한 트랜스폼 
	_matrix PreTransformMatrix = XMMatrixIdentity();	

#pragma region 지형
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 100, 100, 1, nullptr , nullptr))))
		return E_FAIL;


	/* For.Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),	
		CTerrain::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	
#pragma endregion 



#pragma region Sky

	lstrcpyW(m_szLoadingText, TEXT("Sky 생성중"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Thymesia_SkyBox.dds"), 1))))
		return E_FAIL;

	
	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 




#pragma region 카메라 
	/* For.Prototype_GameObject_Camera_Free */

	lstrcpyW(m_szLoadingText, TEXT("카메라 생성중"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 셰이더

	lstrcpyW(m_szLoadingText, TEXT("셰이더 원형을 생성한다."));


	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxInstanceMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxInstanceMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstanceMesh.hlsl"), VTX_MODEL_INSTANCE::Elements, VTX_MODEL_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Shader_VtxMeshNoDefferd */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshNoDefferd"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMeshNoDefferd.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))	
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMeshNoDefferd */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMeshNoDefferd"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMeshNoDefferd.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))	
		return E_FAIL;	


	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTexInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTexInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTexInstance.hlsl"), VTX_RECT_INSTANCE::Elements, VTX_RECT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl"), VTX_POINT_INSTANCE::Elements, VTX_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance_Compute_Drop */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Drop"),
		CShader_Compute::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance_Compute.hlsl"), "CSMain_Particle_Drop", COMPUTE_POINT_INSTANCE::Elements, COMPUTE_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex_UI */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex_UI.hlsl"), VTXPOSTEX_UI::Elements, VTXPOSTEX_UI::iNumElements))))
		return E_FAIL;
#pragma endregion 

#pragma region Effect_Mesh

#pragma endregion

#pragma region Effect_Particle


	//아마 이렇게 Desc 들을 Save & Load 로 받아와야함
	CVIBuffer_Point_Compute::PARTICLE_COMPUTE_DESC particle_Drop_Desc = {};

	particle_Drop_Desc.iNumInstance = 64 * 2 * 2;
	particle_Drop_Desc.vCenter = _float3(0.f, 0.f, 0.f);
	particle_Drop_Desc.vRange = _float3(25.f, 25.f, 25.f);
	particle_Drop_Desc.vSpeed = _float2(0.5f, 1.f);
	particle_Drop_Desc.vLifeTime = _float2(1.f, 1.5f);
	particle_Drop_Desc.vSize = _float2(1.f, 1.f);

	/* For.Prototype_Component_VIBuffer_Point_Compute */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Compute"),
		CVIBuffer_Point_Compute::Create(m_pDevice, m_pContext, &particle_Drop_Desc))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Particle_Example*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Particle_Example"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Example/T_Y_Ring_02.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Compute_Example */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Compute_Example"),
		CParticle_Compute_Example::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion


#pragma region Navigation 
	lstrcpyW(m_szLoadingText, TEXT("네비게이션 원형을 생성한다."));	
	/* For.Prototype_Component_Navigation */	
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),	
	//	CNavigation::Create(m_pDevice, m_pContext, TEXT("../Navigation_File/test49.bin")))))	
	//	return E_FAIL;	




	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"), 
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Navigation_File/TestNavigation7.txt")))))
		return E_FAIL;



#pragma endregion 

#pragma region Collider
	lstrcpyW(m_szLoadingText, TEXT("Collider 생성중"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext,CCollider::TYPE_AABB))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))	
		return E_FAIL;

#pragma endregion


#pragma region UI 텍스쳐
	lstrcpyW(m_szLoadingText, TEXT("UI 생성한다."));
	//====================================================================================================================================== 게임 인트로
	/* For.Prototype_Component_Texture_UI_GameLogoImage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_GameLogoImage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_GameLogoImage.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_GameLogoImage */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_GameLogoImage"),
		CUI_GameLogoImage::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== 키보드 텍스처
	/* For.Prototype_Component_Texture_UI_KeyBox_Long*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_KeyBox_Long"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_KeyBox_Long.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_KeyBox_Long"),
		CUI_KeyBox_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== 텍스트 박스

	/* For.Prototype_GameObject_UI_TextBox */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_TextBox"),
		CUI_TextBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//====================================================================================================================================== 공용 꾸밈 텍스처
	/* For.Prototype_Component_Texture_UI_ButtonHighlight*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ButtonHighlight"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_ButtonHighlight_0%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ButtonHighlight"),
		CUI_ButtonHighlight::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== 공용 꾸밈 텍스처
	/* For.Prototype_Component_Texture_UI_DecorationLine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DecorationLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_DecorationLine_%d.dds"), 2))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_KeyBox_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DecorationLine"),
		CUI_DecorationLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//====================================================================================================================================== 넛지 텍스처
	/* For.Prototype_Component_Texture_UI_DiamondIcon_01*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DiamondIcon_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_DiamondIcon_01.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DiamondIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DiamondIcon"),
		CUI_DiamondIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Indicator_Selected*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Indicator_Selected"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Indicator_Selected_%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Indicator_Selected */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Indicator_Selected"),
		CUI_Indicator_Selected::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LockedIcon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LockedIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_LockedIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LockedIcon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LockedIcon"),
		CUI_LockedIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_NewHint*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_NewHint"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_NewHint.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_NewHint */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_NewHint"),
		CUI_NewHint::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//==================================================================================================================================== 플레이어 메뉴창(의자착석)
		/* For.Prototype_Component_Texture_UI_HighlightBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HighlightBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_HighlightBar_0%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HighlightBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HighlightBar"),
		CUI_HightlightBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LeftBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LeftBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_LeftBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LeftBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LeftBackground"),
		CUI_LeftBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_RightBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_RightBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_RightBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_RightBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_RightBackground"),
		CUI_RightBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_LevelImage*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_LevelImage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_LevelImage_Circus.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelImage */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_LevelImage"),
		CUI_LevelImage::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//==================================================================================================================================== 플레이어 특성 창

	/* For.Prototype_Component_Texture_UI_Arrow*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Arrow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Arrow.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Arrow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Arrow"),
		CUI_Arrow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Attribute_Slot_Active*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Attribute_Slot_Active"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Attribute_Slot_Active_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Attribute_Slot_Active */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Attribute_Slot_Active"),
		CUI_Attribute_Slot_Active::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_BaseTalentIndicator*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_BaseTalentIndicator"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_BaseTalentIndicator.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_BaseTalentIndicator */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_BaseTalentIndicator"),
		CUI_BaseTalentIndicator::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_ContentBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ContentBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_ContentBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_ContentBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ContentBackground"),
		CUI_ContentBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_DescBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DescBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_DescBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DescBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DescBackground"),
		CUI_DescBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Frame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Frame_%d.dds"), 4))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Frame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Frame"),
		CUI_Frame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SceneAboveBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SceneAboveBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_SceneAboveBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SceneAboveBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SceneAboveBackground"),
		CUI_SceneAboveBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SceneBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SceneBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_SceneBackground_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SceneBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SceneBackground"),
		CUI_SceneBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SceneUnderBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_SceneUnderBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_SceneUnderBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SceneUnderBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SceneUnderBackground"),
		CUI_SceneUnderBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_UnderLine*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_UnderLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_UnderLine.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_UnderLine */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_UnderLine"),
		CUI_UnderLine::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	//==================================================================================================================================== 플레이어 레벨 업 창

		/* For.Prototype_Component_Texture_UI_Arrow_Long*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Arrow_Long"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Arrow_Long.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Arrow_Long */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Arrow_Long"),
		CUI_Arrow_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_BlackBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_BlackBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_BlackBar.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_BlackBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_BlackBar"),
		CUI_BlackBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_DecorationFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_DecorationFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_DecorationFrame_01.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_DecorationFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_DecorationFrame"),
		CUI_DecorationFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_SkillLeftBackground*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_ButtonBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_ButtonBackground.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_SkillLeftBackground */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_ButtonBackground"),
		CUI_ButtonBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_TitleBackgroundBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_TitleBackgroundBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_TitleBackgroundBar_0%d.dds"), 2))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_TitleBackgroundBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_TitleBackgroundBar"),
		CUI_TitleBackgroundBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//==================================================================================================================================== 플레이어 기본 화면

	/* For.Prototype_Component_Texture_UI_Feather_Icon*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Feather_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Feather_Icon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Feather_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Feather_Icon"),
		CUI_Feather_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HPBar1_Border*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HPBar1_Border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_HPBar1_Border.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HPBar1_Border */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HPBar1_Border"),
		CUI_HPBar1_Border::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_TitleBackgroundBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HPBar2_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_HPBar2_BG.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_TitleBackgroundBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HPBar2_BG"),
		CUI_HPBar2_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_HPBar3_MainBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_HPBar3_MainBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_HPBar3_MainBar.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_HPBar3_MainBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_HPBar3_MainBar"),
		CUI_HPBar3_MainBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_TitleBackgroundBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MemoryShardsIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_MemoryShardsIcon.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_TitleBackgroundBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MemoryShardsIcon"),
		CUI_MemoryShardsIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MPBar1_Border*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MPBar1_Border"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_MPBar1_Border.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MPBar1_Border */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MPBar1_Border"),
		CUI_MPBar1_Border::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MPBar2_BG*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MPBar2_BG"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_MPBar2_BG.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MPBar2_BG */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MPBar2_BG"),
		CUI_MPBar2_BG::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_MPBar3_MainBar*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_MPBar3_MainBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_MPBar3_MainBar.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MPBar3_MainBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MPBar3_MainBar"),
		CUI_MPBar3_MainBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_PlunderSlotFrame*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_PlunderSlotFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_PlunderSlotFrame_%d.dds"), 3))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_PlunderSlotFrame */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_PlunderSlotFrame"),
		CUI_PlunderSlotFrame::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Potion_DefaultType*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Potion_DefaultType"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/General/UI_Potion_DefaultType.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_Potion_DefaultType */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Potion_DefaultType"),
		CUI_Potion_DefaultType::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	//====================================================================================================================================
	/* For.Prototype_Component_Texture_MouseCursor*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_MouseCursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/TexUI_GameCursor.dds"), 1))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_MouseCursor */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MouseCursor"),
		CUI_MouseCursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_GameIntro"),
		CUIGroup_GameIntro::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_TestScene_TextTest */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"),
		CUIGroup_PlayerMenu::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"),
		CUIGroup_PlayerLevelUP::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"),
		CUIGroup_PlayerTalent::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For.Prototype_GameObject_UI_LevelUP */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"),
		CUIGroup_PlayerScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion


#pragma region 티메시아 캐릭터 
	lstrcpyW(m_szLoadingText, TEXT("티메시아 모델을 생성한다."));
	/* For.Prototype_Component_Model_Kaku*/
	PreTransformMatrix = /*XMMatrixScaling(0.015f, 0.015f, 0.015f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corner"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus/Corvus.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 



#pragma region 티메시아 캐릭터 오른손 무기 모델 
	lstrcpyW(m_szLoadingText, TEXT("주인공 오른손 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Right_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Right_Weapon/Corvus_Right_Weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Right_Weapon"),
		CRightWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 


#pragma region 티메시아 캐릭터 왼손 무기 모델 
	lstrcpyW(m_szLoadingText, TEXT("주인공 왼손 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Left_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Left_Weapon/Corvus_Left_Weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Left_Weapon"),
		CLeftWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 


#pragma region Objects
	lstrcpyW(m_szLoadingText, TEXT("오브젝트들"));

	lstrcpyW(m_szLoadingText, TEXT("오브젝트들"));
	//LobTrap
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LobTrap"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/LobTrap/LobTrap.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//SM_Wall_Shelf
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Wall_Shelf"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Sall_Shelf/SM_Wall_Shelf.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//P_Rag03
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Rag03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Rag03/P_Rag03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//SM_WoodFence03
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_WoodFence03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodFence03/SM_WoodFence03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//SM_WoodFence04
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_WoodFence04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodFence04/SM_WoodFence04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//SM_WoodStairs03
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_WoodStairs03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_WoodStairs03/SM_WoodStairs03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//SM_WoodStairs03
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_HORSE_P_WoodenFrame02_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/HORSE_P_WoodenFrame02_05/HORSE_P_WoodenFrame02_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//P_BossAtriumCircle01
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossAtriumCircle01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircle01/P_BossAtriumCircle01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//P_BossCemetery_02_02
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossCemetery_02_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossCemetery_02_02/P_BossCemetery_02_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//P_BossCemetery_04
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossCemetery_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossCemetery_04/P_BossCemetery_04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossCemetery_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossCemetery_05/P_BossCemetery_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossCemetery_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossCemetery_06/P_BossCemetery_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossInteriorArches02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossInteriorArches02/P_BossInteriorArches02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_ChurchWindowGrilles01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_ChurchWindowGrilles01/P_ChurchWindowGrilles01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_LongStairs01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_LongStairs01/P_LongStairs01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Plains_CastleArch_Ruin_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Plains_CastleArch_Ruin_01/SM_Plains_CastleArch_Ruin_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Trim_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Trim_01a/SM_Trim_01a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01c"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Wall_8x8_Broken_01c/SM_Wall_8x8_Broken_01c.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01d"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Wall_8x8_Broken_01d/SM_Wall_8x8_Broken_01d.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Wall_8x8_Broken_01e"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Wall_8x8_Broken_01e/SM_Wall_8x8_Broken_01e.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Railing_base01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railing_base01/Railing_base01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Railing_pillar01_2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railing_pillar01_2/Railing_pillar01_2.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Railing01_3"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railing01_3/Railing01_3.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Brick_stone_stairs_1_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Brick_stone_stairs_1_a/SM_Brick_stone_stairs_1_a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Gate_17d"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Gate_17d/SM_Gate_17d.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_ground_Road_Middle_250x250cm_1_a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_ground_Road_Middle_250x250cm_1_a/SM_ground_Road_Middle_250x250cm_1_a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_T_P_BossRoomVines01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossRoomVines01/P_BossRoomVines01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossArtriumCircleRailing_Down02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossArtriumCircleRailing_Down02/P_BossAtriumCircleRailing_Down02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossArtriumCircleRailing_Down03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossArtriumCircleRailing_Down03/P_BossAtriumCircleRailing_Down03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossArtriumCircleRailing_Down04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossArtriumCircleRailing_Down04/P_BossAtriumCircleRailing_Down04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Top01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Top01/P_BossAtriumCircleRailing_Top01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Up01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Up01/P_BossAtriumCircleRailing_Up01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Up02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Up02/P_BossAtriumCircleRailing_Up02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Up04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Up04/P_BossAtriumCircleRailing_Up04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BossAtriumCircleRailing_Up03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_BossAtriumCircleRailing_Up03/P_BossAtriumCircleRailing_Up03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Railing03_1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Railing03_1/Railing03_1.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Fortress_BossDoor_Left01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Fortress_BossDoor_Left01/P_Fortress_BossDoor_Left01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Fortress_BossDoor_Right01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Fortress_BossDoor_Right01/P_Fortress_BossDoor_Right01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Debris_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Debris_01a/SM_Debris_01a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Debris_02a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Debris_02a/SM_Debris_02a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Scafold_01b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Scafold_01b/SM_Scafold_01b.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Scafold_01c"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Scafold_01c/SM_Scafold_01c.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_14"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_fence_14/SM_fence_14.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_16"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_fence_16/SM_fence_16.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_13"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_fence_13/SM_fence_13.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_CemeteryStairs01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_CemeteryStairs01/P_CemeteryStairs01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Brick_Floor"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Brick_Floor/brickFloor.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Archive_Chair01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/P_Archive_Chair01/P_Archive_Chair01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ladder"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Ladder/Ladder.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BaseWall_01_Corner"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_BaseWall_01_Corner.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BaseWall_02_Plain"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_BaseWall_02_Plain.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_LogPile_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_LogPile_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Separator"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Separator.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_crypt_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_crypt_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_crypt_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_crypt_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_crypt_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_crypts/SM_crypt_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_curb_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_curb_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_curb_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_curb_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_curb_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_curb_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_curb_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_curb_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Curb/SM_curb_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Cloth01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Cloth02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Cloth03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Cloth04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Cloth05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Cloth06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Cloth07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Cloth08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Clothes/P_Cloth08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_rock_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_rock/SM_rock_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_rock_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_rock/SM_rock_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_rock_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_rock/SM_rock_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_CemeteryStairs02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Stairs/P_CemeteryStairs02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_CemeteryStairs03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Stairs/P_CemeteryStairs03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Stairs"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Stairs/SM_Stairs.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Stairs02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Stairs/SM_Stairs02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_urn_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_urn_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_urn_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_urn_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_urn_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_urn_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_urn_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Urn/SM_urn_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_05"),

		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_10.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_12"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_12.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_15"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_15.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_19"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_19.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_20"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_20.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_21"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_21.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_22"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_22.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_23"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_23.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_24"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_24.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_25"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_25.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_26"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_26.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_27"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_27.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_29"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_29.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_30"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_30.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_31"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_31.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_32"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_32.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_33"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_33.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_34"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_34.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_fence_35"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Fences/SM_fence_35.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Barrel_Closed"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Barrel/SM_Barrel_Closed.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_07"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_07.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_09.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_10.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_11"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_11.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_12"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_12.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_13"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_13.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_15"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_15.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_16"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_16.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_17"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_17.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_18"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_18.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_20"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_20.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_21"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_21.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_22"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_22.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL
		;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_23"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_23.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_25"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_25.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_26"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_26.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_27"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_27.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_28"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_28.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_29"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_29.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_31"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_31.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_32"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_32.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_33"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_33.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_34"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_34.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_35"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_35.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_36"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_36.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_37"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_37.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_38"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_38.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_41"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_41.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_42"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_42.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_44"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_44.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_46"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_46.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_48"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_48.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_50"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_50.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_51"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_51.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_53"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_53.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_54"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_54.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_55"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_55.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_56"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_56.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_60"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_60.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_61"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_61.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_62"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_62.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_gravestone_63"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/GraveStone/SM_gravestone_63.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Fence_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Fence_004.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Wall_Combined_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/SM_Woods/SM_Wall_Combined_03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Deceased01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Deceased02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Deceased03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Deceased04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased04.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Deceased05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased05.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_Deceased06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Deceased/P_Deceased06.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_DeadTree_01a"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_DeadTree_01a.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_DeadTree_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_DeadTree_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BigTree001"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree001.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BigTree002"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree002.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BigTree003"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree003.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BigTree004"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree004.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BigTree005"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree005.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BigTree006"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree006.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BigTree006_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree006_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_BigTree006_03_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/SM_BigTree006_03_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_TreeAerialRoot01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_TreeAerialRoot01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_TreeAerialRoot02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_TreeAerialRoot02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_BrokenTree01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_BrokenTree01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_DeadTree02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_DeadTree02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_P_DeadTree03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/P_DeadTree03.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_DryTreeT3_2_SM_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_2_SM_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_DryTreeT3_branches_1_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_1_SM.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_DryTreeT3_branches_1_SM_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_1_SM_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_DryTreeT3_branches_3_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_3_SM.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_DryTreeT3_branches_5_SM"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/Trees/DryTreeT3_branches_5_SM.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_01.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_08"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_08.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_10.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_11"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_11.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_14"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_14.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_15"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_15.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_16"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_16.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_19"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_19.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_20"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_20.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_28"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_28.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_35"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_35.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_36"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_36.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_38"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_38.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_39"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_39.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_40"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_40.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_41"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_41.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_tombstone_42"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Objects/TombStones/SM_tombstone_42.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Object_StaticObject"),
	//	CStaticObject::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
#pragma endregion 


















	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Object_StaticObject"),
		CStaticObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region  인스턴싱용 환경 오브젝트

	lstrcpyW(m_szLoadingText, TEXT("인스턴싱용 환경 오브젝트"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Grass0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/Grass0/Grass0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Tree0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/Tree0/Tree0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_House0"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/GroundObjects/House0/House0.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Object_GroundObject"),
		CGroundObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion





#pragma region 테스트용 사다리
	//lstrcpyW(m_szLoadingText, TEXT("사다리 모델을 생성한다."));
	/* For.Prototype_Component_Model_Ladder*/
	//PreTransformMatrix = /*XMMatrixScaling(0.015f, 0.015f, 0.015f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Ladder"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Ladder/Ladder.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
	//	return E_FAIL;

	/////* For.Prototype_GameObject_Ladder */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Ladder"),
	//	CLadder::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;



#pragma endregion 
#pragma region 티메시아 보스
	lstrcpyW(m_szLoadingText, TEXT("보스 모델을 생성한다."));
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Magician"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Magician/Boss_Magician.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Magician"),
		CBoss_Magician::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Varg_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Varg/Boss_Varg.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg_Body"),
		CBody_Varg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Varg_Knife"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Varg/Knife/VargKnife.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg_Knife"),
		CVargKnife::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg"),
		CBoss_Varg::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//보스 HP바

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Boss_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Boss_HP/BossHP%d.png"), 4))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Boss_HP_Bar"),
		CUI_Boss_HP_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Boss_HP_Bar_Gage"),
		CUI_Boss_HP_Bar_Gage::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Boss_HP_Bar_Gage_Effect"),
		CUI_Boss_HP_Bar_Gage_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion 

#pragma region 엘리트 몬스터

	lstrcpyW(m_szLoadingText, TEXT("엘리트 몬스터 모델을 생성한다."));
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Elite_Joker_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Elite/Elite_Joker/Elite_Joker.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Elite_Joker_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Elite/Elite_Joker/Weapon/Joker_Weapon.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Elite_Joker_Body"),
		CBody_Joker::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Elite_Joker_Weapon"),
		CJoker_Weapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Elite_Joker"),
		CElite_Joker::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region 일반 몬스터
	lstrcpyW(m_szLoadingText, TEXT("일반 몬스터 모델을 생성한다."));
	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Normal_VillageM0_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageM0/Normal_VillageM0.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Normal_VillageM1_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageM1/Normal_VillageM1.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Normal_VillageF0_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageF0/Normal_VillageF0.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Normal_VillageF1_Body"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Normal_VillageF1/Normal_VillageF1.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Normal_VillageM_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Weapon/Axe/HandAxe.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = /*XMMatrixScaling(0.002f, 0.002f, 0.002f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Normal_VillageF_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Normal/Weapon/Knife/Knife.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageM0_Body"),
		CBody_VillageM0::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageM1_Body"),
		CBody_VillageM1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageF0_Body"),
		CBody_VillageF0::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageF1_Body"),
		CBody_VillageF1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageM_Weapon"),
		CVillageM_Weapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageF_Weapon"),
		CVillageF_Weapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageM0"),
		CNormal_VillageM0::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageM1"),
		CNormal_VillageM1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageF0"),
		CNormal_VillageF0::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageF1"),
		CNormal_VillageF1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Monster_HP*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Monster_HP"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/ThymesiaUI/Monster_HP/MonsterHP%d.dds"), 6))))
		return E_FAIL;
	/* For.Prototype_GameObject_Monster_HP_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_HP_Bar"),
		CMonster_HP_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_HP_Gage"),
		CMonster_HP_Gage::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_HP_Gage_Effect"),
		CMonster_HP_Gage_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster_HP_Bar_Effect"),
		CMonster_HP_Bar_Effect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

	/* 로딩이 완료되었습니다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));	
	m_isFinished = true;
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
