#include "stdafx.h"
#include "equip.h"

// �������� �����Ͽ� �ϳ��� ������. ( �������� ������ ���� �ڵ�)
equip * equip::cloneItem(void)
{
	equip* clone = new equip(_itemInfo.itemName,
		_itemInfo.filePath,
		_itemInfo.texturePath,
		_itemInfo.invenImagePath,
		_itemInfo.invensize.x,
		_itemInfo.invensize.y,
		_itemInfo.invenImagePartCount,
		_itemInfo.invenImageWidth,
		_itemInfo.invenImageHeight,
		_itemInfo.equipPart,
		_itemInfo.price,
		_itemInfo.stack,
		&_matCorrection);

	for (int i = 0; i < 3; i++)
	{
		if (_itemInfo.dyeingColor[i] != NULL)
			clone->dyeingColor(i, _itemInfo.dyeingColor[i]);
	}

	return clone;
}

// ������ �˾�â ����
void equip::createItemInfoPop(LPDIRECT3DTEXTURE9* pPopTex)
{
	if (*pPopTex) SAFE_RELEASE(*pPopTex);

	D3DXCreateTexture(device,
		200,
		100,
		0,
		D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		pPopTex);

	LPDIRECT3DSURFACE9	texSurf;
	(*pPopTex)->GetSurfaceLevel(0, &texSurf);

	D3DLOCKED_RECT lockRect;

	texSurf->LockRect(&lockRect, NULL, D3DLOCK_DISCARD);

	DWORD* colors = (DWORD*)lockRect.pBits;
	for (int y = 0; y <= 100; y++)
	{
		for (int x = 0; x <= 200; x++)
		{
			int index = (y * 200) + x;

			colors[index] = 0xf0000000;
		}
	}
	texSurf->UnlockRect();

	SAFE_RELEASE(texSurf);
}

void equip::renderItemInfoPop(int destX, int destY)
{
	SPRITEMANAGER->spriteRender(_itemInfoPop, NULL, destX, destY, 0xf0c0c0c0);

	int nameSize = 6 * (_itemName.length() - 1);
	int nameDest = (destX + 100) - (nameSize / 2);
	FONTMANAGER->fontOut("uiNameKr", _itemName, nameDest, destY + 10, 0xffffffff);

	string priceStr = string("���� : ") + to_string(_price);
	int priceSize = 6 * (priceStr.length() - 1);
	int priceDest = (destX + 100) - (priceSize / 2);
	FONTMANAGER->fontOut("uiNameKr", priceStr, priceDest, destY + 50, 0xffffffff);
}

equip::equip(string name,					// ������ �̸�
			 string itemPath,				// �޽� ���� ���
			 string mtrlPath,				// �ؽ�ó ���
			 string invenImage,				// �κ��丮 �̹��� ���
			 int invenSizeWidth,			// �κ��丮 ���� ������ ���
			 int invenSizeHeight,			// �κ��丮 ���� ������ ���
			 int invenImageFrameCount,		// �κ��丮 �̹����� �� ���������� �����Ǿ� �ֳ�
			 int invenImageWidth,			// �κ��丮 �̹��� ���� ������ ũ��
			 int invenImageHeight,			// �κ��丮 �̹��� ���� ������ ũ��
			 EQUIP_PARTS equipParts,		// ���� ����
			 int price,						// ������ ����
			 int stack,						// ������ ����(��ġ�� ��)
			 D3DXMATRIXA16 * pMatCorrection) :_originInvenImage(NULL), _invenImage(NULL), _itemInfoPop(NULL)
{
	_itemInfo.itemName = name;
	_itemInfo.filePath = itemPath;
	_itemInfo.texturePath = mtrlPath;
	_itemInfo.invenImagePath = invenImage;
	_itemInfo.invensize = { invenSizeWidth, invenSizeHeight };
	_itemInfo.invenImagePartCount = invenImageFrameCount;
	_itemInfo.invenImageWidth = invenImageWidth;
	_itemInfo.invenImageHeight = invenImageHeight;
	_itemInfo.equipPart = equipParts;
	_itemInfo.price = price;
	_itemInfo.stack = stack;

	//�����÷� �ʱ�ȭ
	for (int i = 0; i < 3; i++)
	{
		_partColor[i] = NULL;
		_itemInfo.dyeingColor[i] = NULL;
	}

	_itemName = name;	// ������ �̸� ����

	string partPath[4];			// �� ������ ��� ����
	partPath[0] = itemPath + string("aPart.x");
	partPath[1] = itemPath + string("bPart.x");
	partPath[2] = itemPath + string("cPart.x");
	partPath[3] = itemPath + string("dPart.x");

	_vecMesh.resize(4);			// ��Ʈ�޽� ���� ��������

	for (int i = 0; i < 4; i++)
	{
		xMeshSkinned* pPart =  new xMeshSkinned;
		_vecMesh[i] = new partItem;
		if (FAILED(pPart->init(partPath[i].c_str(), mtrlPath, pMatCorrection)))
		{
			// �޽� ������ �����ϸ�
			_vecMesh[i]->setMesh(NULL);
			continue;
		}
		_vecMesh[i]->setMesh(pPart);
	}
	
	// ������ ��Ʈ�� �κ��丮 ũ��
	_equipParts = equipParts;
	_invenSize = { invenSizeWidth, invenSizeHeight };

	// ���� �κ� �̹��� �ؽ�ó
	D3DXCreateTextureFromFile(device,
		invenImage.c_str(),
		&_originInvenImage);

	// ������ �κ� �̹��� �ؽ�ó
	D3DXCreateTextureFromFile(device,
		invenImage.c_str(),
		&_invenImage);


	_invenImagePartCount = invenImageFrameCount;	// �κ��丮 �̹��� ������ ����
	_invenImageWidth = invenImageWidth;					// �κ��丮 �̹��� ������ ����
	_invenImageHeight = invenImageHeight;				// �κ��丮 �̹��� ������ ����
	_price = price;
	_stack = stack;

	this->createItemInfoPop(&_itemInfoPop);
}

