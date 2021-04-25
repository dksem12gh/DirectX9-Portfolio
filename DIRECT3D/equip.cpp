#include "stdafx.h"
#include "equip.h"

// 아이템을 복사하여 하나를 만들어낸다. ( 상점구입 구현을 위한 코드)
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

// 아이템 팝업창 생성
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

	string priceStr = string("가격 : ") + to_string(_price);
	int priceSize = 6 * (priceStr.length() - 1);
	int priceDest = (destX + 100) - (priceSize / 2);
	FONTMANAGER->fontOut("uiNameKr", priceStr, priceDest, destY + 50, 0xffffffff);
}

equip::equip(string name,					// 아이템 이름
			 string itemPath,				// 메시 파일 경로
			 string mtrlPath,				// 텍스처 경로
			 string invenImage,				// 인벤토리 이미지 경로
			 int invenSizeWidth,			// 인벤토리 가로 사이즈 경로
			 int invenSizeHeight,			// 인벤토리 세로 사이즈 경로
			 int invenImageFrameCount,		// 인벤토리 이미지가 몇 프레임으로 구성되어 있나
			 int invenImageWidth,			// 인벤토리 이미지 가로 프레임 크기
			 int invenImageHeight,			// 인벤토리 이미지 세로 프레임 크기
			 EQUIP_PARTS equipParts,		// 장착 부위
			 int price,						// 아이템 가격
			 int stack,						// 아이템 스택(겹치는 수)
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

	//염색컬러 초기화
	for (int i = 0; i < 3; i++)
	{
		_partColor[i] = NULL;
		_itemInfo.dyeingColor[i] = NULL;
	}

	_itemName = name;	// 아이템 이름 저장

	string partPath[4];			// 각 파츠별 경로 생성
	partPath[0] = itemPath + string("aPart.x");
	partPath[1] = itemPath + string("bPart.x");
	partPath[2] = itemPath + string("cPart.x");
	partPath[3] = itemPath + string("dPart.x");

	_vecMesh.resize(4);			// 파트메쉬 벡터 리사이즈

	for (int i = 0; i < 4; i++)
	{
		xMeshSkinned* pPart =  new xMeshSkinned;
		_vecMesh[i] = new partItem;
		if (FAILED(pPart->init(partPath[i].c_str(), mtrlPath, pMatCorrection)))
		{
			// 메쉬 생성에 실패하면
			_vecMesh[i]->setMesh(NULL);
			continue;
		}
		_vecMesh[i]->setMesh(pPart);
	}
	
	// 장착될 파트와 인벤토리 크기
	_equipParts = equipParts;
	_invenSize = { invenSizeWidth, invenSizeHeight };

	// 원본 인벤 이미지 텍스처
	D3DXCreateTextureFromFile(device,
		invenImage.c_str(),
		&_originInvenImage);

	// 염색될 인벤 이미지 텍스처
	D3DXCreateTextureFromFile(device,
		invenImage.c_str(),
		&_invenImage);


	_invenImagePartCount = invenImageFrameCount;	// 인벤토리 이미지 프레임 갯수
	_invenImageWidth = invenImageWidth;					// 인벤토리 이미지 프레임 넓이
	_invenImageHeight = invenImageHeight;				// 인벤토리 이미지 프레임 높이
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
	// pMesh에 장비를 대입해준다.
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
	_itemInfo.dyeingColor[part] = color;		// 2018. 06. 16. 박인호
	dyeing dye;
	dye.dyeingImage(_originInvenImage, _invenImage, part, _invenImageWidth, color);
}