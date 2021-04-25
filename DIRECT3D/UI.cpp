#include "stdafx.h"
#include "UI.h"

HRESULT UI::init(player* pPlayer, dyeing* dyeingInterface)
{
	//UI TEXTURE 초기화
	this->textureInit();


	_winPos = { WINSIZEX / 2 ,WINSIZEY / 2 };

	this->rectInit();
	//아이콘 과 마우스 충돌 bool변수  
	for (int i = 0; i < 3; i++)
	{
		//렉트 0->2->4 순으로 충돌 체크 충돌용 불값 배열 3개;
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

	_mouseInItem = NULL;//EQUIP->findItem("초보자용 바지");
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
	

	// 버튼 눌렀고 마우스가 렉트랑 출돌중 아이콘 클릭 불값에 따라 창이 켜지고 닫힘
	//0618 추가
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
	//0618 추가
	if (KEYMANAGER->isOnceKeyDown('S'))
	{
		if (_opSkillWindow) { _opSkillWindow = false; }
		else if (!_opSkillWindow) { _opSkillWindow = true; }
	}
	

	////아이콘 과 마우스 가 충돌 범위 밖에 있을때 닫히지 않기 때문에... 
	//0618 추가
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
	//스킬창이 켜져있을때만 업데이트 
	if (_opSkillWindow)
	{
		if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
		{
			for (int i = 1; i < _windowSkillRc.size(); i++)
			{   
				
				//마우스랑 스킬렉트랑 충돌중이고 선택이 아닐때만
				if (PtInRect(&_windowSkillRc[i], MYUTIL::getMousePos()) && !pickSkill)
				{
					//픽 스킬 트루
					pickSkill = true;
					//원래 자리 기억해라...
					_returnToIcon = _windowSkillRc[i];
					//스킬 배열의 값을 저장 
					skillRcNumber = i;
				}
			

				if (pickSkill)
				{    
					//마우스에 아이콘이 따라감 ㅇㅇ
					_windowSkillRc[skillRcNumber].left = MYUTIL::getMousePos().x - 23;
					_windowSkillRc[skillRcNumber].top = MYUTIL::getMousePos().y - 23;
					_windowSkillRc[skillRcNumber].right = MYUTIL::getMousePos().x + 23;
					_windowSkillRc[skillRcNumber].bottom = MYUTIL::getMousePos().y + 23;
				}
			}
		}

		//충돌용 
		RECT tempRect;
		RECT tempRect1;
		//키업이 되었을때 		
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
		{
			for (int i = 1; i < _windowSkillRc.size(); i++)
			{
				for (int j = 0; j <8; j++)
				{   //템프 렉트 두개 의 충돌 조건중
					if (IntersectRect(&tempRect, &_windowSkillRc[i], &_skillIconRc[j]) && IntersectRect(&tempRect1, &_windowSkillRc[i], &_skillIconRc[j+1]))
					{
						//화면상의 스킬 바 의 렉트 충돌중에 
						//드래그 한 렉트와 스킬바 렉트 두개가 한번에
						//충돌 될 경우 한곳에만 들어가야하는데...
						
						//템프의 넓이 가 템프1 의 넓이 보다 크면  
						if (tempRect.left*tempRect.top > tempRect1.left*tempRect1.top)
						{
							//문제는 왼쪽으로 넣어져야하는데..... 오른쪽으로 가버림...
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
			//스킬바에 충돌이 안돼었어도 원래자리로 ㅇㅇ	
			_windowSkillRc[skillRcNumber] = _returnToIcon;
		}
	}
	//0621 프로그래스바 추가
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

	//uiBar   //0621 변경 및 텍스트 추가 
	SPRITEMANAGER->spriteRender(_uiBar, NULL, _uiBarRc.left, _uiBarRc.top, 1.545,1,0,0xf000ffff);
	FONTMANAGER->fontOutLine("tap1", "LV", _winPos.x -115, _winPos.y + 375, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", Lv, _winPos.x -95, _winPos.y + 375, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));


	//마우스가 아이콘렉트랑 충돌 판정.
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

	

	//아이콘 클릭이 트루일때 인벤토리 윈도우 표시
	if (_inventory->getShow())
	{
		//아이템창 텍스트 정보	
		_inventory->render();

	}
	if (_mouseInItem) _mouseInItem->renderImage();
	this->iconAni();

	//skill icon
	//플레이어의 스킬 맵을 가져옴 그후 변경
	map<int, int> getPlayerIcon = _player->getPlayerSkillIcon();
	for (int i = 0; i < 8; i++)
	{
		SPRITEMANAGER->spriteRender(_skillIcon[getPlayerIcon.find(i)->second], NULL, _skillIconRc[i].left, _skillIconRc[i].top);
	}
	for (int i = 0; i < 6; i++)
	{
		//플레이어의 스킬카운트가 0이 아닐때(아무것도 없을때) 
		//플레이어의 스킬카운트가 5가 아닐때(휴식이 아닐때)
		//스킬을 실행하면 바뀐다!
		if (_player->getPlayerSkillCount() == i && 
			_player->getPlayerSkillCount() != 0 && 
			_player->getPlayerSkillCount() != 5)
		{
			SPRITEMANAGER->spriteRender(_skillIconPlayer[_player->getPlayerSkillCount()], NULL, _playerSkillIconRc.left, _playerSkillIconRc.top);
		}
	}	
	//0618 윈도우창이 켜져있을때 랜더
	if (_opSkillWindow)
	{
		SPRITEMANAGER->spriteRender(_skillWindowTexture[0], NULL, _skillWinRc.left, _skillWinRc.top,1.37,1,0, 0xf000ffff);

		for (int i = 1; i < _windowSkillRc.size(); i++)
		{		
			//보여주기용 스킬 아이콘
			SPRITEMANAGER->spriteRender(_skillIcon[i], NULL, 100, 100 + (i * 70));
			// 스킬 이름표기 바 인데... 포토샵에서 합친 이미지를 부를경우 크기가 틀어져버려서 그냥 따로 랜더 시켰음.
			SPRITEMANAGER->spriteRender(_skillWindowTexture[1], NULL, 163, 100 + (i * 70), 0xf000ffff);
			//드래그용 스킬 아이콘 
			//만들어논거 그대로 씀.... 그래서 i=1부터
			SPRITEMANAGER->spriteRender(_skillIcon[i], NULL, _windowSkillRc[i].left, _windowSkillRc[i].top, 0x75ffffff);
		}
		this->skillTextOut();
	}
	//0621 프로그래스바 랜더 추가  0~ 3은 ui바 용 
	for (int i = 0; i < 3; i++)
	{
		_uibarProgess[i]->render();
	}

	//2018 0619  //0621 추가 
	if (_opPlayerInfo)
	{
		SPRITEMANAGER->spriteRender(_infoWindow, NULL, _infoWinRc.left, _infoWinRc.top,1.25,1.25,0 ,0xf000ffff);
		
		//0621 추가 3~ 6은 캐릭터 정보창
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
	//Menu icon 0618 이름바꿈
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

	//201806십팔 스킬창 배경 ㅇㅇ
	_skillWindowTexture[0] = RM_TEXTURE->addResource("Resources/mabinogi/UI/s_windowUi.dds");
	_skillWindowTexture[1] = RM_TEXTURE->addResource("Resources/mabinogi/UI/skillName.png");

	//20180619
	_infoWindow = RM_TEXTURE->addResource("Resources/mabinogi/UI/P_Info.png");

}

void UI::rectInit()
{
	//0621변경 
	D3DSURFACE_DESC uiBardesc;
	_uiBar->GetLevelDesc(0, &uiBardesc);
	//ui bar Rc
	_uiBarRc = { _winPos.x-390,_winPos.y+335,(LONG)((_winPos.x+(LONG)uiBardesc.Width)*1.545),(LONG)(_winPos.y+(LONG)uiBardesc.Height) };

	

	//icon Rc   //0621  값 변경 
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
	//스킬창 내부 스킬 RC 벡터 초기화
	for (int i = 0; i < 6; i++)
	{
		RECT _winSkillRc;
		_winSkillRc = { 100,100 + (i * 70),162,162 + (i * 72) };
		_windowSkillRc.push_back(_winSkillRc);
	}
	//스킬창 RC //  226x331
	
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
	//불값에 따라 아이콘 효과
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
	FONTMANAGER->addFont("skill", "나눔고딕 ExtraBold ", 15);
	FONTMANAGER->addFont("info", "나눔고딕 ExtraBold", 18);
	FONTMANAGER->addFont("tap1", "나눔고딕 ExtraBold", 13);
	FONTMANAGER->addFont("stat", "나눔고딕 ExtraBold", 11);
	FONTMANAGER->addFont("Name", "나눔고딕 ExtraBold", 15);
}

void UI::skillTextOut()
{
	FONTMANAGER->fontOutLine("skill", "Skill", _winPos.x - 525 , _winPos.y - 285 ,D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "스킬",  _winPos.x - 525 ,  _winPos.y - 270 , D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "Smash", _winPos.x - 505, _winPos.y - 215, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "스매시", _winPos.x - 504, _winPos.y - 200, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "Defence", _winPos.x - 510, _winPos.y - 145, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "디펜스", _winPos.x - 504, _winPos.y - 130, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "Counterattack", _winPos.x - 525, _winPos.y - 75, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "카운터 어택", _winPos.x - 515, _winPos.y - 60, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "WindMill", _winPos.x - 510, _winPos.y - 5, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "윈드밀", _winPos.x - 504, _winPos.y +10, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "Rest", _winPos.x - 500, _winPos.y +65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("skill", "휴식", _winPos.x - 499, _winPos.y + 80, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
}

void UI::infoTextOut()
{
	char str[10][256];
	sprintf(str[0], "%d", _player->getLv());
	//힘 왤케 높냐;;;
	sprintf(str[1], "%d", _player->getStr());
	sprintf(str[2], "%d", _player->getIntellect());
	sprintf(str[3], "%d", _player->getDex());
	sprintf(str[4], "%d", _player->getWill());
	sprintf(str[5], "%d", _player->getLuck());
	sprintf(str[6], "%d~%d", _player->getMinAtk(), _player->getMaxAtk());
	// 0621 추가
	sprintf(str[7], "%d", _player->getCriticalChance());
	sprintf(str[8], "%d", _player->getDefense());
	sprintf(str[9], "%d", _player->getProtect());
	

	FONTMANAGER->fontOutLine("info", "캐릭터 정보", _winPos.x - 140, _winPos.y - 180, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "이름  ", _winPos.x - 190, _winPos.y - 115, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	
	FONTMANAGER->fontOutLine("Name", _player->getName(), _winPos.x - 140, _winPos.y - 118, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "종족/성별  ", _winPos.x - 190, _winPos.y - 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "인간/남성", _winPos.x - 130, _winPos.y - 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "레벨", _winPos.x -40, _winPos.y - 115, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[0], _winPos.x +10, _winPos.y - 115, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "경험치", _winPos.x - 40, _winPos.y - 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("stat", "체력", _winPos.x - 190, _winPos.y +45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[1], _winPos.x - 160, _winPos.y +45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));	
	FONTMANAGER->fontOutLine("stat", "지력", _winPos.x - 190, _winPos.y + 55, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[2], _winPos.x - 160, _winPos.y + 55, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", "솜씨", _winPos.x - 190, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[3], _winPos.x - 160, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", "의지", _winPos.x - 190, _winPos.y + 75, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[4], _winPos.x - 160, _winPos.y + 75, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", "행운", _winPos.x - 190, _winPos.y + 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("stat", str[5], _winPos.x - 160, _winPos.y + 85, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	/*FONTMANAGER->fontOutLine("tap1", "대미지", _winPos.x -95, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[6], _winPos.x - 55, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "크리티컬", _winPos.x - 95, _winPos.y + 75, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));*/


	//0621 추가 변경 위에 주석된거 에서 밑으로 변경했음  합친후 위에주석  지워주세요....
	FONTMANAGER->fontOutLine("tap1", "생명력", _winPos.x - 190, _winPos.y - 35, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "마나", _winPos.x - 190, _winPos.y - 17, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "스태미나", _winPos.x - 190, _winPos.y +1, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "대미지", _winPos.x - 98, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[6], _winPos.x - 55, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", "크리티컬", _winPos.x - 98, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[7], _winPos.x - 35, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "방어", _winPos.x+10 , _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[8], _winPos.x+65, _winPos.y + 45, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	FONTMANAGER->fontOutLine("tap1", "보호", _winPos.x + 10, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));
	FONTMANAGER->fontOutLine("tap1", str[9], _winPos.x + 65, _winPos.y + 65, D3DCOLOR_XRGB(255, 255, 255), D3DCOLOR_XRGB(0, 0, 0));

	

}



