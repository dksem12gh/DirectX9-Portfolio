#include "stdafx.h"
#include "UI.h"

HRESULT UI::init(player* pPlayer, dyeing* dyeingInterface)
{
	//UI TEXTURE �ʱ�ȭ
	this->textureInit();


	_winPos = { WINSIZEX / 2 ,WINSIZEY / 2 };

	this->rectInit();
	//������ �� ���콺 �浹 bool����  
	for (int i = 0; i < 3; i++)
	{
		//��Ʈ 0->2->4 ������ �浹 üũ �浹�� �Ұ� �迭 3��;
		_collision[i] = false;
	}

	//20180618 UP
	_opSkillWindow = false;
	pickSkill = false;
	skillRcNumber = 0;
	//2018 06 19
	_opPlayerInfo = false;



	_player = pPlayer;
	_player->getUIInterface(this);

	_inventory = new inventory();
	_inventory->init();
	_inventory->setMeshInfo(_player->getMesh());

	_mouseInItem = NULL;//EQUIP->findItem("�ʺ��ڿ� ����");
	_inventory->setMouseOnItemPointer(&_mouseInItem);


	//2018 0619
	this->textInit();

	//2018 06 21 new 
	_uibarProgess[0] = new spriteProgress(_winPos.x - 294, _winPos.y + 342, 150, 15, _player->getMaxHpAddress(), _player->getCurHpAddress(), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(207, 64, 144), D3DCOLOR_XRGB(123, 65, 113));
	_uibarProgess[1] = new spriteProgress(_winPos.x - 294, _winPos.y + 359, 150, 15, _player->getMaxMpAddress(), _player->getCurMpAddress(), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(90, 108, 182), D3DCOLOR_XRGB(75, 80, 148));
	_uibarProgess[2] = new spriteProgress(_winPos.x - 294, _winPos.y + 376, 150, 15, _player->getMaxStAddress(), _player->getCurStAddress(), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(243, 193, 32), D3DCOLOR_XRGB(164, 120, 11));
	_uibarProgess[3] = new spriteProgress(_winPos.x - 130, _winPos.y - 37, 200, 15, _player->getMaxHpAddress(), _player->getCurHpAddress(), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(207, 64, 144), D3DCOLOR_XRGB(123, 65, 113));
	_uibarProgess[4] = new spriteProgress(_winPos.x - 130, _winPos.y - 19, 200, 15, _player->getMaxMpAddress(), _player->getCurMpAddress(), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(90, 108, 182), D3DCOLOR_XRGB(75, 80, 148));
	_uibarProgess[5] = new spriteProgress(_winPos.x - 130, _winPos.y - 1, 200, 15, _player->getMaxStAddress(), _player->getCurStAddress(), D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(243, 193, 32), D3DCOLOR_XRGB(164, 120, 11));

	_dyeingInterface = dyeingInterface;

	_inventory->linkDyeingInterface(_dyeingInterface);

	_shop = new shopInventory(100, 100);
	_shop->init();
	return S_OK;
}

void UI::release(void)
{
}

