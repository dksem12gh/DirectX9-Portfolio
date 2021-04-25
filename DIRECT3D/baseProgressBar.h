#pragma once
#include "stdafx.h"

class baseProgressBar
{
protected:
	LPDIRECT3DTEXTURE9		_barTexture;			// 바 텍스처
	int						_barWidth;				// 바 넓이
	int						_barHeight;				// 바 높이
	int*					_maxValue;				// 최대값
	int*					_curValue;				// 현재값
	DWORD					_downColor;				// 빈칸 색
	DWORD					_upColor;				// 채운칸 색

	float computeProgress() { return ((float)*_curValue / (float)*_maxValue); }

public:
	virtual void release() PURE;
	virtual void update() PURE;

	baseProgressBar() : _barTexture(NULL) {};
	~baseProgressBar() {};
};

