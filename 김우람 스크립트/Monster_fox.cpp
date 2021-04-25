#include "stdafx.h"
#include "Monster_fox.h"


HRESULT Monster_fox::init(player* character, baseObject* _map, D3DXVECTOR3 randPos, camera* pCamera)
{
	D3DXMATRIXA16 matScale, matRotateX, matRotateY, matCorrection;

	D3DXMatrixScaling(&matScale, 0.0001f, 0.0001f, 0.0001f);
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(-90));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(10));
	matCorrection = matRotateX * matRotateY * matScale;

	this->setMesh(RM_SKINNED->addResource("Resources/mabinogi/Monster/fox/fox.x", &matCorrection));
	this->setActive(true);

	_char = character;
	initForRandomSeed();

	_foxPosition = new transform;
	_transform->setWorldPosition(10, 0, -25);
	_SpawnPos = _transform->getWorldPosition();
	_foxPosition->setWorldPosition(_transform->getWorldPosition());

	_scene = _map;
	_mainCamera = pCamera;

	_FoxBox = new boundBox;
	_FoxBox->setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.2f, 0.2f, 0.2f));

	// Ȥ�� �Ǿ��ٸ� ������ ü�ºκ� �ٲټ� ������ 
	_hpBar = new billboardProgress(_foxPosition, &_fox_maxHp, &_fox_hp, 0, 0,
		0xffc0c000, 0xffff0000, 0.55f);

	_curSkill[0] = NULL;
	_curSkill[1] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Smash.png");
	_curSkill[2] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Defence.png");
	_curSkill[3] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Counter.png");
	_curSkill[4] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Windmill.png");

	return S_OK;
}

void Monster_fox::release(void)
{
}

void Monster_fox::update(void)
{
	baseObject::update();
	baseObject::_transform->setWorldPosition(_transform->getWorldPosition());

	_SpawnPos.y = _foxPosition->getWorldPosition().y;

	state();
	
	// ������� ���ư��� �Լ� (5.0f �Ѿ�� ���� ������� ���ư� ) 
	returnback();
	
	if (_skinnedAnim->getFactor() > 0.90f)
	{
		if (_fox_hp <= 0)
		{
			_foxstate = f_die;
		}
	}

	if (_fox_hp <= 0)
	{
		_fox_hp = 0;
	}
	
	// �񼱰� ���̱⶧���� ������ ���� �����ʴ� �̻� �����Ӱ� �ٴѴ�.
	if (!_appreciate && !_distanceOverCheck) noPlayer();

	if (!_die)
	{
		if (_appreciate && !_distanceOverCheck) //���࿡ ������ �������� ���� �ν��� Ʈ�縦 endure�Լ����� �����Ŵ 
		{
			// �����鼭�� ������ ������Ѽ� �ȵȴ�.
			if (!_ing)
			{
				pattern();
			}
		}

		// 3Ÿ�� �����ϰ� ������ �ɾ���
		if (_foxstate == f_attack)
		{
			this->hit();
		}
	}	
}

void Monster_fox::render(void)
{
	baseObject::render();

	//_foxPosition->renderGizmo();
	//_transform->renderGizmo();
	//_FoxBox->renderGizmo(_foxPosition);

	if (_appreciate || GET_HPVIEW)
	{
		_hpBar->render(_mainCamera);	// �ǰ������ٴ� ���� ����ϋ� ������ �Ѵ�
	}

	// ��ų�������� ����
	_hpBar->skillRender(_mainCamera, _tex);

	// ���࿡ ���� ������ ���� �Ŀ� ���͸� ���Ϳ��� �����Ѵ� . 
	char info[256];

	sprintf(info, "_ing  : %d", _ing);
	FONTMANAGER->fontOut(info, 10, 270, 0xffffff00);
}

// crossfadeTime : ������ �ٸ��ɷ� �ٲ�°� 

