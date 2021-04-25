#include "stdafx.h"
#include "spriteProgress.h"

spriteProgress::spriteProgress(int posX, int posY,		// 프로그래스바 좌표
	int width, int height,								// 사이즈
	int* maxValue, int* curValue,						// 최대값, 현재값(포인터)
	DWORD downColor,									// 빈 칸 컬러
	DWORD upColor,										// 채운칸 컬러
	DWORD outlineColor)									// 외곽선 컬러
{
	// 텍스처 생성(32 X 32 백색 텍스처)
	D3DXCreateTextureFromFile(device,
		"Resources/mabinogi/UI/whiteBlock.dds",
		&_barTexture);

	_positionX = posX;
	_positionY = posY;
	_barWidth = width;
	_barHeight = height;
	_maxValue = maxValue;
	_curValue = curValue;
	_downColor = downColor;
	_upColor = upColor;
	_outlineColor = outlineColor;

	// 바에 사용될 글꼴 추가
	FONTMANAGER->addFont("barText", "나눔고딕 ExtraBold ", 14);
}

void spriteProgress::release(void)
{
	if (_barTexture != NULL) SAFE_RELEASE(_barTexture);
}

void spriteProgress::render(void)
{
	// 외곽선이 그려질 영역 렉트
	RECT outlineRect = MYUTIL::rectMake(_positionX, _positionY, _barWidth, _barHeight);
	// 먼저 외곽선 역활을 할 영역을 그려준다.
	SPRITEMANAGER->spriteRender(_barTexture, &outlineRect, _positionX, _positionY, _outlineColor);

	// 비어있는 바가 그려질 영역
	RECT downRect = MYUTIL::rectMake(_positionX + OUTLINEWIDTH, _positionY + OUTLINEWIDTH,
		_barWidth - OUTLINEWIDTH, _barHeight - OUTLINEWIDTH);
	// 먼저 비어있는 칸을 그려 준 후
	SPRITEMANAGER->spriteRender(_barTexture, &downRect, _positionX + 1, _positionY + 1, _downColor);

	// 채워져있는 바를 그려준다.
	float compute = this->computeProgress();	// 현재값과 최대값을 비율을 얻어온다.
	RECT upRect = MYUTIL::rectMake(_positionX + OUTLINEWIDTH, _positionY + OUTLINEWIDTH,
		(_barWidth - OUTLINEWIDTH) * compute, _barHeight - OUTLINEWIDTH);
	SPRITEMANAGER->spriteRender(_barTexture, &upRect, _positionX + 1, _positionY + 1, _upColor);

	// 갑 정보 출력
	string barStr = to_string(*_curValue) + string(" / ") + to_string(*_maxValue);
	int fontPosY = (_positionY + OUTLINEWIDTH) + ((_barHeight - OUTLINEWIDTH) / 2) - 7;
	FONTMANAGER->fontOutLine("barText", barStr, _positionX + 6, fontPosY, 0xffffffff, 0xff000000);
}
