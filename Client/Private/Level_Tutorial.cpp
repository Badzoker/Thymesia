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

#include "UI_LeftBackground.h"

#include "Button.h"
#include "LockLine.h"
#include "GameItem.h"
#include "DeadBranch.h"

#include "Effect_Mesh.h"
#include "Effect_Sword.h"
#include "Effect_Particle.h"

#include "Terrain.h"


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
    //현재 몬스터와 기본맵이 있는 Prototype용 맵							-> Load_Objects(16);
    //Map Tool 기능 및 Test용 맵											-> Load_Objects(87);
    //튜토리얼 맵 수정 중 (크기 조절 중 98번 맵파일은 잠시 봉인합니다.		-> Load_Objects(107);
    // 
    //Load_Objects(140); //Tutorial Map
    //Load_Objects(142); //Tutorial Map
    //if (FAILED(Load_Objects(145)))
    if (FAILED(Load_Objects(151)))
        return E_FAIL;//Tutorial Map
    //Load_Objects(301); //Circus Map
    //Load_Objects(303); //Circus Map


    //Load_TriggerObjects(0);			// 원래 의자 쪽에 있었던 트리거 오브젝트 파일
    Load_TriggerObjects(2);				// 이제 보스 입구 쪽에 심어져있는 파일임.

    Load_SpecificObjects(12);

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

    //_vector vTestPosition = { 111.64f, 15.88f, -41.30f, 1.f };
    //XMStoreFloat4(&pDesc.fPosition, vTestPosition);
    //for (_uint i = 0; i < 20; i++)
    //{
    //	if (FAILED(m_pGameInstance->Add_Projectile(LEVEL_STATIC, TEXT("Prototype_GameObject_Projectile_Card"), PROJECTILE_CARD, &pDesc)))
    //		return E_FAIL;
    //}
    //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Magician"), CATEGORY_BOSS, &pDesc)))
    //	return E_FAIL;

    //pDesc.fPosition = m_MonsterSpawnInfos[0].vMonsterPos;
    //if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_STATIC, TEXT("Prototype_GameObject_Building_Circus_Balloon"), LEVEL_TUTORIAL, TEXT("Layer_Monster_Building"), &pDesc)))
    //    return E_FAIL;


    for (size_t i = 0; i < m_MonsterSpawnInfos.size(); i++)
    {
        switch (m_MonsterSpawnInfos[i].iMonsterIndex)
        {
        case BOSS_VARG:
            //pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Varg"), CATEGORY_BOSS, &pDesc)))
            //    return E_FAIL;
            pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Boss_Urd"), CATEGORY_BOSS, &pDesc)))
                return E_FAIL;
            break;
        default:
            break;
        case ELITE_JOKER:
            pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Joker"), CATEGORY_ELITE, &pDesc)))
                return E_FAIL;
            //pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Punch_Man"), CATEGORY_ELITE, &pDesc)))
            //    return E_FAIL;
            break;
        case ELITE_HARMORLV2:
             pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_HArmorLV2"), CATEGORY_ELITE, &pDesc)))
                return E_FAIL;
            //pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            //if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Elite_Grace"), CATEGORY_ELITE, &pDesc)))
            //    return E_FAIL;
            break;

        case NORMAL_VILLAGE_M0:
            pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
            for (_uint i = 0; i < 2; i++)
            {
                //if (i == 0)
                //{
                //    if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM0"), CATEGORY_NORMAL, &pDesc)))
                //        return E_FAIL;
                //}
                //else
                //{
                //    if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_ScytheM"), CATEGORY_NORMAL, &pDesc)))
                //        return E_FAIL;
                //}
            }
            break;
        //case NORMAL_VILLAGE_M1:
        //    pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
        //    if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageM1"), CATEGORY_NORMAL, &pDesc)))
        //        return E_FAIL;
        //
        //    break;
        //case NORMAL_VILLAGE_F0:
        //    pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
        //    if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF0"), CATEGORY_NORMAL, &pDesc)))
        //        return E_FAIL;
        //    break;
        //case NORMAL_VILLAGE_F1:
        //    pDesc.fPosition = m_MonsterSpawnInfos[i].vMonsterPos;
        //    if (FAILED(m_pGameInstance->Add_Monster(LEVEL_STATIC, TEXT("Prototype_GameObject_Normal_VillageF1"), CATEGORY_NORMAL, &pDesc)))
        //        return E_FAIL;
        //    break;
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
        EFFECT_TYPE::EFFECT_TYPE_MESH, EFFECT_NAME::EFFECT_PLAYER_AXE)))
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
        EFFECT_TYPE::EFFECT_TYPE_PARTICLE, EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET, 2)))
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
        //else if (Desc.ObjectName == "Ladder")
        //{
        //	pObject = reinterpret_cast<CLadder*>(
        //		m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_Ladder"),
        //			LEVEL_TUTORIAL, TEXT("Layer_SpecificObject"), &Desc));
        //}
        //else
        //{
        //	// 디폴트: 그냥 SpecificObject로 로드
        //	pObject = reinterpret_cast<CSpecificObject*>(
        //		m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_STATIC, TEXT("Prototype_GameObject_SpecificObject"),
        //			LEVEL_TUTORIAL, TEXT("Layer_SpecificObject"), &Desc));
        //}
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
        case 13:
            pDesc.szModelName = TEXT("Prototype_Component_Model_Effect_HealingCurve");
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

        _uint iParticle_Function = {}; //약간 Tool 용도라서 미리 빼두는 느낌(본 Project 에선 필요없을듯)

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
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_VERTICAL_DUST:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Vertical_Dust");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_HORIZON_DUST_VARG:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Horizon_Dust_Varg");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_LEFT_DUST_VARG:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Left_Dust_Varg");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_NARROW_DUST_VARG:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Narrow_Dust_Varg");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_RIGHT_DUST_VARG:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Right_Dust_Varg");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_NARROW_SPARK_VARG:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Narrow_Spark_Varg");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_HORIZON_SPARK_VARG:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Horizon_Spark_Varg");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_VARG_EXECUTION_1_2:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Varg_Execution_1_2");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_VARG_EXECUTION3:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Varg_Execution3");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_VARG_EXECUTION1:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Blood_Varg_Execution1");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_VARG_EXECUTION2:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Blood_Varg_Execution2");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_BLOOD_VARG_EXECUTION3:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Blood");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Varg_Execution3");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_VARG_DEAD:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_Varg_Dead");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_VARG_DEAD:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Varg_Dead");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_INTRO:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Delay");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_DustDelay_Mutation_Intro");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SCYTHE_PLAYER_SCYTHE_1:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Scythe");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Scythe_Player_Scythe_1");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SCYTHE_PLAYER_SCYTHE_2:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Scythe");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Scythe_Player_Scythe_2");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_PLAYER_HALBERD_1:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Delay");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Scythe_Player_Halberd_1");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_PLAYER_HALBERD_2:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Delay");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Scythe_Player_Halberd_2");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_HEAL:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Hurricane");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Hurricane_Heal");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_ITEM_GET:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Hurricane");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Hurricane_Item_Get");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_BURST:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Delay");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_DustDelay_Mutation_Burst");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_EXPLOSION_MUTATION_BURST:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Explosion");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Explosion_Mutation_Burst");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_MUTATION_IMPACT:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Mutation_Impact");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_RISING_IMPACT:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Hurricane");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Hurricane_Mutation_Rising_Impact");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_HURRICANE_MUTATION_FALLING_IMPACT:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Hurricane");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Hurricane_Mutation_Falling_Impact");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_EXPLOSION_MUTATION_STAB:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Explosion");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Explosion_Mutation_Stab");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_MUTATION_STAB_DUST:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Delay");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_DustDelay_Mutation_Stab_Dust");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_MUTATION:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Mutation");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_HORIZON:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Horizon");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_LEFT_STAB:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Left_Stab");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_VERTICAL:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Vertical");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_HORIZON:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_Horizon");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_NARROW:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_Narrow");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_JOKER_INTRO:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_Joker_Intro");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_JOKER_INTRO:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Joker_Intro");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_LEFT:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_Left");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_RIGHT:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_Right");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_JOKER_SMASH:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_Joker_Smash");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_SMASH:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_Smash");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_SHOCKWAVE:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_ShockWave");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUSTDELAY_SHOCKWAVE:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust_Delay");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_DustDelay_ShockWave");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SCYTHE_WHEELATTACK:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Scythe");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Scythe_WheelAttack");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_SPARK_PLAYER_AXE:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Spark");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Spark_PlayerAxe");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_PLAYER_AXE_HORIZON:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_PlayerAxe");
            break;
        case Engine::EFFECT_NAME::EFFECT_PARTICLE_DUST_PLAYER_AXE_CROSS:
            pDesc.szShaderName = TEXT("Prototype_Component_Shader_VtxPointInstance_Compute_Dust");
            pDesc.szBufferName = TEXT("Prototype_Component_VIBuffer_Point_Compute_Dust_PlayerAxe_Cross");
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
