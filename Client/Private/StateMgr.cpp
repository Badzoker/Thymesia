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


	/* 0번 Idle 상태 */
	CPlayerIdle* pStateIdle = CPlayerIdle::Create();
	m_vecState.push_back(pStateIdle);


	/* 1번 Run 앞으로 뛰는 상태 애니메이션  W 키 */
	CPlayerRun* pStateRun = CPlayerRun::Create();
	m_vecState.push_back(pStateRun);

	/* 2번 Attack 좌클릭 공격 첫번째 모션 LButton 키 */
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