void Monster_fox::state(void)
{
	_heightRay.origin = _foxPosition->getWorldPosition() + D3DXVECTOR3(0, 2, 0);
	_heightRay.direction = D3DXVECTOR3(0, -1, 0);

	D3DXVECTOR3 heightRayHitPos;
	PHYSICSMANAGER->isRayHitStaticMeshObject(&_heightRay, _scene, &heightRayHitPos, NULL);

	this->setYPosition(heightRayHitPos.y);

	_char->getPlayerPosition();

	_distance = _char->getPlayerPosition() - _foxPosition->getWorldPosition();
	_distanceResult = D3DXVec3Length(&_distance);

	switch (_foxstate)
	{
	case f_IDLE:
	{
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->Play("IDLE");
		
		eMonsterState = 0;
	}
	break;
	case f_run:
	{
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->Play("run");		
	}
	break;

	case f_attack:
	{
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->Play("attack");

		_secondTime = 0;
	}
	break;
	case f_smash:
	{
		eMonsterState = 6;
		_tex = _curSkill[1];
		_ing = true;

		if (_distanceResult >= 1.1f && !_motionstop)
		{
			_skinnedAnim->SetPlaySpeed(3.0f);
			_skinnedAnim->Play("run");

			_foxPosition->movePositionSelf(0.0f, 0.0f, 0.055f);		//�̵��ӵ���� �̵�
			_foxPosition->lookDirection(_transform->getForward());
		}

		else
		{
			_motionstop = true;
		}

		if (_char->getPlayerSkillCount() != 3)
		{
			if (_motionstop)
			{
				_skinnedAnim->SetPlaySpeed(3.0f);
				_skinnedAnim->Play("smash");
				_transform->setWorldPosition(_foxPosition->getWorldPosition());

				if (!dpfkdlrotRldi && _skinnedAnim->getFactor() >= 0.38f)
				{
					_char->playerHit(6, _fox_atk);
					dpfkdlrotRldi = true;
				}
				if (_skinnedAnim->getFactor() >= 0.90f) // ok
				{
					_foxstate = f_IDLE;
					_changeAttack = false;
					_ing = false;
					_motionstop = false;
					_fox_atkCount = 1;

					_foxstate = (Foxstate)randomIntRange(4, 6);
					cout << " ���ҳı� " << endl;
				}
			}
		}
		// ���࿡ ī����
		if (_char->getPlayerSkillCount() == 3)
		{
			_skinnedAnim->SetPlaySpeed(3.0f);
			_skinnedAnim->Play("smash");
			_transform->setWorldPosition(_foxPosition->getWorldPosition());

			if (_skinnedAnim->getFactor() >= 0.38f)
			{
				_char->playerHit(6, _fox_atk);
				_foxstate = f_fly;
			}
		}
	}

	break;
	case f_defense:
	{
		_skinnedAnim->SetPlaySpeed(1.0f);
		_skinnedAnim->Play("defense");
		eMonsterState = 5;

		// ������ ������������ �ȷο�� �� ���̰�
		this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
			_transform->getWorldPosition().y, _char->getPlayerPosition().z));

		// ���࿡ ���潺�� 15�ʵ��� ���潺 ��������  
		// ��ġ�� �׳� �����Ű� ġ�� endureA���� ó���Ұ��� 
		_changeAttack = true;
		_ing = true;

		if (TIMEMANAGER->getWorldTime() - _secondTime >= 15.0f)
		{
			_secondTime = TIMEMANAGER->getWorldTime();
			_ing = false;
			_changeAttack = false;
			_foxstate = (Foxstate)randomIntRange(2, 6);
			pattern();
		}

		_tex = _curSkill[2];
	}
	break;

	case f_counter:
	{
		_skinnedAnim->SetPlaySpeed(1.0f);
		_skinnedAnim->Play("defense");
		eMonsterState = 4;

		// ������ ������������ �ȷο�� �� ���̰�
		this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
			_transform->getWorldPosition().y, _char->getPlayerPosition().z));

		// ���࿡ ī���͸� 15�ʵ��� ���潺 ��������  
		// ��ġ�� �׳� �����Ű� ġ�� endureA���� ó���Ұ��� 
		_changeAttack = true;
		_tex = _curSkill[3];

		if (TIMEMANAGER->getWorldTime() - _secondTime >= 15.0f)
		{
			_secondTime = TIMEMANAGER->getWorldTime();
			_ing = false;
			_changeAttack = false;
			_foxstate = (Foxstate)randomIntRange(2, 6);
			pattern();
		}
	}
	break;

	case f_wating:
	{
		_skinnedAnim->SetPlaySpeed(2.0f);
		_skinnedAnim->Play("defense");

		if (TIMEMANAGER->getWorldTime() - _secondTime >= 5.0f)
		{
			_secondTime = TIMEMANAGER->getWorldTime();
			_ing = false;
			_changeAttack = false;
			_foxstate = (Foxstate)randomIntRange(2, 6);		
			pattern();
		}
	}
	break;

	case f_endureA:
	{
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->Play("endureA");
	
		if (_skinnedAnim->getFactor() >= 0.90f)
		{
			_skinnedAnim->Play("defense");
			_foxstate = f_wating;
			_ing = false;
			_foxstate = (Foxstate)randomIntRange(2, 6);
			_secondTime = TIMEMANAGER->getWorldTime();
			pattern();
		}
	}
	break;

	case f_endureB:
	{
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->Play("endureB");

		if (_skinnedAnim->getFactor() >= 0.90f)
		{
			_skinnedAnim->Play("defense");
			_foxstate = f_wating;
			_ing = false;
			_foxstate = (Foxstate)randomIntRange(2, 6);
			_secondTime = TIMEMANAGER->getWorldTime();
			pattern();
		}
	}
	break;
	case f_smashEndure:
	{
		if (_char->getMesh()->getAniFactor() < 0.3f)
		{
			this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
				_transform->getWorldPosition().y, _char->getPlayerPosition().z));
			
			_foxPosition->lookDirection(_transform->getForward());

			_skinnedAnim->Play("smash_endure");
			_skinnedAnim->Stop();
		}

		else if (_char->getMesh()->getAniFactor() > 0.3f)
		{
			_skinnedAnim->SetPlaySpeed(3.0f);
			_skinnedAnim->Play("smash_endure");
			this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
				_transform->getWorldPosition().y, _char->getPlayerPosition().z));

			_foxPosition->lookDirection(_transform->getForward());

			_transform->setWorldPosition(_foxPosition->getWorldPosition());

			_foxPosition->movePositionWorld(_foxPosition->getForward() * -0.015f);		//�̵��ӵ���� �̵�

		}

		if (_skinnedAnim->getFactor() >= 0.90f)
		{
			_skinnedAnim->Play("fly");
			_secondTime = TIMEMANAGER->getWorldTime();
			_foxstate = f_die;
		}
	}
	break;
	case f_fly:
	{
		this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
			_transform->getWorldPosition().y, _char->getPlayerPosition().z));

		_foxPosition->lookDirection(_transform->getForward());

		_transform->setWorldPosition(_foxPosition->getWorldPosition());

		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->Play("fly");

		_foxPosition->movePositionWorld(_foxPosition->getForward() * -0.015f);		//�̵��ӵ���� �̵�
		
		if (_skinnedAnim->getFactor() > 0.94f)
		{
			_skinnedAnim->Play("endureA");
			_foxstate = f_die;
		}
	}
	break;

	// �ణ�� �������ӹ��� but �ҽ��� ����x 
	case f_die:
	{
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->PlayOneShotAfterHold("die");

		// ���Ƿ� �������� 
		if (TIMEMANAGER->getWorldTime() -  4.0f >= _secondTime)
		{
			_secondTime = TIMEMANAGER->getWorldTime();
			cout << _secondTime << endl;
			if (_fox_hp > 0)
			{
				//die->�׾��� : die �״�� / ��ҳ� : die to up
				_skinnedAnim->Play("fly");
				_die = false;
				_ing = false;

				_foxstate = f_dieToup;

				cout << "����" << endl;
			}
		}
		if (_fox_hp <= 0)
		{
			_fox_hp = 0;
			if (!_die)
			{
				_char->playerLvUp();
			}
			_die = true;
		}		
	}
	break;

	// ��Ǻ�ȭ ����x
	case f_dieToup:
	{
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->Play("die_to_up");

		if (_skinnedAnim->getFactor() > 0.90f)
		{
			_skinnedAnim->Play("fly");
			_foxstate = f_null;

			_foxstate = (Foxstate)randomIntRange(2, 6);
			_secondTime = TIMEMANAGER->getWorldTime();
			pattern();
			cout << "��Ȱ" << endl;
		}
	}
	break;

	case f_follow:
	{	
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->Play("run");

		if (!_distanceOverCheck)
		{
			this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
				_transform->getWorldPosition().y, _char->getPlayerPosition().z));
		}

		_foxPosition->movePositionSelf(0.0f, 0.0f, 0.055f);		//�̵��ӵ���� �̵�
		_foxPosition->lookDirection(_transform->getForward());

		if (_distanceResult < 1.1f)
		{
			_skinnedAnim->Play(f_IDLE);
			_transform->setWorldPosition(_foxPosition->getWorldPosition());
			_changeAttack = true;
			_ing = false;

			if (_changeAttack && !_ing)
			{
				_foxstate = (Foxstate)randomIntRange(2, 6);
				pattern();
			}
		}

		// ��������� ������ �����ϰ� ������
		if (_distanceResult < 1.1f && _changeAttack && !_ing)
		{
			_changeAttack = false;
		}

	}
	break;

	case f_CounterAttack:
	{
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->Play("counter");
		_ing = true;

		if (!_distanceOverCheck)
		{
			this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
				_transform->getWorldPosition().y, _char->getPlayerPosition().z));
		}

		// �������� �ٸ��� ��� �������� 
		if (_skinnedAnim->getFactor() >= 0.98f)
		{
			// ���¸� ��� ���潺 ��Ǹ� �����ְ� 
			_foxstate = f_null;
			_secondTime = TIMEMANAGER->getWorldTime();
		}
	}
	break;

	case f_null :
	{
		_skinnedAnim->SetPlaySpeed(1.0f);
		_skinnedAnim->Play("defense");

		// ������ �����ִٰ� ���ٰ��� 
		if (TIMEMANAGER->getWorldTime() - _secondTime >= 5.0f)
		{
			_secondTime = TIMEMANAGER->getWorldTime();
			_ing = false;
			_changeAttack = false;
			_foxstate = (Foxstate)randomIntRange(2, 6);
			pattern();
		}
	}
	break;

	default:
		break;
	}

	if (_foxstate != f_smash && _foxstate != f_defense && _foxstate != f_counter)
	{
		_tex = _curSkill[0];

	}

	if (_foxstate == f_smash && _foxstate == f_defense && _foxstate == f_counter)
	{
		_secondTime = TIMEMANAGER->getWorldTime();
	}
}

