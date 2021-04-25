#include "stdafx.h"
#include "mabinogi.h"

HRESULT mabinogi::init(void)
{
	ShowCursor(false);

	this->lightInit();

	D3DXMATRIXA16 matScale, matRotateX, matRotateY;

	//스케일 줄이고~ X축 돌리고~ Y축 돌리고~ 곱해서~ 넣어버렷!
	// _matCorrection 을 글로벌로 빼둬서 어디서든 불러올수 있게끔 했다.
	D3DXMatrixScaling(&matScale, 0.0001f, 0.0001f, 0.0001f);
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(-90));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(180));
	_matCorrection = matRotateX * matRotateY * matScale;

	_player = new player;
	_player->init();
	_player->setCamera(_mainCamera);

	D3DXMATRIXA16 matStage;
	D3DXMatrixScaling(&matStage, 0.1f, 0.1f, 0.1f);
	matStage = matStage * matRotateY;
	_scene = new tirchonailField;
	_scene->init("Resources/mabinogi/terrain/tirchonail.x", &matStage);

	_player->setScene(_scene->getField());

	//06.07전효재
	//카메라를 캐릭터에 고정시키기위한 초기화, 캐릭터를 바라보게한다
	D3DXVECTOR3 add(0, 2, 3);   //위치값을 더할 새로운 변수를 두고
	//메인카메라의 월드포지션에 플레이어의 포지션 + add를 하여 시작시 캐릭터를 바라보게 만듬
	_mainCamera->setWorldPosition(_player->getPlayerPosition() + add);
	//메인카메라가 플레이어를 보고 따라니게 함
	_mainCamera->lookPosition(_player->getCharacterPosition()->getWorldPosition());

	//구름펫
	_petCloud = new Pet_cloud;
	_petCloud->init(_player->getMesh());
	_petCloud->setScene(_scene->getField());
	_player->setPet(_petCloud);

	//몬스터매니저
	_mob = new MonsterManager;
	_mob->init(_player,_scene->getField(),_mainCamera);

	_dyeingInterface = new dyeing;
	_dyeingInterface->init();

	//UI
	_ui = new UI;
	_ui->init(_player, _dyeingInterface);

	_clickMonster = NULL;

	OPTION->init(_mainCamera);
	EFFECTMANAGER->init();

	return S_OK;
}

void mabinogi::lightInit(void)
{
	lightDirection* frontLight = new lightDirection;
	frontLight->_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	_lights.push_back(frontLight);

	lightDirection* backLight = new lightDirection;
	backLight->_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	backLight->_transform.setRotateWorld(0.0f, D3DXToRadian(180), 0.0f);
	_lights.push_back(backLight);

	lightDirection* leftLight = new lightDirection;
	leftLight->_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	leftLight->_transform.setRotateWorld(D3DXToRadian(90), 0.0f, 0.0f);
	_lights.push_back(leftLight);

	lightDirection* rightLight = new lightDirection;
	rightLight->_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	rightLight->_transform.setRotateWorld(D3DXToRadian(-90), 0.0f, 0.0f);
	_lights.push_back(rightLight);

	lightDirection* topLight = new lightDirection;
	topLight->_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	topLight->_transform.setRotateWorld(D3DXToRadian(90), 0.0f, 0.0f);
	_lights.push_back(topLight);

	_sceneBaseDirectionLight = new lightDirection;
	_sceneBaseDirectionLight->_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	_sceneBaseDirectionLight->_intensity = 2.0f;
}

void mabinogi::release(void)
{
	_scene->release();
	SAFE_DELETE(_scene);

	_player->release();
	SAFE_DELETE(_player);

	_petCloud->release();
	SAFE_DELETE(_petCloud);

	_mob->release();
	SAFE_DELETE(_mob);

	for (int i = 0; i < _lights.size(); i++)
	{
		SAFE_DELETE(_lights[i]);
	}

	_ui->release();
	SAFE_DELETE(_ui);
}

