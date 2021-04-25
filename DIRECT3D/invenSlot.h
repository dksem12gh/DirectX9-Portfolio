#pragma once
#include "equip.h"

#define SLOTSIZE 32

class invenSlot
{
private:
	POINT						_slotNumber;			// 슬롯 넘버
	LPDIRECT3DTEXTURE9			_slotTexture;			// 슬롯 텍스처
	vector<invenSlot*>			_childSlot;				// 자식 슬롯 벡터
	invenSlot*					_parent;				// 부모 슬롯
	
	equip*						_item;					// 아이템

	bool						_isParent;				// 부모인가?

	RECT						_slotRC;				// 슬롯 렉트 영역

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

