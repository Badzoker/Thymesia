#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Client)

class CUIGroup_PlayerLevelUP final : public CUIObject
{
private:
	CUIGroup_PlayerLevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_PlayerLevelUP(const CUIGroup_PlayerLevelUP& Prototype);
	virtual ~CUIGroup_PlayerLevelUP() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_UIObject();
	

private:
	vector<UI_TextInfo> m_TextInfo = {};

public:
	static CUIGroup_PlayerLevelUP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END