void mabinogi::update(void)
{
	this->_cullObjects.clear();
	_lights[0]->_transform.setRotateWorld(_mainCamera->getWorldRotateMatrix());
	//플레이어 업데이트
	_player->update();
	this->playerMonsterBattle();
	//펫 업데이트
	_petCloud->update();
	//몬스터 업데이트
	_mob->update();
	//이펙트매니저 업데이트
	EFFECTMANAGER->update();

	_dyeingInterface->update();

	if (_clickMonster != NULL) this->playerMonsterAtk();

	if (KEYMANAGER->isOnceKeyDown('Z'))
	{
		SET_HPVIEW(true);
	}
	if (KEYMANAGER->isOnceKeyUp('Z'))
	{
		SET_HPVIEW(false);
	}

	_click = false;
	//마우스 클릭부분
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//인벤토리가 켜져있을때 인벤토리위를 클릭했을경우
		if (_ui->getInventory()->getShow() && _ui->getInventory()->isMouseInInventory() && !_click)
		{
			_ui->inventoryUpdate();
			_click = true;
		}
		//염색 인터페이스가 열려 있고 염색 인터페이스 내 이벤트가 발생했을 경우
		if (_dyeingInterface->getShow() && _dyeingInterface->mouseOnInterface())
		{
			// 염색 파트 버튼을 눌렀을 경우
			if (_dyeingInterface->mouseOnPartButton())
			{
				_dyeingInterface->EventClickPartButton();
			}
			// 염색 팔레트에서 클릭했을경우
			if (_dyeingInterface->mouseOnPalette())
			{
				_dyeingInterface->EventPickColor();
			}
			// 염색 시작 버튼을 클릭했을 경우
			if (_dyeingInterface->mouseOnStartButton())
			{
				_dyeingInterface->EventClickStartButton();
			}
			// 취소 버튼을 클릭했을 경우
			if (_dyeingInterface->mouseOnCancelButton())
			{
				_dyeingInterface->EventClickCancelButton();
			}
			_click = true;
		}
		if (_ui->isOpenShop() && _ui->isMouseInShop())
		{
			if (_ui->getInventory()->getPickItem() == NULL)
			{
				_ui->addItem(_ui->buyShop());
			}
			else
			{
				_ui->sellShop();
			}
			_click = true;
		}
		if (_ui->isActiveSkill() && !_click)
		{
			_ui->inventoryUpdate();
			_click = true;
		}

		if (_player->getPlayerSkillCount() == 4)	//플레이어가 윈드밀 시전중일때
		{
			vector<Monster*> mobVec = _mob->getMonsterVec();	//모든 몬스터의 정보를 받아옴
			int MonsterCount = 0;	//윈드밀 범위내에 몬스터 마릿수 카운터

			D3DXVECTOR3 playerPos = _player->getPlayerPosition();	//플레이어 좌표받아옴
			D3DXVECTOR3 mobPos;	//몬스터 좌표 받아올 변수

			D3DXVECTOR3 distance;	//거리용 백터3
			float		fDistance;	//거리용 float

			for (int i = 0; i < mobVec.size(); i++)	//모든 몬스터의 거리를 검사할 반복문
			{
				mobPos = mobVec[i]->monsterTr()->getWorldPosition();	//몬스터 좌표 받아옴

				distance = mobPos - playerPos;	//몬스터와 플레이어 거리용 벡터 계산

				fDistance = D3DXVec3Length(&distance);	//몬스터와 플레이어 거리 계산

				if (fDistance <= 10.0f)	//윈드밀 사거리 내에 몬스터가 있을경우
				{
					MonsterCount++;	//카운터를 증가시키고
					mobVec[i]->endureA(4);	//몬스터에게 윈드밀을 맞았다는 반응을줌
					int Damage = _player->getSkill()->baseAttack(_player->getMinAtk(), _player->getMaxAtk(), _player->getCriticalChance());
					mobVec[i]->setMonsterHp(Damage);
					EFFECTMANAGER->addMonsterHit(&mobVec[i]->monsterTr()->getWorldPosition(), Damage);
				}
			}

			if (MonsterCount != 0)	//윈드밀의 사거리내에 몬스터가 있었다면 윈드밀 모션 실행
			{						//없으면 실행안함
				_player->playerAtkSkill();
			}

			_click = true;
		}

		//몬스터 클릭하는 부분
		if (!_click)
		{
			Ray cameraRay;

			//카메라클릭한 부분에서 화면으로 나가는 레이를 얻는 부분
			D3DXVECTOR2 temp(getMousePos().x, getMousePos().y);
			_mainCamera->computeRay(&cameraRay, &temp);				//화면 마우스에 대한 레이 받아오는함수

			//클릭몬스터는 선택된 몬스터를 가지고있음
			_clickMonster = _mob->MonsterRayHit(&cameraRay);

			//클릭한 몬스터가 있을때 플레이어를 클릭한 몬스터에게 접근하게함
			if (_clickMonster != NULL && !_player->getPlayerDie())
			{
				_player->setTarget(_clickMonster->_transform->getWorldPosition(), true);
				_click = true;
			}
		}

		//위 모두가 아니면 땅을 클릭
		if (!_click && !_player->getPlayerDie())
		{
			_player->playerMoveKey();
			_click = true;
		}
	}
	//카메라 좌우 회전
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		if (_ui->getInventory()->getShow() && _ui->getInventory()->isMouseInInventory())
		{
			if (_ui->getInventory()->isMouseInSlotRC() && _ui->getInventory()->dyeing())
			{
				// 염색 인터페이스가 실행되면 인벤토리를 닫는다.
				_ui->getInventory()->setShow(false);
			}
			return;
		}

		if (getMousePos().x > (WINSIZEX / 2) + 200)
		{
			D3DXVECTOR3 cameraMove(0.1f, 0, 0);
			_mainCamera->movePositionSelf(cameraMove);
			_mainCamera->lookPosition(_player->getPlayerPosition());
		}
		else if (getMousePos().x < (WINSIZEX / 2) - 200)
		{
			D3DXVECTOR3 cameraMove(-0.1f, 0, 0);
			_mainCamera->movePositionSelf(cameraMove);
			_mainCamera->lookPosition(_player->getPlayerPosition());
		}
		//맵보여주기 or 엔딩
		/*else  if (getMousePos().y > (WINSIZEY / 2) - 200)
		{
			D3DXVECTOR3 cameraMove(0, -0.1f, 0);
			_mainCamera->movePositionSelf(cameraMove);
			_mainCamera->lookPosition(_player->getPlayerPosition());
		}*/
	}

	EFFECTMANAGER->update();
}

