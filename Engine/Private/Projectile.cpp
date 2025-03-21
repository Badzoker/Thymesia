#include "Projectile.h"

CProjectile::CProjectile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CProjectile::CProjectile(const CProjectile& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CProjectile::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CProjectile::Initialize(void* pArg)
{
	PROJECTILE_DESC* pDesc = static_cast<PROJECTILE_DESC*>(pArg);

	m_fDamage = pDesc->fDamage;
	m_fDelete_Time = pDesc->fDelete_Time;
	m_fSpeed = pDesc->fSpeedPerSec;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Rotation(90.f, 0.f, 90.f);

	return S_OK;
}

void CProjectile::Priority_Update(_float fTimeDelta)
{
	m_fDelete_Time -= fTimeDelta;
	if (m_fDelete_Time <= 0.f)
	{
		Set_IsFire(false);
		return;
	}
}

void CProjectile::Update(_float fTimeDelta)
{
	if (m_bIsFire)
		Fire_OneShoot(XMLoadFloat4(&m_vStartPos), XMLoadFloat4(&m_vEndPos), fTimeDelta);
	else if (m_bMultiFire)
		Fire_MultiShoot(XMLoadFloat4(&m_vStartPos), XMLoadFloat4(&m_vEndPos), fTimeDelta);
}

void CProjectile::Late_Update(_float fTimeDelta)
{
}

HRESULT CProjectile::Render()
{
	return S_OK;
}

void CProjectile::Reset_Projectile()
{
	m_fDelete_Time = 5.f;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_vStartPos));
}

void CProjectile::Set_Target(_fvector vStartPos, _fvector vEndPos)
{
	XMStoreFloat4(&m_vStartPos, vStartPos);
	XMStoreFloat4(&m_vEndPos, vEndPos);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vStartPos);
	_vector vLook = XMVector3Normalize(vEndPos - vStartPos);
	m_pTransformCom->Look(vLook);

}

void CProjectile::Fire_OneShoot(_fvector vStartPos, _fvector vEndPos, _float _fTimeDelta)
{
	m_pTransformCom->Go_Straight_NoNavi(_fTimeDelta);
}

void CProjectile::Fire_MultiShoot(_fvector vStartPos, _fvector vEndPos, _float _fTimeDelta)
{
}

void CProjectile::Free()
{
	__super::Free();
}
