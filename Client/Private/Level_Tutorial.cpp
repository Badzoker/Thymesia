#include "pch.h" 
#include "Level_Tutorial.h"
#include "Level_Loading.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	

#include "Object.h"					// 일반 오브젝트라고 생각하세요ㅋ (깨작깨작한 사물 오브젝트)
#include "EnvironmentObject.h"		// 인스턴싱용 환경용 오브젝트ㅋㅋ
#include "TriggerObject.h"			// 말그대로 트리거 오브젝트ㅋㅋ(피직스 콜라이더 렌더링 담아줄 오브젝트)
#include "SpecificObject.h"			// 맵 어디서나 다 쓰일 오브젝트ㅋㅋ (사다리 or 의자 같은 거)
#include "Chair.h"
#include "ChairLamp.h"
#include "BlackScreen.h"
#include "DestructObject.h"
#include "DoorObject.h"
#include "DoorManager.h"

#include "UI_LeftBackground.h"

#include "Button.h"
#include "LockLine.h"
#include "GameItem.h"
#include "DeadBranch.h"

#include "Effect_Mesh.h"
#include "Effect_Sword.h"
#include "Effect_Particle.h"
#include "Effect_Instancing.h"

#include "Terrain.h"
#include "BarrierScreen.h"


CLevel_Tutorial::CLevel_Tutorial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel{ pDevice, pContext }
{

}




