#include "pch.h" 
#include "Level_Tutorial.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	

#include "Object.h"
#include "EnvironmentObject.h"
#include "TriggerObject.h"
#include "BlackScreen.h"

#include "UI_LeftBackground.h"

#include "Button.h"
#include "GameItem.h"
#include "DeadBranch.h"

#include "Effect_Mesh.h"
#include "Effect_Particle.h"



CLevel_Tutorial::CLevel_Tutorial(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	
}

HRESULT CLevel_Tutorial::Initialize()
{
	m_iCurrentLevel = LEVEL_TUTORIAL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Structure(TEXT("Layer_Structure"))))	
		return E_FAIL;		

	if (FAILED(Ready_Layer_Monster()))	
		return E_FAIL;

	if (FAILED(Ready_Layer_NPC(TEXT("Layer_NPC"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
			
	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))	
		return E_FAIL;

	if (FAILED(Ready_Layer_Fade(TEXT("Layer_Fade"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_Inventory(TEXT("Layer_PlayerInventory"))))
		return E_FAIL;

 	if (FAILED(Ready_Layer_UIGroup_GameIntro(TEXT("Layer_GameIntro"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_PlayerMenu(TEXT("Layer_PlayerMenu"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_PlayerLevelUP(TEXT("Layer_PlayerLevelUP"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_PlayerTalent(TEXT("Layer_PlayerTalent"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_PlayerScreen(TEXT("Layer_PlayerScreen"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Item(TEXT("Layer_GameItem"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))	
		return E_FAIL;	

	m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_OUT);
	m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_IN);


	// 플레이어 화면 키기
	m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerScreen"), true);
	m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);

	m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	m_pGameInstance->PlayBGM(L"TutoMapBGM.ogg", 0.8f);

	return S_OK;
}

void CLevel_Tutorial::Update(_float fTimeDelta)
{
	if (m_pGameInstance->isKeyEnter(DIK_TAB))
	{
		if (!m_pGameInstance->Get_Scene_Render_State(m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")))
		{
			if (m_bStopMenuOpen)
			{

				m_bStopMenuOpen = false;
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerInventory"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
			}
			else
			{
				m_bStopMenuOpen = true;
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), true); // 마우스 이미지 켜기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerInventory"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), true);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerScreen"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
			}
		}
		
	}
	//}
	//if (m_pGameInstance->isKeyEnter(DIK_E))
	//{
	//	m_iOpenSceneCount++;
	//	if (1 == m_iOpenSceneCount) // 게임 인트로
	//	{
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_GameIntro"), true);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), true);
	//	}
	//	if (2 == m_iOpenSceneCount) // 플레이어 메뉴
	//	{
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_GameIntro"), false);
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerMenu"), true);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), false);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
	//	}
	//	if (3 == m_iOpenSceneCount) // 플레이어 레벨 업
	//	{
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerMenu"), false);
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerLevelUP"), true);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), true);
	//	}
	//	if (4 == m_iOpenSceneCount) // 플레이어 특성
	//	{
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerLevelUP"), false);
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerTalent"), true);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), false);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent")), true);
	//	}
	//	if (5 == m_iOpenSceneCount) // 플레이어 화면
	//	{
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerTalent"), false);
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerScreen"), true);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent")), false);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
	//	}
	//	if (6 == m_iOpenSceneCount) // 플레이어 화면
	//	{
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerScreen"), false);
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerInventory"), true);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), true);
	//	}
	//	if (7 == m_iOpenSceneCount) // 플레이어 화면
	//	{
	//		m_iOpenSceneCount = 0;
	//		m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_PlayerInventory"), false);
	//		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), false);
	//	}



	//}
}

HRESULT CLevel_Tutorial::Render() 
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Lights()
{
	CTransform* pCamTransform = static_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_TUTORIAL, TEXT("Layer_Camera"), TEXT("Com_Transform")));

	_matrix matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	_vector vCamInfo = { 60.f, 16.f / 9.f , 0.1f, 800.f };

	if (FAILED(m_pGameInstance->SetUp_ShadowLight(XMVectorSet(200.7f, 100.3f, -1.f, 1.0f), XMVectorSet(100.f, 0.f, -1.f, 1.f),
		XMConvertToRadians(60.f), g_iWinSizeX / static_cast<_float>(g_iWinSizeY), 0.1f, 800.f
		, matView, vCamInfo, pCamTransform)))
		return E_FAIL;

	CTransform* pPlayerTransform = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Layer_Player"), "PLAYER")->Get_Transfrom();

	LIGHT_DESC LightDesc{};
	/* 2월 8일 빛 */
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 0.f, 0.f);
	LightDesc.vDiffuse = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	ZeroMemory(&LightDesc, sizeof(LightDesc));

	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
	LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vPosition = _float4(85.84f, 6.3999f, -118.63f, 1.f);
	LightDesc.fRange = 3.f;

	if (FAILED(m_pGameInstance->Add_Light(LightDesc, pPlayerTransform)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC pDesc = {};

	pDesc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Terrain"), LEVEL_TUTORIAL, pLayerTag, &pDesc)))
		return E_FAIL;

	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_ForkLift"), LEVEL_TUTORIAL, pLayerTag, nullptr)))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Sky"), LEVEL_TUTORIAL, pLayerTag, &pDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Structure(const _tchar* pLayerTag)
{
	//현재 몬스터와 기본맵이 있는 Prototype용 맵							-> Load_Objects(16);
	//Map Tool 기능 및 Test용 맵											-> Load_Objects(87);
	//튜토리얼 맵 수정 중 (크기 조절 중 98번 맵파일은 잠시 봉인합니다.		-> Load_Objects(107);
	// 
	//Load_Objects(140); //Tutorial Map
	//Load_Objects(142); //Tutorial Map
	if (FAILED(Load_Objects(145)))
		return E_FAIL;//Tutorial Map
	//Load_Objects(301); //Circus Map
	//Load_Objects(303); //Circus Map


	//Load_TriggerObjects(0);			// 원래 의자 쪽에 있었던 트리거 오브젝트 파일
	Load_TriggerObjects(1);				// 이제 보스 입구 쪽에 심어져있는 파일임.
	/* 여기서 맵 파일 하나하나 다 읽어와야함 */

	//_ulong dwByte = {}; 
	////HANDLE hFile = CreateFile(TEXT("../Map_File/real76.bin"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//HANDLE hFile = CreateFile(TEXT("../Map_File/real143.bin"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//while (true)
	//{
	//	_tchar Layer_Name[MAX_PATH] = {}; //레이어 이름										
	//	ReadFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);

	//	if (dwByte == 0)
	//		break;
	//	/* 이름 작업 */
	//	_char   Prototype_Name[MAX_PATH] = {};

	//	ReadFile(hFile, Prototype_Name, MAX_PATH, &dwByte, nullptr);


	//	_float4x4 WorldMatrix = {};
	//	ReadFile(hFile, &WorldMatrix, sizeof(_float4x4), &dwByte, nullptr);
	//	//int a = 4;

	//	_tchar Translate_wchar[MAX_PATH] = {};
	//	MultiByteToWideChar(CP_ACP, 0, Prototype_Name, static_cast<_int>(strlen(Prototype_Name)), Translate_wchar, MAX_PATH);

	//	/* 이제 TRANSFORM만 건들면 될듯함.*/
	//	//int b = 4;
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_TUTORIAL, Translate_wchar, LEVEL_TUTORIAL, Layer_Name)))
	//		return E_FAIL;

	//	//CTransform* pTrasnform = dynamic_cast<CTransform*>(
	//	//* Find GaemObject 만들어야 할듯
	//	// 구분할 수 있는 방법을 생각해봐야할듯.
	//	map<const _wstring, class CLayer*>* Level_Layers = m_pGameInstance->Get_Layers();		

	//	auto& Level_Tutorial = Level_Layers[3];

	//	for (auto& Layers : Level_Tutorial)
	//	{
	//		//auto& iter = find(Level_Tutorial.begin(), Level_Tutorial.end(), Layer_Name);	
	//		auto iter = Level_Tutorial.find(Layer_Name);

	//		if (iter == Level_Tutorial.end())
	//			return E_FAIL;

	//		else
	//		{
	//			CTransform* pTranform = dynamic_cast<CTransform*>(
	//				iter->second->Get_GameObject_List().back()->Find_Component(TEXT("Com_Transform")));	

	//			pTranform->Set_WorldMatrix(WorldMatrix);	
	//		}
	//	}
	//}
	//CloseHandle(hFile);
	
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Player(const _tchar* pLayerTag)
{
	/* 여기서 디스크립션으로 넘겨주기 */
	CGameObject::GAMEOBJECT_DESC        Desc{};

	Desc.fSpeedPerSec = 45.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"), m_iCurrentLevel, pLayerTag, &Desc)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CCamera_Free::FREE_CAMERA_DESC		Desc = {};

	
	Desc.vEye = _float3(0.f, 10.f, -7.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);

	Desc.fFovy = XMConvertToRadians(50.f);
	Desc.fNear = 0.1f;
	Desc.fFar = 800.f;
	Desc.fMouseSensor = 0.05f;
	Desc.fSpeedPerSec = 25.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Free"), LEVEL_TUTORIAL, pLayerTag, &Desc)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Debug"), LEVEL_TUTORIAL, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Monster()
{

	Load_MonsterIndex(3);

	CGameObject::GAMEOBJECT_DESC pDesc = {};

	pDesc.iCurLevel = m_iCurrentLevel;

	_vector vTestPosition = { 111.64f, 15.88f, -41.30f, 1.f };
	XMStoreFloat4(&pDesc.fPosition, vTestPosition);
	for (_uint i = 0; i < 20; i++)
	{
		if (FAILED(m_pGameInstance->Add_Projectile(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Card"), PROJECTILE_CARD, &pDesc)))
			return E_FAIL;
	}
	if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician"), CATEGORY_BOSS, &pDesc)))
		return E_FAIL;
	
	for (size_t i = 0; i < m_MonsterSpawnInfos.size(); i++)
	{
		switch (m_MonsterSpawnInfos[i].iMonsterIndex)
		{
		case BOSS_VARG:
			pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
			if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg"), CATEGORY_BOSS, &pDesc)))
				return E_FAIL;
			break;
		default:
			break;
		case ELITE_JOKER:
			pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
			if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Joker"), CATEGORY_ELITE, &pDesc)))
				return E_FAIL;
			break;
		case ELITE_HARMORLV2:
			pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
			if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_HArmorLV2"), CATEGORY_ELITE, &pDesc)))
				return E_FAIL;
			break;

		case NORMAL_VILLAGE_M0:
			pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
			if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM0"), CATEGORY_NORMAL, &pDesc)))
				return E_FAIL;
			break;
		case NORMAL_VILLAGE_M1:
			pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
			if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM1"), CATEGORY_NORMAL, &pDesc)))
				return E_FAIL;
			break;
		case NORMAL_VILLAGE_F0:
			pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
			if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF0"), CATEGORY_NORMAL, &pDesc)))
				return E_FAIL;
			break;
		case NORMAL_VILLAGE_F1:
			pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
			if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF1"), CATEGORY_NORMAL, &pDesc)))
				return E_FAIL;
			break;
		}
	}
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_NPC(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_NPC_Aisemy"), LEVEL_TUTORIAL, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Effect(const _tchar* pLayerTag)
{
	//Mesh Effect
	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerClaw1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_CLAW1)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerClaw2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_CLAW2)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerLAttack1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_SWORD1)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerLAttack2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_SWORD2)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerLAttack3.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_SWORD3)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerLAttack4_1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_SWORD4_1)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerLAttack4_2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_SWORD4_2)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerLAttack5.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_SWORD5)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerAttack5_Dust_Distortion.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
		EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_ATTACK5_DUST)))
		return E_FAIL;

	//Particle Effect
	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK, 3)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Left.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_LEFT, 2)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Right.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_RIGHT, 2)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_CalwEffect_Green.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_CLAW_GREEN_HOLDING, 64)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_CalwEffect_White.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_CLAW_WHITE_HOLDING, 64)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_1_HOLDING, 4)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_2_HOLDING, 4)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_3.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_3_HOLDING, 4)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_4_1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_4_1_HOLDING, 4)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_4_2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_4_2_HOLDING, 4)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_5.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_5_HOLDING, 4)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_PlayerAttack5_Dust.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_PLAYERATTACK_5_DUST_EXPLOSION, 2)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_Player_Hit.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_PLAYER_HIT_HOLDING, 4)))
		return E_FAIL;

	if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Explosion.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
		EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_EXPLOSION, 4)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Fade(const _tchar* pLayerTag)
{
	CBlackScreen::BLACKSCREEN_DESC BlackScreenDesc = {};

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Black"), LEVEL_TUTORIAL, pLayerTag, &BlackScreenDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_Tutorial::Ready_Layer_Button(const _tchar* pLayerTag)
{
	CButton::BUTTON_DESC ButtonDesc = {};

	for (_uint i = 0; i < 1; ++i)
	{
		ButtonDesc._iButtonTypeIndex = i;
		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_InteractionButton"), LEVEL_TUTORIAL, pLayerTag, &ButtonDesc)))
			return E_FAIL;
	}

	return S_OK;

}

HRESULT CLevel_Tutorial::Ready_Layer_Item(const _tchar* pLayerTag)
{
	/*char* m_strObjectNames[256] =
	{
		"Sphere"
	};

	CGameItem::GAMEITEM_DESC ItemDesc = {};
	ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_KEY1;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_KEY2;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_MEMORY;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_FORGIVEN;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;

	ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_SKILLPIECE;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;


	return S_OK;*/

	char* m_strObjectNames[256] =
	{
		"DeadBranch"
	};

	CGameItem::GAMEITEM_DESC ItemDesc = {};
	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_KEY1;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_KEY2;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_MEMORY;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_FORGIVEN;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_SKILLPIECE;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), LEVEL_TUTORIAL, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//============================================================================================

	CDeadBranch::DEADBRANCH_DESC BranchDesc = {};
	BranchDesc.GameItemName = m_strObjectNames[0];
	BranchDesc.eItemType = ITEM_TYPE::ITEM_DEADBRANCH;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_DeadBranch"), LEVEL_TUTORIAL, pLayerTag, &BranchDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_GameIntro"), LEVEL_TUTORIAL, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"), LEVEL_TUTORIAL, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"), LEVEL_SEAOFTREES, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag)
{
	

	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"), LEVEL_SEAOFTREES, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag)
{
	

	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"), LEVEL_SEAOFTREES, pLayerTag, &Desc, "PlayerScreen")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_Inventory(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Inventory"), LEVEL_SEAOFTREES, pLayerTag, nullptr, "Inventory")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Tutorial::Load_Objects(_int iObject_Level)
{
	_ulong dwByte = {};
	_ulong dwByte2 = {};

	string strDataPath = "../Bin/DataFiles/ObjectData/ObjectData";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	_uint iSize = 0;
	_uint iSize2 = 0;

	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	for (size_t i = 0; i < iSize; i++)
	{
		CObject::OBJECT_DESC Desc{};

		_char szLoadName[MAX_PATH] = {};

		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte, nullptr);
		ReadFile(hFile, &Desc.fPosition, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fRotation, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fScaling, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fFrustumRadius, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &Desc.iPassIndex, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &Desc.iObjectType, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &Desc.iBillBoardMeshNum, sizeof(_uint), &dwByte, nullptr);

		Desc.ObjectName = szLoadName;
		Desc.eLevelID = static_cast<LEVELID>(m_iCurrentLevel);

		CObject* pObject = nullptr;

		if (Desc.iObjectType == CObject::OBJECT_DEFAULT)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Object_StaticObject"), LEVEL_TUTORIAL, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
		else if (Desc.iObjectType == CObject::OBJECT_BILLBOARD)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Object_BillBoardObject"), LEVEL_TUTORIAL, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
	}

	ReadFile(hFile, &iSize2, sizeof(_uint), &dwByte2, nullptr);
	CEnvironmentObject::ENVIRONMENT_OBJECT_DESC Desc = {};

	vector<_int> vecBoxSize;
	_int vectorBoxSize = {};

	/*for (_uint i = 0; i < iSize2; ++i)
	{
		_uint iVecInstanceDataSize = 0;
		ReadFile(hFile, &iVecInstanceDataSize, sizeof(_uint), &dwByte2, nullptr);

		vector<VTX_MODEL_INSTANCE> vecInstanceData = {};
		vecInstanceData.resize(iVecInstanceDataSize);

		for (_uint i = 0; i < iVecInstanceDataSize; ++i)
		{
			ReadFile(hFile, &vecInstanceData[i].InstanceMatrix, sizeof(XMFLOAT4X4), &dwByte2, nullptr);
		}
	}

	_uint iInstancedGroundObjectNumSize = 0;
	ReadFile(hFile, &iInstancedGroundObjectNumSize, sizeof(_uint), &dwByte2, nullptr);


	vector<_float3>                         vecInstancedGroundObjectPos;
	vector<_float3>                         vecInstancedGroundObjectScale;
	vector<_float3>                         vecInstancedGroundObjectRotation;

	vecInstancedGroundObjectPos.resize(iInstancedGroundObjectNumSize);
	vecInstancedGroundObjectScale.resize(iInstancedGroundObjectNumSize);
	vecInstancedGroundObjectRotation.resize(iInstancedGroundObjectNumSize);

	for (size_t i = 0; i < vecInstancedGroundObjectPos.size(); i++)
	{
		_float3 fGroundObjectPos;
		ReadFile(hFile, &fGroundObjectPos, sizeof(_float3), &dwByte2, nullptr);
		vecInstancedGroundObjectPos[i] = (fGroundObjectPos);
		Desc.vecInstancePosition.push_back(fGroundObjectPos);
	}

	for (size_t i = 0; i < vecInstancedGroundObjectScale.size(); i++)
	{
		_float3 fGroundObjectScale;
		ReadFile(hFile, &fGroundObjectScale, sizeof(_float3), &dwByte2, nullptr);
		vecInstancedGroundObjectScale[i] = (fGroundObjectScale);
		Desc.vecInstanceScale.push_back(fGroundObjectScale);
	}

	for (size_t i = 0; i < vecInstancedGroundObjectRotation.size(); i++)
	{
		_float3 fGroundObjectRotation;
		ReadFile(hFile, &fGroundObjectRotation, sizeof(_float3), &dwByte2, nullptr);
		vecInstancedGroundObjectRotation[i] = (fGroundObjectRotation);
		Desc.vecInstanceRotation.push_back(fGroundObjectRotation);
	}

	for (size_t i = 0; i < iSize2; i++)
	{
		_char szLoadName[MAX_PATH] = {};

		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte2, nullptr);
		Desc.ObjectName = szLoadName;

		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Object_GroundObject"), LEVEL_TUTORIAL, TEXT("Layer_GroundObject"), &Desc)))
			return E_FAIL;
	}*/

	for (_uint i = 0; i < iSize2; ++i)
	{
		CEnvironmentObject::ENVIRONMENT_OBJECT_DESC Desc = {};
		_char szLoadName[MAX_PATH] = {};

		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte2, nullptr);
		ReadFile(hFile, &Desc.iPassIndex, sizeof(_uint), &dwByte, nullptr);
		Desc.ObjectName = szLoadName;

		ReadFile(hFile, &Desc.isCullingObject, sizeof(_bool), &dwByte2, nullptr);

		_uint iInstanceCount = 0;
		ReadFile(hFile, &iInstanceCount, sizeof(_uint), &dwByte2, nullptr);



		vector<VTX_MODEL_INSTANCE> vecInstanceData(iInstanceCount);
		vector<XMFLOAT3> vecInstancePosition(iInstanceCount);
		vector<XMFLOAT3> vecInstanceScale(iInstanceCount);
		vector<XMFLOAT4> vecInstanceRotation(iInstanceCount);

		vectorBoxSize++;
		for (_uint k = 0; k < iInstanceCount; ++k)
		{
			ReadFile(hFile, &vecInstanceData[k].InstanceMatrix, sizeof(XMFLOAT4X4), &dwByte2, nullptr);

			XMFLOAT4X4 matrix;
			memcpy(&matrix, vecInstanceData[k].InstanceMatrix, sizeof(XMFLOAT4X4));

			XMMATRIX matWorld = XMLoadFloat4x4(&matrix);

			XMVECTOR scale, rotation, translation;
			XMMatrixDecompose(&scale, &rotation, &translation, matWorld);

			XMStoreFloat3(&vecInstancePosition[k], translation);
			XMStoreFloat3(&vecInstanceScale[k], scale);

			XMFLOAT4 quaternion;
			ReadFile(hFile, &quaternion, sizeof(XMFLOAT4), &dwByte2, nullptr);
			rotation = XMLoadFloat4(&quaternion);

			vecInstanceRotation[k] = quaternion;

			vecBoxSize.resize(vectorBoxSize);
			ReadFile(hFile, &vecBoxSize[i], sizeof(_int), &dwByte2, nullptr);
		}

		Desc.vecInstancePosition = vecInstancePosition;
		Desc.vecInstanceScale = vecInstanceScale;
		Desc.vecInstanceRotation = vecInstanceRotation;
		Desc.vecBoxSize = vecBoxSize;
		Desc.eLevelID = static_cast<LEVELID>(m_iCurrentLevel);

		CEnvironmentObject* pEnvironment = reinterpret_cast<CEnvironmentObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Object_GroundObject"), LEVEL_TUTORIAL, TEXT("Layer_GroundObject"), &Desc));

		if (nullptr == pEnvironment)
			return E_FAIL;

		//if (pEnvironment != nullptr)
		//{
		//	pEnvironment->Set_ModelInstanceVector(vecInstanceData);
		//	m_EnvironmentObjects.push_back(pEnvironment);

		//	m_vecInstancedGroundObjectPos.insert(m_vecInstancedGroundObjectPos.end(), vecInstancePosition.begin(), vecInstancePosition.end());
		//	m_vecInstancedGroundObjectScale.insert(m_vecInstancedGroundObjectScale.end(), vecInstanceScale.begin(), vecInstanceScale.end());
		//	m_vecInstancedGroundObjectRotation.insert(m_vecInstancedGroundObjectRotation.end(), vecInstanceRotation.begin(), vecInstanceRotation.end());
		//	//m_vecBoxSize.insert(m_vecBoxSize.end(), vecBoxSize.begin(), vecBoxSize.end());
		//	m_vecBoxSize = vecBoxSize;


		//	for (_uint t = 0; t < vecBoxSize.size(); ++t)
		//	{
		//		dynamic_cast<CGroundObject*>(pEnvironment)->Set_BoxSize(vecBoxSize[t]);
		//	}
		//}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLevel_Tutorial::Load_TriggerObjects(_int iObject_Level)
{
	string strDataPath = "../Bin/DataFiles/TriggerData/TriggerObject";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	_uint iSize = 0;

	DWORD dwByte = 0;
	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	vector<CTriggerObject*>		vecTriggerObject;

	vecTriggerObject.resize(iSize);

	CTriggerObject::TC_INFO Info = {};
	for (size_t i = 0; i < iSize; i++)
	{
		CTriggerObject::TC_DESC Desc{};
		ReadFile(hFile, &Desc.fPosition, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fRotation, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fScale, sizeof(_float3), &dwByte, nullptr);

		CTriggerObject* pTriggerObject = reinterpret_cast<CTriggerObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_TriggerObject"), LEVEL_TUTORIAL, TEXT("Layer_TriggerObject"), &Desc));

		if (nullptr != pTriggerObject)
			vecTriggerObject.push_back(pTriggerObject);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLevel_Tutorial::Load_InstancingObjects(_int iObject_Level)
{
	_ulong dwByte = {};

	string strDataPath = "../Bin/DataFiles/ObjectData/ObjectData";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	_uint iSize = 0;

	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	return S_OK;
}

HRESULT CLevel_Tutorial::Load_Height(_int iObject_Level)
{
	_ulong dwByte = {};

	string strDataPath = "../Bin/DataFiles/HeightData/TerrainHeight";

	strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load ObjectData File!");
		return E_FAIL;
	}

	_uint numVertices = {};
	ReadFile(hFile, &numVertices, sizeof(_uint), &dwByte, nullptr);



	return S_OK;
}

HRESULT CLevel_Tutorial::Load_MonsterIndex(_int iMonsterIndex_Level)
{
	string strDataPath = "../Bin/DataFiles/SpawnPoint/SpawnPoint";

	strDataPath = strDataPath + to_string(iMonsterIndex_Level) + ".txt";

	_tchar		szLastPath[MAX_PATH] = {};

	MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

	HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Failed To Load SpawnPoint File!");
		return E_FAIL;
	}

	DWORD dwByte = 0;

	_uint iSize = 0;

	// 일반 오브젝트
	ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

	_float4 vMonsterPos = {};
	_int	iMonsterIndex = { -1 };

	for (size_t i = 0; i < iSize; i++)
	{
		MONSTERSPAWNINFO SpawnInfo = {};

		ReadFile(hFile, &SpawnInfo.vMonsterPos,sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &SpawnInfo.iMonsterIndex, sizeof(_int), &dwByte, nullptr);

		if (iMonsterIndex < -1)
		{
			CloseHandle(hFile);

			MSG_BOX("Failed To Load SpawnPoint File!");
			return E_FAIL;
		}

		m_MonsterSpawnInfos.push_back(SpawnInfo);
	}

	CloseHandle(hFile);
	
	return S_OK;
}

