#include "stdafx.h"
#include "dyeing.h"

HRESULT dyeing::init(void)
{
	// ���� �ķ�Ʈ �ε�
	_dyeingPallet = RM_TEXTURE->addResource("Resources/mabinogi/UI/pallete.dds");
	// UI �̹��� �ε�
	_ui = RM_TEXTURE->addResource("Resources/mabinogi/UI/dollpink.dds");
	// ��ư �̹��� �ε�
	_button = RM_TEXTURE->addResource("Resources/mabinogi/UI/button.dds");
	// ������ ��ư �̹��� �ε�
	_inButton = RM_TEXTURE->addResource("Resources/mabinogi/UI/inButton.dds");
	// �����̵� �÷� ��¿� �ؽ�ó ��� �ε�
	_spoidTex = RM_TEXTURE->addResource("Resources/mabinogi/UI/whiteBlock.dds");

	// ������ ��� ��ġ
	_windowPos = { WINSIZEX - 520, 50 };
	// ������ ������Ʈ
	_baseWindowRC = MYUTIL::rectMake(_windowPos.x, _windowPos.y, 480, 540);

	D3DSURFACE_DESC desc;
	_dyeingPallet->GetLevelDesc(0, &desc); // �ȷ�Ʈ �̹��� ������ ����.
										   // �ȷ�Ʈ ������Ʈ
	_palletWindowRC = MYUTIL::rectMake(_baseWindowRC.right - 22 - desc.Width, _baseWindowRC.top + 60, desc.Width, desc.Height);

	// UI �̹������� �ʿ��� �κи��� �ڸ��� ���� ��Ʈ��
	_windowSourRC = { 0, 0, 14, 41 };
	_windowSourCenterRC = { 4, 3, 12, 31 };
	_slotRC = MYUTIL::rectMake(_baseWindowRC.left + 20, _palletWindowRC.top, 80, 128);
	_slotSourRC = { 378, 122, 399, 143 };
	_slotSourCenterRC = { 379, 123, 398, 142 };
	for (int i = 0; i < 5; i++)
	{
		_sampleItem[i] = NULL;
		_sampleSlotRC[i] = MYUTIL::rectMake(_slotRC.left + (i * (MYUTIL::getRectSize(&_slotRC).x + 8)), _palletWindowRC.bottom + 20, 80, 128);
	}

	// ��Ʈ��ư ����
	_partButton[0] = MYUTIL::rectMake(_sampleSlotRC[0].left, _sampleSlotRC[0].top - 135, 90, 40);
	_partButton[1] = MYUTIL::rectMake(_sampleSlotRC[0].left, _sampleSlotRC[0].top - 90, 90, 40);
	_partButton[2] = MYUTIL::rectMake(_sampleSlotRC[0].left, _sampleSlotRC[0].top - 45, 90, 40);
	_partName[0] = "��Ʈ A";
	_partName[1] = "��Ʈ B";
	_partName[2] = "��Ʈ C";

	// ���� ��ư�� ���� ��ư ���� ����
	_startButtonRC = MYUTIL::rectMake(_baseWindowRC.right - 180 - 40, _baseWindowRC.bottom - 60, 90, 40);
	_cancelButtonRC = MYUTIL::rectMake(_baseWindowRC.right - 90 - 20, _baseWindowRC.bottom - 60, 90, 40);

	// ���� ��Ʈ ǥ�ÿ� ���� ����
	_glitterColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	_glitterValue = -0.05f;


	_pickSelectColor = false;
	_clickPallet = false;
	return S_OK;
}

void dyeing::release(void)
{
	this->endDyeing();
}

void dyeing::update(void)
{
	// ���� ��Ʈ ǥ�ÿ� �߰��÷����� ���ĸ� ��������ŭ �����ش�.
	_glitterColor.a += _glitterValue;

	// ���� ���İ� 1.0�� �Ѱų� 0.0���� �۾��� ��� �������� ��ȣ�� ����
	// (�̷ν� ���� ��Ʈ ǥ�úΰ� ��¦�δ�.)
	if (_glitterColor.a > 1.0f || _glitterColor.a < 0.0f)
	{
		_glitterValue *= -1;
	}
}

