#include "stdafx.h"
#include "player.h"


player::player()
{
	/*
	캐릭터 이름 : 풋새
	레벨 : 1
	최대생명력 : 100
	현재생명력 : 100
	최대마나 : 50
	현재마나 : 50
	최대스태미너 : 100
	현재스태미너 : 100
	힘(체력) : 111
	솜씨 : 111
	지력 : 111
	의지 : 111
	행운 : 1111
	최소공격력 : 10
	최대공격력 : 20
	크리티컬 확률 : 50 %
	*/
	_name = "풋새";
	_lv = 1;
	_maxHp = 100;
	_curHp = 100;
	_maxMp = 50;
	_curMp = 50;
	_maxSt = 50;
	_curSt = 50;
	_str = 111000;
	_dex = 111;
	_intellect = 111;
	_will = 111;
	_luck = 1111;
	_minAtk = 1;
	_maxAtk = 10;
	_criticalChance = 50;

	//0621 김광조 추가 
	_defense = 5;
	_protect = 2;

	_moveState = false;
	_moveSpeed = 0.025f;

	playerMotionCount = 0;
	playerSkillCount = 0;
	playerStateCount = 0;
	playerMainAtkCount = 0;

	_atkTimer = 0.0f;
	_hyusicTimer = 0.0f;

	_battleMode = false;
	_ridingMode = false;
	_flyMode = false;
	_targetBool = false;
	_isStiffen = false;
	_bDie = false;
	_isDown = false;
	_damageDelay = false;

	for (int i = 0; i < 8; i++)
	{
		//맵에 일단 아무것도 없는 값을 다 넣는다.
		_mPlayerSkillIcon.insert(make_pair(i, 0));
	}

	_flyHeight = 0.0f;
}

player::~player()
{
}

HRESULT player::init(void)
{
	_playerSkill = new skill;
	_meshInfo = new meshInfo;
	_meshInfo->init();

	_meshInfo->play("stand_idle");			//처음 init으로 기본모션이 기본-서있는 모션으로 설정

	_hpBar = new billboardProgress(_meshInfo->getCharacterPosition(), this->getMaxHpAddress(), this->getCurHpAddress(),
		0, 0,
		D3DCOLOR_XRGB(0, 0, 0), D3DCOLOR_XRGB(207, 64, 144), 0.725f);

	return S_OK;
}

void player::release(void)
{
}

void player::update(void)
{
	_meshInfo->update();
	_ui->update();

	if (!_bDie)							//플레이어가 죽지않았을때만 실행
	{
		this->playerUi();				//플레이어의 기본 UI함수
		this->playerMove();				//플레이어의 이동 함수
		this->petRiding();				//펫 탑승 함수
		this->petFlyMoveKey();			//비행시 이동함수
		this->playerAtkSkillIcon();		//플레이어의 스킬 사용(단축키)
	}
	this->playerDown();					//플레이어 날아갑니다~
	this->playerDie();					//플레이어가 죽었다

	if (_isStiffen && _meshInfo->getAniFactor() >= 0.98f)
	{
		_isStiffen = false;
		_damageDelay = false;
	}

	//hp mp st는 0이하로 내려가지 않는다
	if (_curHp <= 0) _curHp = 0;
	if (_curMp <= 0) _curMp = 0;
	if (_curSt <= 0) _curSt = 0;
	//hp mp st는 최대이상 올라가지 않는다
	if (_curHp >= _maxHp) _curHp = _maxHp;
	if (_curMp >= _maxMp) _curMp = _maxMp;
	if (_curSt >= _maxSt) _curSt = _maxSt;




	//test
	if (KEYMANAGER->isOnceKeyDown(VK_F5))
	{
		_curHp += 10;
		_curSt += 10;
	}
}

