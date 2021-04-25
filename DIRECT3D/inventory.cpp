#include "stdafx.h"
#include "inventory.h"

HRESULT inventory::init(void)
{
	_itemWindowPos = { WINSIZEX / 2 + 100,WINSIZEY/2 - 300 };
	_windowTexture = RM_TEXTURE->addResource("Resources/mabinogi/UI/itemUi.dds");
	this->textInit();

	D3DSURFACE_DESC	desc;

	_windowTexture->GetLevelDesc(0, &desc);

	_inventoryRC = { _itemWindowPos.x, _itemWindowPos.y,
		_itemWindowPos.x + (LONG)desc.Width, _itemWindowPos.y + (LONG)desc.Height };

	for (int y = 0; y < INVENSLOTHEIGHT; y++)
	{
		for (int x = 0; x < INVENSLOTWIDTH; x++)
		{
			
			_invenSlot[y][x] = new invenSlot(_itemWindowPos.x +285+ x * SLOTSIZE, _itemWindowPos.y+127+y * SLOTSIZE);
			_invenSlot[y][x]->setSlotNumber({ x, y });
		}
	}

	_invenSlotTotalRC = { _invenSlot[0][0]->getRect()->left, 
		_invenSlot[0][0]->getRect()->top, 
		_invenSlot[INVENSLOTHEIGHT - 1][INVENSLOTWIDTH - 1]->getRect()->right,
		_invenSlot[INVENSLOTHEIGHT - 1][INVENSLOTWIDTH - 1]->getRect()->bottom };

	_equipSlot[HEAD] = new equipSlot("Resources/mabinogi/UI/head.dds",			_itemWindowPos.x + 104,		_itemWindowPos.y + 125, HEAD);
	_equipSlot[BODY] = new equipSlot("Resources/mabinogi/UI/body.dds",			_itemWindowPos.x + 104,		_itemWindowPos.y + 191, BODY);
	_equipSlot[SHOES] = new equipSlot("Resources/mabinogi/UI/shoes.dds",		_itemWindowPos.x + 104,		_itemWindowPos.y + 321, SHOES);
	_equipSlot[GLOVE] = new equipSlot("Resources/mabinogi/UI/gloves.dds",		_itemWindowPos.x + 38,		_itemWindowPos.y + 321, GLOVE);
	_equipSlot[MANTLE] = new equipSlot("Resources/mabinogi/UI/cloak.dds",		_itemWindowPos.x + 170,		_itemWindowPos.y + 321, MANTLE);
	_equipSlot[WEAPON] = new equipSlot("Resources/mabinogi/UI/leftHand.dds",	_itemWindowPos.x + 38,		_itemWindowPos.y + 191, WEAPON);

	//_equipSlot[BODY]->setItem(EQUIP->findItem("초보자용 바지"));

	_totalGold = 10000;

	_isShow = false;

	return S_OK;
}

bool inventory::dyeing()
{
	if (_mouseOnItem == NULL) return false;

	_dyeingInterface->setItem(_mouseOnItem);
	_dyeingInterface->setShow(true);

	return true;
}

void inventory::release(void)
{
	for (int y = 0; y < INVENSLOTHEIGHT; y++)
	{
		for (int x = 0; x < INVENSLOTWIDTH; x++)
		{
			_invenSlot[y][x]->release();
			SAFE_DELETE(_invenSlot[x][y]);
		}
	}

	for (int i = 0; i < 6; i++)
	{
		_equipSlot[i]->release();
		SAFE_DELETE(_equipSlot[i]);
	}
}

