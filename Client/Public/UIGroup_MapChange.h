#pragma once
#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CUI_Scene;
END

BEGIN(Client)

class CUIGroup_MapChange final : public CUIObject
{
private:
	CUIGroup_MapChange(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIGroup_MapChange(const CUIGroup_MapChange& Prototype);
	virtual ~CUIGroup_MapChange() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:


public:
	HRESULT Ready_UIObject();
	HRESULT LoadData_UIObject(_uint iLevelIndex, _uint iSceneIndex, const _tchar* szSceneName);
	HRESULT LoadData_UIText_Info(const _tchar* szSceneName);
private:
	vector<UI_TextInfo> m_TextInfo = {};

	CUI_Scene* m_pMyScene = {};
	CUI_Scene* m_pMapChangePop = {};
	_bool m_bPopOpen = { false };
	_tchar* m_pPoptitle = {};

	LEVELID m_eNextLevel = {};


	LEVELID m_eMyLevel = {};
public:
	static CUIGroup_MapChange* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
