#include "pch.h" 
#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"
#include "RightWeapon.h"
#include "LeftWeapon.h"
#include "StateMgr.h"
#include "Animation.h"
#include "ClawWeapon.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	:CContainerObject(Prototype)
	, m_iState(Prototype.m_iState)
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	strcpy_s(m_szName, "PLAYER");

	CGameObject::GAMEOBJECT_DESC        Desc{};

	Desc.fSpeedPerSec = 45.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);


	m_pStateMgr = CStateMgr::Create();
	if (m_pStateMgr == nullptr)
	{
		MSG_BOX("Failed to Created : StateMgr");
	}

	m_pActor = m_pGameInstance->Create_Actor(COLLIDER_TYPE::COLLIDER_CAPSULE, _float3{ 0.2f,0.2f,0.15f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,0.f,1.f });
	_uint settingColliderGroup = GROUP_TYPE::MONSTER | GROUP_TYPE::MONSTER_WEAPON;
	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::PLAYER, settingColliderGroup);
	m_pGameInstance->Add_Actor_Scene(m_pActor);


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;


	// 시작 지점의 플레이어 위치 1_23일 
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 111.7f, 15.3f, 51.5f, 1.0f });

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 111.7f, 15.3f, 51.5f, 1.0f });
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 70.7f, 1.3f, -110.5f, 1.0f });


	//m_pGameInstance->Add_ObjCollider(GROUP_TYPE::PLAYER, this);

	m_pTransformCom->Scaling(_float3{ 0.002f, 0.002f, 0.002f });


	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
#pragma region Mouse_Input
	/* 마우스 입력키는 (LB, RB ) 동시 입력을 불가능 하도록 설정 */
	Mouse_section(fTimeDelta);
#pragma endregion 

#pragma region KeyBoard Input
	Keyboard_section(fTimeDelta);
#pragma endregion 

	__super::Priority_Update(fTimeDelta);
}

void CPlayer::Mouse_section(_float fTimeDelta)
{
	if (m_pGameInstance->isMouseEnter(DIM_MB) && m_bLockOn)
	{
		m_iPhaseState ^= PHASE_LOCKON;
	}

	if (m_pGameInstance->isMouseEnter(DIM_LB) && !(m_iPhaseState & CPlayer::PHASE_HITTED))
	{
		if (m_iState == STATE_ATTACK_L1
			&& (m_pModel->Get_CurrentAnmationTrackPosition() > 15.f
				&& m_pModel->Get_CurrentAnmationTrackPosition() < 50.f))
		{
			m_pStateMgr->Get_VecState().at(3)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_ATTACK_L2;
		}

		else if (m_iState == STATE_ATTACK_L2
			&& (m_pModel->Get_CurrentAnmationTrackPosition() > 15.f
				&& m_pModel->Get_CurrentAnmationTrackPosition() < 50.f))
		{
			m_pStateMgr->Get_VecState().at(4)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_ATTACK_L3;
		}

		else
		{
			if (m_iState != STATE_ATTACK_L1 && m_iState != STATE_ATTACK_L2 && m_iState != STATE_ATTACK_L3)
			{
				m_pStateMgr->Get_VecState().at(2)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_ATTACK_L1;
			}

		}

		m_iPhaseState |= PHASE_FIGHT;

		/* 페이즈 상태 해제 */
		m_iPhaseState &= ~PHASE_DASH;
	}

	else if (m_pGameInstance->isMouseEnter(DIM_RB) && !(m_iPhaseState & CPlayer::PHASE_HITTED))
	{
		if (m_iState == STATE_ATTACK_LONG_CLAW_01
			&& (m_pModel->Get_CurrentAnmationTrackPosition() > 30.f
				&& m_pModel->Get_CurrentAnmationTrackPosition() < 100.f))
		{
			m_pStateMgr->Get_VecState().at(6)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_ATTACK_LONG_CLAW_02;
		}

		else if (m_iState != STATE_ATTACK_LONG_CLAW_01
			&& m_iState != STATE_ATTACK_LONG_CLAW_02)
		{
			m_pStateMgr->Get_VecState().at(5)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_ATTACK_LONG_CLAW_01;
		}

		m_iPhaseState |= PHASE_FIGHT;


		/* 페이즈 상태 해제 */
		m_iPhaseState &= ~PHASE_DASH;
	}

}

