#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
BEGIN(Client)

class CUIGroup_PlayerScreen final : public CUIObject
{
private:
	CUIGroup_PlayerScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_PlayerScreen(const CUIGroup_PlayerScreen& Prototype);
	virtual ~CUIGroup_PlayerScreen() = default;

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
	static CUIGroup_PlayerScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

