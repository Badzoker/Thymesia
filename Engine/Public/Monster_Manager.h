#pragma once

#include "Base.h"
#include "Monster.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CMonster_Manager : public CBase
{
private:
	CMonster_Manager();
	virtual ~CMonster_Manager() = default;

public:
	HRESULT Initialize();
	void	Priority_Update(_float _fTimeDelta);
	void	Update(_float _fTimeDelta);
	void	Late_Update(_float _fTimeDelta);

public:
	HRESULT Add_Monster(_uint _iPrototypeLevelIndex, const _wstring& _strPrototypeTag, MONSTER_CATEGORY _eCategory, void* _pArg);
	deque<class CMonster*>& Get_Check_Monsters() { return m_pCheck_Monsters; }

public:
	HRESULT Active_Monster();
	HRESULT Add_Delete_Monster(class CMonster* pMonster);
	HRESULT Delete_Monster();
private:
	map<MONSTER_CATEGORY, vector<class CMonster*>> m_pMonsters;
	deque<class CMonster*> m_pCheck_Monsters;
	deque<class CMonster*> m_pDelete_Monsters;
	class CGameInstance* m_pGameInstance = { nullptr };
public:
	static CMonster_Manager* Create();
	virtual void Free() override;
};

END