#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "UI_Skill_Slot.h"
BEGIN(Engine)
class CUI_Scene;
END

BEGIN(Client)

class CUIGroup_Skill final : public CUIObject
{
private:
	CUIGroup_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_Skill(const CUIGroup_Skill& Prototype);
	virtual ~CUIGroup_Skill() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Slot_Update_State_Value(SkILL_SLOTSTATE eSteteNum, CUI_Skill_Slot* pSlotUIObj);

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadData_UIText_Info(const _tchar* szSceneName);
private:
	vector<UI_TextInfo> m_TextInfo = {};

	CUI_Scene* m_pBaseScene = {};
	CUI_Scene* m_pEquipWeapon = {};
	CUI_Scene* m_pEquipCondition = {};

public:
	static CUIGroup_Skill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
