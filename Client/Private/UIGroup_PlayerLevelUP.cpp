#include "pch.h"
#include "UIGroup_PlayerLevelUP.h"
#include "UI_Scene.h"
#include "GameInstance.h"

CUIGroup_PlayerLevelUP::CUIGroup_PlayerLevelUP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CUIGroup_PlayerLevelUP::CUIGroup_PlayerLevelUP(const CUIGroup_PlayerLevelUP& Prototype)
	: CUIObject( Prototype )
{
}

HRESULT CUIGroup_PlayerLevelUP::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::Initialize(void* pArg)
{

	if (FAILED(Ready_UIObject()))
		return E_FAIL;

	return S_OK;
}

void CUIGroup_PlayerLevelUP::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CUIGroup_PlayerLevelUP::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
	if (m_bRenderOpen)
	{
		m_pGameInstance->UIScene_UIObject_Render_OnOff((m_pGameInstance->Find_UIScene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP")), true);
	}
}

void CUIGroup_PlayerLevelUP::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_FONT, this);
}

HRESULT CUIGroup_PlayerLevelUP::Render()
{
	if (m_bRenderOpen)
	{
		vector<UI_TextInfo>::iterator it;
		for (it = m_TextInfo.begin(); it != m_TextInfo.end(); it++)
		{
			m_pGameInstance->Render_Font(it->strFontName.c_str(), it->srtTextContent.c_str(), it->fTextStartPos);

		}
	}
	return S_OK;
}

HRESULT CUIGroup_PlayerLevelUP::Ready_UIObject()
{
	//LoadData_UI_Scene(UISCENE_LEVELUP, L"UIScene_PlayerLevelUP");

	//CUIObject::UIOBJECT_DESC Desc{};


	//if (FAILED(m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY, L"Prototype_GameObject_UI_MouseCursor", UISCENE_LEVELUP, L"UIScene_PlayerLevelUP", UI_IMAGE, &Desc)))
	//return S_OK;

	/*CUIObject::UIOBJECT_DESC Desc{};
	Desc.fSizeX = 400.f;
	Desc.fSizeY = 900.f;
	Desc.fX = 300.f;
	Desc.fY = 450.f;
	m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY,TEXT("Prototype_GameObject_UI_Component"),UISCENE_LEVELUP,TEXT("UIScene_LevelUP"),CUI_Scene::UI_IMAGE,&Desc);

	m_pGameInstance->Add_UIObject_To_UIScene(LEVEL_GAMEPLAY,TEXT("Prototype_GameObject_UI_Slot_Attribute"),UISCENE_LEVELUP,TEXT("UIScene_LevelUP"),CUI_Scene::UI_BUTTONPLAYER,&Desc);

	*/

	m_pGameInstance->LoadDataFile_UIObj_Info(g_hWnd, LEVEL_GAMEPLAY, UISCENE_LEVELUP, L"UIScene_PlayerLevelUP");
	//m_pGameInstance->LoadDataFile_UIText_Info(g_hWnd, L"UIScene_PlayerLevelUP", m_TextInfo);
	
	//CUI_Component::UI_COMPONENT_DESC Desc{};
//__super::Add_UI_Type(CUI_Scene::UITYPE::UI_IMAGE, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Component"), &Desc);

//CAttribute_Button::BUTTON_DESC ButtonDesc{};
//__super::Add_UI_Type(CUI_Scene::UITYPE::UI_BUTTONPLAYER, LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Slot_Attribute"), &ButtonDesc);

	return S_OK;
}

CUIGroup_PlayerLevelUP* CUIGroup_PlayerLevelUP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUIGroup_PlayerLevelUP* pInstance = new CUIGroup_PlayerLevelUP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CUIGroup_PlayerLevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUIGroup_PlayerLevelUP::Clone(void* pArg)
{
	CUIGroup_PlayerLevelUP* pInstance = new CUIGroup_PlayerLevelUP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CUIGroup_PlayerLevelUP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIGroup_PlayerLevelUP::Free()
{
	__super::Free();
}