void player::render(void)
{
	char info[256];
	
	sprintf(info, "타켓좌표 : %f %f %f", _targetPos.x, _targetPos.y, _targetPos.z);
	FONTMANAGER->fontOut(info, 10, 390, 0xffffff00);

	sprintf(info, "이동좌표 : %f %f %f", _movePos.x, _movePos.y, _movePos.z);
	FONTMANAGER->fontOut(info, 10, 420, 0xffffff00);

	sprintf(info, "내좌표 : %f %f %f", _meshInfo->getPlayerPosition().x, _meshInfo->getPlayerPosition().y, _meshInfo->getPlayerPosition().z);
	FONTMANAGER->fontOut(info, 10, 450, 0xffffff00);

	_hpBar->render(_mainCamera);
	_meshInfo->render();
	//마우스가 땅을 클릭했을 때 어디로 클릭했는지 알려주는 기즈모
	if(_moveState) GIZMOMANAGER->sphereMake(_movePos, 0.05f);
}
//플레이어의 공격 함수
void player::playerAtkKey()
{
	//버튼을 누르면 (임시 : F1 / 나중에 좌클릭으로 바꿔야함)
	if (KEYMANAGER->isOnceKeyDown(VK_F1) && !_moveState)
	{
		playerSkillCount = 0;			//스킬 : 기본
		playerStateCount = 1;			//상태 : 공격
		_battleMode = true;				//전투중 ? ok
		playerMainAtkCount++;			//기본공격 몇번? : 누를때마다 1개씩 증가
	}
	//버튼을 때면 플레이어의 모션은 기본으로(한번만 실행되게)
	else if (KEYMANAGER->isOnceKeyUp(VK_F1) && playerSkillCount == 0)
	{
		playerSkillCount = 999;		//그러니 스킬은 999로 변경(가상)
	}
	//버튼을 누를때마다 ++되는 기본공격때문에 모션이 한개씩 바뀐다.
	//버튼을 누를때마다 atkTimer갱신
	/*if (playerMainAtkCount == 1 && playerStateCount == 1 && playerSkillCount == 0)
	{
		_meshInfo->play("atk01", "atk_idle");
		_atkTimer = TIMEMANAGER->getWorldTime();
	}
	else if (playerMainAtkCount == 2 && playerStateCount == 1 && playerSkillCount == 0)
	{
		_meshInfo->play("atk02", "atk_idle");
		_atkTimer = TIMEMANAGER->getWorldTime();
	}
	else if (playerMainAtkCount == 3 && playerStateCount == 1 && playerSkillCount == 0)
	{
		_meshInfo->play("atk03", "atk_idle");
		_atkTimer = TIMEMANAGER->getWorldTime();
	}*/
	//기본공격이 3이상 되면 기본공격이 1로 다시 돌아오게끔
	if (playerMainAtkCount >= 3)
	{
		playerMainAtkCount = 0;
	}
	//플레이어가 장시간 공격을 하지 않을 시
	//타임매니저의 getWorldTime을 불러 와서 메인공격이 1보다 크거나 같을 때(모션이01일때) 2초마다 실행한다. 
	if ((TIMEMANAGER->getWorldTime() - _atkTimer) >= 2.0f && playerMainAtkCount >= 1)
	{
		playerMainAtkCount = 0;		//기본공격이 0으로 바뀜(누를때마다 ++되니 0으로 해야함)
	}
	//타임매니저의 getWorldTime을 불러와서 5초~5.5초동안 아무런 공격이 없을 때 기본 서있는 모션으로 변경한다	
	if ((TIMEMANAGER->getWorldTime() - _atkTimer) >= 5.0f &&
		(TIMEMANAGER->getWorldTime() - _atkTimer) < 5.5f && !_ridingMode
		&& playerSkillCount == 0 && _battleMode)
	{
		_meshInfo->moveStop();					//움직이지않음
		_meshInfo->play("stand_idle");			//모션 : 기본-서있음
		_battleMode = false;					//전투중? no
		playerStateCount = 0;
	}
}
//플레이어 이동 함수
void player::playerMoveKey()
{
	if (_flyMode || _isStiffen || playerSkillCount == 3) return;		//비행중일때 혹은 경직중일때 마우스클릭으로 이동막기

	D3DXVECTOR2 temp(getMousePos().x, getMousePos().y);
	_mainCamera->computeRay(&_moveRay, &temp);	//화면 마우스에 대한 레이 받아오는함수
	//레이케스트가 땅에 충돌했을때 충돌한 그 위치를 _movePos에 받아옴
	if (PHYSICSMANAGER->isRayHitStaticMeshObject(&_moveRay, _scene, &_movePos, NULL))
	{
		if ((_targetBool || _moveState) && !_ridingMode)	//이동중? ok
		{
			if (!_battleMode)						//전투중? no
			{
				_meshInfo->play("stand_idle");		//모션 : 기본-서있음
			}
			else
			{
				_meshInfo->play("atk_idle");		//그게 아닐때 모션 : 공격-서있음
			}
			_meshInfo->moveStop();					//뭐든 움직이지마
		}
		_targetBool = false;
		_movePos.y = _meshInfo->getPlayerPosition().y;	//목표지점과 케릭터의 높이를 같게만듬
		_moveState = true;								//이동안하면 이동으로 바꿔줘
		_meshInfo->lookPosition(_movePos);				//좌표로 이동한다
		if (!_battleMode)						//배틀모드가 아니면?
		{
			playerMotionCount = 3;				//모션카운트 : 달리기
		}
		else
		{
			playerMotionCount = 4;				//배틀모드면 모션카운트 : 전투달리기
		}
	}
}