void equip::release(void)
{
	for (int i = 0; i < _vecMesh.size(); i++)
	{
		if(_vecMesh[i]->_pMesh) _vecMesh[i]->_pMesh->release();
		SAFE_DELETE(_vecMesh[i]);
	}

	if(_originInvenImage) _originInvenImage->Release();
	if(_invenImage) _invenImage->Release();
	if (_itemInfoPop) _itemInfoPop->Release();
}

void equip::update(void)
{
	for (int i = 0; i < _vecMesh.size(); i++)
	{
		_vecMesh[i]->update();
		SHADOWMANAGER->addShadowObject(_vecMesh[i]);
	}

	_itemRC = { (LONG)(_imagePositon.x), (LONG)(_imagePositon.y), (LONG)(_imagePositon.x + _invenImageWidth), (LONG)(_imagePositon.y + _invenImageHeight) };
}

void equip::render(void)
{
	for (int i = 0; i < _vecMesh.size(); i++)
	{
		_vecMesh[i]->render();
	}
}

void equip::renderImage(void)
{
	if (_invenImage == NULL) return;

	for (int i = 0; i < 4; i++)
	{
		try
		{
			RECT rc = { i * _invenImageWidth, 0, i * _invenImageWidth + _invenImageWidth, _invenImageHeight };
			SPRITEMANAGER->spriteRender(_invenImage, &rc, _imagePositon.x, _imagePositon.y);
		}
		catch (...) {}
	}
}

void equip::renderImage(int destX, int destY, int passIndex, DWORD glitterColor)
{
	if (_invenImage == NULL) return;

	if (passIndex > -1)
	{
		RECT rc = MYUTIL::rectMake(passIndex * _invenImageWidth, 0, _invenImageWidth, _invenImageHeight);
		SPRITEMANAGER->spriteRender(_originInvenImage, &rc, destX, destY, glitterColor);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			try
			{
				if (passIndex != -1 && i == passIndex) continue;
				RECT rc = { i * _invenImageWidth, 0, i * _invenImageWidth + _invenImageWidth, _invenImageHeight };
				SPRITEMANAGER->spriteRender(_invenImage, &rc, destX, destY, glitterColor);
			}
			catch (...) {}
		}
	}
}

void equip::setMesh(baseObject* pObject)
{
	xMeshSkinned * pMesh = pObject->getSkinnedMesh();
	// pMesh�� ��� �������ش�.
	for (int i = 0; i < _vecMesh.size(); i++)
	{
		if (_vecMesh[i]->_pMesh == NULL) continue;

		pMesh->changeMesh(_vecMesh[i]->getSkinnedMesh()->getRootBone());

		_vecMesh[i]->setActive(true);
	}

}

void equip::releaseMesh(void)
{
	for (int i = 0; i < _vecMesh.size(); i++)
	{
		if(_vecMesh[i]->_pMesh != NULL)
			_vecMesh[i]->setActive(false);
	}
}

void equip::dyeingColor(int part, DWORD color)
{
	if (part >= _vecMesh.size() || _vecMesh[part]->_pMesh == NULL) return;

	_partColor[part] = color;
	_itemInfo.dyeingColor[part] = color;

	_vecMesh[part]->dyeingTex(color);
	_itemInfo.dyeingColor[part] = color;		// 2018. 06. 16. ����ȣ
	dyeing dye;
	dye.dyeingImage(_originInvenImage, _invenImage, part, _invenImageWidth, color);
}