HRESULT CLevel_Tutorial::Initialize()
{
    m_iCurrentLevel = LEVEL_TUTORIAL;

    m_pGameInstance->Set_Level_ForEventManager(m_iCurrentLevel);

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

    if (FAILED(Ready_Layer_UIGroup_Skill(TEXT("Layer_PlayerSkill"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_UIGroup_PlayerScreen(TEXT("Layer_PlayerScreen"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_UIGroup_LandingMessage(TEXT("Layer_Landing"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_UIGroup_Dialogue(TEXT("Layer_Dialogue"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Item(TEXT("Layer_GameItem"))))
        return E_FAIL;

    if (FAILED(Ready_Lights()))
        return E_FAIL;

    m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_OUT);
    m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_IN);
    m_pGameInstance->Add_Trigger(TRIGGER_TYPE::TT_FADE_AUTO);


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

    if (m_pGameInstance->isKeyEnter(DIK_B))
    {
        m_pGameInstance->UIGroup_Render_OnOff(LEVEL_TUTORIAL, TEXT("Layer_Landing"), true);
        m_pGameInstance->UIScene_UIObject_Render_OnOff(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_2Beacon")), true);
        m_pGameInstance->Set_All_UIObject_Condition_Open(m_pGameInstance->Find_UIScene(UISCNEN_MESSAGE, TEXT("UIScene_Landing_2Beacon")), true);
    }
    if (m_bNextLevelOpen)
    {
        m_pGameInstance->Clear_ItemInfo();
        m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, static_cast<LEVELID>(m_iNextLevel), 2, false));
    }

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

    //CTransform* pCamTransform = static_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_SEAOFTREES, TEXT("Layer_Camera"), TEXT("Com_Transform")));

    CTransform* pCamTransform = (m_pGameInstance->Get_GameObject_To_Layer(m_iCurrentLevel, TEXT("Layer_Camera"), "Camera_Free"))->Get_Transfrom();


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
    LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
    LightDesc.vAmbient = _float4(0.5f, 0.5f, 0.5f, 1.f);
    LightDesc.vSpecular = _float4(0.3f, 0.3f, 0.3f, 1.f);
    LightDesc.vPosition = _float4(85.84f, 6.3999f, -118.63f, 1.f);
    LightDesc.fRange = 3.f;

    if (FAILED(m_pGameInstance->Add_Light(LightDesc, pPlayerTransform)))
        return E_FAIL;


    FOGPARAMS FogDesc{};
    FogDesc.fFogFactor = _float4(0.2f, 0.f, 5.f, 0.f);
    FogDesc.fFogStartDistance = _float2(0.07f, 8.f);
    FogDesc.fHeightNoiseFactor = _float2(0.f, 2.f);
    FogDesc.g_FogColor = _float4(0.5f, 0.5f, 0.5f, 1.f);

    m_pGameInstance->Set_FogFactors(FogDesc);

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
    CGameObject::GAMEOBJECT_DESC pDesc = {};

    pDesc.iCurLevel = m_iCurrentLevel;

    CTerrain::TERRAININFO pTerrainInfo = {};
    pTerrainInfo.vTerrainPos = _float4(30.0f, 0.0f, -150.0f, 1.0f);
    pTerrainInfo.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_Terrain"), LEVEL_TUTORIAL, pLayerTag, &pTerrainInfo)))
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
    if (FAILED(Load_Objects(151)))//Tutorial Map
        return E_FAIL;

    if (FAILED(Load_TriggerObjects(2)))// 이제 보스 입구 쪽에 심어져있는 파일임.
        return E_FAIL;

    if (FAILED(Load_SpecificObjects(15)))
        return E_FAIL;

    if (FAILED(Load_DestructObjects(6)))
        return E_FAIL;


    CBarrierScreen::BARRIER_SCREEN_DESC     BarrierDesc = {};
    _float4 vVargBossEntrancePosition = { 111.56f, 18.61f, -46.5f, 1.f };           // 보스 바그 방 앞에 있음.
    BarrierDesc._fPosition = vVargBossEntrancePosition;
    BarrierDesc.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_BarrierScreen"), m_iCurrentLevel, pLayerTag, &BarrierDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Player(const _tchar* pLayerTag)
{
    /* 여기서 디스크립션으로 넘겨주기 */
    CGameObject::GAMEOBJECT_DESC        Desc{};

    Desc.fSpeedPerSec = 45.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);
    Desc.iCurLevel = m_iCurrentLevel;

    _float4 vTestPosition = { 111.80f, 15.51f, -68.2f, 1.f }; // 보스 정문	
    //_float4 vTestPosition = { 83.19f, 5.3f, -117.27f, 1.f }; //의자 옆 위치  // 3월 19일	
    //_float4 vTestPosition = { 70.7f, 1.3f, -110.5f, 1.0f }; //NPC 옆 위치
    //_float4 vTestPosition = { 111.64f, 15.88f, -41.30f, 1.f }; //범승이 보스옆 위치	

    Desc._fPosition = vTestPosition;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Player"), m_iCurrentLevel, pLayerTag, &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_Camera(const _tchar* pLayerTag)
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

    for (size_t i = 0; i < m_MonsterSpawnInfos.size(); i++)
    {
        switch (m_MonsterSpawnInfos[i].iMonsterIndex)
        {
        case BOSS_VARG:
            pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg"), CATEGORY_BOSS, &pDesc)))
                return E_FAIL;
           
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Bat"), CATEGORY_BOSS, &pDesc)))
            //    return E_FAIL;

            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd"), CATEGORY_BOSS, &pDesc)))
            //    return E_FAIL;
            break;
        default:
            break;
        case ELITE_JOKER:
            pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Joker"), CATEGORY_ELITE, &pDesc)))
            //    return E_FAIL;
            
            if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Punch_Man"), CATEGORY_ELITE, &pDesc)))
                return E_FAIL;
            break;
        case ELITE_HARMORLV2:
             pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_HArmorLV2"), CATEGORY_ELITE, &pDesc)))
            //    return E_FAIL;
            
            if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Grace"), CATEGORY_ELITE, &pDesc)))
                return E_FAIL;
            break;
        case NORMAL_VILLAGE_M0:
            pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM0"), CATEGORY_NORMAL, &pDesc)))
            //    return E_FAIL;
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_ScytheM"), CATEGORY_NORMAL, &pDesc)))
            //    return E_FAIL;
            break;
        case NORMAL_VILLAGE_M1:
            pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM1"), CATEGORY_NORMAL, &pDesc)))
                return E_FAIL;
            break;
        case NORMAL_VILLAGE_F0:
            pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF0"), CATEGORY_NORMAL, &pDesc)))
            //    return E_FAIL;
            if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF1"), CATEGORY_NORMAL, &pDesc)))
                 return E_FAIL;
            break;
        case NORMAL_VILLAGE_F1:
            //pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF1"), CATEGORY_NORMAL, &pDesc)))
            //    return E_FAIL;
            break;
        }
    }

    _float4 vVargBossEntrancePosition = { 111.56f, 18.61f, -62.5f, 1.f };
    //_uint iTypenumber = {};

    pDesc.iCurLevel = m_iCurrentLevel;
    pDesc._fPosition = vVargBossEntrancePosition;

    //for (_uint i = 0; i < 3; ++i)
    {
      //  pDesc._fPosition.z -= 1.0f;
        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Building_Circus_Balloon"), m_iCurrentLevel, TEXT("Layer_Monster_Building"), &pDesc)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_NPC(const _tchar* pLayerTag)
{
    CGameObject::GAMEOBJECT_DESC        Desc{};
    Desc.iCurLevel = m_iCurrentLevel;
    Desc.fSpeedPerSec = 1.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);

    //if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_NPC_Aisemy"), LEVEL_TUTORIAL, pLayerTag, &Desc)))
    //	return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_GhostSemy"), LEVEL_TUTORIAL, pLayerTag, &Desc)))
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

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Varg_Roar_Line1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_VARG_ROAR)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Varg_Roar_Line2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_VARG_ROAR)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Varg_Roar_Line3.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_VARG_ROAR)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Varg_Roar_Line4.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_VARG_ROAR)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Varg_Roar_Line5.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_VARG_ROAR)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Varg_Roar_Blink.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_VARG_ROAR_BLINK)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Varg_Dead_Blink.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_VARG_DEAD_BLINK)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerLScythe_1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_SCYTHE_1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerLScythe_2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_SCYTHE_2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerHalberd.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_HALBERD)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerHealCurve_1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_HEAL)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerHealCurve_2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_HEAL)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Mutation_Narrow_Burst.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_MUTATION_BURST)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Mutation_ShockWave_Burst.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_MUTATION_BURST)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Mutation_Impact_Glow.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_MUTATION_IMPACT)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Mutation_Sweep.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_MUTATION_SWEEP)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Mutation_Sweep_Line.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_MUTATION_SWEEP)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Joker_ShockWave.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_JOKER_SHOCKWAVE)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerAxe.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_AXE, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerGreatSword.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_GREATSWORD)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerJavelin_Distortion.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_JAVELIN_DISTORTION)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_PlayerChargeClaw.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_CHARGECLAW)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Bat_Air_InSphere.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_BAT_AIR)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Bat_Air_OutSphere.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_BAT_AIR)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Bat_Air_Sonic_Distortion.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_BAT_AIR)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Varg_Intro_Blink.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_VARG_INTRO)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Mesh/MeshEffect_Varg_Catch_Eye.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Mesh"),
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_VARG_CATCH_EYE)))
        return E_FAIL;

    //Particle Effect
    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK, 3)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Left.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_LEFT, 4)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Right.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_RIGHT, 4)))
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
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_EXPLOSION, 8)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Vertical_Dust.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_VERTICAL_DUST, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Horizon_Dust_Varg.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HORIZON_DUST_VARG, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Left_Dust_Varg.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_LEFT_DUST_VARG, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Narrow_Dust_Varg.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_NARROW_DUST_VARG, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Right_Dust_Varg.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_RIGHT_DUST_VARG, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Narrow_Spark_Varg.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_NARROW_SPARK_VARG, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Horizon_Varg.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HORIZON_SPARK_VARG, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Varg_Execution_1_2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_VARG_EXECUTION_1_2, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Varg_Execution3.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_VARG_EXECUTION3, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_Varg_Execution1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_VARG_EXECUTION1, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_Varg_Execution2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_VARG_EXECUTION2, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_Varg_Execution3.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_VARG_EXECUTION3, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Varg_Dead.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_VARG_DEAD, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Varg_Dead.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_VARG_DEAD, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_DustDelay_Mutatin_Intro.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_INTRO, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_PlayerScythe_1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SCYTHE_PLAYER_SCYTHE_1, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_PlayerScythe_2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SCYTHE_PLAYER_SCYTHE_2, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_PlayerHalberd_1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_PLAYER_HALBERD_1, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_PlayerHalberd_2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_PLAYER_HALBERD_2, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Hurricane_Heal.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_HEAL, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Hurricane_Item_Get.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_GREEN, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Hurricane_Item_Get_Red.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_RED, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Hurricane_Item_Get_Blue.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_BLUE, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Hurricane_Item_Get_Yellow.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET_YELLOW, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_DustDelay_Mutation_Burst.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_BURST, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Explosion_Mutation_Burst.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_EXPLOSION_MUTATION_BURST, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Explosion_Mutation_Impact.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_MUTATION_IMPACT, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Hurricane_Mutation_RisingFast_Impact.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_RISING_IMPACT, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Hurricane_Mutation_FallingSlow_Impact.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_FALLING_IMPACT, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Explosion_Mutation_Stab.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_EXPLOSION_MUTATION_STAB, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_DustDelay_Mutation_Stab_Dust.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_STAB_DUST, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Mutation.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_MUTATION, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Horizon.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_HORIZON, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Narrow.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_NARROW, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Vertical.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_VERTICAL, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Left_Stab.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_LEFT_STAB, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Horizon.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_HORIZON, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Scythe_WheelAttack.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SCYTHE_WHEELATTACK, 3)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_DustDelay_ShockWave.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_SHOCKWAVE, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_ShockWave.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_SHOCKWAVE, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Smash.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_SMASH, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Joker_Smash.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_JOKER_SMASH, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Right.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_RIGHT, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Left.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_LEFT, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Joker_Intro.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_JOKER_INTRO, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Joker_Intro.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_JOKER_INTRO, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_PlayerAxe_1_Spark.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_PLAYER_AXE, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_PlayerAxe_1_Dust.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_PLAYER_AXE_HORIZON, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_PlayerAxe_1_Dust_Cross.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_PLAYER_AXE_CROSS, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_Claw.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_PLAYER_CLAW, 4)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_CaneSword_Hand.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_PLAYER_CANESWORD, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Burst_NarrowVertical.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BURST_NARROW_VERTICAL, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Burst_CaneSword.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BURST_PLAYER_CANESWORD, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_NarrowVertical.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_NARROW_VERTICAL, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_Skill_GreatSword_Start.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_GREATSWORD_START, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_Skill_GreatSword.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_GREATSWORD, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_Skill_Javelin_Start.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_JAVELIN_START, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_Skill_Javelin_Throwing.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_JAVELIN_THROWING, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_Skill_Javelin.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_JAVELIN, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_ChargeClaw.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_ChargeClaw_Hit.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_ChargeClaw_Hit_Steal.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT_STEAL, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_ChargeClaw_Hit_Steal_Complete.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_HIT_STEAL_COMPLETE, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_ChargeClaw_Loop.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_LOOP, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_World_ChargeClaw_Ready.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLD_CHARGECLAW_READY, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Player_Varg_SpAttack.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_PLAYER_VARG_SPATTACK, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Blood_Player_Varg_SpAttack.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_BLOOD_PLAYER_VARG_SPATTACK, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Spark_Player_Varg_SpAttack.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_SPARK_PLAYER_VARG_SPATTACK, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Player_Varg_SpAttack_Hit.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_PLAYER_VARG_SPATTACK_HIT, 1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_Dust_Spore.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_DUST_SPORE, 2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Particle/ParticleEffect_WorldGas_Spore.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Particle"),
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_WORLDGAS_SPORE, 1)))
        return E_FAIL;

    //Sword Effect

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Sword/SwordEffect_Varg.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Sword"),
        EFFECT_TYPE::EFFECT_TYPE_SWORD, EFFECT_NAME::EFFECT_SWORD_VARG)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Sword/SwordEffect_Claw_1.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Sword"),
        EFFECT_TYPE::EFFECT_TYPE_SWORD, EFFECT_NAME::EFFECT_SWORD_CLAW_1)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Sword/SwordEffect_Claw_2.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Sword"),
        EFFECT_TYPE::EFFECT_TYPE_SWORD, EFFECT_NAME::EFFECT_SWORD_CLAW_2)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Sword/SwordEffect_Mutation.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Sword"),
        EFFECT_TYPE::EFFECT_TYPE_SWORD, EFFECT_NAME::EFFECT_SWORD_MUTATION)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Sword/SwordEffect_HArmor.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Sword"),
        EFFECT_TYPE::EFFECT_TYPE_SWORD, EFFECT_NAME::EFFECT_SWORD_HARMOR)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Sword/SwordEffect_Joker.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Sword"),
        EFFECT_TYPE::EFFECT_TYPE_SWORD, EFFECT_NAME::EFFECT_SWORD_JOKER)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Sword/SwordEffect_Player_Eye.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Sword"),
        EFFECT_TYPE::EFFECT_TYPE_SWORD, EFFECT_NAME::EFFECT_SWORD_PLAYER_EYE)))
        return E_FAIL;

    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/Sword/SwordEffect_Varg_Eye.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Sword"),
        EFFECT_TYPE::EFFECT_TYPE_SWORD, EFFECT_NAME::EFFECT_SWORD_VARG_EYE)))
        return E_FAIL;

    //Effect_Instancing
    if (FAILED(Load_Effect(TEXT("../Bin/DataFiles/Effect/MeshInstancing/MeshInstance_Rock.dat"), LEVEL_STATIC, TEXT("Prototype_GameObject_Effect_Instancing"),
        EFFECT_TYPE::EFFECT_TYPE_MESH_INSTANCING, EFFECT_NAME::EFFECT_MESH_INSTANCING_ROCK, 3)))
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
    CLockLine::LOCKLINE_DESC LineDesc = {};
    LineDesc.iCurLevel = m_iCurrentLevel;
    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_LockLine"), LEVEL_TUTORIAL, pLayerTag, &LineDesc)))
        return E_FAIL;

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

    ItemDesc.iCurLevel = m_iCurrentLevel;

    //ItemDesc.GameItemName = m_strObjectNames[0];
    ItemDesc.iItemCount = 0;
    ItemDesc.eItemType = ITEM_TYPE::ITEM_KEY1;
    ItemDesc.bTaken = false;

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
    BranchDesc.bTaken = false;
    BranchDesc.iCurLevel = m_iCurrentLevel;

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

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"), LEVEL_TUTORIAL, pLayerTag, &Desc, "PlayerLevelUp")))
        return E_FAIL;
    return S_OK;
}
HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag)
{


    CGameObject::GAMEOBJECT_DESC        Desc{};
    Desc.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"), LEVEL_TUTORIAL, pLayerTag, &Desc)))
        return E_FAIL;
    return S_OK;
}
HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag)
{


    CGameObject::GAMEOBJECT_DESC        Desc{};
    Desc.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"), LEVEL_TUTORIAL, pLayerTag, &Desc, "PlayerScreen")))
        return E_FAIL;
    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_Inventory(const _tchar* pLayerTag)
{

    CGameObject::GAMEOBJECT_DESC        Desc{};
    Desc.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Inventory"), LEVEL_TUTORIAL, pLayerTag, &Desc, "Inventory")))
        return E_FAIL;
    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_LandingMessage(const _tchar* pLayerTag)
{
    CGameObject::GAMEOBJECT_DESC        Desc{};
    Desc.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Landing"), LEVEL_TUTORIAL, pLayerTag, &Desc)))
        return E_FAIL;
    return S_OK;

}

HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_Dialogue(const _tchar* pLayerTag)
{
    CGameObject::GAMEOBJECT_DESC        Desc{};
    Desc.iCurLevel = m_iCurrentLevel;
    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Dialogue"), LEVEL_TUTORIAL, pLayerTag, &Desc)))
        return E_FAIL;
    return S_OK;
}

HRESULT CLevel_Tutorial::Ready_Layer_UIGroup_Skill(const _tchar* pLayerTag)
{
    CGameObject::GAMEOBJECT_DESC        Desc{};
    Desc.iCurLevel = m_iCurrentLevel;

    if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_UIGroup_Skill"), LEVEL_TUTORIAL, pLayerTag, &Desc, "UI_Skill")))
        return E_FAIL;
    return S_OK;
}

HRESULT CLevel_Tutorial::Load_Objects(_int iObject_Level)
{
    _ulong dwByte = {};
    _ulong dwByte2 = {};

    string strDataPath = "../Bin/DataFiles/ObjectData/Level_Tutorial/ObjectData";

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
        Desc.iCurLevel = m_iCurrentLevel;

        CTriggerObject* pTriggerObject = reinterpret_cast<CTriggerObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_TUTORIAL, TEXT("Prototype_GameObject_TriggerObject"), LEVEL_TUTORIAL, TEXT("Layer_TriggerObject"), &Desc));

        if (nullptr != pTriggerObject)
            vecTriggerObject.push_back(pTriggerObject);
    }

    CloseHandle(hFile);

    return S_OK;
}

