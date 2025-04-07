#pragma once

#include "PartObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CModel;
class CNavigation;
END

BEGIN(Client)

class CStand_Stack_Sword final : public CPartObject
{
public:
	struct STAND_STACK_SWORD_DESC : public CPartObject::PARTOBJECT_DESC
	{
		_float4* pPlayerPos = { nullptr };
		const _float4x4* pSocketMatrix = { nullptr };
		const _uint* pParentState = { nullptr };
		CModel* pParentModel = { nullptr };
		_bool* bNeed_Memory_Position = { nullptr };
		_bool* bIs_Equipped_To_LeftHand = { nullptr };
		_bool* bIs_Stand_In_Ground = { nullptr };
		_bool* bNeed_Fly_To_Player = { nullptr };
	};
private:
	CStand_Stack_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStand_Stack_Sword(const CStand_Stack_Sword& Prototype);
	virtual ~CStand_Stack_Sword() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
public:
	HRESULT Ready_Components(void* pArg);
	HRESULT Bind_ShaderResources();
public:
	void Store_CombinedMatrix();
	void Fly_To_Player(_float fTimeDelta);
private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CModel* m_pParentModelCom = { nullptr };

	CNavigation* m_pNavigationCom = { nullptr };
	PxRigidDynamic* m_pActor = { nullptr };
private:
	// 처음 우르드 허리에 있는 위치들
	const _float4x4* m_pSocketMatrix = { nullptr };
	//왼손 위치
	const _float4x4* m_pLeft_SocketMatrix = { nullptr };
	_float4x4 m_Store_CombinedMatrix = {};

private:
	_float4* m_vPlayer_Position = { nullptr };
	_bool* m_bNeed_Memory_Position = { nullptr };
	_bool* m_bIs_Equipped_To_LeftHand = { nullptr };
	_bool* m_bIs_Stand_In_Ground = { nullptr };
	_bool* m_bNeed_Fly_To_Player = { nullptr };

	_float4x4 m_FlyWorldMatrix = {};         // 비행 중 실제 월드 위치
	_float4 m_vFlyTargetPos = {};            // 목표 위치
	_bool	m_bHas_Finished_Flying = {};
	_bool   m_bHasStartedFlying = {};

	_float4 m_fStartPos = {};
	_float4 m_fEndPos = {};
	_float m_fSpeed = {};
	_float m_fLinearTime = {};
public:
	virtual void OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollision(CGameObject* _pOther, PxContactPair _information);
	virtual void OnCollisionExit(CGameObject* _pOther, PxContactPair _information);

public:
	static CStand_Stack_Sword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END