void dyeing::showWindows()
{
	if (_dyeingItem == NULL) return;

	// ������ �̹����� ����� �޾Ƴ��´�.
	D3DXVECTOR2			sampleItemSize = { (FLOAT)_dyeingItem->getFrameWidth(), (FLOAT)_dyeingItem->getFrameHeight() };

	// �������̽� �����츦 ������ġ������ �׷��ش�.
	SPRITEMANAGER->ninePatchRender(_ui, &_windowSourRC, &_baseWindowRC, &_windowSourCenterRC, 0xf000ffff);
	SPRITEMANAGER->ninePatchRender(_ui, &_slotSourRC, &_slotRC, &_slotSourCenterRC, 0xf000ffff);

	// ���� ��� �̹����� ������ ���߾ӿ� ���߱� ���Ͽ�...
	D3DXVECTOR2 sampleItemPos;
	sampleItemPos.x = (_slotRC.left + (MYUTIL::getRectSize(&_slotRC).x / 2))
		- (sampleItemSize.x / 2);
	sampleItemPos.y = (_slotRC.top + (MYUTIL::getRectSize(&_slotRC).y / 2))
		- (sampleItemSize.y / 2);

	// ���� �ȷ�Ʈ ����
	SPRITEMANAGER->spriteRender(_dyeingPallet, NULL, _palletWindowRC.left, _palletWindowRC.top);

	// ��������Ʈ ���� ��ü�� ���׷� ���� �̹������� ũ�� �׷����� ������ 
	// (���ϴ� ��ư ũ�� / �ҽ��� �ҷ��� �̹����� ũ��)�� ����Ͽ� ������ ���Ѵ�.
	int buttonRCWidth = MYUTIL::getRectSize(&_partButton[0]).x;
	int buttonRCHeight = MYUTIL::getRectSize(&_partButton[0]).y;

	D3DSURFACE_DESC desc;
	_inButton->GetLevelDesc(0, &desc);
	float sizeWidth = (float)buttonRCWidth / (float)desc.Width;
	float sizeHeight = (float)buttonRCHeight / (float)desc.Height;


	for (int i = 0; i < 3; i++)
	{
		// ������ ��Ʈ�� ���� ��ư�� ��ġ�ϸ� �� �̹����� ����
		// �� �� �̹����� �������� ������ ������ ������ �����Ѵ�.
		if (_texNumber == i)
		{
			SPRITEMANAGER->spriteRender(_inButton, NULL, _partButton[i].left, _partButton[i].top, sizeWidth, sizeHeight, 0.0f, 0xf000ffff);
		}
		else
		{
			SPRITEMANAGER->spriteRender(_button, NULL, _partButton[i].left, _partButton[i].top, sizeWidth, sizeHeight, 0.0f, 0xf000ffff);
		}

		// ��Ʈ ��ġ�� ���缭 ��Ʈ ����
		D3DXVECTOR2 fontPos;
		fontPos.x = _partButton[i].left + (MYUTIL::getRectSize(&_partButton[i]).x / 2) - ((int)(_partName[i].length() * 6.5f) / 2);
		fontPos.y = _partButton[i].top + (MYUTIL::getRectSize(&_partButton[i]).y / 2) - 6.5f;

		if (_dyeingItem->PartCount() - 1 >= i)
		{
			FONTMANAGER->fontOut("tap1", _partName[i], fontPos.x, fontPos.y, ACTIVEFONT);
		}
		else
		{
			FONTMANAGER->fontOut("tap1", _partName[i], fontPos.x, fontPos.y, INACTIVEFONT);
		}

	}
	// ������ ��� �̹����� ����
	_dyeingItem->renderImage(sampleItemPos.x, sampleItemPos.y);
	// ��¦�̴� ȿ���� ���Ͽ� ��� �̹����� �����̹���(������ ���̴� �������� ���� �������� �̹���)�� ���� �������.
	_dyeingItem->renderImage(sampleItemPos.x, sampleItemPos.y, _texNumber, _glitterColor);

	// 5���� ���� ���԰� �̹����� ����
	for (int i = 0; i < 5; i++)
	{
		SPRITEMANAGER->ninePatchRender(_ui, &_slotSourRC, &_sampleSlotRC[i], &_slotSourCenterRC, 0xf000ffff);
		D3DXVECTOR2 pos;
		pos.x = (_sampleSlotRC[i].left + (MYUTIL::getRectSize(&_sampleSlotRC[i]).x / 2))
			- (sampleItemSize.x / 2);
		pos.y = (_sampleSlotRC[i].top + (MYUTIL::getRectSize(&_sampleSlotRC[i]).y / 2))
			- (sampleItemSize.y / 2);
		_sampleItem[i]->renderImage(pos.x, pos.y);
	}

	// ���� ��ư
	SPRITEMANAGER->spriteRender(_button, NULL, _startButtonRC.left, _startButtonRC.top, sizeWidth, sizeHeight, 0.0f, 0xf000ffff);
	FONTMANAGER->fontOut("tap1", "���� ����",
		_startButtonRC.left + (MYUTIL::getRectSize(&_startButtonRC).x / 2) - ((9 * 6.5f) / 2),
		_startButtonRC.top + (MYUTIL::getRectSize(&_startButtonRC).y / 2) - 6.5f, ACTIVEFONT);

	// ���� ��ư
	SPRITEMANAGER->spriteRender(_button, NULL, _cancelButtonRC.left, _cancelButtonRC.top, sizeWidth, sizeHeight, 0.0f, 0xf000ffff);
	FONTMANAGER->fontOut("tap1", "���",
		_cancelButtonRC.left + (MYUTIL::getRectSize(&_cancelButtonRC).x / 2) - ((4 * 6.5f) / 2),
		_cancelButtonRC.top + (MYUTIL::getRectSize(&_cancelButtonRC).y / 2) - 6.5f, ACTIVEFONT);

	if (_isShow && _clickPallet == false)
	{
		string str = "������ �ϽǷ��� �ȷ�Ʈ�� Ŭ���ϼ���.";
		int fontPosX = (WINSIZEX / 2) - ((str.length() * 14) / 2);
		FONTMANAGER->fontOutLine("systemFont", str, fontPosX, 300, 0xffffffff, 0xff000000);
	}
}