HRESULT CLevel_Tutorial::Load_SpecificObjects(_int iObject_Level)
{
    string strDataPath = "../Bin/DataFiles/SpecificObjectData/SpecificObjectData";

    strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

    _tchar		szLastPath[MAX_PATH] = {};

    MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

    HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        MSG_BOX("Failed To Load ObjectData File!");
        return E_FAIL;
    }

    DWORD dwByte = 0;

    _uint iSize = 0;
    ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);


    _uint iChairNum = 0;
    _uint iLampNum = 0;
    _uint iDoorNum = 0;
    _uint iDoorManager = 0;

    CDoorManager::DOOR_MANAGER_DESC DoorManagerDesc = {};

    for (size_t i = 0; i < iSize; i++)
    {
        CSpecificObject::SpecificObject_Desc Desc{};

        _char szLoadName[MAX_PATH] = {};

        ReadFile(hFile, szLoadName, MAX_PATH, &dwByte, nullptr);
        ReadFile(hFile, &Desc.fPosition, sizeof(_float4), &dwByte, nullptr);
        ReadFile(hFile, &Desc.fRotation, sizeof(_float4), &dwByte, nullptr);
        ReadFile(hFile, &Desc.fScaling, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &Desc.fFrustumRadius, sizeof(_float), &dwByte, nullptr);

        string strName = szLoadName;
        Desc.iCurLevel = m_iCurrentLevel;
        CGameObject* pObject = nullptr;

        if (strName == "P_Archive_Chair01")
        {
            Desc.iPairNum = iChairNum;
            Desc.ObjectName = strName + "_" + to_string(iChairNum);
            pObject = reinterpret_cast<CChair*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_Chair"), LEVEL_TUTORIAL, TEXT("Layer_SpecificObject"), &Desc));
            ++iChairNum;
        }
        else if (strName == "NPCLamp")
        {
            Desc.iPairNum = iLampNum;
            Desc.ObjectName = strName + "_" + to_string(iLampNum);
            pObject = reinterpret_cast<CChairLamp*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_ChairLamp"), LEVEL_TUTORIAL, TEXT("Layer_SpecificObject"), &Desc));
            ++iLampNum;
        }
        else if (strName == "SM_Door_Left")
        {

            CDoorObject::DOOR_DESC DoorDesc;
            DoorDesc.iPairNum = iDoorNum;
            DoorDesc.ObjectName = strName + "_" + to_string(iDoorNum);
            DoorDesc.fPosition = Desc.fPosition;
            DoorDesc.fRotation = Desc.fRotation;
            DoorDesc.fScaling = Desc.fScaling;
            DoorDesc.fFrustumRadius = Desc.fFrustumRadius;
            DoorDesc.iCurLevel = Desc.iCurLevel;
            DoorDesc.fRotationPerSec = 30.f;

            pObject = reinterpret_cast<CDoorObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_Door_Object"), LEVEL_TUTORIAL, TEXT("Layer_SpecificObject"), &DoorDesc));
            ++iDoorNum;

            DoorManagerDesc.pObjectLeft = DoorDesc.pDoorObject;
            DoorManagerDesc.pLeftPosition = DoorDesc.fPosition;
        }
        else if (strName == "SM_Door_Right")
        {
            CDoorObject::DOOR_DESC DoorDesc;
            DoorDesc.iPairNum = iDoorNum;
            DoorDesc.ObjectName = strName + "_" + to_string(iDoorNum);
            DoorDesc.fPosition = Desc.fPosition;
            DoorDesc.fRotation = Desc.fRotation;
            DoorDesc.fScaling = Desc.fScaling;
            DoorDesc.fFrustumRadius = Desc.fFrustumRadius;
            DoorDesc.iCurLevel = Desc.iCurLevel;
            DoorDesc.fRotationPerSec = 30.f;

            pObject = reinterpret_cast<CDoorObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_Door_Object"), LEVEL_TUTORIAL, TEXT("Layer_SpecificObject"), &DoorDesc));
            ++iDoorNum;

            DoorManagerDesc.pObjectRight = DoorDesc.pDoorObject;
            DoorManagerDesc.pRightPosition = DoorDesc.fPosition;
        }

        if ((DoorManagerDesc.pObjectLeft != nullptr) && (DoorManagerDesc.pObjectRight != nullptr))
        {
            DoorManagerDesc.ObjectName = "Door_Manager" + iDoorManager;
            XMStoreFloat4(&DoorManagerDesc.fPosition, (XMLoadFloat4(&DoorManagerDesc.pLeftPosition) + XMLoadFloat4(&DoorManagerDesc.pRightPosition)) / 2.f);
            DoorManagerDesc.fRotation = _float4(0.f, 0.f, 0.f, 1.f);
            DoorManagerDesc.fScaling = _float3(0.001f, 0.001f, 0.001f);
            DoorManagerDesc.fFrustumRadius = 0.f;
            DoorManagerDesc.iCurLevel = Desc.iCurLevel;

            if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Door_Manager_Object"), LEVEL_TUTORIAL, TEXT("Layer_SpecificObject"), &DoorManagerDesc)))
                return E_FAIL;

            DoorManagerDesc.pObjectLeft = nullptr;
            DoorManagerDesc.pObjectRight = nullptr;

            CDoorManager::DOOR_MANAGER_DESC eDesc = {};
            DoorManagerDesc = eDesc;
        }
    }

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

        ReadFile(hFile, &SpawnInfo.vMonsterPos, sizeof(_float4), &dwByte, nullptr);
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
        ReadFile(hFile, &pDesc.bMinus_X, sizeof(_bool), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.vScale, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vRot, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vTranslation, sizeof(_float3), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.bMinus_Y, sizeof(_bool), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.bGray, sizeof(_bool), &dwByte, nullptr);

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
        case 13:
            pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_HealingCurve");
            break;
        case 14:
            pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Rock");
            break;
        case 17:
            pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Sphere");
            break;
        }
