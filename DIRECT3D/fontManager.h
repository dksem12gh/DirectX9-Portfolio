#pragma once
#include "singletonBase.h"

class fontManager : public singletonBase <fontManager>
{
private:
	typedef map<string, LPD3DXFONT>	mapFont;
	typedef map<string, LPD3DXFONT> ::iterator mapFontIter;

private:
	LPD3DXFONT		_font;		//  디폴트용 폰트
	mapFont			_mapFont;	//  맵안에 담아둘 폰트(폰트 종류를 반드시 지정 할 것)



public:
	HRESULT	init(void);
	void	release(void);

	//  폰트 종류 추가
	void	addFont(string strKey, string fontName, int fontSize);

	//  디폴트 폰트를 출력 (키값을 추가하면 맵에 담긴 폰트로 출력 함)
	void	fontOut(string str, int x, int y, DWORD color);
	void	fontOut(string strKey, string str, int x, int y, DWORD color);
	void	fontOutShadow(string str, int x, int y, DWORD color, DWORD shadowColor = 0xff000000);
	void	fontOutShadow(string strKey, string str, int x, int y, DWORD color, DWORD shaDowColor = 0xff000000);
	void	fontOutLine(string str, int x, int y, DWORD color, DWORD outLineColor = 0xff000000);
	void	fontOutLine(string strKey, string str, int x, int y, DWORD color, DWORD outLineColor = 0xff000000);



	fontManager() {};
	~fontManager() {};
};