void Monster_fox::noPlayer()
{
	//Animation ���� Factor ( 0 �̸� ó�� 1 �ϼ��� ������ )

	// �ִϸ��̼� ���� �����ϸ� ���Ӱ� ���������� ������
	if (_skinnedAnim->getFactor() < 0.97f) _Check = false;
	else _Check = true;

	if (_Check && _foxstate == f_IDLE)
	{
		rotateAngle = randomIntRange(0, 360);
		_foxstate = (Foxstate)randomIntRange(0, 1);
	}

	if (_foxstate == f_run)
	{
		_foxPosition->movePositionSelf(0.0f, 0.0f, 0.035f);		//�̵��ӵ���� �̵�

		if (_Check)
		{
			D3DXMATRIXA16 matRotate;
			D3DXMatrixRotationY(&matRotate, D3DXToRadian(rotateAngle));
			_transform->setRotateWorld(matRotate);
			_foxPosition->lookDirection(_transform->getForward());
			_transform->rotateWorld(0, D3DXToRadian(-10), 0);
		}

		if (_skinnedAnim->getFactor() >= 0.98)
		{
			_skinnedAnim->Play(f_IDLE);
			_transform->setWorldPosition(_foxPosition->getWorldPosition());
			_foxstate = f_IDLE;
		}
	}
}