void dyeing::dyeingColor(LPDIRECT3DTEXTURE9 originTexture, LPDIRECT3DTEXTURE9 diffuseTexutre, DWORD color, float alpha)
{
	DWORD levelCount = originTexture->GetLevelCount(); // �ؽ�ó�� ������ ��

	LPDIRECT3DSURFACE9 originSurface = NULL;		// ���� �ؽ�ó�� ǥ�� ����
	LPDIRECT3DSURFACE9 diffuseSurface = NULL;		// ������ �ؽ�ó�� ǥ�� ����

	D3DSURFACE_DESC desc;	// �ؽ�ó�� �̹��� ����

	for (int i = 0; i < (int)levelCount; i++)				// �ؽ�ó�� ���� ����ŭ �ݺ�
	{
		originTexture->GetLevelDesc(i, &desc);				// ���� ������ ���� �ؽ�ó �̹��� ������ ���´�.
		originTexture->GetSurfaceLevel(i, &originSurface);	// ���� ������ ���� �ؽ�ó�� ǥ���� ���´�.
		diffuseTexutre->GetSurfaceLevel(i, &diffuseSurface);	// ���� ������ ������ �ؽ�ó ������ ���´�.
		D3DLOCKED_RECT originLockRect, diffuseLockRect;			// ǥ���� ��״� ����ü

		originSurface->LockRect(&originLockRect, NULL, D3DLOCK_READONLY);	// �ؽ�ó�� ��ٴ�. ������ ������ �����⸸ �ϱ⿡ READONLY �÷��׸� ���
		DWORD* originImgData = (DWORD*)(originLockRect.pBits);				// �ؽ�ó�� ��Ʈ�� ������ DWORD �迭�� ��ȯ�Ѵ�.

		diffuseSurface->LockRect(&diffuseLockRect, NULL, D3DLOCK_DISCARD);
		DWORD* diffuseImgData = (DWORD*)(diffuseLockRect.pBits);

		// ���İ� ����
		if (alpha > 1.0f) alpha = 1.0f;
		if (alpha < 0.0f) alpha = 0.0f;

		for (int y = 0; y < desc.Height; y++)			// �ؽ�ó ���� �����ŭ �ݺ�
		{
			for (int x = 0; x < desc.Width; x++)		// �ؽ�ó ���� �����ŭ �ݺ� 
			{
				int index = (y * desc.Width) + x;		// �ȼ� �ε���

														/*
														�ȼ��� ������ D3DXCOLOR�� ��ȯ�Ѵ�.
														�ؽ�ó�� ������ 32bit RGBA�� �ƴϸ� ������ �߻��Ѵ�. (!! �߿� !!)
														*/
				D3DXCOLOR targetColor = originImgData[index];
				D3DXCOLOR dyeingColor = color;
				if (targetColor.r == targetColor.b) //R, B�� �����ϸ� �����Ǵ� �ȼ��̴�.
				{
					/*
					D3DXCOLOR�� �� �÷����� 0 ~ 1 ���̷� �����ǹǷ� ���ϴ� ���� �����ָ� �ڿ������� �����ȴ�.
					*/

					D3DXCOLOR newColor;
					newColor.r = dyeingColor.r * targetColor.r;
					newColor.g = dyeingColor.g * targetColor.g;
					newColor.b = dyeingColor.b * targetColor.b;
					newColor.a = 1.0f;

					// ���Ӱ� ������ �÷����� ������ �ؽ�ó�� �ȼ����� �����Ѵ�.
					diffuseImgData[index] = newColor;
				}
			}
		}

		originSurface->UnlockRect();		// ǥ�� ���� ����� �����Ѵ�.
		originSurface->Release();			// ǥ�� ���� ������
		diffuseSurface->UnlockRect();
		diffuseSurface->Release();
	}
}