void player::playerMove()
{
	_movePos.y = _meshInfo->getPlayerPosition().y;	//목표지점과 케릭터의 높이를 같게만듬
	//멈출지점 = 마우스가 찍은 지점 - 플레이어의 실제 좌표
	D3DXVECTOR3 stop = _movePos - _meshInfo->getPlayerPosition();

	if ((_moveState || _targetBool) && D3DXVec3Length(&stop) < 0.1f) //거리체크용, 0.1보다 작아지면 멈춘다
	{
		if (_ridingMode)
		{
			_meshInfo->play("riding_Fly_Stand");
		}
		else if (!_battleMode)							//배틀모드? no
		{
			_meshInfo->play("stand_idle");			//모션 : 기본-서있음
		}
		else
		{
			_meshInfo->play("atk_idle");			//배틀모드? ok 모션 : 공격-서있음
		}
		_meshInfo->moveStop();						//뭐든 멈춰!
		_moveState = false;							//움직일거니? nono
		_targetBool = false;
	}
	//이제 움직일거다 움직임일때
	if (_moveState)
	{
		if (playerMotionCount == 4 && _battleMode && playerSkillCount == 2)
		{
			_meshInfo->movePosition(0.0135f);
			_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * 0.0135f);
		}
		else if (!_ridingMode)
		{
			_meshInfo->movePosition(_moveSpeed);		//움직이는게 스피드만큼 움직여줌(0.025)
			_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * _moveSpeed);
		}
		else
		{
			_meshInfo->movePosition(0.04f);
			_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * 0.04f);
		}
		//06.07전효재
		//이동중일때 카메라가 플레이어를 플레이어의 스피드만큼 움직이며 바라보게 만듬

		
		if (playerMotionCount == 3 && !_battleMode && !_ridingMode)	//모션카운트 : 달리기, 배틀모드가 아닐때
		{
			_meshInfo->play("stand_run");			//모션 : 기본-달리기
		}
		else if (playerStateCount == 2 && _moveState && _ridingMode)
		{
			_meshInfo->play("riding_Fly_Run");
		}
		else if (playerMotionCount == 4 && _battleMode && playerSkillCount == 2)
		{
			_meshInfo->play("atk_Run02");
			_atkTimer = TIMEMANAGER->getWorldTime();
		}
		else if (playerMotionCount == 4 && _battleMode)	//모션카운트 : 전투달리기, 배틀모드일때
		{
			_meshInfo->play("atk_Run01");			//모션 : 공격-달리기
			_atkTimer = TIMEMANAGER->getWorldTime();	//달리는 상태일때는 atkTimer를 계속 갱신시켜준다. 왜? 안하면 모션이 엉키니까!
		}
	}
	else if (_targetBool)
	{
		_meshInfo->movePosition(_moveSpeed);		//움직이는게 스피드만큼 움직여줌(0.025)
		_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * _moveSpeed);
		//06.15민병덕
		//이동중일때 카메라가 플레이어를 플레이어의 스피드만큼 움직이며 바라보게 만듬

		if (playerMotionCount == 4 && _battleMode)	//모션카운트 : 전투달리기, 배틀모드일때
		{
			_meshInfo->play("atk_Run01");			//모션 : 공격-달리기
			_atkTimer = TIMEMANAGER->getWorldTime();	//달리는 상태일때는 atkTimer를 계속 갱신시켜준다. 왜? 안하면 모션이 엉키니까!
		}
	}

	this->playerheight();			//플레이어의 y축이동 함수

	
}