void inventory::update(void)
{
	list<equip*>::iterator iter = _itemList.begin();

	// 슬롯 컬러 초기화
	for (int y = 0; y < INVENSLOTHEIGHT; y++)
	{
		for (int x = 0; x < INVENSLOTWIDTH; x++)
		{
			// 슬롯에 아이템이 없으면 오버라이드 컬러 없음
			if (_invenSlot[y][x]->getItem() == NULL)
				_invenSlot[y][x]->setColor(0xff00ffff);
			else
			{
				// 아이템이 있으면 진하늘색으로 오버라이드
				_invenSlot[y][x]->setColor(0xff00e0ff);
				// 슬롯영역에 마우스가 위치하면 mouseOnItem 변수에 저장
				if (PtInRect(_invenSlot[y][x]->getRect(), MYUTIL::getMousePos()))
				{
					_mouseOnItem = _invenSlot[y][x]->getItem();
				}
			}
		}
	}

	// 아이템과 겹치는 슬롯을 저장할 임시 벡터
	_interSlot.clear();

	bool firstContact = true;

	int firstSlotX = 0;
	// 현재 마우스로 잡은 아이템이 있다면
	if ((*_pickItem))
	{
		POINT invenSize = (*_pickItem)->getInvenSize();

		for (int y = 0; y < INVENSLOTHEIGHT; y++)
		{
			for (int x = 0; x < INVENSLOTWIDTH; x++)
			{
				RECT temp;
				// 잡은 아이템과 슬롯의 렉트가 충돌하면
				if (IntersectRect(&temp, (*_pickItem)->getRect(), _invenSlot[y][x]->getRect()))
				{
					// 아이템 슬롯 범위가 잘못되는걸 방지하는 코드
					if (firstContact)
					{
						// 첫번째 슬롯이라면 첫번째 슬롯의 가로 슬롯 넘버를 저장하고 플래그 오프
						firstSlotX = x;
						firstContact = false;
					}
					// 플래그가 오프된 상태에서 가로 사이즈가 아이템의 가로 사이즈를 넘지않도록 조치
					if (!firstContact && abs(x - firstSlotX) >= invenSize.x) continue;
					// ex) 가로 2 세로 3칸이라면 총 사이즈는 6
					int itemSize = (*_pickItem)->getInvenSize().x * (*_pickItem)->getInvenSize().y;
					if (_interSlot.size() >= 1 && _interSlot.size() >= itemSize)
					{
						y += 100;		// 2중 반복문을 바로 빠져나가기위하여 첫번째 반복 변수에 큰 수를 더해준다.
						break;
					}
					// 임시 벡터에 슬롯을 추가
					// 이때 제일 처음 추가되는 슬롯이 부모 슬롯이 된다.
					_interSlot.push_back(_invenSlot[y][x]);
				}
			}
		}

		for (int i = 0; i < _interSlot.size(); i++)
		{
			POINT itemSize = (*_pickItem)->getInvenSize();
			// 아이템과 충돌된 슬롯은 오버라이드 컬러
			if (this->possibleaddItem(&_interSlot, itemSize))
			{
				_interSlot[i]->setColor(0xff00e0ff);
			}
			else
			{
				_interSlot[i]->setColor(0xfff00000);
			}
		}
	}
}



void inventory::render(void)
{
	SPRITEMANAGER->spriteRender(_windowTexture, NULL, _itemWindowPos.x, _itemWindowPos.y, 0xf000ffff);
	this->inventoryTextOut();

	equip* mouseOnItem = NULL;

	// 인벤토리 슬롯 렌더
	for (int y = 0; y < INVENSLOTHEIGHT; y++)
	{
		for (int x = 0; x < INVENSLOTWIDTH; x++)
		{
			_invenSlot[y][x]->render();
			LPRECT slotRC = _invenSlot[y][x]->getRect();
			if (_invenSlot[y][x]->getItem() && PtInRect(slotRC, MYUTIL::getMousePos()))
			{
				mouseOnItem = _invenSlot[y][x]->getItem();
			}
		}
	}
	
	// 아이템 이미지 렌더
	list<equip*>::iterator iter = _itemList.begin();
	for (; iter != _itemList.end(); ++iter)
	{
		(*iter)->renderImage();
	}

	// 장착 슬롯 렌더
	for (int i = 0; i < 6; i++)
	{
		_equipSlot[i]->render();
		if (_equipSlot[i]->getEquipItem())
		{
			_equipSlot[i]->getEquipItem()->renderImage();
		}
	}

	
	FONTMANAGER->fontOut("playerGold", to_string(_totalGold), _itemWindowPos.x + 160, _itemWindowPos.y + 473, D3DCOLOR_XRGB(255, 255, 255, 150));
	if (mouseOnItem)
	{
		POINT mousePos = MYUTIL::getMousePos();
		mouseOnItem->renderItemInfoPop(mousePos.x, mousePos.y);
	}
}

