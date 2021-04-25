#include "stdafx.h"
#include "spriteProgress.h"

spriteProgress::spriteProgress(int posX, int posY,		// ���α׷����� ��ǥ
	int width, int height,								// ������
	int* maxValue, int* curValue,						// �ִ밪, ���簪(������)
	DWORD downColor,									// �� ĭ �÷�
	DWORD upColor,										// ä��ĭ �÷�
	DWORD outlineColor)									// �ܰ��� �÷�
{
	// �ؽ�ó ����(32 X 32 ��� �ؽ�ó)
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

	// �ٿ� ���� �۲� �߰�
	FONTMANAGER->addFont("barText", "������� ExtraBold ", 14);
}

void spriteProgress::release(void)
{
	if (_barTexture != NULL) SAFE_RELEASE(_barTexture);
}

void spriteProgress::render(void)
{
	// �ܰ����� �׷��� ���� ��Ʈ
	RECT outlineRect = MYUTIL::rectMake(_positionX, _positionY, _barWidth, _barHeight);
	// ���� �ܰ��� ��Ȱ�� �� ������ �׷��ش�.
	SPRITEMANAGER->spriteRender(_barTexture, &outlineRect, _positionX, _positionY, _outlineColor);

	// ����ִ� �ٰ� �׷��� ����
	RECT downRect = MYUTIL::rectMake(_positionX + OUTLINEWIDTH, _positionY + OUTLINEWIDTH,
		_barWidth - OUTLINEWIDTH, _barHeight - OUTLINEWIDTH);
	// ���� ����ִ� ĭ�� �׷� �� ��
	SPRITEMANAGER->spriteRender(_barTexture, &downRect, _positionX + 1, _positionY + 1, _downColor);

	// ä�����ִ� �ٸ� �׷��ش�.
	float compute = this->computeProgress();	// ���簪�� �ִ밪�� ������ ���´�.
	RECT upRect = MYUTIL::rectMake(_positionX + OUTLINEWIDTH, _positionY + OUTLINEWIDTH,
		(_barWidth - OUTLINEWIDTH) * compute, _barHeight - OUTLINEWIDTH);
	SPRITEMANAGER->spriteRender(_barTexture, &upRect, _positionX + 1, _positionY + 1, _upColor);

	// �� ���� ���
	string barStr = to_string(*_curValue) + string(" / ") + to_string(*_maxValue);
	int fontPosY = (_positionY + OUTLINEWIDTH) + ((_barHeight - OUTLINEWIDTH) / 2) - 7;
	FONTMANAGER->fontOutLine("barText", barStr, _positionX + 6, fontPosY, 0xffffffff, 0xff000000);
}