void player::playerUi()
{
	//f2를 누르면 펫소환한다응아아아
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		if (_pet->getState() == NONE)				//펫의 상태값이 NONE일 때, 소환되지 않았으면 NONE
		{
			_pet->summon();							//펫소환!
		}
		else if (_pet->getState() != NONE && !_pet->getRiding())	//펫의 상태값이 NONE이 아닐때(소환이 되면 NONE이 아님) 그리고 펫을 타지 않았을때
		{
			_pet->Unsummon();						//펫소환해제!
		}
	}
}
//플레이어의 스킬 공격부분
void player::playerAtkSkillIcon()
{
	for (int i = 0; i < 8; i++)
	{
		// 1+i , 총 8까지 한번 스킬단축키를 누르면(스킬이 있을때 실행)
		if (KEYMANAGER->isOnceKeyDown('1' + i))
		{
			//스위치 오랫만, 스킬단축키에 있는 second값을 실행(스킬을 실행)
			switch (_mPlayerSkillIcon.find(i)->second)
			{
				//공격스킬은 state를 1로, 휴식은 0으로
				//스킬 아이콘대로 스킬카운터를 바꿔준다.
				//휴식을 제외한 모든 모션은 atk_idle로 바꿈(머리위의 아이콘때문에 대기상태)
			case smash:
				if (_curSt < 10)
				{
					return;
				}
				playerSkillCount = 1;
				playerStateCount = 1;
				if (!_moveState)			//움직이는 중이 아닐땐 공격모드 모션으로
				{
					_meshInfo->play("atk_idle");
				}
				else if (!_battleMode)		//움직이는 중이고 공격모드가 아닐땐 공격모드 달리는모션으로 
				{
					_meshInfo->AniMeshPosition();
					_meshInfo->play("atk_Run01");
				}
				_curSt -= 10;
				_battleMode = true;
				break;

			case defens:
				if (_curSt < 5)
				{
					return;
				}
				playerSkillCount = 2;
				playerStateCount = 1;
				this->playerMoveStop();
				_meshInfo->play("atk_idle");
				_curSt -= 5;
				_battleMode = true;
				break;

			case counter:
				if (_curSt < 10)
				{
					return;
				}
				playerSkillCount = 3;
				playerStateCount = 1;
				this->playerMoveStop();
				_meshInfo->play("atk_idle");
				_curSt -= 10;
				_battleMode = true;
				break;

			case windmill:
				if (_curSt < 20)
				{
					return;
				}
				playerSkillCount = 4;
				playerStateCount = 1;
				this->playerMoveStop();
				_meshInfo->play("atk_idle");
				_curSt -= 20;
				_battleMode = true;
				break;

			case hyusic:
				//스킬카운트가 5가 아닐때, 즉 일어나있을때 앉는다
				if (playerSkillCount != 5)
				{
					playerSkillCount = 5;
					playerStateCount = 0;
					_meshInfo->play("natural_To_Sit", "natural_Sit");
					_hyusicTimer = TIMEMANAGER->getWorldTime();
				}
				//앉앗다가 일어나라
				else
				{
					playerSkillCount = 0;
					playerSkillCount = 0;
					_meshInfo->play("natural_Sit_To", "stand_idle");
				}
				break;
			}
		}
	}
	if (playerSkillCount != 0 && KEYMANAGER->isOnceKeyDown(VK_ESCAPE))
	{
		playerSkillCount = 0;
		_atkTimer = TIMEMANAGER->getWorldTime();
	}

	//스킬휴식
	if (playerSkillCount == 5 && TIMEMANAGER->getWorldTime() - _hyusicTimer > 1.0f)
	{
		_curHp += 1;
		_hyusicTimer = TIMEMANAGER->getWorldTime();
	}
}