#pragma endregion

        for (_uint i = 0; i < _iEffectCount; i++)
        {
            if (FAILED(m_pGameInstance->Add_Effect(_iPrototypeLevelIndex, _pEffectPrototypeName, _eEffectName, &pDesc)))
                return E_FAIL;
        }


    }
    else if (EFFECT_TYPE::EFFECT_TYPE_PARTICLE == _eEffectType)
    {
        //Particle 용도
        DWORD dwByte = 0;

        CEffect_Particle::EFFECT_PARTICLE_DESC pDesc = {};

        _uint iParticle_Function = {}; 

        ReadFile(hFile, &pDesc.iParticle_Count, sizeof(_uint), &dwByte, nullptr);
        ReadFile(hFile, &iParticle_Function, sizeof(_uint), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.iShaderPass, sizeof(_uint), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.iDiffuse, sizeof(_uint), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fMaxTimer, sizeof(_float), &dwByte, nullptr);
        pDesc.fMaxTimer += 1.f;
        ReadFile(hFile, &pDesc.vRGB, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vScale, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vRot, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vTranslation, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fAlpha_Amount, sizeof(_float), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.iNumInstance, sizeof(_uint), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.vCenter.x, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vCenter.y, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vCenter.z, sizeof(_float), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.vRange.x, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vRange.y, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vRange.z, sizeof(_float), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.vSpeed.x, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vSpeed.y, sizeof(_float), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.vLifeTime.x, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vLifeTime.y, sizeof(_float), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.vSize.x, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vSize.y, sizeof(_float), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.vPivot, sizeof(_float3), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.bReverse_XYZ, sizeof(_bool) * 3, &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vSpeed_Weight, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vScale_Weight, sizeof(_float3), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.iParticle_Initialize_Type, sizeof(_uint), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.vDelayTime, sizeof(_float2), &dwByte, nullptr);

#pragma region Switch For Particle Buffer&Shader Name

        switch (iParticle_Function)
        {
        case 0:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Drop");
            break;
        case 1:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Explosion");
            break;
        case 2:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            break;
        case 3:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Holding");
            break;
        case 4:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            break;
        case 5:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Delay");
            break;
        case 6:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Hurricane");
            break;
        case 7:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Scythe");
            break;
        case 8:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_World");
            break;
        case 9:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Burst");
            break;
        case 10:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_World_Continue");
            break;
        case 11:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
            break;
        case 12:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Round");
            break;
        case 13:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Falling_World");
            break;
        case 14:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark_Update");
            break;
        case 15:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_World_Continue_Gas");
            break;
        }
#pragma endregion

        for (_uint i = 0; i < _iEffectCount; i++)
        {
            if (FAILED(m_pGameInstance->Add_Effect(_iPrototypeLevelIndex, _pEffectPrototypeName, _eEffectName, &pDesc)))
                return E_FAIL;
        }

    }
    else if (EFFECT_TYPE::EFFECT_TYPE_MESH_INSTANCING == _eEffectType)
    {
        DWORD dwByte = 0;

        CEffect_Instancing::EFFECT_INSTANCING_DESC pDesc = {};

        _uint iNumber_Mesh_Effect{}, iMesh_Model_Count{}; //약간 Tool 용도라서 미리 빼두는 느낌(본 Project 에선 필요없을듯)
        ReadFile(hFile, &iNumber_Mesh_Effect, sizeof(_uint), &dwByte, nullptr);
        ReadFile(hFile, &iMesh_Model_Count, sizeof(_uint), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.iDiffuse, sizeof(_uint), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fMaxTimer, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fTimer_SpeedX, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fTimer_SpeedY, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fDissolve_Speed, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fHeightX, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fHeightY, sizeof(_float), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.bLoop, sizeof(_bool), &dwByte, nullptr);

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
        case 13:
            pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_HealingCurve");
            break;
        case 14:
            pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Rock");
            break;
        case 17:
            pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_Sphere");
            break;
        }