void Monster_fox::pattern(void)
{

	if (_distanceResult <= 3.0f && _distanceResult > 0.9f && !_changeAttack && !_ing)
	{
		_foxstate = f_follow;
	}
	//�÷��̾ �׾����� �ν��� Ǯ�������
	if (_char->getPlayerDie())
	{
		_foxstate = f_IDLE;
		_appreciate = false;  // ���ִٰ� �ٽó�����
		_changeAttack = false;
		_ing = false;
	}
	// �÷��̾�� �Ÿ� ������ ������ �����Ѵ�.
	if (_distanceResult < 1.1f)
	{
		// ���찡 ó�� ������ �÷��̾� �ʿ� �ٰ� �����Ѵ� 	
		this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
			_transform->getWorldPosition().y, _char->getPlayerPosition().z));

		if (!_ing) _secondTime = TIMEMANAGER->getWorldTime();

		if (_foxstate == f_attack && !_ing)
		{
			_ing = true;
			this->hit();
		}
	}

	// ���࿡ ���������� ������ �׳� ���÷��̾�� ��������
	if (_distanceResult > 3.0f && !_changeAttack)
	{
		_foxstate = f_IDLE;
		_appreciate = false;  // ���ִٰ� �ٽó�����
		_changeAttack = false;
		_ing = false;
	}
}

