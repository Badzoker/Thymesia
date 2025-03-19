#pragma once

#include "Base.h"
#include "Item.h"

BEGIN(Engine)
class CBase;
class CGameInstance;

class ENGINE_DLL CItemMgr : public CBase
{
private:
	CItemMgr();
	virtual ~CItemMgr() = default;


public:
	HRESULT Initialize();
	void	Update(_float _fTimeDelta);

	HRESULT Add_Item(ITEM_TYPE _eItemType, _uint _iItemCount, CItem* _pGameObject);
	HRESULT Sub_Item(ITEM_TYPE _eItemType, const _wstring& _ItemName);

	void	Set_ItemPos(_fvector _vItemWorldPos);
	HRESULT Acquire_Item(ITEM_TYPE _eItemType);
	HRESULT Acquire_Item2(ITEM_TYPE _eItemType,_uint iCount);
	HRESULT	Drop_Item(ITEM_TYPE _eItemType, _fvector _vDropPosition, class CGameObject* _GameObject);

	_uint Get_Item_Count(ITEM_TYPE _eItemType) { return m_mapItems[_eItemType].first; }
	
	void Item_Save_Info(UI_Item SaveItem); // 현재 아이템을 획득하는 순간 획득 알림을 띄우기 위한 용도
	vector<UI_Item>&	 Get_Item_Save_Info() { return m_vecSaveItem;} // 현재 아이템을 획득하는 순간 획득 알림을 띄우기 위한 용도

	const map<ITEM_TYPE, pair<_uint, vector<CItem*>>> Get_Item_Info() { return m_mapItems; } // 아이템이 저장되는 컨테이너 읽기 전용으로 반환  - 유빈 ui에서 사용
	_bool Use_Item(ITEM_TYPE _eItemType,_uint iNum); // 인벤토리에서 아이템 사용 시 컨테이너에 저장하고 있는 아이템 개수 감소 - 유빈
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	vector<CItem*>* Find_ItemVector(ITEM_TYPE _eItemType);

private:
	map<ITEM_TYPE, pair<_uint, vector<CItem*>>>		m_mapItems;
	vector<UI_Item>									m_vecSaveItem;
	ITEM_TYPE										m_eItemType = { ITEM_TYPE::ITEM_END };
	_uint											m_eItemCount[(_uint)ITEM_TYPE::ITEM_END] = {};
public:
	static CItemMgr* Create();
	virtual void Free() override;

};
END
