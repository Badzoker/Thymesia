#include "pch.h" 
#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "Layer.h"	

#include "Object.h"
#include "EnvironmentObject.h"

#include "UI_LeftBackground.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel { pDevice, pContext }
{
	
}

HRESULT CLevel_GamePlay::Initialize()
{
	/*if (FAILED(Ready_Lights()))
		return E_FAIL;*/

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Structure(TEXT("Layer_Structure"))))
		return E_FAIL;	


	// 테스트용 사다리 레이어 준비 함수 호출
	//if (FAILED(Ready_Layer_Ladder(TEXT("Layer_Ladder"))))
	//	return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))	
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

	if (FAILED(Ready_Lights()))	
		return E_FAIL;	

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	if (m_pGameInstance->isKeyEnter(DIK_E))
	{
		m_iOpenSceneCount++;
		if (1 == m_iOpenSceneCount) // 게임 인트로
		{
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_GameIntro"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), true);
		}
		if (2 == m_iOpenSceneCount) // 플레이어 메뉴
		{
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_GameIntro"), false);
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerMenu"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_INTRO, L"UIScene_Intro")), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), true);
		}
		if (3 == m_iOpenSceneCount) // 플레이어 레벨 업
		{
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerMenu"), false);
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerLevelUP"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_MENU, L"UIScene_PlayerMenu")), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), true);
		}
		if (4 == m_iOpenSceneCount) // 플레이어 특성
		{
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerLevelUP"), false);
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerTalent"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent")), true);
		}
		if (5 == m_iOpenSceneCount) // 플레이어 화면
		{
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerTalent"), false);
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerScreen"), true);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_TALENT, L"UIScene_PlayerTalent")), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), true);
		}
		if (6 == m_iOpenSceneCount) // 플레이어 화면
		{
			m_iOpenSceneCount = 0;
			m_pGameInstance->UIScene_Render_OnOff(LEVEL_GAMEPLAY, TEXT("Layer_PlayerScreen"), false);
			m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_PLAYERSCREEN, L"UIScene_PlayerScreen")), false);
		}
		

	}
}

HRESULT CLevel_GamePlay::Render() 
{
#ifdef _DEBUG
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
#endif

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	CTransform* pCamTransform = static_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

	_matrix matView = XMLoadFloat4x4(&m_pGameInstance->Get_Transform_Float4x4_Inverse(CPipeLine::D3DTS_VIEW));
	_vector vCamInfo = { XMConvertToRadians(60.f), 16.f / 9.f , 0.1f, 1000.f };

	if (FAILED(m_pGameInstance->SetUp_ShadowLight(XMVectorSet(111.827f, 0.0f, 108.493f, 1.0f), XMVectorSet(0.f, -1.f, 0.f, 1.f),
		XMConvertToRadians(60.f), g_iWinSizeX / static_cast<_float>(g_iWinSizeY), 0.1f, 1000.f
		, matView, vCamInfo, pCamTransform)))
		return E_FAIL;

	LIGHT_DESC LightDesc{};
	/* 2월 8일 빛 */
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);
	LightDesc.vDiffuse   = _float4(0.9f, 0.9f, 0.9f, 1.f);
	LightDesc.vAmbient   = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vSpecular  = _float4(0.3f, 0.3f, 0.3f, 1.f);


	/*LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;	
	LightDesc.vDirection = _float4(1.f, 1.f, 1.f, 0.f);	
	LightDesc.vDiffuse   = _float4(0.9f, 0.9f, 0.9f, 1.f);	
	LightDesc.vAmbient   = _float4(0.5f, 0.5f, 0.5f, 1.f);	
	LightDesc.vSpecular  = _float4(0.3f, 0.3f, 0.3f, 1.f);	*/

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	//for (size_t i = 0; i < 3; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ForkLift"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
	//		return E_FAIL;
	//}

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;



	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Structure(const _tchar* pLayerTag)
{
	//현재 몬스터와 기본맵이 있는 Prototype용 맵							-> Load_Objects(16);
	//Map Tool 기능 및 Test용 맵											-> Load_Objects(87);
	//튜토리얼 맵 수정 중 (크기 조절 중 98번 맵파일은 잠시 봉인합니다.		-> Load_Objects(107);
	// 
	//Load_Objects(107);
	//Load_Objects(98);
	Load_Objects(129);
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
	//	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, Translate_wchar, LEVEL_GAMEPLAY, Layer_Name)))
	//		return E_FAIL;

	//	//CTransform* pTrasnform = dynamic_cast<CTransform*>(
	//	//* Find GaemObject 만들어야 할듯
	//	// 구분할 수 있는 방법을 생각해봐야할듯.
	//	map<const _wstring, class CLayer*>* Level_Layers = m_pGameInstance->Get_Layers();		

	//	auto& Level_GamePlay = Level_Layers[3];

	//	for (auto& Layers : Level_GamePlay)
	//	{
	//		//auto& iter = find(Level_GamePlay.begin(), Level_GamePlay.end(), Layer_Name);	
	//		auto iter = Level_GamePlay.find(Layer_Name);

	//		if (iter == Level_GamePlay.end())
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

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))	
		return E_FAIL;	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CCamera_Free::FREE_CAMERA_DESC		Desc = {};

	
	Desc.vEye = _float3(0.f, 10.f, -7.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);

	Desc.fFovy = XMConvertToRadians(60.f);
	Desc.fNear = 0.1f;
	Desc.fFar = 800.f;
	Desc.fMouseSensor = 0.05f;
	Desc.fSpeedPerSec = 25.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"), LEVEL_GAMEPLAY, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Magician"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
	//	return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Elite_Joker"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;
	
	
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageM0"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageM1"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageF0"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Normal_VillageF1"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar* pLayerTag)
{

	//if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Compute_Example"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
	//	return E_FAIL;
	

	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_GameIntro"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerMenu"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerLevelUP"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerTalent"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UIGroup_PlayerScreen"), LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_GamePlay::Load_Objects(_int iObject_Level)
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
		ReadFile(hFile, &Desc.fRotation, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fScaling, sizeof(_float3), &dwByte, nullptr);
		ReadFile(hFile, &Desc.fFrustumRadius, sizeof(_float), &dwByte, nullptr);

		Desc.ObjectName = szLoadName;

		if(FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Object_StaticObject"), LEVEL_GAMEPLAY, TEXT("Layer_Object"), &Desc)))
			return E_FAIL;
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

		if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Object_GroundObject"), LEVEL_GAMEPLAY, TEXT("Layer_GroundObject"), &Desc)))
			return E_FAIL;
	}*/

	for (_uint i = 0; i < iSize2; ++i)
	{
		CEnvironmentObject::ENVIRONMENT_OBJECT_DESC Desc = {};
		_char szLoadName[MAX_PATH] = {};

		ReadFile(hFile, szLoadName, MAX_PATH, &dwByte2, nullptr);
		Desc.ObjectName = szLoadName;

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

		CEnvironmentObject* pEnvironment = reinterpret_cast<CEnvironmentObject*>(m_pGameInstance->Add_GameObject_To_Layer_Take(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Object_GroundObject"),LEVEL_GAMEPLAY, TEXT("Layer_GroundObject"), &Desc));

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

HRESULT CLevel_GamePlay::Load_InstancingObjects(_int iObject_Level)
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

HRESULT CLevel_GamePlay::Load_Height(_int iObject_Level)
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

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_GamePlay::Free()
{
	__super::Free();

}