void UI::update(void)
{
	

	// ��ư ������ ���콺�� ��Ʈ�� �⵹�� ������ Ŭ�� �Ұ��� ���� â�� ������ ����
	//0618 �߰�
	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		if (_opPlayerInfo) { _opPlayerInfo = false; }
		else if (!_opPlayerInfo) { _opPlayerInfo = true; }
	}
	

	if (KEYMANAGER->isOnceKeyDown('I'))
	{
		if (_inventory->getShow())
		{ 
			SOUNDMANAGER->play("InvenOpen", 1.0f);
			_inventory->setShow(false);
		}
		else
		{ 
			SOUNDMANAGER->play("InvenClose", 1.0f);
			_inventory->setShow(true);
		}
		_shop->setOpen(!_shop->getIsOpen());
	}
	//0618 �߰�
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		if (_opSkillWindow) { _opSkillWindow = false; }
		else if (!_opSkillWindow) { _opSkillWindow = true; }
	}
	

	////������ �� ���콺 �� �浹 ���� �ۿ� ������ ������ �ʱ� ������... 
	//0618 �߰�
	if (KEYMANAGER->isOnceKeyDown(VK_BACK))
	{ 
		_openInventory = false; 
		_opSkillWindow = false;
		_opPlayerInfo = false;
	}
	
	if (_openInventory)
	{
		if (_mouseInItem)
		{
			_mouseInItem->setImagePosition(MYUTIL::getMousePos().x - _mouseInItem->getFrameWidth() / 2,
				MYUTIL::getMousePos().y - _mouseInItem->getFrameHeight() / 2);
			_mouseInItem->update();
		}
		
		if(_inventory->isMouseInInventory())
			_inventory->update();
	}
	
	for (int i = 0; i < 3; i++)
	{
		_collision[i] = false;
	}
	
	//0618 
	//��ųâ�� ������������ ������Ʈ 
	if (_opSkillWindow)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			for (int i = 1; i < _windowSkillRc.size(); i++)
			{   
				
				//���콺�� ��ų��Ʈ�� �浹���̰� ������ �ƴҶ���
				if (PtInRect(&_windowSkillRc[i], MYUTIL::getMousePos()) && !pickSkill)
				{
					//�� ��ų Ʈ��
					pickSkill = true;
					//���� �ڸ� ����ض�...
					_returnToIcon = _windowSkillRc[i];
					//��ų �迭�� ���� ���� 
					skillRcNumber = i;
				}
			

				if (pickSkill)
				{    
					//���콺�� �������� ���� ����
					_windowSkillRc[skillRcNumber].left = MYUTIL::getMousePos().x - 23;
					_windowSkillRc[skillRcNumber].top = MYUTIL::getMousePos().y - 23;
					_windowSkillRc[skillRcNumber].right = MYUTIL::getMousePos().x + 23;
					_windowSkillRc[skillRcNumber].bottom = MYUTIL::getMousePos().y + 23;
				}
			}
		}

		//�浹�� 
		RECT tempRect;
		RECT tempRect1;
		//Ű���� �Ǿ����� 		
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
		{
			for (int i = 1; i < _windowSkillRc.size(); i++)
			{
				for (int j = 0; j <8; j++)
				{   //���� ��Ʈ �ΰ� �� �浹 ������
					if (IntersectRect(&tempRect, &_windowSkillRc[i], &_skillIconRc[j]) && IntersectRect(&tempRect1, &_windowSkillRc[i], &_skillIconRc[j+1]))
					{
						//ȭ����� ��ų �� �� ��Ʈ �浹�߿� 
						//�巡�� �� ��Ʈ�� ��ų�� ��Ʈ �ΰ��� �ѹ���
						//�浹 �� ��� �Ѱ����� �����ϴµ�...
						
						//������ ���� �� ����1 �� ���� ���� ũ��  
						if (tempRect.left*tempRect.top > tempRect1.left*tempRect1.top)
						{
							//������ �������� �־������ϴµ�..... ���������� ������...
							_player->setPlayerSkillIcon(j, i);
						}
					}
					else if (IntersectRect(&tempRect, &_windowSkillRc[i], &_skillIconRc[j]))
					{
						_player->setPlayerSkillIcon(j, i);
					}
				}
			}
			pickSkill = false;
			//��ų�ٿ� �浹�� �ȵž�� �����ڸ��� ����	
			_windowSkillRc[skillRcNumber] = _returnToIcon;
		}
	}
	//0621 ���α׷����� �߰�
	for (int i = 0; i < 6; i++)
	{
		_uibarProgess[i]->update();
	}

	if (_shop->getIsOpen() && _shop->isMouseInShop()) _shop->update();
}

