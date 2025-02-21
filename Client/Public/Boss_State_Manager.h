#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "Boss_State.h"

BEGIN(Client)
template <typename T>
class CBoss_State_Manager : public CBase
{
public:
	CBoss_State_Manager() = default;
	virtual ~CBoss_State_Manager() = default;

public:
	HRESULT     Initialize()
	{
		return S_OK;
	}

	void State_Update(_float fTimeDelta, T* pObject)
	{
		if (m_pState == nullptr)
			return;
		m_pState->State_Update(fTimeDelta, pObject);
	}

	void        ChangeState(CBoss_State<T>* pState, T* pObject)
	{
		if (m_pState != nullptr)
		{
			m_pState->State_Exit(pObject);
			Safe_Release(m_pState);
		}

		m_pState = pState;
		if (m_pState == nullptr)
			return;

		m_pState->State_Enter(pObject);
	}

private:
	CBoss_State<T>* m_pState = { nullptr };
public:
	static CBoss_State_Manager* Create()
	{
		CBoss_State_Manager* pInstance = new CBoss_State_Manager();

		if (FAILED(pInstance->Initialize()))
		{
			MSG_BOX("Failed To Created : CBoss_State_Manager");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual void Free()
	{
		__super::Free();
		Safe_Release(m_pState);
	}
};

END