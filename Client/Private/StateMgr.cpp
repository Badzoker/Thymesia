#include "pch.h"
#include "StateMgr.h"
#include "PlayerIdle.h"
#include "PlayerRun.h"

#pragma region ��Ŭ�� ���� 
#include "Attack_LButton_1.h"
#include "Attack_LButton_2.h"
#include "Attack_LButton_3.h"
#pragma endregion 

#pragma region ��Ŭ�� ���� 
#include "Attack_LongClaw_1.h"	
#include "Attack_LongClaw_2.h"	
#pragma endregion	


#pragma region ���¸�� ���� �̵� 
#include "Rock_On_Run_R.h"
#include "Rock_On_Run_L.h"
#include "Rock_On_Run_FR.h"
#include "Rock_On_Run_FL.h"
#include "Rock_On_Run_BR.h"
#include "Rock_On_Run_BL.h"
#include "Rock_On_Run_B.h"
#include "Rock_On_Run_F.h"

#include "LockOn_Evade_B.h"	
#include "LockOn_Evade_L.h"
#include "LockOn_Evade_R.h"
#include "LockOn_Evade_F.h"
#pragma endregion 

#pragma region �и�
#include "Parry_L.h"
#include "Parry_R.h"
#pragma endregion 

#pragma region �ǰ� ���� ��
#include "HurtMFL.h"	
#include "HurtMFR.h"	
#pragma endregion 


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

	/* 2�� Attack ��Ŭ�� ���� ù ��° ��� LButton Ű */
	CAttack_LButton_1* pAttackLButton_1 = CAttack_LButton_1::Create();
	m_vecState.push_back(pAttackLButton_1);

	/* 3�� Attack ��Ŭ�� ���� �� ��° ��� LButton Ű */
	CAttack_LButton_2* pAttackLButton_2 = CAttack_LButton_2::Create();
	m_vecState.push_back(pAttackLButton_2);

	/* 4�� Attack ��Ŭ�� ���� �� ��° ��� LButton Ű */
	CAttack_LButton_3* pAttackLButton_3 = CAttack_LButton_3::Create();
	m_vecState.push_back(pAttackLButton_3);

	/* 5�� Attack ��Ŭ�� ���� ù ��° ��� RButton Enter Ű */
	CAttack_LongClaw_1* pAttack_LongClaw_1 = CAttack_LongClaw_1::Create();
	m_vecState.push_back(pAttack_LongClaw_1);

	/* 6�� Attack ��Ŭ�� ���� ù ��° ��� RButton Enter Ű */
	CAttack_LongClaw_2* pAttack_LongClaw_2 = CAttack_LongClaw_2::Create();
	m_vecState.push_back(pAttack_LongClaw_2);


#pragma region ���� ��� �϶��� �����̵� 

	/* 7�� ���¸���϶� 'D' Ű�� ���� �� ������ �̵� ��� */
	CRock_On_Run_R* pRock_On_Run_R = CRock_On_Run_R::Create();
	m_vecState.push_back(pRock_On_Run_R);

	/* 8�� ���¸���϶� 'A' Ű�� ���� �� ������ �̵� ��� */
	CRock_On_Run_L* pRock_On_Run_L = CRock_On_Run_L::Create();
	m_vecState.push_back(pRock_On_Run_L);


	/* 9�� ���¸���϶� 'W','D' Ű�� ���ÿ�  ���� �� �� �밢�� ������  �̵� ��� */
	CRock_On_Run_FR* pRock_On_Run_FR = CRock_On_Run_FR::Create();
	m_vecState.push_back(pRock_On_Run_FR);

	/* 10�� ���¸���϶� 'W','A' Ű�� ���ÿ�  ���� �� �� �밢�� ���� �̵� ��� */
	CRock_On_Run_FL* pRock_On_Run_FL = CRock_On_Run_FL::Create();
	m_vecState.push_back(pRock_On_Run_FL);

	/* 11�� ���¸���϶� 'S','D' Ű�� ���ÿ�  ���� �� �Ʒ� �밢�� ������ �̵� ��� */
	CRock_On_Run_BR* pRock_On_Run_BR = CRock_On_Run_BR::Create();
	m_vecState.push_back(pRock_On_Run_BR);

	/* 12�� ���¸���϶� 'S','A' Ű�� ���ÿ�  ���� �� �Ʒ� �밢�� ������ �̵� ��� */
	CRock_On_Run_BL* pRock_On_Run_BL = CRock_On_Run_BL::Create();
	m_vecState.push_back(pRock_On_Run_BL);

	/* 13�� ���¸���϶� 'S' Ű�� ���� �� �Ʒ� �밢�� ������ �̵� ��� */
	CRock_On_Run_B* pRock_On_Run_B = CRock_On_Run_B::Create();
	m_vecState.push_back(pRock_On_Run_B);

	/* 14�� ���¸���϶� 'W' Ű�� ���� �� �� �̵� ��� */
	CRock_On_Run_F* pRock_On_Run_F = CRock_On_Run_F::Create();
	m_vecState.push_back(pRock_On_Run_F);

	/* 15�� ���¸���϶� 'SPACE' Ű�� ���� �� �ڷ� ȸ�� ��� */
	CLockOn_Evade_B* LockOn_Evade_B = CLockOn_Evade_B::Create();
	m_vecState.push_back(LockOn_Evade_B);

	/* 16�� ���¸���϶� 'A' Ű�� �����鼭 'SPACE' Ű�� ���� �� ������ ȸ�� ���*/
	CLockOn_Evade_L* LockOn_Evade_L = CLockOn_Evade_L::Create();
	m_vecState.push_back(LockOn_Evade_L);

	/* 17�� ���¸���϶� 'D Ű�� �����鼭'SPACE' Ű�� ���� �� ������ ȸ�� ���*/
	CLockOn_Evade_R* LockOn_Evade_R = CLockOn_Evade_R::Create();
	m_vecState.push_back(LockOn_Evade_R);

	/* 18�� ���¸���϶� 'W' Ű�� �����鼭 'SPACE' Ű�� ���� �� ������ ȸ�� ���*/
	CLockOn_Evade_F* LockOn_Evade_F = CLockOn_Evade_F::Create();
	m_vecState.push_back(LockOn_Evade_F);
#pragma endregion 

#pragma region �и�
	/* 19�� �и� ��� 'F' Ű�� ���� �� ���� ���� ������ �缱���� �ö󰡸鼭 �ϴ� �и� ���*/
	CParry_L* Parry_L = CParry_L::Create();
	m_vecState.push_back(Parry_L);

	/* 20�� �и� ��� 'F' Ű�� ���� �� ������ ���� ���� �缱���� �������鼭 �ϴ� �и� ���*/
	CParry_R* Parry_R = CParry_R::Create();
	m_vecState.push_back(Parry_R);

#pragma endregion 

#pragma region �ǰ�
	/* 21�� �ǰ� ��� (�������� ���� ��û�̸鼭 �ڷΰ��� ��� ) */
	CHurtMFL* pHurtMFL = CHurtMFL::Create();
	m_vecState.push_back(pHurtMFL);

	/* 22�� �ǰ� ��� (���������� ���� ��û�̸鼭 �ڷΰ��� ��� ) */
	CHurtMFR* pHurtMFR = CHurtMFR::Create();
	m_vecState.push_back(pHurtMFR);


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