void dyeing::dyeingImage(LPDIRECT3DTEXTURE9 originTexture, LPDIRECT3DTEXTURE9 diffuseTexutre, int partIndex, int width, DWORD color)
{
	if (originTexture == NULL || diffuseTexutre == NULL) return;
	LPDIRECT3DSURFACE9 originSurface = NULL;		// ���� �ؽ�ó�� ǥ�� ����
	LPDIRECT3DSURFACE9 diffuseSurface = NULL;		// ������ �ؽ�ó�� ǥ�� ����
	D3DSURFACE_DESC desc;	// �ؽ�ó�� �̹��� ����
	originTexture->GetSurfaceLevel(0, &originSurface);	// ���� �ؽ�ó�� ǥ���� ���´�.
	originTexture->GetLevelDesc(0, &desc);				// ���� �ؽ�ó�� �̹��� ������ ���´�.
	diffuseTexutre->GetSurfaceLevel(0, &diffuseSurface);	// ������ �ؽ�ó�� ������ ���´�.
	D3DLOCKED_RECT originLockRect, diffuseLockRect;			// ǥ���� ��״� ����ü

	originSurface->LockRect(&originLockRect, NULL, D3DLOCK_READONLY);	// �ؽ�ó�� ��ٴ�. ������ ������ �����⸸ �ϱ⿡ READONLY �÷��׸� ���
	DWORD* originImgData = (DWORD*)(originLockRect.pBits);				// �ؽ�ó�� ��Ʈ�� ������ DWORD �迭�� ��ȯ�Ѵ�.

	diffuseSurface->LockRect(&diffuseLockRect, NULL, D3DUSAGE_WRITEONLY);
	DWORD* diffuseImgData = (DWORD*)(diffuseLockRect.pBits);

	for (int y = 0; y < desc.Height; ++y)			// �ؽ�ó ���� �����ŭ �ݺ�
	{
		for (int x = partIndex * width; x < partIndex * width + width; ++x)		// �ؽ�ó ���� �����ŭ �ݺ� 
		{
			int index = (y * desc.Width) + x;		// �ȼ� �ε���

													/*
													�ȼ��� ������ D3DXCOLOR�� ��ȯ�Ѵ�.
													�ؽ�ó�� ������ 32bit RGBA�� �ƴϸ� ������ �߻��Ѵ�. (!! �߿� !!)
													*/
			D3DXCOLOR targetColor = D3DXCOLOR(originImgData[index]);
			D3DXCOLOR dyeingColor = color;
			/*
			D3DXCOLOR�� �� �÷����� 0 ~ 1 ���̷� �����ǹǷ� ���ϴ� ���� �����ָ� �ڿ������� �����ȴ�.
			*/

			D3DXCOLOR newColor;
			newColor.r = dyeingColor.r * targetColor.r;
			newColor.g = dyeingColor.g * targetColor.g;
			newColor.b = dyeingColor.b * targetColor.b;
			newColor.a = targetColor.a;

			// ���Ӱ� ������ �÷����� ������ �ؽ�ó�� �ȼ����� �����Ѵ�.
			diffuseImgData[index] = newColor;
		}
	}

	originSurface->UnlockRect();		// ǥ�� ���� ����� �����Ѵ�.
	originSurface->Release();			// ǥ�� ���� ������
	diffuseSurface->UnlockRect();
	diffuseSurface->Release();
}