void CPlayer::Keyboard_section(_float fTimeDelta)
{
#pragma region 패링	
	if (m_pGameInstance->isKeyEnter(DIK_F) && !(m_iPhaseState & CPlayer::PHASE_HITTED))
	{
		if (m_iState == STATE_PARRY_L) // 패링 모션		
		{
			m_pStateMgr->Get_VecState().at(20)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_PARRY_R;


		}
		else if (m_iState != STATE_PARRY_L)  // 패링 2번째 모션	
		{
			m_pStateMgr->Get_VecState().at(19)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_PARRY_L;
		}

	}
#pragma endregion	

#pragma region 8방향 Run 
	if (!(m_iPhaseState & PHASE_FIGHT) && !(m_iPhaseState & PHASE_LOCKON) && !(m_iPhaseState & CPlayer::PHASE_HITTED)) // 공격 페이즈와 락온 페이즈가 아닐 때 			
	{
#pragma region 대쉬 
		if (m_pGameInstance->isKeyEnter(DIK_SPACE))
		{

			if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_A))
			{
				m_pStateMgr->Get_VecState().at(31)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_FL;

			}
			else if (m_pGameInstance->isKeyPressed(DIK_W) && m_pGameInstance->isKeyPressed(DIK_D))
			{
				m_pStateMgr->Get_VecState().at(30)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_FR;
			}
			else if (m_pGameInstance->isKeyPressed(DIK_B) && m_pGameInstance->isKeyPressed(DIK_A))
			{
				m_pStateMgr->Get_VecState().at(34)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_BL;

			}
			else if (m_pGameInstance->isKeyPressed(DIK_B) && m_pGameInstance->isKeyPressed(DIK_D))
			{
				m_pStateMgr->Get_VecState().at(33)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_BR;
			}
			else if (m_pGameInstance->isKeyPressed(DIK_S))
			{
				m_pStateMgr->Get_VecState().at(35)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_B;

			}
			else if (m_pGameInstance->isKeyPressed(DIK_A))
			{
				m_pStateMgr->Get_VecState().at(29)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_L;

			}
			else if (m_pGameInstance->isKeyPressed(DIK_D))
			{
				m_pStateMgr->Get_VecState().at(28)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_R;

			}
			else
			{
				m_pStateMgr->Get_VecState().at(32)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_NORMAL_EVADE_F;
			}

			m_iPhaseState |= PHASE_DASH;

		}
#pragma endregion 

		else if (((GetKeyState('W') & 0x8000) || (GetKeyState('S') & 0x8000) || (GetKeyState('A') & 0x8000) || (GetKeyState('D') & 0x8000))
			&& !(m_iPhaseState & PHASE_DASH))
		{
			m_pStateMgr->Get_VecState().at(1)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_RUN;
		}

		else
		{
			if (!(m_iPhaseState & PHASE_DASH)
				&& m_iState != STATE_PARRY_L
				&& m_iState != STATE_PARRY_R
				&& !(m_iPhaseState & PHASE_PARRY))
			{
				m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_IDLE;

			}
		}

		m_iPhaseState |= PHASE_IDLE;
	}