void player::playerAtkSkill()
{
	if (playerSkillCount == 0 && (!_isStiffen || playerMainAtkCount != 0))
	{
		playerMainAtkCount++;
		_atkTimer = TIMEMANAGER->getWorldTime();
		
		if (playerMainAtkCount == 1)
		{
			_meshInfo->play("atk01", "atk_idle");
		}
		else if (playerMainAtkCount == 2)
		{
			_meshInfo->play("atk02", "atk_idle");
		}
		else if (playerMainAtkCount == 3)
		{
			_meshInfo->play("atk03", "atk_idle");
		}
		_isStiffen = true;
	}
	else if (playerSkillCount == 1 && !_isStiffen)
	{
		_meshInfo->play("atk_Smash","atk_idle");
		playerSkillCount = 0;
		_isStiffen = true;
	}
	else if (playerSkillCount == 4)
	{
		_meshInfo->play("atk_windmil", "atk_idle");
		playerSkillCount = 0;
		_isStiffen = true;
	}
}


int player::playerCritical()
{
	return _playerSkill->baseAttack(_minAtk, _maxAtk, _criticalChance);

}

void player::playerHit(int state,int damage)
{
	if (!_damageDelay)
	{
		//플레이어가 데미지 들어간다.
		_curHp -= damage;
		if (_curHp <= 0) return;
		_damageDelay = true;
	}

	this->playerMoveStop();

	_atkTimer = TIMEMANAGER->getWorldTime();
	//내가 '디펜스'상태이면서 상대가 평타를 때렷을 때
	if (playerSkillCount == 2 && (state == 1 || state == 2 || state == 3))
	{
		playerMotionCount = 6;							//플레이어 모션 = 날아감

		_meshInfo->play("atk_Guard", "atk_idle");		//모션실행
		_isStiffen = true;								//경직중
		playerSkillCount = 0;
	}
	//내가 '카운터'상태이면서 상대가 평타일때
	else if (playerSkillCount == 3 && (state == 1 || state == 2 || state == 3))
	{
		playerMotionCount = 1;							//플레이어 모션 = 날아감

		_meshInfo->play("atk_Counter", "atk_idle");		//모션실행
		_isStiffen = true;								//날아가는중
		playerSkillCount = 0;
	}
	else if (state == 1 || state == 2)
	{
		playerMotionCount = 5;							//플레이어 모션 = 맞음
		_meshInfo->play("stand_idle");
		int aaa = randomIntRange(0, 100);				//랜덤값을 줘서 랜덤으로 2개중에 모션이 나오게 함
		if (aaa < 50)
		{
			_meshInfo->play("hit_A", "atk_idle");
		}
		else
		{
			_meshInfo->play("hit_B", "atk_idle");
		}
		_isStiffen = true;								//모션이 끝날때까지 경직시킴
	}
	//내가 '카운터'상태이면서 상대가 스메쉬일때
	else if (playerSkillCount == 3 && state == 6)
	{
		playerMotionCount = 1;							//플레이어 모션 = 날아감

		_meshInfo->play("atk_Counter", "atk_idle");		//모션실행
		_isStiffen = true;								//날아가는중
		playerSkillCount = 0;
	}
	//내가 '디펜스'상태이면서 상대가 스메쉬를 썻을 때
	else if (playerSkillCount == 2 && state == 6)
	{
		playerMotionCount = 6;							//플레이어 모션 = 날아감

		_meshInfo->play("hit_satnd", "atk_idle");		//모션실행
		_isStiffen = true;								//경직중
		_isDown = true;									//날아가는중
		playerSkillCount = 0;
	}
	else if (state == 3 || state == 6)
	{
		playerMotionCount = 6;							//플레이어 모션 = 날아감

		_meshInfo->play("hit_satnd", "atk_idle");		//모션실행
		_isStiffen = true;								//경직중
		_isDown = true;									//날아가는중
	}
	//내가 '디펜스'상태이면서 상대가 윈드밀을 썻을 때
	if (playerSkillCount == 2 && state == 8)
	{
		playerMotionCount = 6;
		//경직
		_isStiffen = true;								//경직중
		_meshInfo->play("atk_Guard");
		playerSkillCount = 0;
	}
	//내가 '카운터'상태이면서 상대가 윈드밀일때
	if (playerSkillCount == 3 && state == 8)
	{
		playerMotionCount = 1;							//플레이어 모션 = 날아감

		_meshInfo->play("hit_satnd", "atk_idle");		//모션실행
		_isStiffen = true;								//경직중
		_isDown = true;									//날아가는중
		playerSkillCount = 0;
	}
	//상대가 카운터일때 때렷을때
	if (state == 4)
	{
		playerMotionCount = 6;
		_meshInfo->play("hit_satnd", "atk_idle");
		playerSkillCount = 0;
		_isStiffen = true;
		_isDown = true;
	}
}
//플레이어 죽었따!
void player::playerDie()
{
	if (_curHp <= 0 && !_bDie)
	{
		_bDie = true;
		_isDown = true;
		_meshInfo->play("hit_Ground","hit_die");
	}
}
//플레이어 날아감
void player::playerDown()
{
	if (_isDown)	//날아가는 중일때
	{
		if (_meshInfo->getAniFactor() < 0.4f)		//땅에 닿을때까지 실행
		{
			_meshInfo->movePosition(-0.025);		//뒤로 이동
			_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * -0.025);		//카메라도 뒤로이동
			_meshInfo->AniMeshPosition();			//애니메이션과 매쉬위치를 일치시킴
		}
		else
		{
			_isDown = false;						//뒤로 날라가는거 정지
		}
	}
}

