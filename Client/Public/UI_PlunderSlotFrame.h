#pragma once

#include "Client_Defines.h"
#include "UI_Image.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_PlunderSlotFrame final : public CUI_Image
{
private:
	CUI_PlunderSlotFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PlunderSlotFrame(const CUI_PlunderSlotFrame& Prototype);
	virtual ~CUI_PlunderSlotFrame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Set_TexIcon(_uint iNum) { m_iTexicon = iNum; }
	void Set_SkillOn(_bool bUse) { m_bSkillOn = bUse; }

private:
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	CTexture* m_pTexIconCom = { nullptr };

	_uint m_iTexicon = {0};
	_bool m_bItemIconOn = { true }; // 아이콘 onoff
	_bool m_bSkillOn = { false }; // 스킬 사용

	_float m_fSkill_CoolTime = {};

public:
	HRESULT Ready_Components();

public:
	static CUI_PlunderSlotFrame* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END