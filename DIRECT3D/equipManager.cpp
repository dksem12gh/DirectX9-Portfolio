#include "stdafx.h"
#include "equipManager.h"

void equipManager::release(void)
{
	ITEM_ITER iter = _mapItemObject.begin();

	for (; iter != _mapItemObject.end(); ++iter)
	{
		iter->second->release();
		SAFE_DELETE(iter->second);
	}

	_mapItemObject.clear();

	LIST_ITER listIter = _listItemObject.begin();

	for (; listIter != _listItemObject.end(); ++listIter)
	{
		iter->second->release();
		SAFE_DELETE(iter->second);
	}

	_listItemObject.clear();
}

equip* equipManager::addSampleItem(string name, 
	string itemPath, 
	string mtrlPath, 
	string invenImage, 
	int invenSizeWidth, 
	int invenSizeHeight, 
	int invenImageFrameCount, 
	int invenImageWidth, 
	int invenImageHeight, 
	EQUIP_PARTS equipParts, 
	int price,
	int stack,
	D3DXMATRIXA16 * pMatCorrection)
{
	equip* newItem = new equip(name, 
		itemPath, 
		mtrlPath,
		invenImage,
		invenSizeWidth,
		invenSizeHeight,
		invenImageFrameCount,
		invenImageWidth,
		invenImageHeight,
		equipParts,
		price,
		stack,
		pMatCorrection);

	_mapItemObject.insert(make_pair(newItem->getName(), newItem));

	return newItem;
}

equip * equipManager::findSampleItem(string name)
{
	return _mapItemObject.find(name)->second;
}

equip * equipManager::addItem(equip* item)
{
	equip* clone = item->cloneItem();

	_listItemObject.push_back(clone);

	return clone;
}

equip * equipManager::addItem(string itemName)
{
	equip* originItem = this->findSampleItem(itemName);

	equip* clone = originItem->cloneItem();

	_listItemObject.push_back(clone);

	return clone;
}

void equipManager::deleteItem(equip * item)
{
	LIST_ITER iter = _listItemObject.begin();
	for (; iter != _listItemObject.end(); ++iter)
	{
		if (*iter == item)
		{
			_listItemObject.erase(iter);
			item->release();
			SAFE_DELETE(item);

			item = NULL;
			return;
		}
	}
}
