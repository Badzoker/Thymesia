#include "pch.h" 
#include "Level_RoyalGarden.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	
#include "Level_Loading.h"

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

#include "Terrain.h"
#include "Water.h"


CLevel_RoyalGarden::CLevel_RoyalGarden(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	
}

HRESULT CLevel_RoyalGarden::Initialize()
{
	m_iCurrentLevel = LEVEL_ROYALGARDEN;

	m_pGameInstance->Set_Level_ForEventManager(m_iCurrentLevel);

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Button(TEXT("Layer_Button"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Structure(TEXT("Layer_Structure"))))	
		return E_FAIL;		

	//if (FAILED(Ready_Layer_Monster()))	
	//	return E_FAIL;

	/*if (FAILED(Ready_Layer_NPC(TEXT("Layer_NPC"))))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
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

	if (FAILED(Ready_Layer_UIGroup_Skill(TEXT("Layer_PlayerSkill"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_PlayerScreen(TEXT("Layer_PlayerScreen"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UIGroup_LandingMessage(TEXT("Layer_Landing"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Item(TEXT("Layer_GameItem"))))
		return E_FAIL;

	if (FAILED(Ready_Lights()))	
		return E_FAIL;	

	m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_OUT);
	m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_IN);


	// 플레이어 화면 키기
	m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerScreen"), true);
	m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);

	m_pGameInstance->StopSound(CHANNELID::SOUND_BGM);
	m_pGameInstance->PlayBGM(L"TutoMapBGM.ogg", 0.8f);

	/* 맵 이름 알림*/
	m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_Landing"), true);
	m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_5MapName")), true);
	m_pGameInstance->Set_All_UIObject_Condition_Open(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_5MapName")), true);

	return S_OK;
}

void CLevel_RoyalGarden::Update(_float fTimeDelta)
{
	if (m_pGameInstance->isKeyEnter(DIK_TAB))
	{
		if (!m_pGameInstance->Get_Scene_Render_State(m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")))
		{
			if (m_bStopMenuOpen)
			{

				m_bStopMenuOpen = false;
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), false); // 마우스 이미지 끄기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerInventory"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), false);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerScreen"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
			}
			else
			{
				m_bStopMenuOpen = true;
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_STATIC, TEXT("Layer_Mouse"), true); // 마우스 이미지 켜기
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerInventory"), true);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INVEN, L"UIScene_EscMenuBase")), true);
				m_pGameInstance->UIGroup_Render_OnOff(LEVEL_ROYALGARDEN, TEXT("Layer_PlayerScreen"), false);
				m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
			}
		}
		
	}
	if (m_bNextLevelOpen)
	{
		m_pGameInstance->Clear_ItemInfo();
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVELID>(m_iNextLevel), 2, false));
	}
}

HRESULT CLevel_RoyalGarden::Render() 
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Lights()
{
	CTransform* pCamTransform = static_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_ROYALGARDEN, TEXT("Layer_Camera"), TEXT("Com_Transform")));

	_matrix matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	_vector vCamInfo = { 60.f, 16.f / 9.f , 0.1f, 800.f };

	if (FAILED(m_pGameInstance->SetUp_ShadowLight(XMVectorSet(200.7f, 100.3f, -1.f, 1.0f), XMVectorSet(100.f, 0.f, -1.f, 1.f),
		XMConvertToRadians(60.f), g_iWinSizeX / static_cast<_float>(g_iWinSizeY), 0.1f, 800.f
		, matView, vCamInfo, pCamTransform)))
		return E_FAIL;

	CTransform* pPlayerTransform = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Layer_Player"), "PLAYER")->Get_Transfrom();

	LIGHT_DESC LightDesc{};
	/* 2월 8일 빛 */
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 0.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.1f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.1f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.1f, 1.f, 1.f);

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

