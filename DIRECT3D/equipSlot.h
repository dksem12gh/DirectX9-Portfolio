#pragma once
#include "equip.h"

class equipSlot
{
private:
	LPDIRECT3DTEXTURE9		_texture;
	D3DSURFACE_DESC			_imgInfo;

	EQUIP_PARTS				_equipPart;
	equip*					_item;

	RECT					_rect;
public:
	LPRECT getRect(void) { return &_rect; };
	void setItem(equip* pEquip);
	void releaseItem(void) { _item = NULL; }
	EQUIP_PARTS getEquipParts(void) { return _equipPart; };
	equip* getEquipItem(void) { return _item; };

	void release(void);
	void render(void);
	equipSlot() {};
	equipSlot(string texturePath, int destX, int destY, EQUIP_PARTS parts);
	~equipSlot() {};
};

