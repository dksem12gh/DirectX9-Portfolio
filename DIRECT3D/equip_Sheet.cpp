#include "stdafx.h"
#include "equip_Sheet.h"


HRESULT equip_Sheet::init(string filePath, string mtrlPath, D3DXMATRIXA16* matCorrection)
{
	_object = new baseObject;
	_object->setMesh(RM_SKINNED->addResource(filePath, mtrlPath, matCorrection));

	return S_OK;
}

void equip_Sheet::release(void)
{
}

void equip_Sheet::setEquip(void)
{
	if (_object == NULL) return;

	_object->setActive(!_object->getActive());
}

void equip_Sheet::setEquip(bool equip)
{
	if (_object == NULL) return;

	_object->setActive(equip);
}
