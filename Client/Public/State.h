#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CNavigation;
END

class CState abstract : public CBase
{
protected:
	CState();
	virtual ~CState() = default;


public:
	HRESULT Initialize();

	virtual void Priority_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta);
	virtual void Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta);
	virtual void Late_Update(class CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta);
	virtual void Set_MonsterLookDir(_float4 _vMonsterLookDir) { m_vMonsterLookDir = _vMonsterLookDir; }

protected:
	CGameInstance* m_pGameInstance = { nullptr };
	_float4		   m_vMonsterLookDir = {};


public:
	virtual void Free();

};