void UI::showWindows()
{
	//0621
	char Lv[32];
	sprintf(Lv, "%d", _player->getLv());

	//uiBar   //0621 ���� �� �ؽ�Ʈ �߰� 
	SPRITEMANAGER->spriteRender(_uiBar, NULL, _uiBarRc.left, _uiBarRc.top, 1.545,1,0,0xf000ffff);
	FONTMANAGER->fontOutLine("tap1", "LV", _winPos.x -115, _winPos.y + 375, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", Lv, _winPos.x -95, _winPos.y + 375, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));


	//���콺�� �����ܷ�Ʈ�� �浹 ����.
	if (_opPlayerInfo)
	{
		_collision[0] = true;
	}
	if (_openInventory) 
	{
		_collision[1] = true;
	}
	if (_opSkillWindow)
	{
		_collision[2] = true;
	}

	

	//������ Ŭ���� Ʈ���϶� �κ��丮 ������ ǥ��
	if (_inventory->getShow())
	{
		//������â �ؽ�Ʈ ����	
		_inventory->render();

	}
	if (_mouseInItem) _mouseInItem->renderImage();
	this->iconAni();

	//skill icon
	//�÷��̾��� ��ų ���� ������ ���� ����
	map<int, int> getPlayerIcon = _player->getPlayerSkillIcon();
	for (int i = 0; i < 8; i++)
	{
		SPRITEMANAGER->spriteRender(_skillIcon[getPlayerIcon.find(i)->second], NULL, _skillIconRc[i].left, _skillIconRc[i].top);
	}
	for (int i = 0; i < 6; i++)
	{
		//�÷��̾��� ��ųī��Ʈ�� 0�� �ƴҶ�(�ƹ��͵� ������) 
		//�÷��̾��� ��ųī��Ʈ�� 5�� �ƴҶ�(�޽��� �ƴҶ�)
		//��ų�� �����ϸ� �ٲ��!
		if (_player->getPlayerSkillCount() == i && 
			_player->getPlayerSkillCount() != 0 && 
			_player->getPlayerSkillCount() != 5)
		{
			SPRITEMANAGER->spriteRender(_skillIconPlayer[_player->getPlayerSkillCount()], NULL, _playerSkillIconRc.left, _playerSkillIconRc.top);
		}
	}	
	//0618 ������â�� ���������� ����
	if (_opSkillWindow)
	{
		SPRITEMANAGER->spriteRender(_skillWindowTexture[0], NULL, _skillWinRc.left, _skillWinRc.top,1.37,1,0, 0xf000ffff);

		for (int i = 1; i < _windowSkillRc.size(); i++)
		{		
			//�����ֱ�� ��ų ������
			SPRITEMANAGER->spriteRender(_skillIcon[i], NULL, 100, 100 + (i * 70));
			// ��ų �̸�ǥ�� �� �ε�... ���伥���� ��ģ �̹����� �θ���� ũ�Ⱑ Ʋ���������� �׳� ���� ���� ������.
			SPRITEMANAGER->spriteRender(_skillWindowTexture[1], NULL, 163, 100 + (i * 70), 0xf000ffff);
			//�巡�׿� ��ų ������ 
			//������� �״�� ��.... �׷��� i=1����
			SPRITEMANAGER->spriteRender(_skillIcon[i], NULL, _windowSkillRc[i].left, _windowSkillRc[i].top, 0x75ffffff);
		}
		this->skillTextOut();
	}
	//0621 ���α׷����� ���� �߰�  0~ 3�� ui�� �� 
	for (int i = 0; i < 3; i++)
	{
		_uibarProgess[i]->render();
	}

	//2018 0619  //0621 �߰� 
	if (_opPlayerInfo)
	{
		SPRITEMANAGER->spriteRender(_infoWindow, NULL, _infoWinRc.left, _infoWinRc.top,1.25,1.25,0 ,0xf000ffff);
		
		//0621 �߰� 3~ 6�� ĳ���� ����â
		for (int i = 3; i <6; i++)
		{
			_uibarProgess[i]->render();
		}
		this->infoTextOut();
	}

	if(_shop->getIsOpen()) _shop->render();
}

void UI::textureInit()
{
	//ui bar
	_uiBar = RM_TEXTURE->addResource("Resources/mabinogi/UI/uiBar.png");
	//Menu icon 0618 �̸��ٲ�
	_menuIconUi[0] = RM_TEXTURE->addResource("Resources/mabinogi/UI/info.png");
	_menuIconUi[1] = RM_TEXTURE->addResource("Resources/mabinogi/UI/infoC.png");
	_menuIconUi[2] = RM_TEXTURE->addResource("Resources/mabinogi/UI/itemIcon.png");
	_menuIconUi[3] = RM_TEXTURE->addResource("Resources/mabinogi/UI/itemIconC.png");
	_menuIconUi[4] = RM_TEXTURE->addResource("Resources/mabinogi/UI/skill_icon.png");
	_menuIconUi[5] = RM_TEXTURE->addResource("Resources/mabinogi/UI/skill_iconC.png");
	//skill icon
	_skillIcon[0] = RM_TEXTURE->addResource("Resources/mabinogi/UI/skillBar.png");
	_skillIcon[1] = RM_TEXTURE->addResource("Resources/mabinogi/UI/smash.png");
	_skillIcon[2] = RM_TEXTURE->addResource("Resources/mabinogi/UI/defence.png");
	_skillIcon[3] = RM_TEXTURE->addResource("Resources/mabinogi/UI/counterAtk.png");
	_skillIcon[4] = RM_TEXTURE->addResource("Resources/mabinogi/UI/windmill.png");
	_skillIcon[5] = RM_TEXTURE->addResource("Resources/mabinogi/UI/husic.png");

	//player skill icon
	_skillIconPlayer[1] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Smash.png");
	_skillIconPlayer[2] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Defence.png");
	_skillIconPlayer[3] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Counter.png");
	_skillIconPlayer[4] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Windmill.png");

	//201806���� ��ųâ ��� ����
	_skillWindowTexture[0] = RM_TEXTURE->addResource("Resources/mabinogi/UI/s_windowUi.dds");
	_skillWindowTexture[1] = RM_TEXTURE->addResource("Resources/mabinogi/UI/skillName.png");

	//20180619
	_infoWindow = RM_TEXTURE->addResource("Resources/mabinogi/UI/P_Info.png");

}

void UI::rectInit()
{
	//0621���� 
	D3DSURFACE_DESC uiBardesc;
	_uiBar->GetLevelDesc(0, &uiBardesc);
	//ui bar Rc
	_uiBarRc = { _winPos.x-390,_winPos.y+335,(LONG)((_winPos.x+(LONG)uiBardesc.Width)*1.545),(LONG)(_winPos.y+(LONG)uiBardesc.Height) };

	

	//icon Rc   //0621  �� ���� 
	_iconUiRc[0] = { _winPos.x - 60,_winPos.y + 310,_winPos.x - 4,_winPos.y + 357 };
	_iconUiRc[1] = { _winPos.x - 60,_winPos.y + 310,_winPos.x - 4,_winPos.y + 357 };
	_iconUiRc[2] = { _winPos.x + 20,_winPos.y + 310,_winPos.x + 76,_winPos.y + 357 };
	_iconUiRc[3] = { _winPos.x + 20,_winPos.y + 310,_winPos.x + 76,_winPos.y + 357 };
	_iconUiRc[4] = { _winPos.x + 100,_winPos.y + 310,_winPos.x + 156,_winPos.y + 357 };
	_iconUiRc[5] = { _winPos.x + 100,_winPos.y + 310,_winPos.x + 156,_winPos.y + 357 };
	
	//skill Rc
	for(int i = 0; i < 8; i++)
	{
		_skillIconRc[i] = { 10 + (i * 70), 10, 52 + (i * 70), 52 };
	}
	//0618
	//��ųâ ���� ��ų RC ���� �ʱ�ȭ
	for (int i = 0; i < 6; i++)
	{
		RECT _winSkillRc;
		_winSkillRc = { 100,100 + (i * 70),162,162 + (i * 72) };
		_windowSkillRc.push_back(_winSkillRc);
	}
	//��ųâ RC //  226x331
	
	D3DSURFACE_DESC desc;
	_skillWindowTexture[0]->GetLevelDesc(0, &desc);

	_skillWinRc = { 88,96, (LONG)((88 + (LONG)desc.Width) * 1.37f), 96 + (LONG)desc.Height };

	//20180619
	_infoWindow->GetLevelDesc(0, &desc);
	_infoWinRc = {_winPos.x-220,_winPos.y-200,(LONG)((_winPos.x+(LONG)desc.Width)*1.25f),(LONG)((_winPos.x + (LONG)desc.Height)*1.25f) };

	
	//player skill rc
	_playerSkillIconRc = { WINSIZEX / 2 - 35, WINSIZEY / 2 - 200 };

}

void UI::iconAni()
{
	//�Ұ��� ���� ������ ȿ��
	if (!_collision[0])
	{
		SPRITEMANAGER->spriteRender(_menuIconUi[0], NULL, _iconUiRc[0].left, _iconUiRc[0].top, 0xf000ffff);
	}
	else if (_collision[0])
	{
		SPRITEMANAGER->spriteRender(_menuIconUi[1], NULL, _iconUiRc[1].left, _iconUiRc[1].top, 0xf000ffff);
	}


	if (!_collision[1])
	{
		SPRITEMANAGER->spriteRender(_menuIconUi[2], NULL, _iconUiRc[2].left, _iconUiRc[2].top, 0xf000ffff);
	}
	else if (_collision[1])
	{
		SPRITEMANAGER->spriteRender(_menuIconUi[3], NULL, _iconUiRc[3].left, _iconUiRc[3].top, 0xf000ffff);
	}

	if (!_collision[2])
	{
		SPRITEMANAGER->spriteRender(_menuIconUi[4], NULL, _iconUiRc[4].left, _iconUiRc[4].top, 0xf000ffff);
	}
	else if (_collision[2])
	{
		SPRITEMANAGER->spriteRender(_menuIconUi[5], NULL, _iconUiRc[5].left, _iconUiRc[5].top, 0xf000ffff);
	}
}


//2018 0618 
void UI::textInit()
{
	FONTMANAGER->addFont("skill", "������� ExtraBold ", 15);
	FONTMANAGER->addFont("info", "������� ExtraBold", 18);
	FONTMANAGER->addFont("tap1", "������� ExtraBold", 13);
	FONTMANAGER->addFont("stat", "������� ExtraBold", 11);
	FONTMANAGER->addFont("Name", "������� ExtraBold", 15);
}

void UI::skillTextOut()
{
	FONTMANAGER->fontOutLine("skill", "Skill", _winPos.x - 525 , _winPos.y - 285 ,D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "��ų",  _winPos.x - 525 ,  _winPos.y - 270 , D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "Smash", _winPos.x - 505, _winPos.y - 215, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "���Ž�", _winPos.x - 504, _winPos.y - 200, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "Defence", _winPos.x - 510, _winPos.y - 145, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "���潺", _winPos.x - 504, _winPos.y - 130, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "Counterattack", _winPos.x - 525, _winPos.y - 75, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "ī���� ����", _winPos.x - 515, _winPos.y - 60, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "WindMill", _winPos.x - 510, _winPos.y - 5, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "�����", _winPos.x - 504, _winPos.y +10, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "Rest", _winPos.x - 500, _winPos.y +65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "�޽�", _winPos.x - 499, _winPos.y + 80, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
}

void UI::infoTextOut()
{
	char str[10][256];
	sprintf(str[0], "%d", _player->getLv());
	//�� ���� ����;;;
	sprintf(str[1], "%d", _player->getStr());
	sprintf(str[2], "%d", _player->getIntellect());
	sprintf(str[3], "%d", _player->getDex());
	sprintf(str[4], "%d", _player->getWill());
	sprintf(str[5], "%d", _player->getLuck());
	sprintf(str[6], "%d~%d", _player->getMinAtk(), _player->getMaxAtk());
	// 0621 �߰�
	sprintf(str[7], "%d", _player->getCriticalChance());
	sprintf(str[8], "%d", _player->getDefense());
	sprintf(str[9], "%d", _player->getProtect());
	

	FONTMANAGER->fontOutLine("info", "ĳ���� ����", _winPos.x - 140, _winPos.y - 180, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "�̸�  ", _winPos.x - 190, _winPos.y - 115, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	
	FONTMANAGER->fontOutLine("Name", _player->getName(), _winPos.x - 140, _winPos.y - 118, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "����/����  ", _winPos.x - 190, _winPos.y - 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "�ΰ�/����", _winPos.x - 130, _winPos.y - 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "����", _winPos.x -40, _winPos.y - 115, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[0], _winPos.x +10, _winPos.y - 115, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "����ġ", _winPos.x - 40, _winPos.y - 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("stat", "ü��", _winPos.x - 190, _winPos.y +45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[1], _winPos.x - 160, _winPos.y +45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));	
	FONTMANAGER->fontOutLine("stat", "����", _winPos.x - 190, _winPos.y + 55, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[2], _winPos.x - 160, _winPos.y + 55, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", "�ؾ�", _winPos.x - 190, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[3], _winPos.x - 160, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", "����", _winPos.x - 190, _winPos.y + 75, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[4], _winPos.x - 160, _winPos.y + 75, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", "���", _winPos.x - 190, _winPos.y + 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[5], _winPos.x - 160, _winPos.y + 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	/*FONTMANAGER->fontOutLine("tap1", "�����", _winPos.x -95, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[6], _winPos.x - 55, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "ũ��Ƽ��", _winPos.x - 95, _winPos.y + 75, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));*/


	//0621 �߰� ���� ���� �ּ��Ȱ� ���� ������ ��������  ��ģ�� �����ּ�  �����ּ���....
	FONTMANAGER->fontOutLine("tap1", "�����", _winPos.x - 190, _winPos.y - 35, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "����", _winPos.x - 190, _winPos.y - 17, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "���¹̳�", _winPos.x - 190, _winPos.y +1, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "�����", _winPos.x - 98, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[6], _winPos.x - 55, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "ũ��Ƽ��", _winPos.x - 98, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[7], _winPos.x - 35, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "���", _winPos.x+10 , _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[8], _winPos.x+65, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "��ȣ", _winPos.x + 10, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[9], _winPos.x + 65, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	

}