void Monster_fox::setYPosition(float YPosition)
{
	D3DXVECTOR3 framePosition;		//�ִϸ��̼� ���������� ���� vec3
	D3DXVECTOR3 charPosition;		//���� ���������� ���� vec3
									//�ִϸ��̼� ��������ing

	framePosition = _transform->getWorldPosition();
	framePosition.y = YPosition;
	_transform->setWorldPosition(framePosition);
	//�÷��̾� ��������ing
	charPosition = _foxPosition->getWorldPosition();
	charPosition.y = YPosition;
	_foxPosition->setWorldPosition(charPosition);
}

void Monster_fox::distanceOver()
{
	_skinnedAnim->Play("defense");
	_transform->setWorldPosition(_foxPosition->getWorldPosition());
	_foxPosition->lookPosition(_SpawnPos);
	_transform->lookDirection(_foxPosition->getForward());
	_transform->rotateWorld(0, D3DXToRadian(-10), 0);

	_distanceOverCheck = true;
}

void Monster_fox::hit()
{
	_changeAttack = true; 

	// ���潺�� ī���Ͱ� �ƴҋ� �ϼ� 
	if (_char->getPlayerSkillCount() != 2 && _char->getPlayerSkillCount() != 3)
	{
		if (_skinnedAnim->getFactor() > 0.23f && _skinnedAnim->getFactor() < 0.24f)
		{
			if (_fox_atkCount == 1)
			{
				eMonsterState = 1;	// �÷��̾����� ����������ϴ°�
				_char->playerHit(_fox_atkCount, _fox_atk);
			}

			if (_fox_atkCount == 2.0f)
			{
				eMonsterState = 2;
				_char->playerHit(_fox_atkCount, _fox_atk);
			}

			if (_fox_atkCount >= 3.0f)
			{
				eMonsterState = 3;
				_char->playerHit(_fox_atkCount, _fox_atk);
			}
		}
	}

	if (_char->getPlayerSkillCount() == 2)
	{
		if (_fox_atkCount < 3.0f)
		{
			_foxstate = f_wating;
		}
	}

	if (_char->getPlayerSkillCount() == 3)
	{
		if (_fox_atkCount < 3.0f)
		{
			if (_skinnedAnim->getFactor() > 0.50f)
			{
				_foxstate = f_fly;
				_char->playerHit(_fox_atkCount, 0);
			}
		}
	}

	if (_skinnedAnim->getFactor() >= 0.99f && _skinnedAnim->getFactor() <= 0.998f)
	{
		_fox_atkCount++;
	}

	if (_fox_atkCount > 3.0f)
	{
		_foxstate = (Foxstate)randomIntRange(4, 6);
		_changeAttack = false;
		_ing = false;
	}
}

