#include "stdafx.h"
#include "mabinogi.h"

HRESULT mabinogi::init(void)
{
	ShowCursor(false);

	this->lightInit();

	D3DXMATRIXA16 matScale, matRotateX, matRotateY;

	//������ ���̰�~ X�� ������~ Y�� ������~ ���ؼ�~ �־����!
	// _matCorrection �� �۷ι��� ���ּ� ��𼭵� �ҷ��ü� �ְԲ� �ߴ�.
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

	//06.07��ȿ��
	//ī�޶� ĳ���Ϳ� ������Ű������ �ʱ�ȭ, ĳ���͸� �ٶ󺸰��Ѵ�
	D3DXVECTOR3 add(0, 2, 3);   //��ġ���� ���� ���ο� ������ �ΰ�
	//����ī�޶��� ���������ǿ� �÷��̾��� ������ + add�� �Ͽ� ���۽� ĳ���͸� �ٶ󺸰� ����
	_mainCamera->setWorldPosition(_player->getPlayerPosition() + add);
	//����ī�޶� �÷��̾ ���� ����ϰ� ��
	_mainCamera->lookPosition(_player->getCharacterPosition()->getWorldPosition());

	//������
	_petCloud = new Pet_cloud;
	_petCloud->init(_player->getMesh());
	_petCloud->setScene(_scene->getField());
	_player->setPet(_petCloud);

	//���͸Ŵ���
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
	//�÷��̾� ������Ʈ
	_player->update();
	this->playerMonsterBattle();
	//�� ������Ʈ
	_petCloud->update();
	//���� ������Ʈ
	_mob->update();
	//����Ʈ�Ŵ��� ������Ʈ
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
	//���콺 Ŭ���κ�
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//�κ��丮�� ���������� �κ��丮���� Ŭ���������
		if (_ui->getInventory()->getShow() && _ui->getInventory()->isMouseInInventory() && !_click)
		{
			_ui->inventoryUpdate();
			_click = true;
		}
		//���� �������̽��� ���� �ְ� ���� �������̽� �� �̺�Ʈ�� �߻����� ���
		if (_dyeingInterface->getShow() && _dyeingInterface->mouseOnInterface())
		{
			// ���� ��Ʈ ��ư�� ������ ���
			if (_dyeingInterface->mouseOnPartButton())
			{
				_dyeingInterface->EventClickPartButton();
			}
			// ���� �ȷ�Ʈ���� Ŭ���������
			if (_dyeingInterface->mouseOnPalette())
			{
				_dyeingInterface->EventPickColor();
			}
			// ���� ���� ��ư�� Ŭ������ ���
			if (_dyeingInterface->mouseOnStartButton())
			{
				_dyeingInterface->EventClickStartButton();
			}
			// ��� ��ư�� Ŭ������ ���
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

		if (_player->getPlayerSkillCount() == 4)	//�÷��̾ ����� �������϶�
		{
			vector<Monster*> mobVec = _mob->getMonsterVec();	//��� ������ ������ �޾ƿ�
			int MonsterCount = 0;	//����� �������� ���� ������ ī����

			D3DXVECTOR3 playerPos = _player->getPlayerPosition();	//�÷��̾� ��ǥ�޾ƿ�
			D3DXVECTOR3 mobPos;	//���� ��ǥ �޾ƿ� ����

			D3DXVECTOR3 distance;	//�Ÿ��� ����3
			float		fDistance;	//�Ÿ��� float

			for (int i = 0; i < mobVec.size(); i++)	//��� ������ �Ÿ��� �˻��� �ݺ���
			{
				mobPos = mobVec[i]->monsterTr()->getWorldPosition();	//���� ��ǥ �޾ƿ�

				distance = mobPos - playerPos;	//���Ϳ� �÷��̾� �Ÿ��� ���� ���

				fDistance = D3DXVec3Length(&distance);	//���Ϳ� �÷��̾� �Ÿ� ���

				if (fDistance <= 10.0f)	//����� ��Ÿ� ���� ���Ͱ� �������
				{
					MonsterCount++;	//ī���͸� ������Ű��
					mobVec[i]->endureA(4);	//���Ϳ��� ������� �¾Ҵٴ� ��������
					int Damage = _player->getSkill()->baseAttack(_player->getMinAtk(), _player->getMaxAtk(), _player->getCriticalChance());
					mobVec[i]->setMonsterHp(Damage);
					EFFECTMANAGER->addMonsterHit(&mobVec[i]->monsterTr()->getWorldPosition(), Damage);
				}
			}

			if (MonsterCount != 0)	//������� ��Ÿ����� ���Ͱ� �־��ٸ� ����� ��� ����
			{						//������ �������
				_player->playerAtkSkill();
			}

			_click = true;
		}

		//���� Ŭ���ϴ� �κ�
		if (!_click)
		{
			Ray cameraRay;

			//ī�޶�Ŭ���� �κп��� ȭ������ ������ ���̸� ��� �κ�
			D3DXVECTOR2 temp(getMousePos().x, getMousePos().y);
			_mainCamera->computeRay(&cameraRay, &temp);				//ȭ�� ���콺�� ���� ���� �޾ƿ����Լ�

			//Ŭ�����ʹ� ���õ� ���͸� ����������
			_clickMonster = _mob->MonsterRayHit(&cameraRay);

			//Ŭ���� ���Ͱ� ������ �÷��̾ Ŭ���� ���Ϳ��� �����ϰ���
			if (_clickMonster != NULL && !_player->getPlayerDie())
			{
				_player->setTarget(_clickMonster->_transform->getWorldPosition(), true);
				_click = true;
			}
		}

		//�� ��ΰ� �ƴϸ� ���� Ŭ��
		if (!_click && !_player->getPlayerDie())
		{
			_player->playerMoveKey();
			_click = true;
		}
	}
	//ī�޶� �¿� ȸ��
	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		if (_ui->getInventory()->getShow() && _ui->getInventory()->isMouseInInventory())
		{
			if (_ui->getInventory()->isMouseInSlotRC() && _ui->getInventory()->dyeing())
			{
				// ���� �������̽��� ����Ǹ� �κ��丮�� �ݴ´�.
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
		//�ʺ����ֱ� or ����
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
	// �ø��ȰŸ� �׸����� ������
	this->_cullObjects.clear();

	//�������� �ȿ� �ִ�?
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

	//�� ����
	_scene->rander();
	//�÷��̾� ����
	_player->render();
	//�� ����
	_petCloud->render();
	//�� ����
	_mob->render();
	//UI ��������
	_ui->showWindows();
	_dyeingInterface->showWindows();

	_dyeingInterface->EventMouseOnPallet();
	EFFECTMANAGER->render();
	OPTION->renderCursor();

	this->playerMapstop();				//�ʰ��� �浹�� �� �÷��̾ ���ߴ� �Լ�
}

void mabinogi::playerMonsterBattle()
{
	_player->playerAtkKey();
}

void mabinogi::playerMonsterAtk()
{
	D3DXVECTOR3 dis;				//Ŭ���� ������ ��ǥ - �÷��̾��� ��ǥ�� ���� �ӽ� ����
	float _fDis;						//������ �Ÿ�, false���� �ٲ㼭 ���� ����
	dis = _clickMonster->monsterTr()->getWorldPosition() - _player->getCharacterPosition()->getWorldPosition();
	_fDis = D3DXVec3Length(&dis);
	//�Ÿ��� 1.0f �̸��϶�
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
//�÷��̾��� �����浹
void mabinogi::playerMapstop()
{
	//������ ���� ��ġ = �÷��̾��� ��ǥ (�߹ٴڿ��� ���̸� ��� �ȵǴ� ���ݸ� �÷��ش�.)
	_playerStop.origin = _player->getPlayerPosition() + D3DXVECTOR3(0, 0.1f, 0);
	//���̰� �� ���� = �÷��̾��� �չ��� (�״�� ���� ���̴� ���̳ʽ��� �ٿ��ش�)
	_playerStop.direction = -_player->getMesh()->getForward();
	//���� ��ġ
	D3DXVECTOR3 stopPos;
	//������Ʈ�� �����̴� ���Ϳ��� ������
	for (int i = 0; i < _scene->getObj()->size(); i++)
	{
		//���̸� ��!
		PHYSICSMANAGER->isRayHitStaticMeshObject(&_playerStop, _scene->getObj()->at(i), &stopPos, NULL);
		//PHYSICSMANAGER->isRayHitBound(&_playerStop, &_scene->getObj()->at(i)->_boundBox, _scene->getObj()->at(i)->_transform, &stopPos, NULL);
	}

	//���� ��ġ = ���̸� ���� ���� ��ġ - �÷��̾��� ��ġ
	D3DXVECTOR3 dir = stopPos - _player->getPlayerPosition();
	//������ġ�� ������Ʈ�� ��ġ���� 0.25���� �۾�����
	if (D3DXVec3Length(&dir) < 0.25f)
	{
		_player->playerMoveStop();		//����
		return;
	}
}
