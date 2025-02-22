#pragma once

#include <process.h>

namespace Client 
{
	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
	enum UISCENEID { UISCENE_MENU, UISCENE_LEVELUP, UISCENE_ATTRIBUTE, UISCENE_END }; // UI 그룹 - 유빈

	/* 컬링 거리 */
	const float g_fCullingDistance = 30.f; 
	

}

using namespace Client;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;	


