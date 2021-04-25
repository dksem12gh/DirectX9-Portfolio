#pragma once
#include "singletonBase.h"
#include "equip.h"

class equipManager : public singletonBase<equipManager>
{
private:
	typedef map<string, equip*>					MAP_ITEM;
	typedef map<string, equip*>::iterator		ITEM_ITER;
	typedef list<equip*>						LIST_ITEM;
	typedef list<equip*>::iterator				LIST_ITER;
private:
	MAP_ITEM				_mapItemObject;
	LIST_ITEM				_listItemObject;

public:
	void release(void);
	equip* addSampleItem(string name, string itemPath, string mtrlPath, string invenImage, int invenSizeWidth, int invenSizeHeight, int invenImageFrameCount, int invenImageWidth, int invenImageHeight, EQUIP_PARTS equipParts, int price, int stack = 1, D3DXMATRIXA16 * pMatCorrection = NULL);
	equip* findSampleItem(string name);
	equip* addItem(equip* item);
	equip* addItem(string itemName);

	void deleteItem(equip* item);
	equipManager() {};
	~equipManager() {};
};