void inventory::textInit()
{
	FONTMANAGER->addFont("uiNameUS", "나눔고딕 ExtraBold ", 15);
	FONTMANAGER->addFont("uiNameKr", "나눔고딕 ExtraBold", 15);
	FONTMANAGER->addFont("slotName", "나눔고딕 ExtraBold", 20);
	FONTMANAGER->addFont("playerGoldUs", "나눔고딕 ExtraBold", 15);
	FONTMANAGER->addFont("playerGoldKr", "나눔고딕 ExtraBold", 15);
	FONTMANAGER->addFont("playerGold", "나눔고딕 ExtraBold", 15);
}

void inventory::inventoryTextOut()
{
	char str[64];
	FONTMANAGER->fontOutShadow("uiNameUS", "Inventory", _itemWindowPos.x+100 , _itemWindowPos.y+20 , D3DCOLOR_XRGB(255, 255, 200), D3DCOLOR_XRGB(255, 255, 255));
	FONTMANAGER->fontOut("uiNameKr", "소지품", _itemWindowPos.x + 100, _itemWindowPos.y +35, D3DCOLOR_XRGB(255, 255, 255, 150));
	FONTMANAGER->fontOut("slotName", "장   비", _itemWindowPos.x + 48, _itemWindowPos.y + 88, D3DCOLOR_XRGB(255, 255, 255, 150));
	FONTMANAGER->fontOut("playerGoldUs", "Gold", _itemWindowPos.x + 115, _itemWindowPos.y + 466, D3DCOLOR_XRGB(255, 255, 255, 150));
	FONTMANAGER->fontOut("playerGoldKr", "금 화", _itemWindowPos.x + 115, _itemWindowPos.y + 481, D3DCOLOR_XRGB(255, 255, 255, 150));
}

void inventory::moveItem(void)
{
	// 현재 마우스 영역이 인벤토리 슬롯이 아니라면(장착 슬롯 영역이라면)
	if (!PtInRect(&_invenSlotTotalRC, MYUTIL::getMousePos()))
	{
		// 장착 슬롯 변경
		equipSlot* selectSlot = NULL;		// 장착되어질 슬롯을 가리키는 임시 변수

		for (int i = 0; i < 6; i++)
		{
			// 슬롯 영역을 검사해서 임시 변수에 슬롯을 저장한다.
			if (PtInRect(_equipSlot[i]->getRect(), MYUTIL::getMousePos()))
			{
				selectSlot = _equipSlot[i];
				break;
			}
		}	
		if (!selectSlot) return; // 마우스가 올려진 슬롯이 없다면 바로 리턴(터지는걸 방지하기 위하여)

		// 새로 장착할 아이템의 장비 종류와 장착할 슬롯의 종류가 일치한다면
		if ((*_pickItem) != NULL && selectSlot->getEquipParts() == (*_pickItem)->getEquipParts())
		{
			// 먼저 장착되어져있던 장비를 임시 변수에 저장(NULL이든 장착되어있든)
			equip* tmpEquip = selectSlot->getEquipItem();

			selectSlot->setItem((*_pickItem)); // 슬롯에 현재 마우스에 활성화되어있는 아이템을 정착하고

			*_pickItem = tmpEquip; // 마우스에 임시 변수에 저장된 아이템을 넣는다.

			// 장착된 아이템 메시를 플레이어 캐릭터에 적용
			switch (selectSlot->getEquipParts())
			{
			case BODY:
				_meshInfo->changeBody(selectSlot->getEquipItem());
				break;
			case SHOES:
				_meshInfo->changeShoes(selectSlot->getEquipItem());
				break;
			case WEAPON:
				_meshInfo->changeWeapon(selectSlot->getEquipItem());
			}

			return;
		}

		// 현지 마우스에 잡힌 아이템이 없다면
		if ((*_pickItem) == NULL)
		{
			// 마우스에 장착 슬롯의 아이템을 넣고 장착 슬롯은 클리어
			(*_pickItem) = selectSlot->getEquipItem();

			// 장착 슬롯에 맞춰서 빈 아이템 입력
			switch (selectSlot->getEquipParts())
			{
			case BODY:
				_meshInfo->changeBody(NULL);
				break;
			case SHOES:
				_meshInfo->changeShoes(NULL);
				break;
			case WEAPON:
				_meshInfo->changeWeapon(NULL);
			}

			selectSlot->setItem(NULL); // 장착 슬롯 아이템 클리어
		}
	}
	// 현재 마우스 영역이 인벤토리 슬롯 영역이라면
	else
	{
		equip* temp = NULL;
		if (*_pickItem != NULL)		// 현재 마우스에 붙은 아이템이 있다면(자리 교체 혹은 인벤토리에 추가하는 경우)
		{
			// 인벤토리 사이즈랑 부모가 될 슬롯의 좌표를 로드
			POINT invenSize = (*_pickItem)->getInvenSize();
			POINT parentSlot = _interSlot[0]->getSlotNumber();

			// 만약 아이템 슬롯 공간이 충분히 확보되지 않았거나 다른 아이템이 간섭하는 경우 리턴
			if (!this->possibleaddItem(&_interSlot, invenSize)) return;
			for (int i = 0; i < _interSlot.size(); i++)
			{
				if (_interSlot[i]->getItem())
				{
					temp = _interSlot[i]->getItem();	// 임시 변수에 현재 아이템을 저장
					this->releaseItem(_interSlot[i]);	// 현재 인벤토리 슬롯의 아이템을 제거(제거된 아이템은 자동으로 마우스 아이템으로...)
				}
			}

			// 마우스 아이템을 추가 후 마우스에 임시 아이템 저장
			this->addItem(*_pickItem, &_interSlot, invenSize);
			*_pickItem = temp;
		}
		else  // 현재 마우스에 붙은 아이템이 없다면
		{
			for (int y = 0; y < INVENSLOTHEIGHT; y++)
			{
				for (int x = 0; x < INVENSLOTWIDTH; x++)
				{
					if (PtInRect(_invenSlot[y][x]->getRect(), MYUTIL::getMousePos()) &&
						_invenSlot[y][x]->getItem())
					{
						list<equip*>::iterator iter = _itemList.begin();

						for (; iter != _itemList.end(); ++iter)
						{
							if (*iter == _invenSlot[y][x]->getItem())
							{
								_itemList.erase(iter);
								break;
							}
						}
						*_pickItem = _invenSlot[y][x]->getItem();
						_invenSlot[y][x]->getParent()->releaseItem();
						return;
					}
				}
			}
		}
	}
}

