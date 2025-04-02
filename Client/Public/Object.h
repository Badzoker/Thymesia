#pragma once

#include "Client_Defines.h"

#include "GameObject.h"
#include "GameInstance.h"


BEGIN(Engine)
class CModel;
class CShader;
class CCalculator;
class CMesh;
class CCollider;
END

BEGIN(Client)

class CObject  abstract : public CGameObject 
{
public:
	enum OBJECT_TYPE {
		OBJECT_DEFAULT = 0x00000001,
		OBJECT_BILLBOARD = 0x00000002,
		OBJECT_INTERACTIVE = 0x00000004,
	};

public:
	typedef struct Object_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		_float		fFrustumRadius = { 0.f };
		_float4		fRotation = { };
		string		ObjectName = {};
		_uint		iPassIndex = { 0 };
		_uint		iObjectType = { OBJECT_DEFAULT };
		_uint		iBillBoardMeshNum = { 0 };
		LEVELID		eLevelID = { LEVEL_TUTORIAL };

	}OBJECT_DESC;

protected:
	CObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CObject(const CObject& Prototype);
	virtual ~CObject() = default;

protected:
	virtual HRESULT Initialize(void* pArg) override;

protected:
	//Components
	CShader*	 m_pShaderCom = { nullptr };
	CModel*		 m_pModelCom = { nullptr };

protected:
	_float		   m_fFrustumRadius = { 0.f };
	_float3		   m_fRotation	= { 0.f, 0.f, 0.f };
	_float3		   m_fScale		= { 0.f, 0.f, 0.f };

	_uint		   m_iPassIndex = { 0 };

	_uint		   m_iObjectType = { OBJECT_DEFAULT };

	LEVELID		   m_eCurrentLevel = { LEVEL_TUTORIAL };

protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT Bind_ShaderResources() PURE;

public:

	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END