#pragma endregion

        for (_uint i = 0; i < _iEffectCount; i++)
        {
            if (FAILED(m_pGameInstance->Add_Effect(_iPrototypeLevelIndex, _pEffectPrototypeName, _eEffectName, &pDesc)))
                return E_FAIL;
        }
    }
    else //Sword
    {
#pragma region SwordEffect

        DWORD dwByte = 0;

        CEffect_Sword::EFFECT_SWORD_DESC pDesc = {};

        ReadFile(hFile, &pDesc.iDiffuse, sizeof(_uint), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.iShaderPass, sizeof(_uint), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fLength_Up, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fLength_Right, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fMaxTimer, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.fParent_Look, sizeof(_float), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vRGB, sizeof(_float3), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.vScale, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vRot, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &pDesc.vTranslation, sizeof(_float3), &dwByte, nullptr);

        ReadFile(hFile, &pDesc.iSword_XYZ, sizeof(_uint), &dwByte, nullptr);


        for (_uint i = 0; i < _iEffectCount; i++)
        {
            if (FAILED(m_pGameInstance->Add_Effect(_iPrototypeLevelIndex, _pEffectPrototypeName, _eEffectName, &pDesc)))
                return E_FAIL;
        }
#pragma endregion

    }

    CloseHandle(hFile);

    return S_OK;
}

