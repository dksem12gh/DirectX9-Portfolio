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

	// 혹시 피안줄면 포인터 체력부분 바꾸셈 ㅎㅂㅎ 
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
	
	// 원래대로 돌아가는 함수 (5.0f 넘어가면 지금 원래대로 돌아감 ) 
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
	
	// 비선공 몹이기때문에 공격을 먼저 받지않는 이상 자유롭게 다닌다.
	if (!_appreciate && !_distanceOverCheck) noPlayer();

	if (!_die)
	{
		if (_appreciate && !_distanceOverCheck) //만약에 맞으면 패턴으로 들어간다 인식을 트루를 endure함수에서 실행시킴 
		{
			// 맞으면서는 패턴을 실행시켜선 안된다.
			if (!_ing)
			{
				pattern();
			}
		}

		// 3타로 공격하게 제한을 걸었음
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
		_hpBar->render(_mainCamera);	// 피게이지바는 전투 모드일떄 보여야 한다
	}

	// 스킬뭐쓰는지 랜더
	_hpBar->skillRender(_mainCamera, _tex);

	// 만약에 몹이 죽으면 몇초 후에 몬스터를 벡터에서 뺴야한다 . 
	char info[256];

	sprintf(info, "_ing  : %d", _ing);
	FONTMANAGER->fontOut(info, 10, 270, 0xffffff00);
}

