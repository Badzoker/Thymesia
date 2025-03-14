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
	HRESULT	Drop_Item(ITEM_TYPE _eItemType, _fvector _vDropPosition, class CGameObject* _GameObject);

	_uint Get_Item_Count(ITEM_TYPE _eItemType) { return m_mapItems[_eItemType].first; }

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	vector<CItem*>* Find_ItemVector(ITEM_TYPE _eItemType);

private:
	map<ITEM_TYPE, pair<_uint, vector<CItem*>>>		m_mapItems;
	ITEM_TYPE										m_eItemType = { ITEM_TYPE::ITEM_END };
	_uint											m_eItemCount[(_uint)ITEM_TYPE::ITEM_END] = {};
public:
	static CItemMgr* Create();
	virtual void Free() override;

};
END
