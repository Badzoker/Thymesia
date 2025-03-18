#include "ItemMgr.h"
#include "GameInstance.h"

CItemMgr::CItemMgr()
{

}

HRESULT CItemMgr::Initialize()
{
    return S_OK;
}

void CItemMgr::Update(_float _fTimeDelta)
{

}

HRESULT CItemMgr::Add_Item(ITEM_TYPE _eItemType, _uint _iItemCount, CItem* _pGameObject)
{
    if (nullptr == _pGameObject)
        return E_FAIL;

    m_eItemType = _eItemType;

    m_eItemCount[(_uint)_eItemType] = _iItemCount;

    m_mapItems[_eItemType].first = m_eItemCount[(_uint)_eItemType];
    m_mapItems[_eItemType].second.push_back(_pGameObject);

    for (auto& pItems : m_mapItems[_eItemType].second)
        pItems->Set_ItemCount(_iItemCount);

    return S_OK;
}

HRESULT CItemMgr::Sub_Item(ITEM_TYPE _eItemType, const _wstring& _ItemName)
{
    return S_OK;
}

void CItemMgr::Set_ItemPos(_fvector _vItemWorldPos)
{

}

HRESULT CItemMgr::Drop_Item(ITEM_TYPE _eItemType, _fvector _vDropPosition, class CGameObject* _GameObject)
{
    vector<CItem*>* pVecItems = Find_ItemVector(_eItemType);

    if (nullptr == pVecItems)
        return E_FAIL;

    for (auto& pItems : *pVecItems)
    {
        if (nullptr != pItems)
        {
            //pItems->Set_DropItemCount(_iItemCount);
            pItems->Set_BeAcquired(false);
            pItems->Set_BeDropping(true);

            _float4 vDropPosition;
            XMStoreFloat4(&vDropPosition, _vDropPosition);
            pItems->Set_BezierPosition(vDropPosition, _GameObject);
            break;
        }
    }
    //m_mapItems[_eItemType].first -= _iItemCount;
    //m_mapItems[_eItemType].first--;

    return S_OK;
}

HRESULT CItemMgr::Acquire_Item(ITEM_TYPE _eItemType)
{
    vector<CItem*>* pVecItems = Find_ItemVector(_eItemType);

    if (nullptr == pVecItems)
        return E_FAIL;

    for (auto& pItems : *pVecItems)
    {
        pItems->Set_BeAcquired(true);
    }

    //m_mapItems[_eItemType].first += _iItemCount;
    m_mapItems[_eItemType].first++;

    return S_OK;
}

HRESULT CItemMgr::Acquire_Item2(ITEM_TYPE _eItemType, _uint iCount)
{
    vector<CItem*>* pVecItems = Find_ItemVector(_eItemType);

    if (nullptr == pVecItems)
        return E_FAIL;

    for (auto& pItems : *pVecItems)
    {
        pItems->Set_BeAcquired(true);
    }

    m_mapItems[_eItemType].first += iCount;

    return S_OK;
}

_bool CItemMgr::Use_Item(ITEM_TYPE _eItemType, _uint iNum)
{
    for (auto& ItemBox : m_mapItems)
    {
        if (_eItemType == ItemBox.first)
        {
            ItemBox.second.first -= iNum;
            if (0 > ItemBox.second.first)
                return true; // 이 아이템이 0개가 되었는지 체크
        }
    }
    return false;
}

vector<CItem*>* CItemMgr::Find_ItemVector(ITEM_TYPE _eItemType)
{
    auto iter = m_mapItems.find(_eItemType);
    if (iter == m_mapItems.end())
        return nullptr;

    return &iter->second.second;
}

CItemMgr* CItemMgr::Create()
{
    CItemMgr* pInstance = new CItemMgr();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : ItemMgr");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItemMgr::Free()
{
    __super::Free();
}
