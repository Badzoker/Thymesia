#include "pch.h" 
#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"
#include "RightWeapon.h"
#include "LeftWeapon.h"
#include "StateMgr.h"
#include "Animation.h"

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


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	///* ��Ʈ ��� �ִϸ޼� �ڵ� */
	//m_pRootMatrix = m_pModel->Get_RootMotionMatrix("kaku");		


	// ���� ������ �÷��̾� ��ġ 1_23�� 
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 112.6f,1.85f,107.1f,1.f });

	m_pGameInstance->Add_ObjCollider(GROUP_TYPE::PLAYER, this);

	m_pTransformCom->Scaling(_float3{ 0.002f, 0.002f, 0.002f });



	m_pStateMgr = CStateMgr::Create();
	if (m_pStateMgr == nullptr)
	{
		MSG_BOX("Failed to Created : StateMgr");
	}

	/* PhsyX ���� */
	m_pActor = m_pGameInstance->Add_Actor(COLLIDER_TYPE::COLLIDER_BOX, _float3{ 0.1f,0.1f,0.1f }, _float3{ 0.f,0.f,1.f }, 90.f, this);
	m_pGameInstance->Set_GlobalPos(m_pActor, _fvector{ 0.f,0.f,0.f,1.f });
	_uint settingColliderGroup = GROUP_TYPE::MONSTER | GROUP_TYPE::MONSTER_WEAPON;
	m_pGameInstance->Set_CollisionGroup(m_pActor, GROUP_TYPE::PLAYER, settingColliderGroup);	
	/* --------- */	


	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
#pragma region Mouse_Input
	/* ���콺 �Է�Ű�� (LB, RB ) ���� �Է��� �Ұ��� �ϵ��� ���� */
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

	if (m_pGameInstance->isMouseEnter(DIM_LB))
	{
		if (m_iState == STATE_ATTACK_L1
			&& (m_pModel->Get_CurrentAnmationTrackPosition() > 15.f
				&& m_pModel->Get_CurrentAnmationTrackPosition() < 50.f))
		{
			m_pStateMgr->Get_VecState().at(3)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_ATTACK_L2;
		}

		else if (m_iState == STATE_ATTACK_L2
			&& (m_pModel->Get_CurrentAnmationTrackPosition() > 25.f
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
	}

	else if (m_pGameInstance->isMouseEnter(DIM_RB))
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
	}

}

void CPlayer::Keyboard_section(_float fTimeDelta)
{

#pragma region �и�	
	if (m_pGameInstance->isKeyEnter(DIK_F))
	{
		if (m_iState == STATE_PARRY_L) // �и� ���		
		{
			m_pStateMgr->Get_VecState().at(20)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_PARRY_R;

		}
		else if (m_iState != STATE_PARRY_L)  // �и� 2��° ���	
		{
			m_pStateMgr->Get_VecState().at(19)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_PARRY_L;

		}
	}
#pragma endregion	

#pragma region 8���� Run 
	if (!(m_iPhaseState & PHASE_FIGHT) && !(m_iPhaseState & PHASE_LOCKON)) // ���� ������� ���� ����� �ƴ� �� 			
	{
		if ((GetKeyState('W') & 0x8000) || (GetKeyState('S') & 0x8000) || (GetKeyState('A') & 0x8000) || (GetKeyState('D') & 0x8000))
		{
			m_pStateMgr->Get_VecState().at(1)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_RUN;
		}

		else
		{
			if (m_iState == STATE_RUN)
			{
				m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_IDLE;
			}
		}

		m_iPhaseState |= PHASE_IDLE;
	}
#pragma endregion 
#pragma region ���¸�� 8���� �̵� 
	if (!(m_iPhaseState & PHASE_FIGHT) && (m_iPhaseState & PHASE_LOCKON))
	{
		/* �� Ű�Է��� ���ÿ� ������ �� */
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

			/*else
			{
				m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
				m_iState = STATE_IDLE;
			}*/

		}

		if (m_pGameInstance->isKeyPressed(DIK_W)
			&& m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(18)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_F;
			m_bNextStateCanPlay = false;
		}

		else if (m_pGameInstance->isKeyPressed(DIK_A)
			&& m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(16)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_L;
			m_bNextStateCanPlay = false;
		}

		else if (m_pGameInstance->isKeyPressed(DIK_D)
			&& m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(17)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_R;
			m_bNextStateCanPlay = false;
		}

		else if (m_pGameInstance->isKeyEnter(DIK_SPACE))
		{
			m_pStateMgr->Get_VecState().at(15)->Priority_Update(this, m_pNavigationCom, fTimeDelta);
			m_iState = STATE_LOCK_ON_EVADE_B;
			m_bNextStateCanPlay = false;
		}

		/* �ƹ� Ű�� �ȴ����ִٸ� IDLE ���·� */
		if (!(GetKeyState('W') & 0x8000)
			&& !(GetKeyState('S') & 0x8000)
			&& !(GetKeyState('A') & 0x8000)
			&& !(GetKeyState('D') & 0x8000)
			&& m_bNextStateCanPlay
			&& (m_iState != STATE_PARRY_L)
			&& (m_iState != STATE_PARRY_R)
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

	if (m_pColliderCom->Get_isCollision())
		m_bMove = false;


	// �������� �����صα� ȸ�ǹ� �ٸ� ���ֱ� �� 
	if (m_iState == STATE_LOCK_ON_EVADE_B
		|| m_iState == STATE_LOCK_ON_EVADE_L
		|| m_iState == STATE_LOCK_ON_EVADE_R)
	{
		m_bMove = true;
	}
}




void CPlayer::Update(_float fTimeDelta)
{

	_vector		vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	// �� state ����Ǹ� �ѹ� bool������ �����ϰ� �ش� ������Ʈ���� ���� �߻��ߴ� �ϸ� �������� �ʵ��� ���̻� 


	Can_Move(); // �̰� ���Ϳ� ��ġ�� �ʰ� �ϴ� ��Ʈ �ִϸ��̼� ���� 


	_vector test = { 0.f,0.f,0.f,1.f };
	/* ��Ʈ ��� �ִϸ޼� �ڵ� */
	m_pRootMatrix = m_pModel->Get_RootMotionMatrix("root");

	if (!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModel->Get_LerpFinished() && m_bMove)
	{
		if (m_pNavigationCom->isMove(vCurPosition))
			m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix); // �׷� ��ȯ�ɶ��� ���ϱ������.

		/* 2�� 19�� �߰� �ڵ� */
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

	//m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

	/* PhysX ���� */
	m_pGameInstance->Update_Collider(m_pActor, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()), _vector{ 0.f, 250.f,0.f,1.f });	
	/* ======== */

	__super::Update(fTimeDelta);

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	/* ��� �׺���̼� ��Ȯ�� Ÿ���� Ȯ���ҷ��� �ִ� �ڵ� */
#ifdef _DEBUG
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
#endif
	m_fTimeDelta = fTimeDelta;

	/* ���� ���� �����ϱ� */
	m_iPreState = m_iState;

	m_iPrePhaseState = m_iPhaseState;

}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
	//m_pNavigationCom->Render();	
	//m_pColliderCom->Render();
