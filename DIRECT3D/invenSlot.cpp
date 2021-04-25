#include "stdafx.h"
#include "invenSlot.h"


void invenSlot::release(void)
{
	// �ؽ�ó �̹��� ������
	if (_slotTexture)
		_slotTexture->Release();
}

void invenSlot::render(void)
{
	SPRITEMANAGER->spriteRender(_slotTexture, NULL, (int)_slotRC.left, (int)_slotRC.top, _color);
}

void invenSlot::releaseItem(void)
{
	this->releaseChild();		// �ڽ��� ��� ����
	_isParent = false;			// �θ� �ƴϴ�
	_item = NULL;				// �������� ����
}

void invenSlot::releaseChild(void)
{
	// �ڽ� ���� ����
	for (int i = 0; i < _childSlot.size(); i++)
	{
		_childSlot[i]->releaseParent();
	}

	_childSlot.clear();
}

void invenSlot::releaseParent(void)
{
	// �θ� ���� ����
	_item = NULL;
	_parent = NULL;
}

void invenSlot::addChild(invenSlot * childSlot)
{
	// �ڽ� �߰�
	childSlot->addParent(this);
	_childSlot.push_back(childSlot);	

}

void invenSlot::addParent(invenSlot* parentSlot)
{
	// �θ� �߰�
	_parent = parentSlot;
	// �θ��� ������ ���� ����
	_item = parentSlot->getItem();
	// �θ� �ƴ�
	_isParent = false;
}

void invenSlot::setItem(equip * item, bool isParent)
{
	if (_item != NULL) return;		// �̹� �������� �ִٸ� ����(�Ŀ� �߰� �ʿ�)

	_isParent = isParent;			// �θ��ΰ�?
	_item = item;					// ������ ����
}

invenSlot::invenSlot(int destX, int destY) : _parent(NULL), _slotTexture(NULL), _item(NULL)
{
	// ������
	D3DXCreateTextureFromFile(device,
		"Resources/mabinogi/UI/itemSlot.dds",
		&_slotTexture);

	_slotRC = { destX, destY, destX + SLOTSIZE, destY + SLOTSIZE };

	_color = 0xd000ffff;
}