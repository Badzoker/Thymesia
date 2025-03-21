#pragma once

#include <process.h>

namespace Client
{
	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;

	enum LEVELID {
		LEVEL_STATIC, // UI, Monster, Effect, etc
		LEVEL_LOADING, // Loading(어디로 가냐에 따라 달라질듯)
		LEVEL_LOGO, // Intro Menu
		LEVEL_TUTORIAL, // 튜토맵
		LEVEL_SEAOFTREES, // 나무바다(서커스)
		LEVEL_ROYALGARDEN, // 왕실정원
		LEVEL_FORTRESS, // 헤르메스성채
		LEVEL_HILL, //철학자의 집
		LEVEL_END,
	};
	enum UISCENEID {
		UISCENE_MENU, UISCENE_LEVELUP, UISCENE_TALENT, UISCENE_INTRO,
		UISCENE_INVEN, UISCENE_PLAYERSCREEN, UISCENE_MONSTERHP, UISCENE_ALL, UISCENE_LOADING, UISCENE_END
	}; // UI 그룹 - 유빈

	enum TEXTURE_EFFECT { TEXTURE_DIFFUSE,	TEXTURE_NOISE, TEXTURE_MASK, TEXTURE_END }; //종한 Effect용도

	/* 컬링 거리 */
	const float g_fCullingDistance = 30.f;


}

using namespace Client;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;


