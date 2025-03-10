#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	typedef struct MonsterSpawnInfo
	{
		_float4 vMonsterPos;
		_int	iMonsterIndex = { 0 };
	}MONSTERSPAWNINFO;

	enum Monster_Catergory
	{
		NORMAL_VILLAGE_M0,
		NORMAL_VILLAGE_F0,
		ELITE_HARMORLV2,
		NORMAL_VILLAGE_M1,
		ELITE_JOKER,
		BOSS_VARG,
		NORMAL_VILLAGE_F1
	};

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Structure(const _tchar* pLayerTag);	
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);	
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_NPC(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);


	HRESULT Ready_Layer_UIGroup_GameIntro(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerMenu(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerLevelUP(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerTalent(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UIGroup_PlayerScreen(const _tchar* pLayerTag);


	// 테스트용 사다리 레이어 추가
	//HRESULT	Ready_Layer_Ladder(const _tchar* pLayerTag);

private:
	HRESULT Load_Objects(_int iObject_Level);
	HRESULT Load_InstancingObjects(_int iObject_Level);
	HRESULT Load_Height(_int iObject_Level);
	HRESULT Load_MonsterIndex(_int iMonsterIndex_Level);
private:
	_uint m_iOpenSceneCount = { 0 };
	_bool m_bSceneOpen = { false };

	vector<MONSTERSPAWNINFO> m_MonsterSpawnInfos;

public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END