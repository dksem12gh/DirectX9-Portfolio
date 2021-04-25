#include "stdafx.h"
#include "shopInventory.h"


void shopInventory::init(void)
{

	this->addItem(
		EQUIP->addSampleItem("큐트 너구리 꼬리 의상(남성)",					// 이름
			"Resources/mabinogi/char/Meshs/wear/tanuki/",			// 메쉬 폴더 경로
			"Resources/mabinogi/char/materials/",					// 마테리얼 경로
			"Resources/mabinogi/char/materials/image/male_wear_2010contest_japan.dds",	//인벤토리 이미지 경로
			2,
			4,
			4,
			48,
			96,
			BODY, // 파트 타입
			2000,
			1,
			&_matCorrection)
	);
	
	this->addItem(
		EQUIP->addSampleItem("큐트 너구리 꼬리 신발(남성)",					// 이름
			"Resources/mabinogi/char/Meshs/shoes/tanukiShoes/",		// 메쉬 폴더 경로
			"Resources/mabinogi/char/materials/",					// 마테리얼 경로
			"Resources/mabinogi/char/materials/image/item_shoes_2010contest_japan.dds",	//인벤토리 이미지 경로
			2,
			2,
			2,
			48,
			48,
			SHOES,
			1000,
			1,
			&_matCorrection)
	);

	this->addItem(
		EQUIP->addSampleItem("초보자용 바지",					// 이름
			"Resources/mabinogi/char/Meshs/wear/newbieWear/",							// 메쉬 폴더 경로
			"Resources/mabinogi/char/materials/",										// 마테리얼 경로
			"Resources/mabinogi/char/materials/image/uu_wear_newbiepants.dds",	//인벤토리 이미지 경로
			2,
			3,
			3,
			48,
			80,
			BODY,																		// 파트 타입
			500,
			1,
			&_matCorrection)
	);

	this->addItem(
		EQUIP->addSampleItem("가죽 신발",					// 이름
			"Resources/mabinogi/char/Meshs/shoes/newbieShoes/",							// 메쉬 폴더 경로
			"Resources/mabinogi/char/materials/",										// 마테리얼 경로
			"Resources/mabinogi/char/materials/image/uu_shoes_normal_001.dds",	//인벤토리 이미지 경로
			2,
			2,
			1,
			48,
			48,
			SHOES,																		// 파트 타입
			100,
			1,
			&_matCorrection)
	);

	this->addItem(
		EQUIP->addSampleItem("클레이모어",					// 이름
			"Resources/mabinogi/char/Meshs/weapon/claymore/active/",							// 메쉬 폴더 경로
			"Resources/mabinogi/char/materials/",										// 마테리얼 경로
			"Resources/mabinogi/char/materials/image/uu_weapon_claymore.dds",	//인벤토리 이미지 경로
			1,
			4,
			3,
			24,
			128,
			WEAPON,																		// 파트 타입
			3000,
			1,
			&_matCorrection)
	);

	this->reDyeingColor();

	this->_isOpen = false;

	_mouseOnSlot = NULL;
}

void shopInventory::release(void)
{
	for (int y = 0; y < SHOPSLOTHEIGHT; y++)
	{
		for (int x = 0; x < SHOPSLOTWIDTH; x++)
		{
			_invenSlot[y][x]->release();
			SAFE_DELETE(_invenSlot[y][x]);
		}
	}
}

void shopInventory::update(void)
{
	_mouseOnSlot = NULL;

	// 슬롯 컬러 초기화
	for (int y = 0; y < SHOPSLOTHEIGHT; y++)
	{
		for (int x = 0; x < SHOPSLOTWIDTH; x++)
		{
			// 슬롯에 아이템이 없으면 오버라이드 컬러 없음
			if (_invenSlot[y][x]->getItem() == NULL)
				_invenSlot[y][x]->setColor(0xff00ffff);
			else
				// 아이템이 있으면 진하늘색으로 오버라이드
				_invenSlot[y][x]->setColor(0xff00e0ff);
		}
	}

	for (int y = 0; y < SHOPSLOTHEIGHT; y++)
	{
		for (int x = 0; x < SHOPSLOTWIDTH; x++)
		{
			if (PtInRect(_invenSlot[y][x]->getRect(), MYUTIL::getMousePos()) &&
				_invenSlot[y][x]->getItem())
			{
				_mouseOnSlot = _invenSlot[y][x];
			}
		}
	}
	
	if (_regenTime + 10.0f <= TIMEMANAGER->getWorldTime())
		this->reDyeingColor();
}

void shopInventory::render(void)
{
	for (int y = 0; y < SHOPSLOTHEIGHT; y++)
	{
		for (int x = 0; x < SHOPSLOTWIDTH; x++)
		{
			_invenSlot[y][x]->render();
		}
	}

	list<equip*>::iterator iter = _itemList.begin();

	for (; iter != _itemList.end(); ++iter)
	{
		(*iter)->renderImage();
	}

	if (_mouseOnSlot)
	{
		equip* pEquip = _mouseOnSlot->getItem();

		if (pEquip)
			pEquip->renderItemInfoPop(getMousePos().x, getMousePos().y);
	}
	/*for (int y = 0; y < SHOPSLOTHEIGHT; y++)
	{
		for (int x = 0; x < SHOPSLOTWIDTH; x++)
		{
			POINT mousePos = MYUTIL::getMousePos();
			LPRECT slotRC = _invenSlot[y][x]->getRect();
			bool mouseOnSlot = PtInRect(slotRC, mousePos);
			equip* pEquip = _invenSlot[y][x]->getItem();

			if (pEquip && mouseOnSlot)
			{
				pEquip->renderItemInfoPop(mousePos.x, mousePos.y);
			}
		}
	}*/
}

