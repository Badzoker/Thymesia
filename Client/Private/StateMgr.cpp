#include "pch.h"
#include "StateMgr.h"
#include "PlayerIdle.h"
#include "PlayerRun.h"

#pragma region 좌클릭 공격 
#include "Attack_LButton_1.h"
#include "Attack_LButton_2.h"
#include "Attack_LButton_3.h"
#pragma endregion 

#pragma region 우클릭 공격 
#include "Attack_LongClaw_1.h"	
#include "Attack_LongClaw_2.h"	
#pragma endregion	


#pragma region 락온모드 방향 이동 
#include "Rock_On_Run_R.h"
#include "Rock_On_Run_L.h"
#include "Rock_On_Run_FR.h"
#include "Rock_On_Run_FL.h"
#include "Rock_On_Run_BR.h"
#include "Rock_On_Run_BL.h"
#include "Rock_On_Run_B.h"
#pragma endregion 


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

	/* 2번 Attack 좌클릭 공격 첫 번째 모션 LButton 키 */
	CAttack_LButton_1* pAttackLButton_1 = CAttack_LButton_1::Create();
	m_vecState.push_back(pAttackLButton_1);

	/* 3번 Attack 좌클릭 공격 두 번째 모션 LButton 키 */
	CAttack_LButton_2* pAttackLButton_2 = CAttack_LButton_2::Create();
	m_vecState.push_back(pAttackLButton_2);

	/* 4번 Attack 좌클릭 공격 세 번째 모션 LButton 키 */
	CAttack_LButton_3* pAttackLButton_3 = CAttack_LButton_3::Create();
	m_vecState.push_back(pAttackLButton_3);

	/* 5번 Attack 우클릭 공격 첫 번째 모션 RButton Enter 키 */
	CAttack_LongClaw_1* pAttack_LongClaw_1 = CAttack_LongClaw_1::Create();
	m_vecState.push_back(pAttack_LongClaw_1);

	/* 6번 Attack 우클릭 공격 첫 번째 모션 RButton Enter 키 */
	CAttack_LongClaw_2* pAttack_LongClaw_2 = CAttack_LongClaw_2::Create();
	m_vecState.push_back(pAttack_LongClaw_2);


#pragma region 락온 모드 일때의 방향이동 

	/* 7번 락온모드일때 'D' 키만 누를 시 오른쪽 이동 모션 */
	CRock_On_Run_R* pRock_On_Run_R = CRock_On_Run_R::Create();
	m_vecState.push_back(pRock_On_Run_R);

	/* 8번 락온모드일때 'A' 키만 누를 시 오른쪽 이동 모션 */
	CRock_On_Run_L* pRock_On_Run_L = CRock_On_Run_L::Create();
	m_vecState.push_back(pRock_On_Run_L);


	/* 9번 락온모드일때 'W','D' 키를 동시에  누를 시 위 대각선 오른쪽  이동 모션 */
	CRock_On_Run_FR* pRock_On_Run_FR = CRock_On_Run_FR::Create();
	m_vecState.push_back(pRock_On_Run_FR);

	/* 10번 락온모드일때 'W','A' 키를 동시에  누를 시 위 대각선 왼쪽 이동 모션 */
	CRock_On_Run_FL* pRock_On_Run_FL = CRock_On_Run_FL::Create();
	m_vecState.push_back(pRock_On_Run_FL);

	/* 11번 락온모드일때 'S','D' 키를 동시에  누를 시 아래 대각선 오른쪽 이동 모션 */
	CRock_On_Run_BR* pRock_On_Run_BR = CRock_On_Run_BR::Create();
	m_vecState.push_back(pRock_On_Run_BR);

	/* 12번 락온모드일때 'S','A' 키를 동시에  누를 시 아래 대각선 오른쪽 이동 모션 */
	CRock_On_Run_BL* pRock_On_Run_BL = CRock_On_Run_BL::Create();
	m_vecState.push_back(pRock_On_Run_BL);

	/* 13번 락온모드일때 'S' 키를 누를 시 아래 대각선 오른쪽 이동 모션 */
	CRock_On_Run_B* pRock_On_Run_B = CRock_On_Run_B::Create();
	m_vecState.push_back(pRock_On_Run_B);

#pragma endregion 




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