bool inventory::possibleaddItem(vector<invenSlot*>* pVecSlot, POINT invenSize)
{
	if ((*pVecSlot).size() < invenSize.x * invenSize.y) return false;

	equip* pTemp = NULL;
	
	for (int i = 0; i < pVecSlot->size(); i++)
	{
		if ((*pVecSlot)[i]->getItem() != NULL) // 슬로에 이미 아이템이 있고
		{
			if (pTemp == NULL) // 임시 변수가 비어있으면
			{
				pTemp = (*pVecSlot)[i]->getItem();  // 저장
			}
			// 임시 변수가 비어있지 않고 서로 다른 아이템이면
			else if (pTemp != (*pVecSlot)[i]->getItem())
			{
				return false;
			}
		}
	}
	return true;
}

bool inventory::addItem(ITEMINFO itemInfo)
{
	equip* newEquip = EQUIP->addItem(itemInfo.itemName);

	for (int i = 0; i < 3; i++)
	{
		newEquip->dyeingColor(i, itemInfo.dyeingColor[i]);
	}

	return this->addItem(newEquip);
}

bool inventory::addItem(equip * item)
{
	if (item == NULL) return false;

	POINT invenSize = item->getInvenSize();
	vector<invenSlot*> vecSlot;

	for (int y = 0; y < INVENSLOTHEIGHT; y++)
	{
		for (int x = 0; x < INVENSLOTWIDTH; x++)
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

void inventory::addItem(equip * item, vector<invenSlot*>* pVecSlot, POINT invenSize)
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

void inventory::destroyItem(equip * item)
{
	list<equip*>::iterator iter = _itemList.begin();

	for (; iter != _itemList.end(); ++iter)
	{
		if ((*iter) == item)
		{
			_itemList.erase(iter);

			break;
		}
	}
}

bool inventory::releaseItem(invenSlot * releaseSlot)
{
	// 실제로 아이템을 빼올 부모 슬롯
	invenSlot* parentSlot = releaseSlot->getParent();
	
	// 슬롯에서 아이템을 제거한다.
	parentSlot->releaseItem();

	return true;
}

inventory::inventory() : _windowTexture(NULL), _pickItem(NULL), _meshInfo(NULL)
{
	for (int y = 0; y < INVENSLOTHEIGHT; y++)
	{
		for (int x = 0; x < INVENSLOTWIDTH; x++)
		{
			_invenSlot[y][x] = NULL;
		}
	}
}