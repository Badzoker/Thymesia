#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;

class CUI_Scene final : public CBase
{
public:
	enum UITYPE
	{
		UI_BUTTON,
		UI_BUTTONPLAYER,
		UI_IMAGE,
		UI_TEXTPLAYER,
		UI_END
	};
private:
	CUI_Scene();
	virtual ~CUI_Scene() = default;

public:
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

public:
	HRESULT Add_UIObject_Type(_int iUIType, CGameObject* pUIObj);
	HRESULT UIScene_UIObject_Render_OnOff(_bool bOpen);

	vector<class CUI_Button*> Find_UI_Button() { return m_Button; }
	vector<class CUI_Button_Player*> Find_UI_Button_Player() { return m_ButtonPlayer; }
	vector<class CUI_Image*> Find_UI_Image() { return m_Image; }
	vector<class CUI_Text_PlayerInfo*> Find_UI_Text_PlayerInfo() { return m_TextPlayerInfo; }



private:
	vector<class CUI_Button*>			m_Button;
	vector<class CUI_Button_Player*>	m_ButtonPlayer;
	vector<class CUI_Image*>			m_Image;
	vector<class CUI_Text_PlayerInfo*>	m_TextPlayerInfo;

public:
	static CUI_Scene* Create();
	virtual void Free() override;
};

END