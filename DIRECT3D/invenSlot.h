#pragma once
#include "equip.h"

#define SLOTSIZE 32

class invenSlot
{
private:
	POINT						_slotNumber;			// ���� �ѹ�
	LPDIRECT3DTEXTURE9			_slotTexture;			// ���� �ؽ�ó
	vector<invenSlot*>			_childSlot;				// �ڽ� ���� ����
	invenSlot*					_parent;				// �θ� ����
	
	equip*						_item;					// ������

	bool						_isParent;				// �θ��ΰ�?

	RECT						_slotRC;				// ���� ��Ʈ ����

	DWORD						_color;
public:
	void release(void);
	void render(void);

	void releaseItem(void);
	void releaseChild(void);
	void releaseParent(void);
	void addChild(invenSlot* childSlot);
	void addParent(invenSlot* parentSlot);

	void setItem(equip* item, bool isParent = false);
	equip* getItem(void) { return _item; }
	invenSlot* getParent(void)
	{
		if (_parent == NULL) return this;
		return _parent;
	}

	void setIsParent(bool isParent) { _isParent = isParent; }
	bool getIsParent(void) { return _isParent; }

	LPRECT getRect(void) { return &_slotRC; }

	void setSlotNumber(POINT number) { _slotNumber = number; }
	POINT getSlotNumber(void) { return _slotNumber; }

	void setColor(DWORD color) { _color = color; }
	invenSlot() {};
	invenSlot(int destX, int destY);
	~invenSlot() {};
};

