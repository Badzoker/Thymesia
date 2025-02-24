#pragma once

/* �׸��ڸ� ǥ���ϱ� ���� ������ ������ ��� �ִ´�. */

#include "Base.h"

BEGIN(Engine)

class CShadow final : public CBase
{
private:
	CShadow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CShadow() = default;

public:
	_float4				Get_LightPos();

public:
	HRESULT Initialize();
	HRESULT SetUp_ShadowLight(_fvector vEye, _fvector vAt, _float fLightAngle, _float fAspect, _float fNear, _float fFar, _matrix matInvCam, _fvector vCamInfo); // ���⼭ �÷��̾� ��ġ �����ͷ� �Ѱ�����
	HRESULT Bind_ViewMatrix(class CShader* pShader, const _char* pConstantName);
	HRESULT Bind_ProjMatrix(class CShader* pShader, const _char* pConstantName);
	void    Update();


private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	_float4x4			 m_LightViewMatrix = {};
	_float4x4			 m_LightProjMatrix = {};

	_float4				 m_LightPos = {};

	_float	    		 m_cascadeEnd[4] = {};

	_matrix				 m_shadowOrthoProj[3] = {};

public:
	static CShadow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END