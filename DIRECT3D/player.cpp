#include "stdafx.h"
#include "player.h"


player::player()
{
	/*
	ĳ���� �̸� : ǲ��
	���� : 1
	�ִ����� : 100
	�������� : 100
	�ִ븶�� : 50
	���縶�� : 50
	�ִ뽺�¹̳� : 100
	���罺�¹̳� : 100
	��(ü��) : 111
	�ؾ� : 111
	���� : 111
	���� : 111
	��� : 1111
	�ּҰ��ݷ� : 10
	�ִ���ݷ� : 20
	ũ��Ƽ�� Ȯ�� : 50 %
	*/
	_name = "ǲ��";
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

	//0621 �豤�� �߰� 
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
		//�ʿ� �ϴ� �ƹ��͵� ���� ���� �� �ִ´�.
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

	_meshInfo->play("stand_idle");			//ó�� init���� �⺻����� �⺻-���ִ� ������� ����

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

	if (!_bDie)							//�÷��̾ �����ʾ������� ����
	{
		this->playerUi();				//�÷��̾��� �⺻ UI�Լ�
		this->playerMove();				//�÷��̾��� �̵� �Լ�
		this->petRiding();				//�� ž�� �Լ�
		this->petFlyMoveKey();			//����� �̵��Լ�
		this->playerAtkSkillIcon();		//�÷��̾��� ��ų ���(����Ű)
	}
	this->playerDown();					//�÷��̾� ���ư��ϴ�~
	this->playerDie();					//�÷��̾ �׾���

	if (_isStiffen && _meshInfo->getAniFactor() >= 0.98f)
	{
		_isStiffen = false;
		_damageDelay = false;
	}

	//hp mp st�� 0���Ϸ� �������� �ʴ´�
	if (_curHp <= 0) _curHp = 0;
	if (_curMp <= 0) _curMp = 0;
	if (_curSt <= 0) _curSt = 0;
	//hp mp st�� �ִ��̻� �ö��� �ʴ´�
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
	
	sprintf(info, "Ÿ����ǥ : %f %f %f", _targetPos.x, _targetPos.y, _targetPos.z);
	FONTMANAGER->fontOut(info, 10, 390, 0xffffff00);

	sprintf(info, "�̵���ǥ : %f %f %f", _movePos.x, _movePos.y, _movePos.z);
	FONTMANAGER->fontOut(info, 10, 420, 0xffffff00);

	sprintf(info, "����ǥ : %f %f %f", _meshInfo->getPlayerPosition().x, _meshInfo->getPlayerPosition().y, _meshInfo->getPlayerPosition().z);
	FONTMANAGER->fontOut(info, 10, 450, 0xffffff00);

	_hpBar->render(_mainCamera);
	_meshInfo->render();
	//���콺�� ���� Ŭ������ �� ���� Ŭ���ߴ��� �˷��ִ� �����
	if(_moveState) GIZMOMANAGER->sphereMake(_movePos, 0.05f);
}
//�÷��̾��� ���� �Լ�
void player::playerAtkKey()
{
	//��ư�� ������ (�ӽ� : F1 / ���߿� ��Ŭ������ �ٲ����)
	if (KEYMANAGER->isOnceKeyDown(VK_F1) && !_moveState)
	{
		playerSkillCount = 0;			//��ų : �⺻
		playerStateCount = 1;			//���� : ����
		_battleMode = true;				//������ ? ok
		playerMainAtkCount++;			//�⺻���� ���? : ���������� 1���� ����
	}
	//��ư�� ���� �÷��̾��� ����� �⺻����(�ѹ��� ����ǰ�)
	else if (KEYMANAGER->isOnceKeyUp(VK_F1) && playerSkillCount == 0)
	{
		playerSkillCount = 999;		//�׷��� ��ų�� 999�� ����(����)
	}
	//��ư�� ���������� ++�Ǵ� �⺻���ݶ����� ����� �Ѱ��� �ٲ��.
	//��ư�� ���������� atkTimer����
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
	//�⺻������ 3�̻� �Ǹ� �⺻������ 1�� �ٽ� ���ƿ��Բ�
	if (playerMainAtkCount >= 3)
	{
		playerMainAtkCount = 0;
	}
	//�÷��̾ ��ð� ������ ���� ���� ��
	//Ÿ�ӸŴ����� getWorldTime�� �ҷ� �ͼ� ���ΰ����� 1���� ũ�ų� ���� ��(�����01�϶�) 2�ʸ��� �����Ѵ�. 
	if ((TIMEMANAGER->getWorldTime() - _atkTimer) >= 2.0f && playerMainAtkCount >= 1)
	{
		playerMainAtkCount = 0;		//�⺻������ 0���� �ٲ�(���������� ++�Ǵ� 0���� �ؾ���)
	}
	//Ÿ�ӸŴ����� getWorldTime�� �ҷ��ͼ� 5��~5.5�ʵ��� �ƹ��� ������ ���� �� �⺻ ���ִ� ������� �����Ѵ�	
	if ((TIMEMANAGER->getWorldTime() - _atkTimer) >= 5.0f &&
		(TIMEMANAGER->getWorldTime() - _atkTimer) < 5.5f && !_ridingMode
		&& playerSkillCount == 0 && _battleMode)
	{
		_meshInfo->moveStop();					//������������
		_meshInfo->play("stand_idle");			//��� : �⺻-������
		_battleMode = false;					//������? no
		playerStateCount = 0;
	}
}
//�÷��̾� �̵� �Լ�
void player::playerMoveKey()
{
	if (_flyMode || _isStiffen || playerSkillCount == 3) return;		//�������϶� Ȥ�� �������϶� ���콺Ŭ������ �̵�����

	D3DXVECTOR2 temp(getMousePos().x, getMousePos().y);
	_mainCamera->computeRay(&_moveRay, &temp);	//ȭ�� ���콺�� ���� ���� �޾ƿ����Լ�
	//�����ɽ�Ʈ�� ���� �浹������ �浹�� �� ��ġ�� _movePos�� �޾ƿ�
	if (PHYSICSMANAGER->isRayHitStaticMeshObject(&_moveRay, _scene, &_movePos, NULL))
	{
		if ((_targetBool || _moveState) && !_ridingMode)	//�̵���? ok
		{
			if (!_battleMode)						//������? no
			{
				_meshInfo->play("stand_idle");		//��� : �⺻-������
			}
			else
			{
				_meshInfo->play("atk_idle");		//�װ� �ƴҶ� ��� : ����-������
			}
			_meshInfo->moveStop();					//���� ����������
		}
		_targetBool = false;
		_movePos.y = _meshInfo->getPlayerPosition().y;	//��ǥ������ �ɸ����� ���̸� ���Ը���
		_moveState = true;								//�̵����ϸ� �̵����� �ٲ���
		_meshInfo->lookPosition(_movePos);				//��ǥ�� �̵��Ѵ�
		if (!_battleMode)						//��Ʋ��尡 �ƴϸ�?
		{
			playerMotionCount = 3;				//���ī��Ʈ : �޸���
		}
		else
		{
			playerMotionCount = 4;				//��Ʋ���� ���ī��Ʈ : �����޸���
		}
	}
}