HRESULT CLevel_RoyalGarden::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
   CGameObject::GAMEOBJECT_DESC pDesc = {};

    pDesc.iCurLevel = m_iCurrentLevel;

    CTerrain::TERRAININFO pTerrainInfo = {};
    pTerrainInfo.vTerrainPos = _float4(-26.9196987f, 0.0f, -135.658707f, 1.0f);
    pTerrainInfo.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Terrain"), LEVEL_ROYALGARDEN, pLayerTag, &pTerrainInfo)))
        return E_FAIL;

	CWater::WATERINFO pWaterInfo = {};
	pWaterInfo.fPosition = _float4(-26.9196987f, 10.0f, -135.658707f, 1.0f);
	pWaterInfo.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Water"), LEVEL_ROYALGARDEN, pLayerTag, &pWaterInfo)))
		return E_FAIL;
	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_ForkLift"), LEVEL_ROYALGARDEN, pLayerTag, nullptr)))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Sky"), LEVEL_ROYALGARDEN, pLayerTag, &pDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_Structure(const _tchar* pLayerTag)
{
	if (FAILED(Load_Objects(1)))// 정원
		return E_FAIL;

	if (FAILED(Load_Objects(2)))// 도서관
		return E_FAIL;

	if (FAILED(Load_Objects(3)))// 정원 보스맵
		return E_FAIL;

	//Load_TriggerObjects(0);			// 원래 의자 쪽에 있었던 트리거 오브젝트 파일
	//Load_TriggerObjects(1);				// 이제 보스 입구 쪽에 심어져있는 파일임.

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_Player(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};

	Desc.fSpeedPerSec = 45.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);
	Desc.iCurLevel = m_iCurrentLevel;

	_float4 vTestPosition = { 83.19f, 5.3f, -117.27f, 1.f }; //의자 옆 위치  // 3월 19일	
	//_float4 vTestPosition = { 70.7f, 1.3f, -110.5f, 1.0f }; //NPC 옆 위치
	//_float4 vTestPosition = { 111.64f, 15.88f, -41.30f, 1.f }; //범승이 보스옆 위치	
	//_float4 vTestPosition = { -43.58f, 101.9835f, -147.30f, 1.f }; // 서커스맵 시작위칩

	Desc._fPosition = vTestPosition;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"), LEVEL_ROYALGARDEN, pLayerTag, &Desc)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Free"), LEVEL_ROYALGARDEN, pLayerTag, &Desc)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Camera_Debug"), LEVEL_ROYALGARDEN, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_Monster()
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

HRESULT CLevel_RoyalGarden::Ready_Layer_NPC(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	Desc.fSpeedPerSec = 1.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);

	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_NPC_Aisemy"), LEVEL_ROYALGARDEN, pLayerTag, &Desc)))
	//	return E_FAIL;

	return S_OK;
}


HRESULT CLevel_RoyalGarden::Ready_Layer_Fade(const _tchar* pLayerTag)
{
	CBlackScreen::BLACKSCREEN_DESC BlackScreenDesc = {};

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Black"), LEVEL_ROYALGARDEN, pLayerTag, &BlackScreenDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CLevel_RoyalGarden::Ready_Layer_Button(const _tchar* pLayerTag)
{
	CButton::BUTTON_DESC ButtonDesc = {};

	for (_uint i = 0; i < 1; ++i)
	{
		ButtonDesc._iButtonTypeIndex = i;
		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_InteractionButton"), LEVEL_ROYALGARDEN, pLayerTag, &ButtonDesc)))
			return E_FAIL;
	}

	return S_OK;

}

HRESULT CLevel_RoyalGarden::Ready_Layer_Item(const _tchar* pLayerTag)
{
	char* m_strObjectNames[256] =
	{
		"DeadBranch"
	};

	CGameItem::GAMEITEM_DESC ItemDesc = {};

	ItemDesc.iCurLevel = m_iCurrentLevel;

	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_KEY1;
	ItemDesc.bTaken = true;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_KEY2;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_MEMORY;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_FORGIVEN;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//ItemDesc.GameItemName = m_strObjectNames[0];
	ItemDesc.iItemCount = 0;
	ItemDesc.eItemType = ITEM_TYPE::ITEM_SKILLPIECE;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GameItem"), m_iCurrentLevel, pLayerTag, &ItemDesc)))
		return E_FAIL;

	//============================================================================================

	CDeadBranch::DEADBRANCH_DESC BranchDesc = {};
	BranchDesc.GameItemName = m_strObjectNames[0];
	BranchDesc.eItemType = ITEM_TYPE::ITEM_DEADBRANCH;
	BranchDesc.iCurLevel = m_iCurrentLevel;
	BranchDesc.bTaken = true;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_DeadBranch"), m_iCurrentLevel, pLayerTag, &BranchDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_GameIntro"), LEVEL_ROYALGARDEN, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"), LEVEL_ROYALGARDEN, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"), LEVEL_ROYALGARDEN, pLayerTag, &Desc, "PlayerLevelUp")))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"), LEVEL_ROYALGARDEN, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"), LEVEL_ROYALGARDEN, pLayerTag, &Desc, "PlayerScreen")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_Inventory(const _tchar* pLayerTag)
{

	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Inventory"), LEVEL_ROYALGARDEN, pLayerTag, &Desc, "Inventory")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_LandingMessage(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Landing"), LEVEL_ROYALGARDEN, pLayerTag, &Desc)))
		return E_FAIL;
	return S_OK;
}


