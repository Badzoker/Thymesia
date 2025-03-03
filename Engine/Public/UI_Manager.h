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
	HRESULT Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag, _uint iUIType, void* pArg = nullptr);
	HRESULT UIScene_UIObject_Render_OnOff(CUI_Scene* pScene,_bool bOpen);
	HRESULT LoadDataFile_UIObj_Info(HWND hWnd, _uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadDataFile_UIText_Info(HWND hWnd, const _tchar* szSceneName, vector<UI_TextInfo>& pOut);


private:
	map<const _wstring, class CUI_Scene*>*	m_pScenes = { nullptr };
	_uint									m_iNumScenes = {};

	class CGameInstance*					m_pGameInstance = { nullptr };

public:
	static CUI_Manager* Create(_uint iNumScenes);
	virtual void Free() override;

};

END