#pragma once
#include "stdafx.h"

class baseProgressBar
{
protected:
	LPDIRECT3DTEXTURE9		_barTexture;			// �� �ؽ�ó
	int						_barWidth;				// �� ����
	int						_barHeight;				// �� ����
	int*					_maxValue;				// �ִ밪
	int*					_curValue;				// ���簪
	DWORD					_downColor;				// ��ĭ ��
	DWORD					_upColor;				// ä��ĭ ��

	float computeProgress() { return ((float)*_curValue / (float)*_maxValue); }

public:
	virtual void release() PURE;
	virtual void update() PURE;

	baseProgressBar() : _barTexture(NULL) {};
	~baseProgressBar() {};
};