void player::playerheight()
{
	_heightRay.origin = _meshInfo->getPlayerPosition() + D3DXVECTOR3(0, 2, 0);		
	_heightRay.direction = D3DXVECTOR3(0, -1, 0);

	D3DXVECTOR3 heightRayHitPos;
	D3DXVECTOR3 cameraheiPos;

	cameraheiPos = _mainCamera->getWorldPosition();
	cameraheiPos.y = _meshInfo->getPlayerPosition().y + 2;
	_mainCamera->setWorldPosition(cameraheiPos);

	PHYSICSMANAGER->isRayHitStaticMeshObject(&_heightRay, _scene, &heightRayHitPos, NULL);
	float isRide = 0.0f;

	if (_ridingMode) isRide = 0.4f;

	if (_flyMode && _flyHeight > FLYHEIGHT)
	{
		_flyHeight = FLYHEIGHT;
	}
	else if (!_flyMode && _flyHeight < 0.0f)
	{
		_flyHeight = 0.0f;
	}

	if (_flyMode && _flyHeight < FLYHEIGHT)
	{
		_flyHeight += 0.1f;
	}
	else if(!_flyMode && _flyHeight > 0.0f)
	{
		_flyHeight -= 0.1f;
	}

	//06.18민병덕 + _flyHeight추가
	_meshInfo->setYPosition(heightRayHitPos.y + isRide + _flyHeight);
}

