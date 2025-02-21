#pragma once
#include "Base.h"


BEGIN(Engine)

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

	void Clear(_uint iScenelIndex); // UI ¾À Áö¿ì±â
	void Clear_ALL(); // UI ¸ðµç ¾À Áö¿ì±â
	
public:
	class CUI_Scene* Find_UIScene(_uint iSceneIndex, const _wstring& strSceneTag);
	map<const _wstring, class CUI_Scene*>* Find_UIScene_ALL();

public:
	HRESULT Add_UIObject_To_UIScene(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iSceneIndex, const _wstring& strSceneTag, _uint iUIType, void* pArg = nullptr);
	HRESULT UIScene_UIObject_Render_OnOff(CUI_Scene* pScene,_bool bOpen);
private:
	map<const _wstring, class CUI_Scene*>*	m_pScenes = { nullptr };
	_uint									m_iNumScenes = {};

	class CGameInstance*					m_pGameInstance = { nullptr };

public:
	static CUI_Manager* Create(_uint iNumScenes);
	virtual void Free() override;

};

END