HRESULT CLevel_RoyalGarden::Ready_Layer_UIGroup_Skill(const _tchar* pLayerTag)
{
	CGameObject::GAMEOBJECT_DESC        Desc{};
	Desc.iCurLevel = m_iCurrentLevel;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Skill"), LEVEL_ROYALGARDEN, pLayerTag, &Desc, "UI_Skill")))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_Objects(_int iObject_Level)
{
	_ulong dwByte = {};
	_ulong dwByte2 = {};

	string strDataPath = "../Bin/DataFiles/ObjectData/Level_Royal_Garden/ObjectData";

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
		ReadFile(hFile, &Desc.vColor, sizeof(_float4), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fEmissivePower, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &Desc.iGlassNum, sizeof(_uint), &dwByte, nullptr);
		
		Desc.ObjectName = szLoadName;

		Desc.eLevelID = static_cast<LEVELID>(m_iCurrentLevel);

		if (Desc.iObjectType == CObject::OBJECT_DEFAULT)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_StaticObject"), LEVEL_ROYALGARDEN, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
		else if (Desc.iObjectType == CObject::OBJECT_BILLBOARD)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_BillBoardObject"), LEVEL_ROYALGARDEN, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
		else if (Desc.iObjectType == CObject::OBJECT_EMISSIVE)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_EmissiveObject"), LEVEL_ROYALGARDEN, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
		else if (Desc.iObjectType == CObject::OBJECT_GLASS)
		{
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_GlassObject"), LEVEL_ROYALGARDEN, TEXT("Layer_Object"), &Desc)))
				return E_FAIL;
		}
	}

	ReadFile(hFile, &iSize2, sizeof(_uint), &dwByte2, nullptr);
	CEnvironmentObject::ENVIRONMENT_OBJECT_DESC Desc = {};

	vector<_int> vecBoxSize;
	_int vectorBoxSize = {};

	for (_uint i = 0; i < iSize2; ++i)
	{
		CEnvironmentObject::ENVIRONMENT_OBJECT_DESC Desc = {};
		_char szLoadName[MAX_PATH] = {};

		Desc.eLevelID = static_cast<LEVELID>(m_iCurrentLevel);
		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte2, nullptr);
		ReadFile(hFile, &Desc.iPassIndex, sizeof(_uint), &dwByte, nullptr);
		Desc.ObjectName = szLoadName;

		ReadFile(hFile, &Desc.isCullingObject, sizeof(_bool), &dwByte2, nullptr);
		ReadFile(hFile, &Desc.iInstanceType, sizeof(_uint), &dwByte2, nullptr);

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

		switch (Desc.iInstanceType)
		{
		case CEnvironmentObject::ENV_DEFAULT:
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_GroundObject"), LEVEL_ROYALGARDEN, TEXT("Layer_GroundObject"), &Desc)))
				return E_FAIL;
			break;
		case CEnvironmentObject::ENV_EMISSIVE:
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_Object_EmissiveGroundObject"), LEVEL_ROYALGARDEN, TEXT("Layer_GroundObject"), &Desc)))
				return E_FAIL;
			break;
		}

	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_TriggerObjects(_int iObject_Level)
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

		CTriggerObject* pTriggerObject = reinterpret_cast<CTriggerObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_ROYALGARDEN, TEXT("Prototype_GameObject_TriggerObject"), LEVEL_ROYALGARDEN, TEXT("Layer_TriggerObject"), &Desc));

		if (nullptr != pTriggerObject)
			vecTriggerObject.push_back(pTriggerObject);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLevel_RoyalGarden::Load_InstancingObjects(_int iObject_Level)
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

HRESULT CLevel_RoyalGarden::Load_Height(_int iObject_Level)
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

HRESULT CLevel_RoyalGarden::Load_MonsterIndex(_int iMonsterIndex_Level)
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


CLevel_RoyalGarden * CLevel_RoyalGarden::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_RoyalGarden*	pInstance = new CLevel_RoyalGarden(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_RoyalGarden");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_RoyalGarden::Free()
{
	__super::Free();

	m_pGameInstance->Reset_Effect();
}
