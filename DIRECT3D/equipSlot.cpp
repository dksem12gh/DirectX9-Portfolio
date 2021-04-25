#include "stdafx.h"
#include "equipSlot.h"

void equipSlot::setItem(equip * pEquip)
{
	_item = pEquip;

	if (pEquip != NULL)
	{
		int posX = _rect.left + ((_rect.right - _rect.left) / 2) - (_item->getFrameWidth() / 2);
		int posY = _rect.top + ((_rect.bottom - _rect.top) / 2) - (_item->getFrameHeight() / 2);

		_item->setImagePosition(posX, posY);
	}
}

void equipSlot::release(void)
{
	if (_texture)
		_texture->Release();
}

void equipSlot::render(void)
{
	SPRITEMANAGER->spriteRender(_texture, NULL, _rect.left, _rect.top, 0xd000ffff);

	if (_item)
	{
		_item->renderImage();
	}
}

equipSlot::equipSlot(string texturePath, int destX, int destY, EQUIP_PARTS parts) : _texture(NULL), _item(NULL), _equipPart(MANTLE)
{
	D3DXCreateTextureFromFile(device,
		texturePath.c_str(),
		&_texture);

	if (_texture)
		_texture->GetLevelDesc(0, &_imgInfo);

	_rect = { destX, destY, destX + (LONG)_imgInfo.Width, destY + (LONG)_imgInfo.Height };

	_equipPart = parts;
}
