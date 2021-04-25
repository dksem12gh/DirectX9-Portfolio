#pragma once
#include "baseProgressBar.h"

#define OUTLINEWIDTH  2
class spriteProgress :
	public baseProgressBar
{
private:
	int						_positionX;				// �� ��ġ
	int						_positionY;

	DWORD					_outlineColor;			// �ܰ��� ��

public:
	spriteProgress(int posX, int posY, 
		int width, int height, 
		int* maxValue, int* curValue, 
		DWORD downColor, DWORD upColor, DWORD outlineColor = 0xff000000);

	virtual void release(void) override;
	virtual void update(void) override {};
	void render(void);

	~spriteProgress() {};
};