void player::playerMove()
{
	_movePos.y = _meshInfo->getPlayerPosition().y;	//��ǥ������ �ɸ����� ���̸� ���Ը���
	//�������� = ���콺�� ���� ���� - �÷��̾��� ���� ��ǥ
	D3DXVECTOR3 stop = _movePos - _meshInfo->getPlayerPosition();

	if ((_moveState || _targetBool) && D3DXVec3Length(&stop) < 0.1f) //�Ÿ�üũ��, 0.1���� �۾����� �����
	{
		if (_ridingMode)
		{
			_meshInfo->play("riding_Fly_Stand");
		}
		else if (!_battleMode)							//��Ʋ���? no
		{
			_meshInfo->play("stand_idle");			//��� : �⺻-������
		}
		else
		{
			_meshInfo->play("atk_idle");			//��Ʋ���? ok ��� : ����-������
		}
		_meshInfo->moveStop();						//���� ����!
		_moveState = false;							//�����ϰŴ�? nono
		_targetBool = false;
	}
	//���� �����ϰŴ� �������϶�
	if (_moveState)
	{
		if (playerMotionCount == 4 && _battleMode && playerSkillCount == 2)
		{
			_meshInfo->movePosition(0.0135f);
			_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * 0.0135f);
		}
		else if (!_ridingMode)
		{
			_meshInfo->movePosition(_moveSpeed);		//�����̴°� ���ǵ常ŭ ��������(0.025)
			_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * _moveSpeed);
		}
		else
		{
			_meshInfo->movePosition(0.04f);
			_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * 0.04f);
		}
		//06.07��ȿ��
		//�̵����϶� ī�޶� �÷��̾ �÷��̾��� ���ǵ常ŭ �����̸� �ٶ󺸰� ����

		
		if (playerMotionCount == 3 && !_battleMode && !_ridingMode)	//���ī��Ʈ : �޸���, ��Ʋ��尡 �ƴҶ�
		{
			_meshInfo->play("stand_run");			//��� : �⺻-�޸���
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
		else if (playerMotionCount == 4 && _battleMode)	//���ī��Ʈ : �����޸���, ��Ʋ����϶�
		{
			_meshInfo->play("atk_Run01");			//��� : ����-�޸���
			_atkTimer = TIMEMANAGER->getWorldTime();	//�޸��� �����϶��� atkTimer�� ��� ���Ž����ش�. ��? ���ϸ� ����� ��Ű�ϱ�!
		}
	}
	else if (_targetBool)
	{
		_meshInfo->movePosition(_moveSpeed);		//�����̴°� ���ǵ常ŭ ��������(0.025)
		_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * _moveSpeed);
		//06.15�κ���
		//�̵����϶� ī�޶� �÷��̾ �÷��̾��� ���ǵ常ŭ �����̸� �ٶ󺸰� ����

		if (playerMotionCount == 4 && _battleMode)	//���ī��Ʈ : �����޸���, ��Ʋ����϶�
		{
			_meshInfo->play("atk_Run01");			//��� : ����-�޸���
			_atkTimer = TIMEMANAGER->getWorldTime();	//�޸��� �����϶��� atkTimer�� ��� ���Ž����ش�. ��? ���ϸ� ����� ��Ű�ϱ�!
		}
	}

	this->playerheight();			//�÷��̾��� y���̵� �Լ�

	
}

