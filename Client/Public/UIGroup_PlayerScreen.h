#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Engine)
class CUI_Scene;
class CUI_Text;
END


BEGIN(Client)

class CUIGroup_PlayerScreen final : public CUIObject
{
private:
	CUIGroup_PlayerScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_PlayerScreen(const CUIGroup_PlayerScreen& Prototype);
	virtual ~CUIGroup_PlayerScreen() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Player_Info_GageBar();
	void Item_Save_Info(ITEM_TYPE eItemType); // 아이템 획득 시 출력되는 알림
	void Item_Drop_Info(ITEM_TYPE eItemType); // 아이템 버릴 시 출력되는 알림
	
	void Item_In_Out_Pop();

public:
	void UI_Direction_Monster_MemoryGet();
	void UI_Direction_Item_Nudge();

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);

private:
	CUI_Scene* m_pMyScene = {}; //플레이어 화면
	CUI_Scene* m_pItmeScreen = {}; //아이템 획득, 버리기 팝업

	class CGameObject* m_pPlayer = { nullptr };
	CGameObject* m_pGroupInven = { nullptr };

	_bool m_bNudgeUse[4] = { false,false,false,false };

	LEVELID m_eLevelID = {};

private:
	_float m_fTimeCheck = {};
	vector<UI_TextInfo> m_TextInfo = {};
public:
	void Set_m_bDrop(_bool bdrop) { m_bDrop = bdrop; }
private:
	_bool m_bDrop = { true };
	CUI_Text* m_pMonsterText = {};
	_float m_fMonsterTextOnTime = {};

public:
	static CUIGroup_PlayerScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

