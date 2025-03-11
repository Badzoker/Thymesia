#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CUI_Scene;
//class CUI_Text;
END

BEGIN(Client)

class CUIGroup_Iventory final : public CUIObject
{
public:
	struct UI_Item {

		_uint ItemID = {};
		_uint ItemIconNum = {};
		_uint ItemType = {};
		_tchar* ItemName = {};
		_tchar* ItemDesc = {};
		_uint ItemCount = {};
	};
	enum UIItemType { ITEM_COMMON, ITEM_DIRECT_USE, ITEM_NOUSE, ITEM_ONLYUSE, ITEM_SKILL, ITEM_END };

private:
	CUIGroup_Iventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_Iventory(const CUIGroup_Iventory& Prototype);
	virtual ~CUIGroup_Iventory() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_UIObject();

public:
	HRESULT Change_UI_Tab();
	HRESULT Change_UI_Item_Tab();

public:
	HRESULT Create_Item(_int iKeyNum); // 테스트용 아이템 생성 함수

	HRESULT Item_Get(UI_Item ItemInfo); // 아이템 얻었을 때 컨테이너에 저장
	HRESULT Item_Get2(UI_Item ItemInfo); // 아이템 얻었을 때 컨테이너에 저장
	HRESULT Itme_View_Inventory(vector<UI_Item>& vecContainer); // 인벤토리 슬롯에 가지고 있는 아이템 적재
	//HRESULT InventoryUpdate(); // 인벤토리 슬롯 정보를 갱신 => 아이템을 얻거나 사용, 버릴때에만 실행하도록 할 예정

private:
	_bool m_bCommonOpen = { true };
	_bool m_bSkillOpen = { false };



public:
	HRESULT Slot_Button_Select_Check(vector<UI_Item>& vecContainer);
	HRESULT Slot_Button_MouseOn_Check(vector<UI_Item>& vecContainer);

private:
	_uint m_iMouseOnLastSlot = {}; // 마지막으로 마우스가 가리킨 슬롯의 그룹 아이디


public:
	HRESULT ItemType_PopUP_Open(_uint iItemType, _float3 fSetPos);
	HRESULT ItemType_PopUP_State(_bool bOpen); // 단순 끄고 켜고
	HRESULT ItemType_PopUP_Button(); //팝업에 있는 버튼 눌렀을 때의 처리

	HRESULT ItemUse_PopUP_Open(); //
	HRESULT ItemUse_PopUP_Button();

private:
	_bool m_bItemUsePopOpen = { false }; // 2번째 팝업이 켜진 경우 팝업을 종료하기 전까지는 다른 작업을 하지 못함
	_bool m_bItemTypePopOpen = { false }; // 1번째 팝업이 켜진 경우 슬롯 정보를 다시 받아오지 않음

private:
	CUI_Scene* m_pMyBaseScene = {}; // Base 배경
	CUI_Scene* m_pItemScene = {}; // UI슬롯
	CUI_Scene* m_pItemTypePopUp = {}; // 1 번 팝업
	CUI_Scene* m_pItemUsePopUp = {}; // 2 번 팝업

	vector<UI_TextInfo> m_TextInfo = {};

	vector<UI_Item> m_InvenItem = {}; // 일반 아이템 => 테스트 용으로 만드는거 나중에 아이템 매니저에서 컨테이너를 받아 오는 거임
	vector<UI_Item> m_InvenItemSkill = {}; // 기술의 파편 => 


	_bool m_bInvenCheck = { false }; // 동일 아이템이 있는 지 체크하는 부분
	_bool m_bSkillItemCheck = { false }; // 동일 아이템이 있는 지 체크하는 부분

public:
	HRESULT Ready_MiniView_ItemInfo(); // 반고정 내용 바뀌는 텍스트들 그냥 편하게 가지고 있기
	HRESULT Connect_MiniView_ItemInfo(UI_Item ItemInfo); // 미리 준바 해둔 텍스트 박스 정보에 연결
	HRESULT DisConnect_MiniView_ItemInfo(); // 아이템 선택 없을 시 빈 공간 출력

private:
	CGameObject* m_pItemTextBoxName = {};		//200 이름
	CGameObject* m_pItemTextBoxDesc = {};		//230 설명
	CGameObject* m_pItemTextBoxType = {};		//210 타입 텍스트
	CGameObject* m_pItemTextBoxCount = {};		//220 수량 텍스트
	CGameObject* m_pItemTextBoxCountNum = {};	//221 n/n 설정
	CGameObject* m_pCurrentButton = {};

public:
	static CUIGroup_Iventory* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

