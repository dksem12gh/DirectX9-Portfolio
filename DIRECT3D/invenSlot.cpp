#include "stdafx.h"
#include "invenSlot.h"


void invenSlot::release(void)
{
	// 텍스처 이미지 릴리즈
	if (_slotTexture)
		_slotTexture->Release();
}

void invenSlot::render(void)
{
	SPRITEMANAGER->spriteRender(_slotTexture, NULL, (int)_slotRC.left, (int)_slotRC.top, _color);
}

void invenSlot::releaseItem(void)
{
	this->releaseChild();		// 자식을 모두 제거
	_isParent = false;			// 부모가 아니다
	_item = NULL;				// 아이템을 제거
}

void invenSlot::releaseChild(void)
{
	// 자식 관계 끊기
	for (int i = 0; i < _childSlot.size(); i++)
	{
		_childSlot[i]->releaseParent();
	}

	_childSlot.clear();
}

void invenSlot::releaseParent(void)
{
	// 부모 관계 끊기
	_item = NULL;
	_parent = NULL;
}

void invenSlot::addChild(invenSlot * childSlot)
{
	// 자식 추가
	childSlot->addParent(this);
	_childSlot.push_back(childSlot);	

}

void invenSlot::addParent(invenSlot* parentSlot)
{
	// 부모 추가
	_parent = parentSlot;
	// 부모의 아이템 정보 공유
	_item = parentSlot->getItem();
	// 부모가 아님
	_isParent = false;
}

void invenSlot::setItem(equip * item, bool isParent)
{
	if (_item != NULL) return;		// 이미 아이템이 있다면 리턴(후에 추가 필요)

	_isParent = isParent;			// 부모인가?
	_item = item;					// 아이템 세팅
}

invenSlot::invenSlot(int destX, int destY) : _parent(NULL), _slotTexture(NULL), _item(NULL)
{
	// 생성자
	D3DXCreateTextureFromFile(device,
		"Resources/mabinogi/UI/itemSlot.dds",
		&_slotTexture);

	_slotRC = { destX, destY, destX + SLOTSIZE, destY + SLOTSIZE };

	_color = 0xd000ffff;
}