void player::playerUi()
{
	//f2�� ������ ���ȯ�Ѵ����ƾƾ�
	if (KEYMANAGER->isOnceKeyDown(VK_F2))
	{
		if (_pet->getState() == NONE)				//���� ���°��� NONE�� ��, ��ȯ���� �ʾ����� NONE
		{
			_pet->summon();							//���ȯ!
		}
		else if (_pet->getState() != NONE && !_pet->getRiding())	//���� ���°��� NONE�� �ƴҶ�(��ȯ�� �Ǹ� NONE�� �ƴ�) �׸��� ���� Ÿ�� �ʾ�����
		{
			_pet->Unsummon();						//���ȯ����!
		}
	}
}
//�÷��̾��� ��ų ���ݺκ�
void player::playerAtkSkillIcon()
{
	for (int i = 0; i < 8; i++)
	{
		// 1+i , �� 8���� �ѹ� ��ų����Ű�� ������(��ų�� ������ ����)
		if (KEYMANAGER->isOnceKeyDown('1' + i))
		{
			//����ġ ������, ��ų����Ű�� �ִ� second���� ����(��ų�� ����)
			switch (_mPlayerSkillIcon.find(i)->second)
			{
				//���ݽ�ų�� state�� 1��, �޽��� 0����
				//��ų �����ܴ�� ��ųī���͸� �ٲ��ش�.
				//�޽��� ������ ��� ����� atk_idle�� �ٲ�(�Ӹ����� �����ܶ����� ������)
			case smash:
				if (_curSt < 10)
				{
					return;
				}
				playerSkillCount = 1;
				playerStateCount = 1;
				if (!_moveState)			//�����̴� ���� �ƴҶ� ���ݸ�� �������
				{
					_meshInfo->play("atk_idle");
				}
				else if (!_battleMode)		//�����̴� ���̰� ���ݸ�尡 �ƴҶ� ���ݸ�� �޸��¸������ 
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
				//��ųī��Ʈ�� 5�� �ƴҶ�, �� �Ͼ������ �ɴ´�
				if (playerSkillCount != 5)
				{
					playerSkillCount = 5;
					playerStateCount = 0;
					_meshInfo->play("natural_To_Sit", "natural_Sit");
					_hyusicTimer = TIMEMANAGER->getWorldTime();
				}
				//�ɾѴٰ� �Ͼ��
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

	//��ų�޽�
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
		//�÷��̾ ������ ����.
		_curHp -= damage;
		if (_curHp <= 0) return;
		_damageDelay = true;
	}

	this->playerMoveStop();

	_atkTimer = TIMEMANAGER->getWorldTime();
	//���� '���潺'�����̸鼭 ��밡 ��Ÿ�� ������ ��
	if (playerSkillCount == 2 && (state == 1 || state == 2 || state == 3))
	{
		playerMotionCount = 6;							//�÷��̾� ��� = ���ư�

		_meshInfo->play("atk_Guard", "atk_idle");		//��ǽ���
		_isStiffen = true;								//������
		playerSkillCount = 0;
	}
	//���� 'ī����'�����̸鼭 ��밡 ��Ÿ�϶�
	else if (playerSkillCount == 3 && (state == 1 || state == 2 || state == 3))
	{
		playerMotionCount = 1;							//�÷��̾� ��� = ���ư�

		_meshInfo->play("atk_Counter", "atk_idle");		//��ǽ���
		_isStiffen = true;								//���ư�����
		playerSkillCount = 0;
	}
	else if (state == 1 || state == 2)
	{
		playerMotionCount = 5;							//�÷��̾� ��� = ����
		_meshInfo->play("stand_idle");
		int aaa = randomIntRange(0, 100);				//�������� �༭ �������� 2���߿� ����� ������ ��
		if (aaa < 50)
		{
			_meshInfo->play("hit_A", "atk_idle");
		}
		else
		{
			_meshInfo->play("hit_B", "atk_idle");
		}
		_isStiffen = true;								//����� ���������� ������Ŵ
	}
	//���� 'ī����'�����̸鼭 ��밡 ���޽��϶�
	else if (playerSkillCount == 3 && state == 6)
	{
		playerMotionCount = 1;							//�÷��̾� ��� = ���ư�

		_meshInfo->play("atk_Counter", "atk_idle");		//��ǽ���
		_isStiffen = true;								//���ư�����
		playerSkillCount = 0;
	}
	//���� '���潺'�����̸鼭 ��밡 ���޽��� ���� ��
	else if (playerSkillCount == 2 && state == 6)
	{
		playerMotionCount = 6;							//�÷��̾� ��� = ���ư�

		_meshInfo->play("hit_satnd", "atk_idle");		//��ǽ���
		_isStiffen = true;								//������
		_isDown = true;									//���ư�����
		playerSkillCount = 0;
	}
	else if (state == 3 || state == 6)
	{
		playerMotionCount = 6;							//�÷��̾� ��� = ���ư�

		_meshInfo->play("hit_satnd", "atk_idle");		//��ǽ���
		_isStiffen = true;								//������
		_isDown = true;									//���ư�����
	}
	//���� '���潺'�����̸鼭 ��밡 ������� ���� ��
	if (playerSkillCount == 2 && state == 8)
	{
		playerMotionCount = 6;
		//����
		_isStiffen = true;								//������
		_meshInfo->play("atk_Guard");
		playerSkillCount = 0;
	}
	//���� 'ī����'�����̸鼭 ��밡 ������϶�
	if (playerSkillCount == 3 && state == 8)
	{
		playerMotionCount = 1;							//�÷��̾� ��� = ���ư�

		_meshInfo->play("hit_satnd", "atk_idle");		//��ǽ���
		_isStiffen = true;								//������
		_isDown = true;									//���ư�����
		playerSkillCount = 0;
	}
	//��밡 ī�����϶� ��������
	if (state == 4)
	{
		playerMotionCount = 6;
		_meshInfo->play("hit_satnd", "atk_idle");
		playerSkillCount = 0;
		_isStiffen = true;
		_isDown = true;
	}
}
//�÷��̾� �׾���!
void player::playerDie()
{
	if (_curHp <= 0 && !_bDie)
	{
		_bDie = true;
		_isDown = true;
		_meshInfo->play("hit_Ground","hit_die");
	}
}
//�÷��̾� ���ư�
void player::playerDown()
{
	if (_isDown)	//���ư��� ���϶�
	{
		if (_meshInfo->getAniFactor() < 0.4f)		//���� ���������� ����
		{
			_meshInfo->movePosition(-0.025);		//�ڷ� �̵�
			_mainCamera->movePositionWorld(_meshInfo->getCharacterPosition()->getForward() * -0.025);		//ī�޶� �ڷ��̵�
			_meshInfo->AniMeshPosition();			//�ִϸ��̼ǰ� �Ž���ġ�� ��ġ��Ŵ
		}
		else
		{
			_isDown = false;						//�ڷ� ���󰡴°� ����
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

	//06.18�κ��� + _flyHeight�߰�
	_meshInfo->setYPosition(heightRayHitPos.y + isRide + _flyHeight);
}

void player::petRiding()
{
	if (KEYMANAGER->isOnceKeyDown('R'))
	{
		//���������ʰ� Ÿ���ʾ�����
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
		//�������
		//ſ�� ��������������
		if (playerStateCount == 2 && !_moveState && _ridingMode)
		{
			_meshInfo->play("riding_Mount", "riding_Fly_Stand");
		}
	}

	//Ÿ�������� �ɸ����� ��ġ����
	if (_ridingMode)
	{
		_meshInfo->ridingMeshPosition();
		if (KEYMANAGER->isOnceKeyDown('V'))
		{
			_flyMode = !_flyMode;				//vŰ�� �������� ������¸� �����Ѵ�
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
//������, ����
void player::playerLvUp()
{
	//�ѹ��� �����Ѵٴ� ���� �Ͽ� �Լ��� «
	//�����̶� �ּ�X
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
	else if (!_battleMode)							//��Ʋ���? no
	{
		_meshInfo->play("stand_idle");			//��� : �⺻-������
	}
	else
	{
		_meshInfo->play("atk_idle");			//��Ʋ���? ok ��� : ����-������
	}
	_meshInfo->moveStop();						//���� ����!
	_moveState = false;							//�����ϰŴ�? nono
	_targetBool = false;
}

void player::setTarget(D3DXVECTOR3 targetPos, bool targetBool)
{
	if (_isStiffen || playerSkillCount == 4) return;	//�������Ͻ� ���� Ŭ������
														//������϶� ���� Ŭ������
	if (!_ridingMode || !targetBool)
	{
		_targetPos = targetPos;		//Ÿ����ġ����
		_targetBool = targetBool;	//Ÿ���� �ִ���

		if (_targetBool)
		{
			_meshInfo->moveStop();
			_meshInfo->play("atk_idle");
			_movePos = _targetPos;
			_movePos.y = _meshInfo->getPlayerPosition().y;	//��ǥ������ �ɸ����� ���̸� ���Ը���
			_meshInfo->lookPosition(_movePos);				//��ǥ�� �̵��Ѵ�
			playerStateCount = 1;
			playerMotionCount = 4;
			_battleMode = true;
			_atkTimer = TIMEMANAGER->getWorldTime();
			//playerSkillCount = 0;
			return;
		}
	}
}