HRESULT CLevel_Tutorial::Load_Effect(const _tchar* _pEffectFilePath, _uint _iPrototypeLevelIndex, const _tchar* _pEffectPrototypeName, EFFECT_TYPE _eEffectType, EFFECT_NAME _eEffectName, _uint _iEffectCount)
{
	HANDLE hFile = CreateFile(_pEffectFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MSG_BOX("Effect Loading 실패!");
		return E_FAIL;
	}

	if (EFFECT_TYPE::EFFECT_TYPE_MESH == _eEffectType)
	{
		DWORD dwByte = 0;

		CEffect_Mesh::EFFECT_MESH_DESC pDesc = {};

		_uint iNumber_Mesh_Effect{}, iMesh_Model_Count{}; //약간 Tool 용도라서 미리 빼두는 느낌(본 Project 에선 필요없을듯)
		_float fRGB_R{}, fRGB_G{}, fRGB_B{};
		ReadFile(hFile, &iNumber_Mesh_Effect, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iMesh_Model_Count, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.iShaderPass, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.iDiffuse, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.iNoise, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.iMask, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fMaxTimer, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fTimer_SpeedX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fTimer_SpeedY, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fDissolve_Speed, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fWeightX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fWeightY, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fStartTexcoordX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fStartTexcoordY, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fMaskCountX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fMaskCountY, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &fRGB_R, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &fRGB_G, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &fRGB_B, sizeof(_float), &dwByte, nullptr);

		pDesc.vRGB = _float3(fRGB_R, fRGB_G, fRGB_B);

		ReadFile(hFile, &pDesc.fTexcoord_LerpX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fTexcoord_LerpY, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.bTexcoordX, sizeof(_bool), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.bTexcoordY, sizeof(_bool), &dwByte, nullptr);

		ReadFile(hFile, &pDesc.bUsing_Noise, sizeof(_bool), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.bLoop, sizeof(_bool), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.bMinus, sizeof(_bool), &dwByte, nullptr);

		ReadFile(hFile, &pDesc.vScale, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.vRot, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.vTranslation, sizeof(_float3), &dwByte, nullptr);

#pragma region Switch For Mesh Model Name
		switch (iNumber_Mesh_Effect) //이거 Tool에서의 순서 기반임
		{
		case 0:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Donut");
			break;
		case 1:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_ThinDonut");
			break;
		case 2:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Rainbow");
			break;
		case 3:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Claw");
			break;
		case 4:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Sword");
			break;
		case 5:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Twist");
			break;
		case 6:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Strange");
			break;
		case 7:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Tornado");
			break;
		case 8: //이건 아마 없을예정(플레이어 까마귀 팔 Mesh)
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Quad");
			break;
		case 9:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Cross");
			break;
		case 10:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Ring");
			break;
		case 11:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Circle");
			break;
		case 12:
			pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Quad");
			break;
		}
#pragma endregion

		for (_uint i = 0; i < _iEffectCount; i++)
		{
			if (FAILED(m_pGameInstance->Add_Effect(_iPrototypeLevelIndex, _pEffectPrototypeName, _eEffectName, &pDesc)))
				return E_FAIL;
		}

		
	}
	else
	{
		//Particle 용도
		DWORD dwByte = 0;

		CEffect_Particle::EFFECT_PARTICLE_DESC pDesc = {};

		_uint iParticle_Function = {}; //약간 Tool 용도라서 미리 빼두는 느낌(본 Project 에선 필요없을듯)

		ReadFile(hFile, &pDesc.iParticle_Count, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &iParticle_Function, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.iShaderPass, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.iDiffuse, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.fMaxTimer, sizeof(_float), &dwByte, nullptr);

		ReadFile(hFile, &pDesc.vRGB, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.vScale, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.vRot, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &pDesc.vTranslation, sizeof(_float3), &dwByte, nullptr);

#pragma region Switch For Particle Buffer&Shader Name
		switch (_eEffectName)
		{
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_DROP:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Drop");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Test");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_EXPLOSION:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Explosion");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Test");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_LEFT:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Left");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_RIGHT:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Right");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Holding");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Test");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_CLAW_GREEN_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Holding");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_ClawEffect_Green");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_CLAW_WHITE_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Holding");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_ClawEffect_White");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_1_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Blood_1");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_2_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Blood_2");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_3_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Blood_3");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_4_1_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Blood_4_1");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_4_2_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Blood_4_2");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_5_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Blood_5");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_PLAYERATTACK_5_DUST_EXPLOSION:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_PlayerAttack_5_Dust");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_PLAYER_HIT_HOLDING:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Blood_Player_Hit");
			break;
		case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_EXPLOSION:
			pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
			pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Explosion");
			break;
		}
#pragma endregion

		for (_uint i = 0; i < _iEffectCount; i++)
		{
			if (FAILED(m_pGameInstance->Add_Effect(_iPrototypeLevelIndex, _pEffectPrototypeName, _eEffectName, &pDesc)))
				return E_FAIL;
		}

	}

	CloseHandle(hFile);

	return S_OK;
}

CLevel_Tutorial * CLevel_Tutorial::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Tutorial*	pInstance = new CLevel_Tutorial(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_Tutorial");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Tutorial::Free()
{
	__super::Free();

}