void player::petRiding()
{
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		//움직이지않고 타지않았을때
		if (playerStateCount == 0 && !_moveState)
		{
			playerStateCount = 2;
			_ridingMode = true;
			_pet->setRiding(true);
		}
		else if (playerStateCount == 2 && !_moveState && _ridingMode)
		{
			_ridingMode = false;
			playerStateCount = 0;
			_meshInfo->play("stand_idle");
			_pet->setRiding(false);
			_pet->_transform->releaseParent();
			_pet->_transform->movePositionSelf(0, 0, -0.01f);
		}
		//모션조정
		//탓고 움직이지않으면
		if (playerStateCount == 2 && !_moveState && _ridingMode)
		{
			_meshInfo->play("riding_Mount", "riding_Fly_Stand");
		}
	}

	//타고있을때 케릭터의 위치조정
	if (_ridingMode)
	{
		_meshInfo->ridingMeshPosition();
		if (KEYMANAGER->isOnceKeyDown('V'))
		{
			_flyMode = !_flyMode;				//v키를 눌렀을때 비행상태를 변경한다
			_pet->setFly(_flyMode);
		}
	}
}

void player::petFlyMoveKey()
{
	if (_flyMode)
	{
		if (KEYMANAGER->isStayKeyDown(VK_SPACE))
		{
			_meshInfo->movePosition(0.1f);
			_meshInfo->play("riding_Fly_Run");
			_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * 0.1f);
		}
		else
		{
			_meshInfo->play("riding_Fly_Stand");
		}

		if (KEYMANAGER->isStayKeyDown('A'))
		{
			_meshInfo->characterRotateY(D3DXToRadian(-2));
			_meshInfo->ridingMeshPosition();
		}

		if (KEYMANAGER->isStayKeyDown('D'))
		{
			_meshInfo->characterRotateY(D3DXToRadian(2));
			_meshInfo->ridingMeshPosition();
		}
	}
}
//레벨업, 스텟
void player::playerLvUp()
{
	//한번만 실행한다는 조건 하에 함수를 짬
	//스텟이라 주석X
	_lv++;
	_maxHp += 10;
	_curHp = _maxHp;
	_maxMp += 10;
	_curMp = _maxMp;
	_maxSt += 10;
	_curSt = _maxSt;
	_str += 10;
	_intellect += 10;
	_dex += 10;
	_will += 10;
	_luck += 10;
	_maxAtk += 77;
	_minAtk += 10;
}

void player::playerMoveStop()
{
	if (_ridingMode)
	{
		_meshInfo->play("riding_Fly_Stand");
	}
	else if (!_battleMode)							//배틀모드? no
	{
		_meshInfo->play("stand_idle");			//모션 : 기본-서있음
	}
	else
	{
		_meshInfo->play("atk_idle");			//배틀모드? ok 모션 : 공격-서있음
	}
	_meshInfo->moveStop();						//뭐든 멈춰!
	_moveState = false;							//움직일거니? nono
	_targetBool = false;
}

void player::setTarget(D3DXVECTOR3 targetPos, bool targetBool)
{
	if (_isStiffen || playerSkillCount == 4) return;	//경직중일시 몬스터 클릭막기
														//윈드밀일때 몬스터 클릭막기
	if (!_ridingMode || !targetBool)
	{
		_targetPos = targetPos;		//타켓위치설정
		_targetBool = targetBool;	//타켓이 있는지

		if (_targetBool)
		{
			_meshInfo->moveStop();
			_meshInfo->play("atk_idle");
			_movePos = _targetPos;
			_movePos.y = _meshInfo->getPlayerPosition().y;	//목표지점과 케릭터의 높이를 같게만듬
			_meshInfo->lookPosition(_movePos);				//좌표로 이동한다
			playerStateCount = 1;
			playerMotionCount = 4;
			_battleMode = true;
			_atkTimer = TIMEMANAGER->getWorldTime();
			//playerSkillCount = 0;
			return;
		}
	}
}