#pragma endregion 
#pragma region 락온모드 8방향 이동 
	if (!(m_iPhaseState & PHASE_FIGHT) && (m_iPhaseState & PHASE_LOCKON) && !(m_iPhaseState & CPlayer::PHASE_HITTED))
	{
		/* 두 키입력이 동시에 들어왔을 때 */
		if ((GetKeyState('W') & 0x8000) && (GetKeyState('A') & 0x8000)
			|| (GetKeyState('W') & 0x8000) && (GetKeyState('D') & 0x8000)
			|| (GetKeyState('S') & 0x8000) && (GetKeyState('A') & 0x8000)
			|| (GetKeyState('S') & 0x8000) && (GetKeyState('D') & 0x8000)
			|| (GetKeyState('A') & 0x8000) && (GetKeyState('D') & 0x8000)
			|| (GetKeyState('W') & 0x8000) && (GetKeyState('S') & 0x8000))
		{
			if ((GetKeyState('W') & 0x8000) && (GetKeyState('A') & 0x8000)
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(10)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_FL;
			}
			else if ((GetKeyState('W') & 0x8000) && (GetKeyState('D') & 0x8000)
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(9)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_FR;
			}

			else if ((GetKeyState('S') & 0x8000) && (GetKeyState('A') & 0x8000)
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(12)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_BL;
			}


			else if ((GetKeyState('S') & 0x8000) && (GetKeyState('D') & 0x8000)
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(11)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_BR;
			}


			else if ((GetKeyState('W') & 0x8000) && (GetKeyState('S') & 0x8000)
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_IDLE;
			}

			else if ((GetKeyState('A') & 0x8000) && (GetKeyState('D') & 0x8000)
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_IDLE;
			}

		}

		else
		{
			if ((m_pGameInstance->isKeyEnter(DIK_W) || m_pGameInstance->isKeyPressed(DIK_W))
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(14)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_RUN;
			}

			else if ((m_pGameInstance->isKeyEnter(DIK_A) || m_pGameInstance->isKeyPressed(DIK_A))
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(8)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_L;
			}

			else if ((m_pGameInstance->isKeyEnter(DIK_D) || m_pGameInstance->isKeyPressed(DIK_D))
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(7)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_R;
			}

			else if ((m_pGameInstance->isKeyEnter(DIK_S) || m_pGameInstance->isKeyPressed(DIK_S))
				&& m_bNextStateCanPlay)
			{
				m_pStateMgr->Get_VecState().at(13)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_LOCK_ON_RUN_B;
			}
		}

		if (m_pGameInstance->isKeyPressed(DIK_W)
			&& m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(18)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_F;
			m_bNextStateCanPlay = false;
			//m_pModel->Set_Continuous_Ani(true);	 // 3월 6일 이거 추가됨	
		}

		else if (m_pGameInstance->isKeyPressed(DIK_A)
			&& m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(16)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_L;
			m_bNextStateCanPlay = false;
			//m_pModel->Set_Continuous_Ani(true);	 // 3월 6일 이거 추가됨 
		}

		else if (m_pGameInstance->isKeyPressed(DIK_D)
			&& m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(17)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_R;
			m_bNextStateCanPlay = false;
			//m_pModel->Set_Continuous_Ani(true);	 // 3월 6일 이거 추가됨
		}

		else if (m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(15)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_B;
			m_bNextStateCanPlay = false;
			//m_pModel->Set_Continuous_Ani(true);	 // 3월 6일 이거 추가됨
		}

		/* 아무 키도 안눌려있다면 IDLE 상태로 */
		if (!(GetKeyState('W') & 0x8000)
			&& !(GetKeyState('S') & 0x8000)
			&& !(GetKeyState('A') & 0x8000)
			&& !(GetKeyState('D') & 0x8000)
			&& m_bNextStateCanPlay
			&& (m_iState != STATE_PARRY_L)
			&& (m_iState != STATE_PARRY_R)
			&& (m_iState != STATE_PARRY_DEFLECT_L)
			&& (m_iState != STATE_PARRY_DEFLECT_L_UP)
			&& (m_iState != STATE_PARRY_DEFLECT_R)
			&& (m_iState != STATE_PARRY_DEFLECT_R_UP)
			)
		{
			m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_IDLE;
		}

	}
#pragma endregion 


}

void CPlayer::Can_Move()
{
	if (m_iState != m_iPreState)
		m_bMove = true;

	// 예외조건 설정해두기 회피및 다른 도주기 등 
	if (m_iState == STATE_LOCK_ON_EVADE_B
		|| m_iState == STATE_LOCK_ON_EVADE_L
		|| m_iState == STATE_LOCK_ON_EVADE_R
		|| m_iState == STATE_NORMAL_EVADE_B
		|| m_iState == STATE_NORMAL_EVADE_BL
		|| m_iState == STATE_NORMAL_EVADE_BR
		|| m_iState == STATE_NORMAL_EVADE_FL
		|| m_iState == STATE_NORMAL_EVADE_FR
		|| m_iState == STATE_NORMAL_EVADE_L
		|| m_iState == STATE_NORMAL_EVADE_R
		|| m_iState == STATE_HurtMFR_L
		|| m_iState == STATE_HurtMFR_R
		|| m_iState == STATE_HURT_LF
		|| m_iState == STATE_HURT_SF
		|| m_iState == STATE_HURT_SL
		|| m_iState == STATE_HURT_HURXXLF
		|| m_iState == STATE_HURT_KNOCKBACK
		|| m_iState == STATE_HURT_KNOCKDOWN
		|| m_iState == STATE_HURT_FALLDOWN
		|| m_iState == STATE_WEAK_GETUP_F
		)
	{
		m_bMove = true;
	}
}