HRESULT CLevel_Tutorial::Load_DestructObjects(_int iObject_Level)
{
    string strDataPath = "../Bin/DataFiles/DestructObjectData/DestructObjectData";

    strDataPath = strDataPath + to_string(iObject_Level) + ".txt";

    _tchar		szLastPath[MAX_PATH] = {};

    MultiByteToWideChar(CP_ACP, 0, strDataPath.c_str(), static_cast<_int>(strlen(strDataPath.c_str())), szLastPath, MAX_PATH);

    HANDLE hFile = CreateFile(szLastPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        MSG_BOX("Failed To Load ObjectData File!");
        return E_FAIL;
    }

    DWORD dwByte = 0;

    _uint iSize = 0;
    ReadFile(hFile, &iSize, sizeof(_uint), &dwByte, nullptr);

    for (size_t i = 0; i < iSize; i++)
    {
        CDestructObject::DestructObject_Desc Desc{};

        _char szLoadName[MAX_PATH] = {};

        ReadFile(hFile, szLoadName, MAX_PATH, &dwByte, nullptr);
        ReadFile(hFile, &Desc.fPosition, sizeof(_float4), &dwByte, nullptr);
        ReadFile(hFile, &Desc.fRotation, sizeof(_float4), &dwByte, nullptr);
        ReadFile(hFile, &Desc.fScaling, sizeof(_float3), &dwByte, nullptr);
        ReadFile(hFile, &Desc.fFrustumRadius, sizeof(_float), &dwByte, nullptr);

        Desc.ObjectName = szLoadName;
        Desc.iCurLevel = m_iCurrentLevel;

        CDestructObject* pObject = nullptr;
        if (pObject == nullptr)
        {
            pObject = reinterpret_cast<CDestructObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_DestructObject"), LEVEL_TUTORIAL, TEXT("Layer_DestructObject"), &Desc));
        }
    }

    return S_OK;
}

CLevel_Tutorial* CLevel_Tutorial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLevel_Tutorial* pInstance = new CLevel_Tutorial(pDevice, pContext);

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
    m_pGameInstance->Delete_All_Monster();
    m_pGameInstance->Reset_Effect();
}
