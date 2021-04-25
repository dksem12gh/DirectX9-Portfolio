#pragma once
#include "inventory.h"
#include "shopInventory.h"
#include "player.h"
#include "spriteProgress.h"
#include "dyeing.h"

class player;
class inventory;
class shopInventory;

class UI
{
private:
	//ui 클래스 하위 클래스 생성해서 정리한번 해야할듯



	LPDIRECT3DTEXTURE9 _uiBar;        //ui bar 역활 없음
	LPDIRECT3DTEXTURE9 _menuIconUi[6];    //아이콘
	LPDIRECT3DTEXTURE9 _skillIcon[8]; //플레이어 스킬 아이콘
	LPDIRECT3DTEXTURE9 _skillIconPlayer[8];	//플레이어 머리위에 뜰 스킬 아이콘
	

	RECT _uiBarRc;                    //uiBar Rc
	RECT _iconUiRc[6];				  //iconRc
	RECT _skillIconRc[8];			  //플레이어 스킬 아이콘 RC        
	RECT _playerSkillIconRc;		  //플레이어 머리위에 뜰 스킬 아이콘 Rc
	RECT _itWindowRc;				  //아이템 윈도우창 Rc

	POINT _winPos;                    //윈도우 포지션

	player*			_player;
	equip*			_mouseInItem;
	inventory*		_inventory;
	dyeing*			_dyeingInterface;

	bool _collision[3];               //아이콘 충돌 변수
	bool _openInventory;              //아이콘을 클릭하였는가
  
    //0618 UPDATE
	LPDIRECT3DTEXTURE9 _skillWindowTexture[2];  // 스킬창 ui 
	int skillRcNumber;                       //충돌한 렉트가 몇번째 부터 인가?
	RECT _skillWinRc;                        //스킬창 Rc
	vector<RECT> _windowSkillRc;             //스킬 아이콘 RC 벡터
	RECT _returnToIcon;                      //다시  제자리로 ㅇㅇ	
	bool pickSkill;                          //스킬을 마우스로 선택했냐?
	bool _opSkillWindow;                     //스킬창이 켜져있냐?

	//2018 06 19
	bool _opPlayerInfo;                      //스텟창이 켜져있냐?
	LPDIRECT3DTEXTURE9 _infoWindow;			 //스텟창 텍스쳐
	RECT _infoWinRc;                         //텍스쳐 RC
	//06 21 
	spriteProgress* _uibarProgess[6];        //UI용 프로그래스바 

	shopInventory* _shop;
public:

	HRESULT init(player* pPlayer, dyeing* dyeingInterface);
	void release(void);
	void update(void);
	void inventoryUpdate(void) { _inventory->moveItem(); }
	void showWindows();
	void textureInit();
	void rectInit();
	//0618 new
	void textInit();
	void skillTextOut();
	//0618 new
	void infoTextOut();
	void iconAni();           //아이콘 변경 효과

	
	bool isOpenInventory(void) { return _openInventory; }
	bool isMouseInInventory(void) { return _inventory->isMouseInInventory(); }
	void addItem(equip* item) { this->_inventory->addItem(item); }

	bool isOpenShop(void) { return _shop->getIsOpen(); }
	bool isMouseInShop(void) { return _shop->isMouseInShop(); }
	equip* buyShop(void) { return _shop->buyItem(_inventory->getGoldAddress()); }
	void sellShop(void) { _shop->sellItem(_mouseInItem, _inventory->getGoldAddress()); }
	//0619
    //나중에 필요할수도 있어서
	bool isOpenSkill(void) { return _opSkillWindow;	}
	LPRECT getWinRect(void) { return &_skillWinRc; }

	bool isActiveSkill(void)
	{
		if (this->isOpenSkill() &&
			PtInRect(this->getWinRect(), MYUTIL::getMousePos()))
		{
			return true;
		}

		return false;
	}
	//0619
	bool isOpinfo(void) { return _opPlayerInfo; }
	LPRECT getInfoRc(void) { return &_infoWinRc; }
	
	inventory* getInventory(void) { return _inventory; }
	UI() {};
	~UI() {};
};