bool shopInventory::addItem(equip * item)
{
	POINT invenSize = item->getInvenSize();
	vector<invenSlot*> vecSlot;

	for (int y = 0; y < SHOPSLOTHEIGHT; y++)
	{
		for (int x = 0; x < SHOPSLOTWIDTH; x++)
		{
			if (_invenSlot[y][x]->getItem() == NULL)
			{
				POINT slotNumber = _invenSlot[y][x]->getSlotNumber();

				bool possible = true;
				for (int slotY = slotNumber.y; slotY < slotNumber.y + invenSize.y; slotY++)
				{
					for (int slotX = slotNumber.x; slotX < slotNumber.x + invenSize.x; slotX++)
					{
						if (_invenSlot[slotY][slotX]->getItem() != NULL)
						{
							possible = false;
							break;
						}
						else
						{
							vecSlot.push_back(_invenSlot[slotY][slotX]);
						}
					}
					if (possible == false) break;
				}

				if (vecSlot.size() >= invenSize.x * invenSize.y)
				{
					this->addItem(item, &vecSlot, invenSize);

					return true;
				}
				else
				{
					vecSlot.clear();
				}
			}
		}
	}

	return false;
}

void shopInventory::addItem(equip * item, vector<invenSlot*>* pVecSlot, POINT invenSize)
{
	(*pVecSlot)[0]->setItem(item, true);

	for (int i = 1; i < pVecSlot->size(); i++)
	{
		(*pVecSlot)[0]->addChild((*pVecSlot)[i]);
	}

	int spaceX = abs((invenSize.x * SLOTSIZE) - (*pVecSlot)[0]->getItem()->getFrameWidth());
	int spaceY = abs((invenSize.y * SLOTSIZE) - (*pVecSlot)[0]->getItem()->getFrameHeight());
	(*pVecSlot)[0]->getItem()->setImagePosition((*(*pVecSlot)[0]->getRect()).left + spaceX / 2, (*(*pVecSlot)[0]->getRect()).top + spaceY / 2);

	_itemList.push_back(item);
}

equip* shopInventory::buyItem(int* price)
{
	
	if (_mouseOnSlot != NULL)
	{
		equip* buyItem = _mouseOnSlot->getItem();

		if (buyItem != NULL)
		{
			if (price == NULL || buyItem->getPrice() <= *price)
			{
				if(price != NULL) *price -= buyItem->getPrice();
				return EQUIP->addItem(buyItem);
			}
		}
	}

	return NULL;
}

void shopInventory::sellItem(equip* item, int* price)
{
	*price += (item->getPrice() / 3);

	EQUIP->deleteItem(item);

}

bool shopInventory::releaseItem(invenSlot * releaseSlot)
{
	// 실제로 아이템을 빼올 부모 슬롯
	invenSlot* parentSlot = releaseSlot->getParent();

	// 슬롯에서 아이템을 제거한다.
	parentSlot->releaseItem();

	return true;
}

void shopInventory::reDyeingColor(void)
{
	list<equip*>::iterator	iter = _itemList.begin();

	for (; iter != _itemList.end(); ++iter)
	{
		for (int i = 0; i < 3; i++)
		{
			D3DXCOLOR color;
			color.r = MYUTIL::randomFloatRange(0.0f, 1.0f);
			color.g = MYUTIL::randomFloatRange(0.0f, 1.0f);
			color.b = MYUTIL::randomFloatRange(0.0f, 1.0f);
			color.a = 1.0f;
			(*iter)->dyeingColor(i, DWORD(color));
		}
	}

	_regenTime = TIMEMANAGER->getWorldTime();
}

shopInventory::shopInventory(int windowPosX, int windowPosY)
{
	for (int y = 0; y < SHOPSLOTHEIGHT; y++)
	{
		for (int x = 0; x < SHOPSLOTWIDTH; x++)
		{
			_invenSlot[y][x] = new invenSlot(windowPosX + (x * SLOTSIZE), windowPosY + (y * SLOTSIZE));
			_invenSlot[y][x]->setItem(NULL);
			_invenSlot[y][x]->setSlotNumber({ x, y });
		}
	}

	_slotAreaRect = { _invenSlot[0][0]->getRect()->left,
		_invenSlot[0][0]->getRect()->top,
		_invenSlot[SHOPSLOTHEIGHT - 1][SHOPSLOTWIDTH - 1]->getRect()->right,
		_invenSlot[SHOPSLOTHEIGHT - 1][SHOPSLOTWIDTH - 1]->getRect()->bottom
	};
}


shopInventory::~shopInventory()
{
}
