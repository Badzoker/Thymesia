#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CUI_Scene;
//class CUI_Text;
END

BEGIN(Client)

class CUIGroup_GameIntro final : public CUIObject
{
private:
	CUIGroup_GameIntro(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_GameIntro(const CUIGroup_GameIntro& Prototype);
	virtual ~CUIGroup_GameIntro() = default;

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
	CUI_Scene* m_pMyScene = {};

public:
	static CUIGroup_GameIntro* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
