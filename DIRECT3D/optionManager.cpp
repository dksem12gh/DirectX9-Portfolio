#include "stdafx.h"
#include "optionManager.h"


HRESULT optionManager::init(camera* pCamera)
{
	_outline = true;

	_pCamera = pCamera;

	_defaultCursor = RM_TEXTURE->addResource("Resources/mabinogi/ui/defaultCursor.dds");
	_spoidCursor = RM_TEXTURE->addResource("Resources/mabinogi/ui/spoid.dds");

	this->setCursor(defaultCursor);
	return S_OK;
}

optionManager::optionManager()
{
}


optionManager::~optionManager()
{
}

void optionManager::renderCursor(void)
{
	SPRITEMANAGER->spriteRender(_curCursor, NULL, MYUTIL::getMousePos().x, MYUTIL::getMousePos().y, 0xffffffff);
}

void optionManager::renderCursor(POINT mousePos)
{
	if (_curCursor == _spoidCursor)
	{
		D3DXVECTOR3 center = D3DXVECTOR3(9, 7, 0);
		SPRITEMANAGER->spriteRender(_curCursor, NULL, mousePos.x, mousePos.y, 0xffffffff, &center);
	}
	else
	{
		SPRITEMANAGER->spriteRender(_curCursor, NULL, mousePos.x, mousePos.y, 0xffffffff);
	}
}

void optionManager::setCursor(MABI_CURSOR cursor)
{
	if (cursor == defaultCursor)
	{
		_curCursor = _defaultCursor;
	}
	else
	{
		_curCursor = _spoidCursor;
	}
}