#include "stdafx.h"
#include "fontManager.h"


HRESULT fontManager::init(void)
{
	//  디폴트로 사용할 폰트 객체 만들기
	D3DXCreateFont(
		device,							//  디바이스		
		20,								//  폰트의 높이(간단히 폰트크기)
		0,								//  폰트의 폭(0일때 높이에 비례해서 처리된다)
		FW_BOLD,						//  폰트두께 설정 (볼드스타일)
		1,								//  밉맵은 그냥 1
		FALSE,							//  이텍릭 여부(기울어져 있는 폰트)
		DEFAULT_CHARSET,				//  디폴트 캐릭터셋
		OUT_DEFAULT_PRECIS,				//  디폴트 외각처리방식
		DEFAULT_QUALITY,				//  디폴트 퀄리티
		DEFAULT_PITCH | FF_DONTCARE,	//  디폴트 피치엔 패밀리셋
		"돋움체",						//  사용될 폰트이름 그냥 한글로 폰트이름 쓰면 됨
		&_font);						//  생성된 폰트객체

	return S_OK;
}

void fontManager::release(void)
{
	// 디폴트용 폰트 해제
	SAFE_RELEASE(_font);
	// 맵에 담겨있는 폰트종류 전부 해제
	mapFontIter iter;
	for (iter = _mapFont.begin(); iter != _mapFont.end(); ++iter)
	{
		SAFE_RELEASE(iter->second);
	}
	_mapFont.clear();
}

void fontManager::addFont(string strKey, string fontName, int fontSize)
{
	//  이미 폰트 종류가 있으면 추가시키지 말고 함수를 그냥 빠져나온다
	mapFontIter find = _mapFont.find(strKey);
	if (find != _mapFont.end()) return;

	//  새로운폰트 객체 만들기
	LPD3DXFONT newFont;
	D3DXCreateFont(
		device,							//  디바이스		
		fontSize,						//  폰트의 높이(간단히 폰트크기)
		0,								//  폰트의 폭(0일때 높이에 비례해서 처리된다)
		FW_BOLD,						//  폰트두께 설정 (볼드스타일)
		1,								//  밉맵은 그냥 1
		FALSE,							//  이텍릭 여부(기울어져 있는 폰트)
		DEFAULT_CHARSET,				//  디폴트 캐릭터셋
		OUT_DEFAULT_PRECIS,				//  디폴트 외각처리방식
		DEFAULT_QUALITY,				//  디폴트 퀄리티
		DEFAULT_PITCH | FF_DONTCARE,	//  디폴트 피치엔 패밀리셋
		fontName.c_str(),				//  사용될 폰트이름 그냥 한글로 폰트이름 쓰면 됨
		&newFont);						//  생성된 폰트객체
									//  새로 만든 폰트를 맵에 담는다
	_mapFont.insert(make_pair(strKey, newFont));
}

void fontManager::fontOut(string str, int x, int y, DWORD color)
{
	//  출력영역(윈도우 좌표계)
	RECT rcTestArea = { x, y, 0, 0 };
	_font->DrawText(NULL, str.c_str(), -1, &rcTestArea, DT_LEFT | DT_NOCLIP, color);
}

void fontManager::fontOut(string strKey, string str, int x, int y, DWORD color)
{
	LPD3DXFONT font = NULL;
	//  폰트종류가 키값을 못찾았다면 디폴트용 폰트로 출력을 하고 찾았다면 찾은 폰트로 출력시키기
	mapFontIter find = _mapFont.find(strKey);
	if (find == _mapFont.end()) font = _font;
	else font = find->second;

	//  출력영역(윈도우 좌표계)
	RECT rcTestArea = { x, y, 0, 0 };
	font->DrawText(NULL, str.c_str(), -1, &rcTestArea, DT_LEFT | DT_NOCLIP, color);
}

void fontManager::fontOutShadow(string str, int x, int y, DWORD color, DWORD shadowColor)
{
	//  쉐도우 먼저 그리자
	fontOut(str, x + 1, y + 1, shadowColor);
	//  본래 텍스트를 그린다
	fontOut(str, x, y, color);
}

void fontManager::fontOutShadow(string strKey, string str, int x, int y, DWORD color, DWORD shadowColor)
{
	//  쉐도우 먼저 그리자
	fontOut(strKey, str, x + 1, y + 1, shadowColor);
	//  본래 텍스트를 그린다
	fontOut(strKey, str, x, y, color);
}

void fontManager::fontOutLine(string str, int x, int y, DWORD color, DWORD outLineColor)
{
	//  쉐도우 먼저 그리자
	fontOut(str, x + 1, y, outLineColor);
	fontOut(str, x, y + 1, outLineColor);
	fontOut(str, x - 1, y, outLineColor);
	fontOut(str, x, y - 1, outLineColor);
	//  본래 텍스르를 그린다
	fontOut(str, x, y, color);
}

void fontManager::fontOutLine(string strKey, string str, int x, int y, DWORD color, DWORD outLineColor)
{
	// 쉐도우 먼저 그리자
	fontOut(strKey, str, x + 1, y, outLineColor);
	fontOut(strKey, str, x, y + 1, outLineColor);
	fontOut(strKey, str, x - 1, y, outLineColor);
	fontOut(strKey, str, x, y - 1, outLineColor);
	// 본래 텍스르를 그린다
	fontOut(strKey, str, x, y, color);
}
