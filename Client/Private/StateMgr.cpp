#include "pch.h"
#include "StateMgr.h"
#include "PlayerIdle.h"
#include "PlayerRun.h"
#include "Attack_LButton_1.h"

CStateMgr::CStateMgr()
{

}

HRESULT CStateMgr::Initialize()
{
	m_vecState.reserve(30);


	/* 0�� Idle ���� */
	CPlayerIdle* pStateIdle = CPlayerIdle::Create();
	m_vecState.push_back(pStateIdle);


	/* 1�� Run ������ �ٴ� ���� �ִϸ��̼�  W Ű */
	CPlayerRun* pStateRun = CPlayerRun::Create();
	m_vecState.push_back(pStateRun);

	/* 2�� Attack ��Ŭ�� ���� ù��° ��� LButton Ű */
	CAttack_LButton_1* pAttackLButton_1 = CAttack_LButton_1::Create();
	m_vecState.push_back(pAttackLButton_1);

	return S_OK;
}

CStateMgr* CStateMgr::Create()
{
	CStateMgr* pInstance = new CStateMgr();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CStateMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateMgr::Free()
{
	__super::Free();

	for (auto& iter : m_vecState)
	{
		Safe_Release(iter);
	}

	m_vecState.clear();

}
