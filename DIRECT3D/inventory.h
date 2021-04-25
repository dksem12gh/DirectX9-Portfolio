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
	invenSlot*								_invenSlot[10][6];			// 인벤토리 슬롯 2차 배열
	equipSlot*								_equipSlot[6];				// 장착 슬롯

	dyeing*									_dyeingInterface;			// 염색 인터페이스
private:
	LPDIRECT3DTEXTURE9						_windowTexture;				// 윈도우 텍스처
	POINT									_itemWindowPos;             //아이템 윈도우 포지션
	vector<invenSlot*>						_interSlot;
	equip**									_pickItem;					// 현재 마우스로 잡은 아이템의 2차 포인터 주소
	equip*									_mouseOnItem;				// 현재 마우스가 올려져있는 아이템의 포인터 주소
	list<equip*>							_itemList;

	RECT									_inventoryRC;				// 인벤토리 렉트
	RECT									_invenSlotTotalRC;			// 슬롯 영역 렉트

	meshInfo*								_meshInfo;					// 메쉬 인포
	int										_totalGold;					// 인벤토리에 들어있는 골드 총량
	bool									_isShow;					// 현재 인벤토리가 열려있는가?
public:
	HRESULT init(void);
	void release(void);
	void update(void);

	void render(void);


	void linkDyeingInterface(dyeing* dyeingInterface) { _dyeingInterface = dyeingInterface; }
	bool dyeing();
	// 마우스로 잡은 아이템을 저장하는 포인터 주소의 주소를(더블 포인터) 받아온다.
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
	// 2018. 6. 28. 박인호 - 마우스가 인벤 슬롯 영역에 있는지 확인하는 함수
	bool isMouseInSlotRC(void) { return PtInRect(&_invenSlotTotalRC, MYUTIL::getMousePos()); }
	int* getGoldAddress(void) { return &_totalGold; }

	// 2018. 6. 28. 박인호
	void setShow(bool show) { _isShow = show; }
	bool getShow(void) { return _isShow; }

	equip* getPickItem(void) { return *_pickItem; }
	inventory();
	~inventory() {};
};

