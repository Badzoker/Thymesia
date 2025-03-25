#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "UI_Frame.h"
BEGIN(Engine)
class CUI_Scene;
class CUI_Text;
END

BEGIN(Client)

class CUIGroup_PlayerTalent final : public CUIObject
{
public:
	enum TABSTATE { TAB_LIGHTATTACK, TAB_PARRY, TAB_DODGE, TAB_CLAW, TAB_FEATHER, TAB_MISC, TAB_END };

private:
	CUIGroup_PlayerTalent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_PlayerTalent(const CUIGroup_PlayerTalent& Prototype);
	virtual ~CUIGroup_PlayerTalent() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);

public:
	void Slot_Setting();
	void Slot_Update_State();
	void Slot_Update_State_Value(SLOTSTATE eSteteNum, CUI_Frame* pSlotUIObj );



private:
	CUI_Scene* m_pMySceneBase = {}; // 기본 공용 
	CUI_Scene* m_pSlot_LightAttack = {}; //검 슬롯

private:
	map<_uint, pair<_bool, CUI_Frame*>> m_mapSlot_LightAttack;



private:
	vector<UI_TextInfo> m_TextInfo = {};

public:
	static CUIGroup_PlayerTalent* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END