void CPlayer::Update(_float fTimeDelta)
{

	_vector		vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// 각 state 변경되면 한번 bool값으로 조절하고 해당 스테이트에서 저게 발생했다 하면 움직이지 않도록 더이상 


	Can_Move(); // 이게 몬스터와 겹치지 않게 하는 루트 애니메이션 조절 


	_vector test = { 0.f,0.f,0.f,1.f };
	/* 루트 모션 애니메션 코드 */
	m_pRootMatrix = m_pModel->Get_RootMotionMatrix("root");

	if (!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModel->Get_LerpFinished() && m_bMove)
	{
		if (m_pNavigationCom->isMove(vCurPosition))
			m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix); // 그럼 전환될때만 안하기로하자.

		/* 2월 19일 추가 코드 */
		if (!m_pNavigationCom->isMove(m_pTransformCom->Get_State(CTransform::STATE_POSITION)))
		{
			_float4x4 test = {};
			XMStoreFloat4x4(&test, XMMatrixInverse(nullptr, XMLoadFloat4x4(m_pRootMatrix)));
			const _float4x4* test2 = const_cast<_float4x4*>(&test);
			m_pTransformCom->Set_MulWorldMatrix(test2);
		}
	}

	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPosition, m_pNavigationCom->Compute_Height(vPosition)));

	if (SUCCEEDED(m_pGameInstance->IsActorInScene(m_pActor)))
		m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });

	__super::Update(fTimeDelta);

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	/* 잠시 네비게이션 정확히 타는지 확인할려고 넣는 코드 */
#ifdef _DEBUG
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
#endif
	m_fTimeDelta = fTimeDelta;


	/* 이전 상태 저장하기 */
	m_iPreState = m_iState;

	m_iPrePhaseState = m_iPhaseState;

}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
	//m_pNavigationCom->Render();	
#endif 

	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	/* Com_Navigation */
	CNavigation::NAVIGATION_DESC   Desc{};

	/* 초기 디버깅 플레이어가 서있는 셀의 인덱스 */
	Desc.iCurrentCellIndex = 11;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentActor = m_pActor;
	BodyDesc.pParent = this;
	BodyDesc.pParentNavigationCom = m_pNavigationCom;
	BodyDesc.pParentStateMgr = m_pStateMgr;
	BodyDesc.pParentState = &m_iState;
	BodyDesc.pParentPhaseState = &m_iPhaseState;
	BodyDesc.pParentNextStateCan = &m_bNextStateCanPlay;
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.fSpeedPerSec = 0.f;
	//BodyDesc.fSpeedPerSec = 1.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;

	m_pModel = dynamic_cast<CModel*>(Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));


	/* 오른손 무기를 만든다. */
	CRightWeapon::WEAPON_DESC		RightWeaponDesc{};

	CModel* pBodyModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));
	if (nullptr == pBodyModelCom)
		return E_FAIL;

	RightWeaponDesc.pParent = this;
	RightWeaponDesc.pParentModel = m_pModel;
	RightWeaponDesc.pParentState = &m_iState;
	RightWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* 캐릭터 모델마다 다름 */
	RightWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	RightWeaponDesc.fSpeedPerSec = 0.f;
	RightWeaponDesc.fRotationPerSec = 10.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Right_Weapon"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Right_Weapon"), &RightWeaponDesc)))
		return E_FAIL;


	/* 왼손 무기를 만든다. */
	CLeftWeapon::WEAPON_DESC		LeftWeaponDesc{};

	RightWeaponDesc.pParent = this;
	LeftWeaponDesc.pParentState = &m_iState;
	LeftWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_l"); /* 캐릭터 모델마다 다름 */
	LeftWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	LeftWeaponDesc.fSpeedPerSec = 0.f;
	LeftWeaponDesc.fRotationPerSec = 10.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Left_Weapon"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Left_Weapon"), &LeftWeaponDesc)))
		return E_FAIL;



	/* 오른쪽 손톱 무기를 만든다. */
	CClawWeapon::WEAPON_DESC		RightClawWeaponDesc{};

	RightWeaponDesc.pParent = this;
	RightClawWeaponDesc.pParentModel = m_pModel;
	RightClawWeaponDesc.pParentState = &m_iState;
	RightClawWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* 캐릭터 모델마다 다름 */
	RightClawWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	RightClawWeaponDesc.fSpeedPerSec = 0.f;
	RightClawWeaponDesc.fRotationPerSec = 10.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Right_Claw"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Right_Claw"), &RightClawWeaponDesc)))
		return E_FAIL;


	return S_OK;
}