// crossfadeTime : 서서히 다른걸로 바뀌는거 

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

			_foxPosition->movePositionSelf(0.0f, 0.0f, 0.055f);		//이동속도대로 이동
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
					cout << " 돌았냐구 " << endl;
				}
			}
		}
		// 만약에 카운터
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

		// 가까이 붙을떄까지는 팔로우로 좀 붙이고
		this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
			_transform->getWorldPosition().y, _char->getPlayerPosition().z));

		// 만약에 디펜스면 15초동안 디펜스 돌린거임  
		// 안치면 그냥 있을거고 치면 endureA에서 처리할거임 
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

		// 가까이 붙을떄까지는 팔로우로 좀 붙이고
		this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
			_transform->getWorldPosition().y, _char->getPlayerPosition().z));

		// 만약에 카운터면 15초동안 디펜스 돌린거임  
		// 안치면 그냥 있을거고 치면 endureA에서 처리할거임 
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

			_foxPosition->movePositionWorld(_foxPosition->getForward() * -0.015f);		//이동속도대로 이동

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

		_foxPosition->movePositionWorld(_foxPosition->getForward() * -0.015f);		//이동속도대로 이동
		
		if (_skinnedAnim->getFactor() > 0.94f)
		{
			_skinnedAnim->Play("endureA");
			_foxstate = f_die;
		}
	}
	break;

	// 약간의 끝프레임문제 but 소스상 문제x 
	case f_die:
	{
		_skinnedAnim->SetPlaySpeed(3.0f);
		_skinnedAnim->PlayOneShotAfterHold("die");

		// 임의로 넣은거임 
		if (TIMEMANAGER->getWorldTime() -  4.0f >= _secondTime)
		{
			_secondTime = TIMEMANAGER->getWorldTime();
			cout << _secondTime << endl;
			if (_fox_hp > 0)
			{
				//die->죽었냐 : die 그대로 / 살았냐 : die to up
				_skinnedAnim->Play("fly");
				_die = false;
				_ing = false;

				_foxstate = f_dieToup;

				cout << "다이" << endl;
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

	// 모션변화 문제x
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
			cout << "부활" << endl;
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

		_foxPosition->movePositionSelf(0.0f, 0.0f, 0.055f);		//이동속도대로 이동
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

		// 가까워지면 패턴을 랜덤하게 돌린다
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

		// 끝났으니 다른거 모션 돌릴거임 
		if (_skinnedAnim->getFactor() >= 0.98f)
		{
			// 상태를 잠깐 디펜스 모션만 보여주고 
			_foxstate = f_null;
			_secondTime = TIMEMANAGER->getWorldTime();
		}
	}
	break;

	case f_null :
	{
		_skinnedAnim->SetPlaySpeed(1.0f);
		_skinnedAnim->Play("defense");

		// 랜덤을 몇초있다가 해줄거임 
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
	//Animation 진행 Factor ( 0 이면 처음 1 일수록 마지막 )

	// 애니메이션 끝에 도달하면 새롭게 랜덤값으로 돌린다
	if (_skinnedAnim->getFactor() < 0.97f) _Check = false;
	else _Check = true;

	if (_Check && _foxstate == f_IDLE)
	{
		rotateAngle = randomIntRange(0, 360);
		_foxstate = (Foxstate)randomIntRange(0, 1);
	}

	if (_foxstate == f_run)
	{
		_foxPosition->movePositionSelf(0.0f, 0.0f, 0.035f);		//이동속도대로 이동

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
	//플레이어가 죽었을때 인식을 풀어버린다
	if (_char->getPlayerDie())
	{
		_foxstate = f_IDLE;
		_appreciate = false;  // 좀있다가 다시넣으셈
		_changeAttack = false;
		_ing = false;
	}
	// 플레이어랑 거리 가까우면 전투를 시작한다.
	if (_distanceResult < 1.1f)
	{
		// 여우가 처음 시작은 플레이어 쪽에 붙고 시작한다 	
		this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
			_transform->getWorldPosition().y, _char->getPlayerPosition().z));

		if (!_ing) _secondTime = TIMEMANAGER->getWorldTime();

		if (_foxstate == f_attack && !_ing)
		{
			_ing = true;
			this->hit();
		}
	}

	// 만약에 범위밖으로 나가면 그냥 노플레이어로 가버리셈
	if (_distanceResult > 3.0f && !_changeAttack)
	{
		_foxstate = f_IDLE;
		_appreciate = false;  // 좀있다가 다시넣으셈
		_changeAttack = false;
		_ing = false;
	}
}

void Monster_fox::setYPosition(float YPosition)
{
	D3DXVECTOR3 framePosition;		//애니메이션 높이조절을 위한 vec3
	D3DXVECTOR3 charPosition;		//구름 높이조절을 위한 vec3
									//애니메이션 높이조절ing

	framePosition = _transform->getWorldPosition();
	framePosition.y = YPosition;
	_transform->setWorldPosition(framePosition);
	//플레이어 높이조절ing
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

	// 디펜스나 카운터가 아닐떄 하셈 
	if (_char->getPlayerSkillCount() != 2 && _char->getPlayerSkillCount() != 3)
	{
		if (_skinnedAnim->getFactor() > 0.23f && _skinnedAnim->getFactor() < 0.24f)
		{
			if (_fox_atkCount == 1)
			{
				eMonsterState = 1;	// 플레이어한테 전달해줘야하는값
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
	// 거리가 일정이상 넘어가면 다시 위치로 되돌어가야함
	_SpawnDir = _SpawnPos - _foxPosition->getWorldPosition();
	_SpawnDirResult = D3DXVec3Length(&_SpawnDir);

	if (_SpawnDirResult > 10.0f && !_distanceOverCheck)
	{
		distanceOver();
	}

	if (_distanceOverCheck)
	{
		_foxstate = f_run;
		_foxPosition->movePositionSelf(0.0f, 0.0f, 0.035f);		//이동속도대로 이동

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

// 플레이어가 나를 칠경우 쟤네의 스테이트값을받아서 반격을 해야함 근데 스킬을 받는건지 헷갈려
void Monster_fox::endureA(int playerstate, int playerAtk)
{
	_appreciate = true;
	_fox_atkCount = 1;

	temp1 = playerstate;
	temp2 = playerAtk;

	// 경우의 수 // 
	// 윈드밀 , 스매시 , 어택 , 
		// 1) 플레이어가 평상시일떄 
	if (_foxstate == f_IDLE)
	{
		// 평타로 첫타를 쳤을떄 + 윈드밀로 칠떄
		if (playerstate == 0 && playerAtk == 0)
		{
			_foxstate = f_endureA;
			//_secondTime = TIMEMANAGER->getWorldTime();
		}

		// 평타로 첫타를 쳤을떄 + 윈드밀로 칠떄
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

	// 1-1 ) 런일떄 
	if (_foxstate == f_run)
	{
		// 평타로 첫타를 쳤을떄 + 윈드밀로 칠떄
		if (playerstate == 0 && playerAtk == 0)
		{
			_foxstate = f_endureA;
		}

		if (playerstate == 4 && playerAtk == 0)
		{
			_foxstate = f_fly;
		}

		// 평타로 첫타를 쳤을떄 + 윈드밀로 칠떄
		if (playerstate == 1 && playerAtk == 0)
		{
			Smash_endure();
		}

	}

	if (_foxstate == f_endureA)
	{
		// 2번째 타
		if (playerstate == 0 && playerAtk == 1)
		{
			_foxstate = f_endureB;
			endureB(0, 1);
		}

		// 스매시
		if (playerstate == 1 && playerAtk == 0)
		{
			Smash_endure();
		}

		// 윈드밀 
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


	// 2) 내가 디펜스 상태이고 쟤들이 평타를 칠때 + 윈드밀 칠떄
	if (_foxstate == f_defense && playerstate == 0 && playerAtk == 0 ||
		_foxstate == f_defense && playerstate == 4 && playerAtk == 0)
	{
		_fox_atkCount = 1;
		_skinnedAnim->Play("IDLE");
		_foxstate = f_attack; // 평타로 반격할것임 
	}

	// 2 ) 내가 디펜스 상태이고, 쟤들이 스매시를 칠경우 
	if (_foxstate == f_defense && playerstate == 1 && playerAtk == 0) // 맨마지막 숫자는 뭐가 들어가야해 ? 
	{
		Smash_endure();
	}

	// 3)  내가 카운터 쓸떄 플레이어가 어택 일떄 + 스매시
	if (_foxstate == f_counter && playerstate == 0 && playerAtk == 0 ||
		_foxstate == f_counter && playerstate == 1 && playerAtk == 0)
	{
		// 날쳤으니 이새끼를 조져야함  카운터 모션 재생하고 
		_foxstate = f_CounterAttack;
		_char->playerHit(4, _fox_atk);
	}

	// 4) 내가 카운터 쓸떄 플레이어가 윈드밀 칠떄
	if (_foxstate == f_counter && playerstate == 4 && playerAtk == 0)
	{
		_foxstate = f_fly;
	}

	// 5) 내가 스매시 일떄 플레이어가 평타를 칠떄 
	if (_foxstate == f_smash && playerstate == 0 && playerAtk == 0)
	{
		// 날쳤으니 이새끼를 조져야함  카운터 모션 재생하고 
		_foxstate = f_endureA;
	}

	if (_foxstate == f_smash && playerstate == 4 && playerAtk == 0)
	{
		// 날쳤으니 이새끼를 조져야함  카운터 모션 재생하고 
		_foxstate = f_endureA;
	}

	// 6) 몬스터가 웨이팅 상태일떄 플레이어가 첫타를 쳤을떄 
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

		// 애니메이션 재생이 끝나고 
		if (_skinnedAnim->getFactor() >= 0.80f)
		{
			// 시간안에 공격이 안들어오면 랜덤패턴
			if (TIMEMANAGER->getWorldTime() - _secondTime >= 2.0f)
			{
				_secondTime = TIMEMANAGER->getWorldTime();
				pattern();
			}

			// 시간안에 공격이 들어오면 b실행
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

	// 끝나고 die -> 죽었냐 : die 그대로 / 살았냐 : die to up
	
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