void Monster_fox::returnback()
{
	_appreciate = false;
	_ing = true;
	// �Ÿ��� �����̻� �Ѿ�� �ٽ� ��ġ�� �ǵ������
	_SpawnDir = _SpawnPos - _foxPosition->getWorldPosition();
	_SpawnDirResult = D3DXVec3Length(&_SpawnDir);

	if (_SpawnDirResult > 10.0f && !_distanceOverCheck)
	{
		distanceOver();
	}

	if (_distanceOverCheck)
	{
		_foxstate = f_run;
		_foxPosition->movePositionSelf(0.0f, 0.0f, 0.035f);		//�̵��ӵ���� �̵�

		if (_skinnedAnim->getFactor() >= 0.98f)
		{
			_skinnedAnim->Play("IDLE");
			_transform->setWorldPosition(_foxPosition->getWorldPosition());
		}
	}

	if (_SpawnDirResult < 1.1f && _distanceOverCheck)
	{
		_skinnedAnim->Play("IDLE");
		_transform->setWorldPosition(_foxPosition->getWorldPosition());
		_foxstate = f_IDLE;
		_distanceOverCheck = false;
		_appreciate = false;
		_ing = false;
		this->noPlayer();
	}
}

// �÷��̾ ���� ĥ��� ������ ������Ʈ�����޾Ƽ� �ݰ��� �ؾ��� �ٵ� ��ų�� �޴°��� �򰥷�
void Monster_fox::endureA(int playerstate, int playerAtk)
{
	_appreciate = true;
	_fox_atkCount = 1;

	temp1 = playerstate;
	temp2 = playerAtk;

	// ����� �� // 
	// ����� , ���Ž� , ���� , 
		// 1) �÷��̾ �����ϋ� 
	if (_foxstate == f_IDLE)
	{
		// ��Ÿ�� ùŸ�� ������ + ����з� ĥ��
		if (playerstate == 0 && playerAtk == 0)
		{
			_foxstate = f_endureA;
			//_secondTime = TIMEMANAGER->getWorldTime();
		}

		// ��Ÿ�� ùŸ�� ������ + ����з� ĥ��
		if (playerstate == 1 && playerAtk == 0)
		{
			Smash_endure();
		}

		// windmill
		if (playerstate == 4 && playerAtk == 0)
		{
			_foxstate = f_fly;
		}
	}

	// 1-1 ) ���ϋ� 
	if (_foxstate == f_run)
	{
		// ��Ÿ�� ùŸ�� ������ + ����з� ĥ��
		if (playerstate == 0 && playerAtk == 0)
		{
			_foxstate = f_endureA;
		}

		if (playerstate == 4 && playerAtk == 0)
		{
			_foxstate = f_fly;
		}

		// ��Ÿ�� ùŸ�� ������ + ����з� ĥ��
		if (playerstate == 1 && playerAtk == 0)
		{
			Smash_endure();
		}

	}

	if (_foxstate == f_endureA)
	{
		// 2��° Ÿ
		if (playerstate == 0 && playerAtk == 1)
		{
			_foxstate = f_endureB;
			endureB(0, 1);
		}

		// ���Ž�
		if (playerstate == 1 && playerAtk == 0)
		{
			Smash_endure();
		}

		// ����� 
		if (playerstate == 4 && playerAtk == 0)
		{
			_foxstate = f_fly;
		}
	}

	if (_foxstate == f_endureB)
	{
		if (playerstate == 0 && playerAtk == 2 || playerstate == 1 && playerAtk == 0 || playerstate == 4 && playerAtk == 0)
		{
			_foxstate = f_fly;
			endureC();
		}
	}


	// 2) ���� ���潺 �����̰� ������ ��Ÿ�� ĥ�� + ����� ĥ��
	if (_foxstate == f_defense && playerstate == 0 && playerAtk == 0 ||
		_foxstate == f_defense && playerstate == 4 && playerAtk == 0)
	{
		_fox_atkCount = 1;
		_skinnedAnim->Play("IDLE");
		_foxstate = f_attack; // ��Ÿ�� �ݰ��Ұ��� 
	}

	// 2 ) ���� ���潺 �����̰�, ������ ���Žø� ĥ��� 
	if (_foxstate == f_defense && playerstate == 1 && playerAtk == 0) // �Ǹ����� ���ڴ� ���� ������ ? 
	{
		Smash_endure();
	}

	// 3)  ���� ī���� ���� �÷��̾ ���� �ϋ� + ���Ž�
	if (_foxstate == f_counter && playerstate == 0 && playerAtk == 0 ||
		_foxstate == f_counter && playerstate == 1 && playerAtk == 0)
	{
		// �������� �̻����� ��������  ī���� ��� ����ϰ� 
		_foxstate = f_CounterAttack;
		_char->playerHit(4, _fox_atk);
	}

	// 4) ���� ī���� ���� �÷��̾ ����� ĥ��
	if (_foxstate == f_counter && playerstate == 4 && playerAtk == 0)
	{
		_foxstate = f_fly;
	}

	// 5) ���� ���Ž� �ϋ� �÷��̾ ��Ÿ�� ĥ�� 
	if (_foxstate == f_smash && playerstate == 0 && playerAtk == 0)
	{
		// �������� �̻����� ��������  ī���� ��� ����ϰ� 
		_foxstate = f_endureA;
	}

	if (_foxstate == f_smash && playerstate == 4 && playerAtk == 0)
	{
		// �������� �̻����� ��������  ī���� ��� ����ϰ� 
		_foxstate = f_endureA;
	}

	// 6) ���Ͱ� ������ �����ϋ� �÷��̾ ùŸ�� ������ 
	if (_foxstate == f_wating)
	{
		if (playerstate == 0 && playerAtk == 0)
		{
			_foxstate = f_endureA;
		}

		if (playerstate == 4 && playerAtk == 0)
		{
			_foxstate = f_fly;
		}

		if (playerstate == 1 && playerAtk == 0)
		{
			Smash_endure();
		}

		// �ִϸ��̼� ����� ������ 
		if (_skinnedAnim->getFactor() >= 0.80f)
		{
			// �ð��ȿ� ������ �ȵ����� ��������
			if (TIMEMANAGER->getWorldTime() - _secondTime >= 2.0f)
			{
				_secondTime = TIMEMANAGER->getWorldTime();
				pattern();
			}

			// �ð��ȿ� ������ ������ b����
			if (TIMEMANAGER->getWorldTime() - _secondTime < 2.0f)
			{
				_secondTime = TIMEMANAGER->getWorldTime();

				if (playerstate == 0 && playerAtk == 1)
				{
					endureB(playerstate, playerAtk);
				}
			}
		}
	}


}

void Monster_fox::endureB(int playerstate, int playerAtk)
{
	if (_skinnedAnim->getFactor() >= 0.95) this->pattern();
	
	if (playerstate == 0 && playerAtk == 2)
	{
		endureC();
	}

}

void Monster_fox::endureC()
{
	_foxstate = f_fly;

	// ������ die -> �׾��� : die �״�� / ��ҳ� : die to up
	
	_secondTime = TIMEMANAGER->getWorldTime();
	

}

void Monster_fox::Smash_endure()
{
	_foxstate = f_smashEndure;

}


Monster_fox::Monster_fox()
{
	_fox_hp = 100;
	_fox_maxHp = 100;
	rotateAngle = 0;

	_fox_gold = randomIntRange(50, 500);
	_fox_atk = 10;

	_secondTime = 0;
	_randNum = 0;
	_foxstate = f_IDLE;

	_fox_atkCount = 1;
	_count = 0;

	_Check = false;
	_changeAttack = false;
	_atkcheck = false;
	_appreciate = false;
	_rotate = false;
	_distanceOverCheck = false;
	_die = false;

	temp1 = 0;
	temp2 = 0;

	dpfkdlrotRldi = false;
}


Monster_fox::~Monster_fox()
{
}
