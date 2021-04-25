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
	//ui Ŭ���� ���� Ŭ���� �����ؼ� �����ѹ� �ؾ��ҵ�



	LPDIRECT3DTEXTURE9 _uiBar;        //ui bar ��Ȱ ����
	LPDIRECT3DTEXTURE9 _menuIconUi[6];    //������
	LPDIRECT3DTEXTURE9 _skillIcon[8]; //�÷��̾� ��ų ������
	LPDIRECT3DTEXTURE9 _skillIconPlayer[8];	//�÷��̾� �Ӹ����� �� ��ų ������
	

	RECT _uiBarRc;                    //uiBar Rc
	RECT _iconUiRc[6];				  //iconRc
	RECT _skillIconRc[8];			  //�÷��̾� ��ų ������ RC        
	RECT _playerSkillIconRc;		  //�÷��̾� �Ӹ����� �� ��ų ������ Rc
	RECT _itWindowRc;				  //������ ������â Rc

	POINT _winPos;                    //������ ������

	player*			_player;
	equip*			_mouseInItem;
	inventory*		_inventory;
	dyeing*			_dyeingInterface;

	bool _collision[3];               //������ �浹 ����
	bool _openInventory;              //�������� Ŭ���Ͽ��°�
  
    //0618 UPDATE
	LPDIRECT3DTEXTURE9 _skillWindowTexture[2];  // ��ųâ ui 
	int skillRcNumber;                       //�浹�� ��Ʈ�� ���° ���� �ΰ�?
	RECT _skillWinRc;                        //��ųâ Rc
	vector<RECT> _windowSkillRc;             //��ų ������ RC ����
	RECT _returnToIcon;                      //�ٽ�  ���ڸ��� ����	
	bool pickSkill;                          //��ų�� ���콺�� �����߳�?
	bool _opSkillWindow;                     //��ųâ�� �����ֳ�?

	//2018 06 19
	bool _opPlayerInfo;                      //����â�� �����ֳ�?
	LPDIRECT3DTEXTURE9 _infoWindow;			 //����â �ؽ���
	RECT _infoWinRc;                         //�ؽ��� RC
	//06 21 
	spriteProgress* _uibarProgess[6];        //UI�� ���α׷����� 

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
	void iconAni();           //������ ���� ȿ��

	
	bool isOpenInventory(void) { return _openInventory; }
	bool isMouseInInventory(void) { return _inventory->isMouseInInventory(); }
	void addItem(equip* item) { this->_inventory->addItem(item); }

	bool isOpenShop(void) { return _shop->getIsOpen(); }
	bool isMouseInShop(void) { return _shop->isMouseInShop(); }
	equip* buyShop(void) { return _shop->buyItem(_inventory->getGoldAddress()); }
	void sellShop(void) { _shop->sellItem(_mouseInItem, _inventory->getGoldAddress()); }
	//0619
    //���߿� �ʿ��Ҽ��� �־
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