void CPlayer::OnCollisionEnter(CGameObject* _pOther, PxContactPair _information)
{
	/* 몬스터 무기와의 충돌 */
	if (!strcmp("MONSTER_WEAPON", _pOther->Get_Name()))
	{
		/* 충돌 지점 이 오른쪽 왼쪽 인지 판별 해야함 */
		PxContactPairPoint contactPoints[1]; // 최대 10개까지 저장		
		_information.extractContacts(contactPoints, 1);

		PxVec3 position = contactPoints[0].position;
		_vector PlayerPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

		int a = 4;

		/* 패링 상태 일시 */
		if (m_iPhaseState & CPlayer::PHASE_PARRY)
		{
			int Parry = rand() % 2;

			/* 여기서 플레이어의 특정 프레임 */
			switch (m_iState)
			{
			case STATE::STATE_PARRY_L:
				if (Parry == 0)
				{
					m_iState = STATE_PARRY_DEFLECT_L;
					m_pStateMgr->Get_VecState().at(24)->Set_MonsterModel(dynamic_cast<CPartObject*>(_pOther)->Get_Parent_Ptr()->Get_GameObject_Model());
				}
				else
				{
					m_iState = STATE_PARRY_DEFLECT_L_UP;
					m_pStateMgr->Get_VecState().at(25)->Set_MonsterModel(dynamic_cast<CPartObject*>(_pOther)->Get_Parent_Ptr()->Get_GameObject_Model());
				}
				break;
			case STATE::STATE_PARRY_R:
				if (Parry == 0)
					m_iState = STATE_PARRY_DEFLECT_R;
				else
					m_iState = STATE_PARRY_DEFLECT_R_UP;
				break;
			}
		}


		else  // 여기다가 패링 실패의 상황도 넣어야 겠네. 
		{
			/* 패링 실패 시 ( 즉 맞을 때 ) */
			m_iPhaseState &= ~CPlayer::PHASE_PARRY;
			m_iPhaseState &= ~CPlayer::PHASE_DASH;
			m_iPhaseState &= ~CPlayer::PHASE_FIGHT;

			m_iPhaseState |= CPlayer::PHASE_HITTED;

			_float4 fMonsterLookDir = {};
			const _float4x4* ParentMatrix = dynamic_cast<CPartObject*>(_pOther)->Get_ParentWorldMatrix();
			fMonsterLookDir = { ParentMatrix->_31,ParentMatrix->_32,ParentMatrix->_33,0.f };


			switch (dynamic_cast<CPartObject*>(_pOther)->Get_Parent_Ptr()->Get_Player_Hitted_State())
			{
			case Player_Hitted_State::PLAYER_HURT_FallDown:
				m_iState = CPlayer::STATE_HURT_FALLDOWN;
				/* 몬스터 공격 방향 */
				m_pStateMgr->Get_VecState().at(42)->Set_MonsterLookDir(fMonsterLookDir);

				m_pStateMgr->Get_VecState().at(42)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
				break;
			case Player_Hitted_State::PLAYER_HURT_HURTLF:
				m_iState = CPlayer::STATE_HURT_LF;
				/* 몬스터 공격 방향 */
				m_pStateMgr->Get_VecState().at(38)->Set_MonsterLookDir(fMonsterLookDir);

				m_pStateMgr->Get_VecState().at(38)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
				break;
			case Player_Hitted_State::PLAYER_HURT_HURTMFL: // 31번 애니메이션 인덱스
				m_iState = CPlayer::STATE_HurtMFR_L;  // 22		
				/* 몬스터 공격 방향 */
				m_pStateMgr->Get_VecState().at(21)->Set_MonsterLookDir(fMonsterLookDir);

				m_pStateMgr->Get_VecState().at(21)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
				m_pModel->Set_Continuous_Ani(true);
				break;
			case Player_Hitted_State::PLAYER_HURT_HURTSF:
				m_iState = CPlayer::STATE_HURT_SF;
				/* 몬스터 공격 방향 */
				m_pStateMgr->Get_VecState().at(39)->Set_MonsterLookDir(fMonsterLookDir);

				m_pStateMgr->Get_VecState().at(39)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);

				break;
			case Player_Hitted_State::PLAYER_HURT_HURTSL:
				m_iState = CPlayer::STATE_HURT_SL;
				/* 몬스터 공격 방향 */
				m_pStateMgr->Get_VecState().at(41)->Set_MonsterLookDir(fMonsterLookDir);

				m_pStateMgr->Get_VecState().at(41)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);

				break;
			case Player_Hitted_State::PLAYER_HURT_HURXXLF:
				m_iState = CPlayer::STATE_HURT_HURXXLF;
				/* 몬스터 공격 방향 */
				m_pStateMgr->Get_VecState().at(40)->Set_MonsterLookDir(fMonsterLookDir);

				m_pStateMgr->Get_VecState().at(40)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
				m_pModel->Set_Continuous_Ani(true);
				break;
			case Player_Hitted_State::PLAYER_HURT_KnockBackF:
				m_iState = CPlayer::STATE_HURT_KNOCKBACK;
				/* 몬스터 공격 방향 */
				m_pStateMgr->Get_VecState().at(36)->Set_MonsterLookDir(fMonsterLookDir);

				m_pStateMgr->Get_VecState().at(36)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);

				break;
			case Player_Hitted_State::PLAYER_HURT_KNOCKDOWN:
				m_iState = CPlayer::STATE_HURT_KNOCKDOWN;
				/* 몬스터 공격 방향 */
				m_pStateMgr->Get_VecState().at(37)->Set_MonsterLookDir(fMonsterLookDir);

				m_pStateMgr->Get_VecState().at(37)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);

				break;
			default:
				m_iState = CPlayer::STATE_HurtMFR_R;  // 22		
				/* 몬스터 공격 방향 */
				m_pStateMgr->Get_VecState().at(22)->Set_MonsterLookDir(fMonsterLookDir);

				m_pStateMgr->Get_VecState().at(22)->Priority_Update(this, m_pNavigationCom, m_fTimeDelta);
				break;
			}



		}
	}
}

