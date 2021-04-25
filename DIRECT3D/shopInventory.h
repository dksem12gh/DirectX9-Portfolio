#pragma once
#include "invenSlot.h"

#define	SHOPSLOTWIDTH		10
#define SHOPSLOTHEIGHT		8

class player;

class shopInventory
{
private:
	invenSlot*				_invenSlot[8][10];  // ���� ����
	bool					_isOpen;			// ������ ���ȴ°�?
	list<equip*>			_itemList;			// �ǸŹ�ǰ ����Ʈ
	invenSlot*				_mouseOnSlot;		// ���� ���콺�� �÷��� �ִ� ����

	RECT					_slotAreaRect;		// ���Կ��� ��Ʈ
	float					_regenTime;
public:
	void init(void);
	void release(void);
	void update(void);
	void render(void);

	bool addItem(equip* item);
	void addItem(equip* item, vector<invenSlot*>* pVecSlot, POINT invenSize);

	equip* buyItem(int* price = NULL);
	void sellItem(equip* item, int* price);
	bool releaseItem(invenSlot* releaseSlot);

	void reDyeingColor(void);

	void setOpen(bool open) { _isOpen = open; }
	bool getIsOpen(void) { return _isOpen; }

	LPRECT getSlotArea(void) { return &_slotAreaRect; }

	bool isMouseInShop(void) { return PtInRect(this->getSlotArea(), MYUTIL::getMousePos()); }
	shopInventory(int windowPosX = 0, int windowPosY = 0);
	~shopInventory();
};

