#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"

#include "Button.h"

BEGIN(Engine)
class CModel;
class CShader;
//class CCollider;
END

BEGIN(Client)
class CSpecificObject final : public CGameObject
{
public:
    struct SpecificObject_Desc : public CGameObject::GAMEOBJECT_DESC
    {
        _float      fFrustumRadius = {};
        _float4		fRotation = { };
        _float3     fScale = {};
        string		ObjectName = {};
        _uint		iPassNum = { 0 };
    };

    struct SpecificObject_Info
    {
        _char   szName[MAX_PATH] = {};
        _float4	fPosition;
        _float3 fScale;
        _float4 fRotation;
        _float  fFrustumRadius;
        _uint	iPassNum;
    };

private:
    CSpecificObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSpecificObject(const CSpecificObject& Prototype);
    virtual ~CSpecificObject() = default;

public:
    virtual HRESULT                 Initialize_Prototype() override;
    virtual HRESULT                 Initialize(void* pArg) override;
    virtual void                    Priority_Update(_float fTimeDelta) override;
    virtual void                    Update(_float fTimeDelta) override;
    virtual void                    Late_Update(_float fTimeDelta) override;
    virtual HRESULT                 Render() override;
    virtual HRESULT                 Render_Glow() override;

public:
    virtual void                    OnCollisionEnter(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollision(CGameObject* _pOther, PxContactPair _information);
    virtual void                    OnCollisionExit(CGameObject* _pOther, PxContactPair _information);


private:
    CShader*                        m_pShaderCom = { nullptr };
    CModel*                         m_pModelCom = { nullptr };
    _float		                    m_fFrustumRadius = { 0.f };
    _float4		                    m_fRotation = { 0.f, 0.f, 0.f , 0.0f };
    _float3		                    m_fScale = { 0.f, 0.f, 0.f };
    _float4                         m_fPosition = { 0.0f, 0.0f, 0.0f ,0.0f };
    _char		                    m_MeshName[MAX_PATH] = {};
    _bool		                    m_bFrustumSphere = { true };

    SpecificObject_Info             m_tagSpecificInfo = {};
    SpecificObject_Desc             m_tagDesc = {};

    PxRigidDynamic*                 m_pActor = { nullptr };
private:
    virtual HRESULT                 Ready_Components();
    virtual HRESULT                 Bind_ShaderResources();

private:
    CGameObject*                    m_pButtonGameObject = { nullptr };
    CButton*                        m_pButton = { nullptr };
    _bool						    m_bInteractOn = { false };
    _float						    m_fAlphaValue = {};
    _bool                           m_bFadingIn = { false };
    _bool                           m_bFadingOut = { false };

public:
    static CSpecificObject*         Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject*            Clone(void* pArg) override;
    virtual void                    Free() override;
};
END