// 한개 더 분류 

void CPlayer::OnCollision(CGameObject* _pOther, PxContactPair _information)
{
	if (!strcmp("MONSTER", _pOther->Get_Name()))
	{
		if ((m_pGameInstance->isKeyPressed(DIK_W) || m_pGameInstance->isKeyPressed(DIK_S)
			|| m_pGameInstance->isKeyPressed(DIK_A) || m_pGameInstance->isKeyPressed(DIK_D))
			&& !(m_iPhaseState & PHASE_LOCKON)
			&& (m_iPhaseState & PHASE_IDLE))
		{
			Slide_Move(_pOther);
		}

		m_bMove = false;
	}
}

void CPlayer::OnCollisionExit(CGameObject* _pOther, PxContactPair _information)
{
	m_bMove = true;
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	m_pGameInstance->Sub_Actor_Scene(m_pActor);

	Safe_Release(m_pStateMgr);
	Safe_Release(m_pNavigationCom);
}


void CPlayer::Slide_Move(CGameObject* pGameObject)
{
	//_vector vSlider = vPlayerLook - vMonsterLook;	
	// 플레이어 이동 벡터 (입사 벡터)
	_vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	// 몬스터가 플레이어와 부딪히는 노말 벡터  
	_vector vNoramlVec = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - (pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION)));

	if (XMVectorGetX(XMVector3Dot(vPlayerLook, vNoramlVec)) < 0.f)
	{
		// 벡터 투영: 플레이어 이동 벡터를 몬스터 표면 벡터(Normal)에 투영
		_vector vProj = vNoramlVec * XMVectorGetX(XMVector3Dot(vPlayerLook, vNoramlVec));

		// 슬라이딩 벡터 = 기존 이동 벡터 - 투영된 벡터
		_vector vSlider = vPlayerLook - vProj;

		// 최종 이동 적용
		m_pTransformCom->Go_Backward_With_Navi(m_fTimeDelta * 0.075f, m_pNavigationCom);

		m_pTransformCom->Go_Dir(vSlider, m_pNavigationCom, m_fTimeDelta * 0.075f);
	}
}