DWORD dyeing::getPixelColor(POINT point)
{
	if (!PtInRect(&_palletWindowRC, point)) return NULL;

	LPDIRECT3DSURFACE9 surface = NULL;
	D3DSURFACE_DESC desc;

	_dyeingPallet->GetSurfaceLevel(0, &surface);
	_dyeingPallet->GetLevelDesc(0, &desc);

	// ���콺�� Ŭ���� ����Ʈ�� �ȷ�Ʈ ��� ��ġ�� ���� �����Ѵ�.
	D3DLOCKED_RECT lockRect;

	POINT palletePos;
	palletePos.x = point.x - _palletWindowRC.left;
	palletePos.y = point.y - _palletWindowRC.top;
	surface->LockRect(&lockRect, NULL, D3DLOCK_READONLY);
	DWORD* screenData = (DWORD*)lockRect.pBits;
	DWORD color = screenData[palletePos.y * desc.Width + palletePos.x];

	surface->UnlockRect();
	surface->Release();

	return color;
}

void dyeing::setItem(equip * item)
{
	if (item == NULL) return;

	_dyeingItem = item;
	this->setDyeingPart(0);
	for (int i = 0; i < 5; i++)
	{
		_sampleItem[i] = _dyeingItem->cloneItem();
	}
}

void dyeing::endDyeing(void)
{
	_dyeingItem = NULL;
	for (int i = 0; i < 5; i++)
	{
		if (_sampleItem[i] != NULL)
		{
			_sampleItem[i]->release();
			SAFE_DELETE(_sampleItem[i]);
		}

	}

	_pickSelectColor = false;
	_clickPallet = false;
}

void dyeing::setDyeingPart(int partNumber)
{
	_texNumber = partNumber;
}

