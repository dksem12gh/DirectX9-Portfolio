#pragma once
#include "invenSlot.h"
#include "equipSlot.h"
#include "meshInfo.h"
#include "dyeing.h"

#define INVENSLOTWIDTH		6
#define INVENSLOTHEIGHT		10

class meshInfo;

class inventory
{
private:
	invenSlot*								_invenSlot[10][6];			// �κ��丮 ���� 2�� �迭
	equipSlot*								_equipSlot[6];				// ���� ����

	dyeing*									_dyeingInterface;			// ���� �������̽�
private:
	LPDIRECT3DTEXTURE9						_windowTexture;				// ������ �ؽ�ó
	POINT									_itemWindowPos;             //������ ������ ������
	vector<invenSlot*>						_interSlot;
	equip**									_pickItem;					// ���� ���콺�� ���� �������� 2�� ������ �ּ�
	equip*									_mouseOnItem;				// ���� ���콺�� �÷����ִ� �������� ������ �ּ�
	list<equip*>							_itemList;

	RECT									_inventoryRC;				// �κ��丮 ��Ʈ
	RECT									_invenSlotTotalRC;			// ���� ���� ��Ʈ

	meshInfo*								_meshInfo;					// �޽� ����
	int										_totalGold;					// �κ��丮�� ����ִ� ��� �ѷ�
	bool									_isShow;					// ���� �κ��丮�� �����ִ°�?
public:
	HRESULT init(void);
	void release(void);
	void update(void);

	void render(void);


	void linkDyeingInterface(dyeing* dyeingInterface) { _dyeingInterface = dyeingInterface; }
	bool dyeing();
	// ���콺�� ���� �������� �����ϴ� ������ �ּ��� �ּҸ�(���� ������) �޾ƿ´�.
	void setMouseOnItemPointer(equip** ppItem) { _pickItem = ppItem; }

	void textInit();
	void inventoryTextOut();

	void moveItem(void);
	void setMeshInfo(meshInfo* meshInfo) { _meshInfo = meshInfo; }
	bool possibleaddItem(vector<invenSlot*>* pVecSlot, POINT invenSize);

	bool addItem(ITEMINFO itemInfo);
	bool addItem(equip* item);
	void addItem(equip* item, vector<invenSlot*>* pVecSlot, POINT invenSize);
	void destroyItem(equip* item);

	bool releaseItem(invenSlot* releaseSlot);

	bool isMouseInInventory(void) { return PtInRect(&_inventoryRC, MYUTIL::getMousePos()); }
	// 2018. 6. 28. ����ȣ - ���콺�� �κ� ���� ������ �ִ��� Ȯ���ϴ� �Լ�
	bool isMouseInSlotRC(void) { return PtInRect(&_invenSlotTotalRC, MYUTIL::getMousePos()); }
	int* getGoldAddress(void) { return &_totalGold; }

	// 2018. 6. 28. ����ȣ
	void setShow(bool show) { _isShow = show; }
	bool getShow(void) { return _isShow; }

	equip* getPickItem(void) { return *_pickItem; }
	inventory();
	~inventory() {};
};

