#pragma once
#include "Base.h"


BEGIN(Engine)
class CUIObject;

class  CUI_Manager final : public CBase
{
private:
	CUI_Manager();
	virtual ~CUI_Manager() = default;


public:
	HRESULT Initialize(_uint iNumScenes);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

	void Clear(_uint iScenelIndex); // 특정 UI 씬 지우기
	void Clear_Choice(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag, CUIObject* pUIObj); // UI 씬의 특정 오브젝트 삭제
	void Clear_Last(_uint iUIType, _uint iScenelIndex, const _wstring& strSceneTag);; // UI 씬의 마지막 오브젝트 삭제
	void Clear_ALL(); // UI 모든 씬 지우기
	
public:
	class CUI_Scene* Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag);
	map<const _wstring, class CUI_Scene*>* Find_UIScene_ALL();

public:
	// UIScene 에 UIObj 추가 => A라는 씬에 1,2,3 오브젝트 추가 B라는 씬에 2,3,4 오브젝트 추가 => 씬별로 사용할 오브젝트들만 집어 넣는 방식
	HRESULT Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag, _uint iUIType, void* pArg = nullptr);
	
	HRESULT UIScene_UIObject_Render_OnOff(CUI_Scene* pScene,_bool bOpen); // A라는 씬에 있는 모든 UIObj의 랜더(그리기)를 켜고 끄기
	_bool Get_Scene_Render_State(CUI_Scene* pScene); // A라는 씬 자체가 그려지고 있는지 확인
	HRESULT Set_All_UIObject_Condition_Open(CUI_Scene* pScene, _bool bOpen); // UIObj를 그리고 있지만 화면에 끄고 켜고를 설정
	void Find_TextBox_Monster_Memory(CUI_Scene* pScene,_uint iCount); // 몬스터 사망 시 수치 가져오는 부분을 위해...일단 추가 


	HRESULT LoadDataFile_UIObj_Info(HWND hWnd, _uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName); // UI 오브젝트 데이터 로드
	HRESULT LoadDataFile_UIText_Info(HWND hWnd, const _tchar* szSceneName, vector<UI_TextInfo>& pOut); // 텍스트 데이터 로드


private:
	map<const _wstring, class CUI_Scene*>*	m_pScenes = { nullptr };
	_uint									m_iNumScenes = {};

	class CGameInstance*					m_pGameInstance = { nullptr };

public:
	static CUI_Manager* Create(_uint iNumScenes);
	virtual void Free() override;

};

END