void dyeing::EventMouseOnPallet()
{
	if (!_clickPallet)
	{
		OPTION->setCursor(defaultCursor);
		return;
	}
	if (_pickSelectColor)
	{
		OPTION->setCursor(defaultCursor);
		return; // �̹� ���� ���õǾ����� ����
	}
	if (_dyeingItem == NULL) return;
	POINT spoidPoint[5];		// ���� ����Ʈ 5�ڸ�
	spoidPoint[0] = MYUTIL::getMousePos();
	spoidPoint[1] = { spoidPoint[0].x - 40, spoidPoint[0].y - 40 };
	spoidPoint[2] = { spoidPoint[0].x + 20, spoidPoint[0].y - 36 };
	spoidPoint[3] = { spoidPoint[0].x - 48, spoidPoint[0].y + 40 };
	spoidPoint[4] = { spoidPoint[0].x + 28, spoidPoint[0].y + 40 };

	// �߾� �����͸� ������ �����͵��� �ȷ�Ʈ ������ �Ѿ�� �����ϰ�(����ó��) ����
	for (int i = 1; i < 5; i++)
	{
		if (spoidPoint[i].x < _palletWindowRC.left)
		{
			spoidPoint[i].x += MYUTIL::getRectSize(&_palletWindowRC).x;
		}
		else if (spoidPoint[i].x > _palletWindowRC.right)
		{
			spoidPoint[i].x -= MYUTIL::getRectSize(&_palletWindowRC).x;
		}

		if (spoidPoint[i].y < _palletWindowRC.top)
		{
			spoidPoint[i].y += MYUTIL::getRectSize(&_palletWindowRC).y;
		}
		else if (spoidPoint[i].y > _palletWindowRC.bottom)
		{
			spoidPoint[i].y -= MYUTIL::getRectSize(&_palletWindowRC).y;
		}
	}

	// Ŀ���� �����̵� Ŀ���� ��ȯ
	OPTION->setCursor(spoidCursor);

	// �����̵�� �� 5���� �׸���.
	for (int i = 0; i < 5; i++)
	{
		// �����̵� �÷��� ǥ�õ� ��Ʈ ����
		RECT colorRC = MYUTIL::rectMake(0, 0, 10, 11);

		// �����̵带 �׷��ش�.
		OPTION->renderCursor(spoidPoint[i]);
		// ���� �����̵��� �÷����� ���´�.
		_selectColor[i] = this->getPixelColor(spoidPoint[i]);
		// ���� �̹����� ���� ������.
		_sampleItem[i]->dyeingColor(_texNumber, _selectColor[i]);
		// ��������Ʈ ���������� �� ������ ABGR���̶� Red�� Blue�� ��ġ�� ����
		D3DXCOLOR sampleColor = D3DXCOLOR(_selectColor[i]);
		float tempR = sampleColor.r;
		sampleColor.r = sampleColor.b;
		sampleColor.b = tempR;
		// �����̵� �÷��� ������ ��ġ�� ����
		SPRITEMANAGER->spriteRender(_spoidTex, &colorRC, spoidPoint[i].x + 11, spoidPoint[i].y - 5, DWORD(sampleColor));
	}

	// Ŀ���� �⺻ Ŀ����
	OPTION->setCursor(defaultCursor);
}

void dyeing::EventPickColor()
{
	// ���� ������ ���� �ʾҴٸ� ���� ����
	if (!_clickPallet) _clickPallet = true;
	// �̹� ������ ���۵Ǿ��ٸ� �� ����
	else _pickSelectColor = true;
}

void dyeing::EventClickStartButton()
{
	// ������ ���۵��� �ʾҴٸ� ����
	if (!_pickSelectColor) return;

	// 5���� ���� ���ڸ� �̴´�.
	int pickNumber = MYUTIL::randomIntRange(0, 4);

	// �������� �����ϰ� ���� ����� ȭ�鿡 ���
	_dyeingItem->dyeingColor(_texNumber, _selectColor[pickNumber]);

	EFFECTMANAGER->changeSystemMsg(to_string(pickNumber + 1) + " �� ������ �����Ǿ����ϴ�.");

	// ���� ������ �����ϰ� ����â�� ����
	this->endDyeing();
	_isShow = false;
}

void dyeing::EventClickCancelButton()
{
	// ���� ������ �������ϰ� ����â�� ����
	this->endDyeing();
	_isShow = false;
}

void dyeing::EventClickPartButton()
{
	// �̹� ���� �̾Ҵٸ� ����
	if (_pickSelectColor) return;

	for (int i = 0; i < _dyeingItem->PartCount(); i++)
	{
		if (PtInRect(&_partButton[i], MYUTIL::getMousePos())
			&& _dyeingItem->PartCount() > i) // ������ ��Ʈ�� ��� ���ٸ� ����
		{
			// ���� ��Ʈ ��ȣ�� ����
			this->setDyeingPart(i);
			return;
		}
	}
}

bool dyeing::mouseOnPartButton(void)
{
	for (int i = 0; i < 3; i++)
	{
		if (PtInRect(&_partButton[i], MYUTIL::getMousePos()))
		{
			return true;
		}
	}

	return false;
}