#endif 

	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
	/* Com_Navigation */
	CNavigation::NAVIGATION_DESC   Desc{};

	/* �ʱ� ����� �÷��̾ ���ִ� ���� �ε��� */
	Desc.iCurrentCellIndex = 0;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &Desc)))
		return E_FAIL;


	/* Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC SphereDesc{};


	SphereDesc.fRadius = 115.f;
	SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius + 80.f, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;


	m_pColliderCom->Set_Collider_Name("Player");


	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_PLAYER_DESC BodyDesc{};

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


	/* ������ ���⸦ �����. */
	CRightWeapon::WEAPON_DESC		RightWeaponDesc{};

	CModel* pBodyModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));
	if (nullptr == pBodyModelCom)
		return E_FAIL;

	RightWeaponDesc.pParentState = &m_iState;
	RightWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* ĳ���� �𵨸��� �ٸ� */
	RightWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	RightWeaponDesc.fSpeedPerSec = 0.f;
	RightWeaponDesc.fRotationPerSec = 10.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Right_Weapon"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Right_Weapon"), &RightWeaponDesc)))
		return E_FAIL;


	/* ������ ���⸦ �����. */
	CLeftWeapon::WEAPON_DESC		LeftWeaponDesc{};

	LeftWeaponDesc.pParentState = &m_iState;
	LeftWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_l"); /* ĳ���� �𵨸��� �ٸ� */
	LeftWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	LeftWeaponDesc.fSpeedPerSec = 0.f;
	LeftWeaponDesc.fRotationPerSec = 10.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Left_Weapon"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Left_Weapon"), &LeftWeaponDesc)))
		return E_FAIL;

	return S_OK;
}


void CPlayer::OnCollisionEnter(CGameObject* _pOther)
{

}


void CPlayer::OnCollision(CGameObject* _pOther)
{
	if (!strcmp("Monster", m_pGameInstance->Get_ColliderName(_pOther)))		
	{
		if ((m_pGameInstance->isKeyPressed(DIK_W) || m_pGameInstance->isKeyPressed(DIK_S))
			&& !(m_iPhaseState & PHASE_LOCKON)
			&& (m_iPhaseState & PHASE_IDLE))
		{
			Slide_Move(_pOther);
		}
	}
}

void CPlayer::OnCollisionExit(CGameObject* _pOther)
{

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

	Safe_Release(m_pStateMgr);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pNavigationCom);
}


void CPlayer::Slide_Move(CGameObject* pGameObject)
{
	//_vector vSlider = vPlayerLook - vMonsterLook;	
	// �÷��̾� �̵� ���� (�Ի� ����)
	_vector vPlayerLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	// ���Ͱ� �÷��̾�� �ε����� �븻 ����  
	_vector vNoramlVec = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - (pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION)));

	if (XMVectorGetX(XMVector3Dot(vPlayerLook, vNoramlVec)) < 0.f)
	{
		// ���� ����: �÷��̾� �̵� ���͸� ���� ǥ�� ����(Normal)�� ����
		_vector vProj = vNoramlVec * XMVectorGetX(XMVector3Dot(vPlayerLook, vNoramlVec));

		// �����̵� ���� = ���� �̵� ���� - ������ ����
		_vector vSlider = vPlayerLook - vProj;

		// ���� �̵� ����
		m_pTransformCom->Go_Backward_With_Navi(m_fTimeDelta * 0.075f, m_pNavigationCom);

		m_pTransformCom->Go_Dir(vSlider, m_pNavigationCom, m_fTimeDelta * 0.075f);
	}
}