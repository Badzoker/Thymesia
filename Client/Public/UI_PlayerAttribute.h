
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Client)

class CUI_PlayerAttribute final : public CUIObject
{
private:
	CUI_PlayerAttribute(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PlayerAttribute(const CUI_PlayerAttribute& Prototype);
	virtual ~CUI_PlayerAttribute() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UI_Scene(_uint iSceneIndex, const _tchar* szSceneName); // 현재 UI 불러오기

private:

public:
	static CUI_PlayerAttribute* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
