#include "pch.h"
#include "PlayerRun.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CPlayerRun::CPlayerRun()
{
}

HRESULT CPlayerRun::Initialize()
{
	return S_OK;
}

void CPlayerRun::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

	_vector vLook = XMVector3Normalize(XMVector3Cross(CamRight, vUp)); // 외적을 하면 각이 2개 생기긴 하는데 흠.. 이건 나중에 고민하기. 
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

	if ((GetKeyState('W') & 0x8000))
	{
		// vLook과 캐릭터의 look 의 내적구하기 
		//_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));	

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		//내적의 각도 구하기


		if (!(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
		{
			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);

			if (abs(Radian) <= 0.5f)
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);

			else
			{
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * 0.1f, pNavigation);
			}
		}
		else
		{
			if (m_pGameInstance->isKeyEnter(DIK_A))
				pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // 이러면 안되고 카메라기준이네

			if (GetKeyState('A') & 0x8000)
			{
				pGameObject->Get_Transfrom()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);
			}
			if (m_pGameInstance->isKeyEnter(DIK_D))
				pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f));

			if (GetKeyState('D') & 0x8000)
			{
				pGameObject->Get_Transfrom()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);
			}
		}

		//pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);

		/* 사운드 관련 */

	}

	if ((GetKeyState('S') & 0x8000))
	{
		_vector vLook = XMVector3Normalize(XMVector3Cross(vUp, CamRight)); // 외적을 하면 각이 2개 생기긴 하는데 흠.. 이건 나중에 고민하기. 

		// vLook과 캐릭터의 look 의 내적구하기 
		//_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		//내적의 각도 구하기

		if (!(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
		{
			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);

			if (abs(Radian) <= 0.5f)
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);

			else
			{
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * 0.1f, pNavigation);
			}
		}

		else
		{
			if (m_pGameInstance->isKeyEnter(DIK_A))
				pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // 이러면 안되고 카메라기준이네

			if (GetKeyState('A') & 0x8000)
			{
				pGameObject->Get_Transfrom()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);
			}

			if (m_pGameInstance->isKeyEnter(DIK_D))
				pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));

			if (GetKeyState('D') & 0x8000)
			{
				pGameObject->Get_Transfrom()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);
			}
		}

	}



	if (!(GetKeyState('W') & 0x8000)
		&& !(GetKeyState('S') & 0x8000)
		&& (GetKeyState('A') & 0x8000))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));		

		// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
		float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamLeft, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), -Radian * m_fTurnSpeed * fTimeDelta);


		if (abs(Radian) <= 0.5f)
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);

		else
		{
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * 0.1f, pNavigation);
		}


	}

	if (m_pGameInstance->isKeyPressed(DIK_A)
		&& m_pGameInstance->isKeyEnter(DIK_W))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));	

		// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
		float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamLeft, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // 이러면 안되고 카메라기준이네	

	}

	if (m_pGameInstance->isKeyPressed(DIK_A)
		&& m_pGameInstance->isKeyEnter(DIK_S)
		&& (!(GetKeyState('D') & 0x8000)))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));


		// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
		float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamLeft, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // 이러면 안되고 카메라기준이네	


	}



	if (!(GetKeyState('W') & 0x8000) &&
		!(GetKeyState('S') & 0x8000) &&
		(GetKeyState('D') & 0x8000))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));


		/* 음과 양수를 판별*/


		float dotResult = XMVectorGetX(XMVector3Dot(CamRight, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, CamRight);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);


		if (abs(Radian) <= 0.5f)
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);

		else
		{
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * 0.1f, pNavigation);
		}


	}

	if (m_pGameInstance->isKeyPressed(DIK_D)
		&& m_pGameInstance->isKeyEnter(DIK_W))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));	



		// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
		float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, CamRight));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamRight, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // 이러면 안되고 카메라기준이네	

	}


	if (m_pGameInstance->isKeyPressed(DIK_D)
		&& m_pGameInstance->isKeyEnter(DIK_S))
	{

		// 지금 문제가 캐릭터가 이동할때 방향이 바뀌니깐 앞 뒤 상태는 줘야할거같은데 
		float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, CamRight));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamRight, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // 이러면 안되고 카메라기준이네	

	}
}

void CPlayerRun::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CPlayerRun::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CPlayerRun* CPlayerRun::Create()
{
	CPlayerRun* pInstance = new CPlayerRun();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : PlayerRun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerRun::Free()
{
	__super::Free();
}
