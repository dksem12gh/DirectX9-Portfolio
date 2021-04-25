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

	//_equipSlot[BODY]->setItem(EQUIP->findItem("�ʺ��ڿ� ����"));

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

	// ���� �÷� �ʱ�ȭ
	for (int y = 0; y < INVENSLOTHEIGHT; y++)
	{
		for (int x = 0; x < INVENSLOTWIDTH; x++)
		{
			// ���Կ� �������� ������ �������̵� �÷� ����
			if (_invenSlot[y][x]->getItem() == NULL)
				_invenSlot[y][x]->setColor(0xff00ffff);
			else
			{
				// �������� ������ ���ϴû����� �������̵�
				_invenSlot[y][x]->setColor(0xff00e0ff);
				// ���Կ����� ���콺�� ��ġ�ϸ� mouseOnItem ������ ����
				if (PtInRect(_invenSlot[y][x]->getRect(), MYUTIL::getMousePos()))
				{
					_mouseOnItem = _invenSlot[y][x]->getItem();
				}
			}
		}
	}

	// �����۰� ��ġ�� ������ ������ �ӽ� ����
	_interSlot.clear();

	bool firstContact = true;

	int firstSlotX = 0;
	// ���� ���콺�� ���� �������� �ִٸ�
	if ((*_pickItem))
	{
		POINT invenSize = (*_pickItem)->getInvenSize();

		for (int y = 0; y < INVENSLOTHEIGHT; y++)
		{
			for (int x = 0; x < INVENSLOTWIDTH; x++)
			{
				RECT temp;
				// ���� �����۰� ������ ��Ʈ�� �浹�ϸ�
				if (IntersectRect(&temp, (*_pickItem)->getRect(), _invenSlot[y][x]->getRect()))
				{
					// ������ ���� ������ �߸��Ǵ°� �����ϴ� �ڵ�
					if (firstContact)
					{
						// ù��° �����̶�� ù��° ������ ���� ���� �ѹ��� �����ϰ� �÷��� ����
						firstSlotX = x;
						firstContact = false;
					}
					// �÷��װ� ������ ���¿��� ���� ����� �������� ���� ����� �����ʵ��� ��ġ
					if (!firstContact && abs(x - firstSlotX) >= invenSize.x) continue;
					// ex) ���� 2 ���� 3ĭ�̶�� �� ������� 6
					int itemSize = (*_pickItem)->getInvenSize().x * (*_pickItem)->getInvenSize().y;
					if (_interSlot.size() >= 1 && _interSlot.size() >= itemSize)
					{
						y += 100;		// 2�� �ݺ����� �ٷ� �������������Ͽ� ù��° �ݺ� ������ ū ���� �����ش�.
						break;
					}
					// �ӽ� ���Ϳ� ������ �߰�
					// �̶� ���� ó�� �߰��Ǵ� ������ �θ� ������ �ȴ�.
					_interSlot.push_back(_invenSlot[y][x]);
				}
			}
		}

		for (int i = 0; i < _interSlot.size(); i++)
		{
			POINT itemSize = (*_pickItem)->getInvenSize();
			// �����۰� �浹�� ������ �������̵� �÷�
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

	// �κ��丮 ���� ����
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
	
	// ������ �̹��� ����
	list<equip*>::iterator iter = _itemList.begin();
	for (; iter != _itemList.end(); ++iter)
	{
		(*iter)->renderImage();
	}

	// ���� ���� ����
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
	FONTMANAGER->addFont("uiNameUS", "������� ExtraBold ", 15);
	FONTMANAGER->addFont("uiNameKr", "������� ExtraBold", 15);
	FONTMANAGER->addFont("slotName", "������� ExtraBold", 20);
	FONTMANAGER->addFont("playerGoldUs", "������� ExtraBold", 15);
	FONTMANAGER->addFont("playerGoldKr", "������� ExtraBold", 15);
	FONTMANAGER->addFont("playerGold", "������� ExtraBold", 15);
}

void inventory::inventoryTextOut()
{
	char str[64];
	FONTMANAGER->fontOutShadow("uiNameUS", "Inventory", _itemWindowPos.x+100 , _itemWindowPos.y+20 , D3DCOLOR_XRGB(255, 255, 200), D3DCOLOR_XRGB(255, 255, 255));
	FONTMANAGER->fontOut("uiNameKr", "����ǰ", _itemWindowPos.x + 100, _itemWindowPos.y +35, D3DCOLOR_XRGB(255, 255, 255, 150));
	FONTMANAGER->fontOut("slotName", "��   ��", _itemWindowPos.x + 48, _itemWindowPos.y + 88, D3DCOLOR_XRGB(255, 255, 255, 150));
	FONTMANAGER->fontOut("playerGoldUs", "Gold", _itemWindowPos.x + 115, _itemWindowPos.y + 466, D3DCOLOR_XRGB(255, 255, 255, 150));
	FONTMANAGER->fontOut("playerGoldKr", "�� ȭ", _itemWindowPos.x + 115, _itemWindowPos.y + 481, D3DCOLOR_XRGB(255, 255, 255, 150));
}

void inventory::moveItem(void)
{
	// ���� ���콺 ������ �κ��丮 ������ �ƴ϶��(���� ���� �����̶��)
	if (!PtInRect(&_invenSlotTotalRC, MYUTIL::getMousePos()))
	{
		// ���� ���� ����
		equipSlot* selectSlot = NULL;		// �����Ǿ��� ������ ����Ű�� �ӽ� ����

		for (int i = 0; i < 6; i++)
		{
			// ���� ������ �˻��ؼ� �ӽ� ������ ������ �����Ѵ�.
			if (PtInRect(_equipSlot[i]->getRect(), MYUTIL::getMousePos()))
			{
				selectSlot = _equipSlot[i];
				break;
			}
		}	
		if (!selectSlot) return; // ���콺�� �÷��� ������ ���ٸ� �ٷ� ����(�����°� �����ϱ� ���Ͽ�)

		// ���� ������ �������� ��� ������ ������ ������ ������ ��ġ�Ѵٸ�
		if ((*_pickItem) != NULL && selectSlot->getEquipParts() == (*_pickItem)->getEquipParts())
		{
			// ���� �����Ǿ����ִ� ��� �ӽ� ������ ����(NULL�̵� �����Ǿ��ֵ�)
			equip* tmpEquip = selectSlot->getEquipItem();

			selectSlot->setItem((*_pickItem)); // ���Կ� ���� ���콺�� Ȱ��ȭ�Ǿ��ִ� �������� �����ϰ�

			*_pickItem = tmpEquip; // ���콺�� �ӽ� ������ ����� �������� �ִ´�.

			// ������ ������ �޽ø� �÷��̾� ĳ���Ϳ� ����
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

		// ���� ���콺�� ���� �������� ���ٸ�
		if ((*_pickItem) == NULL)
		{
			// ���콺�� ���� ������ �������� �ְ� ���� ������ Ŭ����
			(*_pickItem) = selectSlot->getEquipItem();

			// ���� ���Կ� ���缭 �� ������ �Է�
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

			selectSlot->setItem(NULL); // ���� ���� ������ Ŭ����
		}
	}
	// ���� ���콺 ������ �κ��丮 ���� �����̶��
	else
	{
		equip* temp = NULL;
		if (*_pickItem != NULL)		// ���� ���콺�� ���� �������� �ִٸ�(�ڸ� ��ü Ȥ�� �κ��丮�� �߰��ϴ� ���)
		{
			// �κ��丮 ������� �θ� �� ������ ��ǥ�� �ε�
			POINT invenSize = (*_pickItem)->getInvenSize();
			POINT parentSlot = _interSlot[0]->getSlotNumber();

			// ���� ������ ���� ������ ����� Ȯ������ �ʾҰų� �ٸ� �������� �����ϴ� ��� ����
			if (!this->possibleaddItem(&_interSlot, invenSize)) return;
			for (int i = 0; i < _interSlot.size(); i++)
			{
				if (_interSlot[i]->getItem())
				{
					temp = _interSlot[i]->getItem();	// �ӽ� ������ ���� �������� ����
					this->releaseItem(_interSlot[i]);	// ���� �κ��丮 ������ �������� ����(���ŵ� �������� �ڵ����� ���콺 ����������...)
				}
			}

			// ���콺 �������� �߰� �� ���콺�� �ӽ� ������ ����
			this->addItem(*_pickItem, &_interSlot, invenSize);
			*_pickItem = temp;
		}
		else  // ���� ���콺�� ���� �������� ���ٸ�
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
		if ((*pVecSlot)[i]->getItem() != NULL) // ���ο� �̹� �������� �ְ�
		{
			if (pTemp == NULL) // �ӽ� ������ ���������
			{
				pTemp = (*pVecSlot)[i]->getItem();  // ����
			}
			// �ӽ� ������ ������� �ʰ� ���� �ٸ� �������̸�
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
	// ������ �������� ���� �θ� ����
	invenSlot* parentSlot = releaseSlot->getParent();
	
	// ���Կ��� �������� �����Ѵ�.
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