void mabinogi::render(void)
{
	// 컬링된거만 그릴거임 ㅇㅅㅇ
	this->_cullObjects.clear();

	//프러스텀 안에 있니?
	/*if (_mainCamera->getFrustum().isInFrustum(_Pcloud))
	{
	this->_cullObjects.push_back(_Pcloud);
	}

	if (_mainCamera->getFrustum().isInFrustum(_Mfox))
	{
	this->_cullObjects.push_back(_Mfox);
	}*/

	xMeshStatic::setCamera(_mainCamera);
	xMeshStatic::setLighting(&_lights);
	xMeshStatic::setTechniqueName("Base");
	xMeshStatic::setBaseLight(_sceneBaseDirectionLight);
	xMeshSkinned::setCamera(_mainCamera);
	xMeshSkinned::setLighting(&_lights);
	xMeshSkinned::setBaseLight(_sceneBaseDirectionLight);

	//맵 랜더
	_scene->rander();
	//플레이어 랜더
	_player->render();
	//펫 랜더
	_petCloud->render();
	//몹 랜더
	_mob->render();
	//UI 쇼윈도우
	_ui->showWindows();
	_dyeingInterface->showWindows();

	_dyeingInterface->EventMouseOnPallet();
	EFFECTMANAGER->render();
	OPTION->renderCursor();

	this->playerMapstop();				//맵과의 충돌할 시 플레이어가 멈추는 함수
}

void mabinogi::playerMonsterBattle()
{
	_player->playerAtkKey();
}

void mabinogi::playerMonsterAtk()
{
	D3DXVECTOR3 dis;				//클릭한 몬스터의 좌표 - 플레이어의 좌표를 담을 임시 변수
	float _fDis;						//변수를 거리, false으로 바꿔서 담을 변수
	dis = _clickMonster->monsterTr()->getWorldPosition() - _player->getCharacterPosition()->getWorldPosition();
	_fDis = D3DXVec3Length(&dis);
	//거리가 1.0f 미만일때
	if (_fDis < 1.0f)
	{
		_player->playerMoveStop();
		_clickMonster->endureA(_player->getPlayerSkillCount(), _player->getPlayerMainAtkCount());
		_player->playerAtkSkill();
		if (_clickMonster->getMonsterState() != 4 && _clickMonster->getMonsterState() != 5)
		{
			int Damage = _player->getSkill()->baseAttack(_player->getMinAtk(), _player->getMaxAtk(), _player->getCriticalChance());
			_clickMonster->setMonsterHp(Damage);
			EFFECTMANAGER->addMonsterHit(&_clickMonster->monsterTr()->getWorldPosition(), Damage);
		}
		else if (_clickMonster->getMonsterState() == 5)
		{
			int Damage = _player->getSkill()->baseAttack(_player->getMinAtk() / 2, _player->getMaxAtk() / 2, _player->getCriticalChance());
			_clickMonster->setMonsterHp(Damage);
			EFFECTMANAGER->addMonsterHit(&_clickMonster->monsterTr()->getWorldPosition(), Damage);
		}
		_clickMonster = NULL;
	}
}

void mabinogi::playerMonsterHit()
{

}
//플레이어의 지형충돌
void mabinogi::playerMapstop()
{
	//레이의 시작 위치 = 플레이어의 좌표 (발바닥에서 레이를 쏘면 안되니 조금만 올려준다.)
	_playerStop.origin = _player->getPlayerPosition() + D3DXVECTOR3(0, 0.1f, 0);
	//레이가 쏠 방향 = 플레이어의 앞방향 (그대로 쓰면 뒤이니 마이너스를 붙여준다)
	_playerStop.direction = -_player->getMesh()->getForward();
	//멈출 위치
	D3DXVECTOR3 stopPos;
	//오브젝트가 벡터이니 벡터에서 꺼낸다
	for (int i = 0; i < _scene->getObj()->size(); i++)
	{
		//레이를 쏨!
		PHYSICSMANAGER->isRayHitStaticMeshObject(&_playerStop, _scene->getObj()->at(i), &stopPos, NULL);
		//PHYSICSMANAGER->isRayHitBound(&_playerStop, &_scene->getObj()->at(i)->_boundBox, _scene->getObj()->at(i)->_transform, &stopPos, NULL);
	}

	//멈출 위치 = 레이를 쏴서 멈출 위치 - 플레이어의 위치
	D3DXVECTOR3 dir = stopPos - _player->getPlayerPosition();
	//멈출위치가 오브젝트의 위치보다 0.25보다 작아지면
	if (D3DXVec3Length(&dir) < 0.25f)
	{
		_player->playerMoveStop();		//멈춰
		return;
	}
}
