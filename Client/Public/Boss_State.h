#pragma once
#include "Client_Defines.h"	
#include "Base.h"


BEGIN(Client)

template <typename T>
class CBoss_State abstract : public CBase
{
protected:
	CBoss_State() = default;
	virtual ~CBoss_State() = default;

public:
	virtual void State_Enter(T* pObject) = 0;	 // 누구의 스테이트인지 알기 위해 게임 오브젝트 넣어야할듯
	virtual void State_Update(_float fTimeDelta, T* pObject) = 0;
	virtual void State_Exit(T* pObject) = 0;
protected:
	_uint m_iIndex = {};
public:
	virtual void Free()
	{
		__super::Free();
	}
};

END
