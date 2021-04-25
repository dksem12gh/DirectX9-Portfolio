#include "stdafx.h"
#include "Wolf01.h"


Wolf01::Wolf01()
{
}


Wolf01::~Wolf01()
{
}

HRESULT Wolf01::init(baseObject* _map, player* character, D3DXVECTOR3 randPos, camera* pCamera, string filePath, string mtrlPath, D3DXMATRIXA16* matCorrection)
{
	_WolfPos = new transform;
	_scene = _map;
	temp = randomFloatRange(1.0f, 3.0f);

	_Check = false;
	_CheckPos = NULL;
	_attackChk = false;
	_isDown = false;
	_isStiffen = false;
	_isWindmill = false;
	_Die = false;
	_isFollow = false;
	_counter = false;
	_ATK = false;
	_die = false;
	_appreciate = false;
	_distanceOverCheck = false;
	_WRldi = false;
	_temp = false;

	rotateAngle = 0;

	_wolf_hp = 150;
	_wolf_maxhp = 150;

	_wolf_gold = randomIntRange(200, 500);
	_wolf_attack = 20;

	this->setMesh(RM_SKINNED->addResource(filePath, mtrlPath, matCorrection));
	this->setActive(TRUE);

	_playerPos = character;
	_secondTime = 0;
	endTime = TIMEMANAGER->getWorldTime();

	_randNum = NULL;
	_randomAngle = D3DXVECTOR3(0, 0, 0);

	_transform->setWorldPosition(randPos);
	_SpawnPos = _transform->getWorldPosition();
	_WolfPos->setWorldPosition(_transform->getWorldPosition());

	//카메라에 렌더위해
	_mainCamera = pCamera;
	_Wolfstate = W_IDLE;
	//구조 바꿔보자
	_attackCount = 1;
	_battle = false;
	_incountPlayer = false;
	_ing = false;
	_distanceResult = 0;
	//바운드박스새로
	_WolfBox = new boundBox;
	_WolfBox->setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.2f, 0.2f, 0.2f));

	//체력바
	_hpBar = new billboardProgress(_WolfPos, &_wolf_maxhp, &_wolf_hp, 0, 0, 0xffc0c000, 0xffff0000, 0.55f);

	_curSkill[0] = NULL;
	_curSkill[1] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Smash.png");
	_curSkill[2] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Defence.png");
	_curSkill[3] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Counter.png");
	_curSkill[4] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Windmill.png");

	return S_OK;
}

void Wolf01::release(void)
{
}

void Wolf01::update(void)
{
	baseObject::update();
	baseObject::_transform->setWorldPosition(_transform->getWorldPosition());
	
	_SpawnPos.y = _WolfPos->getWorldPosition().y;

	animaiton();

	//스폰위치에서 떨어지면 돌아가는 함수
	SpawnPosBack();

	if (_skinnedAnim->getFactor() > 0.85f)
	{
		if (_wolf_hp <= 0)
		{
			_Wolfstate = W_GROGGY;
		}
	}

	if (_wolf_hp <= 0)
	{
		_wolf_hp = 0;
	}

	if (!_appreciate && !_distanceOverCheck)
	{
		noPlayer();
	}

	if (_SpawnDirResult > 10)	//거리가 멀어지면
	{
		moveStop();
		_Check = true;
	}

	if (_Check)		//스폰위치와 가까워질때까지 달리기 함수 실행 ->돌아갈땐 공격 x
	{
		SpawnPosBack();
		if (_SpawnDirResult <= 1.0f)
		{
			moveStop();
			_Check = false;
		}
	}   //스폰위치와 거리가 멀어진다면

	else
	{
		pattern();
	}

	//if (!_appreciate && !_distanceOverCheck)
	//{
	//	noPlayer();
	//}

	//if (!_die)
	//{
	//	if (_appreciate && !_distanceOverCheck) //만약에 맞으면 패턴으로 들어간다 인식을 트루를 endure함수에서 실행시킴 
	//	{
	//		// 맞으면서는 패턴을 실행시켜선 안된다.
	//		if (!_ing)
	//		{
	//			pattern();
	//		}
	//	}

	//	// 3타로 공격하게 제한을 걸었음
	//	if (_Wolfstate == W_ATK_ATTACK)
	//	{
	//		this->hit();
	//	}
	//}

}

void Wolf01::render(void)
{
	baseObject::render();

	//_WolfPos->renderGizmo();
	//_transform->renderGizmo();
	//_WolfBox->renderGizmo(_WolfPos);

	// 스킬뭐쓰는지 랜더
	if (_temp)
	{
		_hpBar->render(_mainCamera);	// 피게이지바는 전투 모드일떄 보여야 한다
	}

	// 스킬뭐쓰는지 랜더
	_hpBar->skillRender(_mainCamera, _tex);
}

void Wolf01::pattern()
{

	//플레이어 죽으면 인식풀자
	if (_playerPos->getPlayerDie())
	{
		_Wolfstate = W_IDLE;
		_appreciate = false;
		_changeAttack = false;
		_ing = false;
	}

	//가까워지면 전투
	if (_distanceResult < 1.0f)
	{
		//플레이어한테 붙음
		this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x, _transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));
		
		if (!_ing) _secondTime = TIMEMANAGER->getWorldTime();

		if (_Wolfstate == W_ATK_ATTACK && !_ing)
		{
			_ing = true;
			this->hit();
		}
	}

	//플레이어 범위 밖으로 나가면 인식하지 않게
	if (_distanceResult > 5.0f && !_changeAttack)
	{
		_Wolfstate = W_IDLE;
		_appreciate = false;
		_changeAttack = false;
		_ing = false;
		_temp = false;
	}
}

void Wolf01::noPlayer()
{
	//Animation 진행 Factor ( 0 이면 처음 1 일수록 마지막 )

	// 애니메이션 끝에 도달하면 새롭게 랜덤값으로 돌린다
	if (_skinnedAnim->getFactor() < 0.85f)
	{
		_Check = false;
	}
	else
	{
		_Check = true;
	}

	if (_Check && _Wolfstate == W_IDLE)
	{
		if (TIMEMANAGER->getWorldTime() - _secondTime >= randomFloatRange(2,5))
		{
			_secondTime = TIMEMANAGER->getWorldTime();
			rotateAngle = randomIntRange(0, 360);
			_Wolfstate = (MONSTER_MOTION)randomIntRange(1, 2);
		}
	}

	if (_Wolfstate == W_WALK)
	{
		_WolfPos->movePositionSelf(0.0f, 0.0f, 0.0260f);		//이동속도대로 이동

		if (_Check)
		{
			D3DXMATRIXA16 matRotate;
			D3DXMatrixRotationY(&matRotate, D3DXToRadian(rotateAngle));
			_transform->setRotateWorld(matRotate);
			_WolfPos->lookDirection(_transform->getForward());
			_transform->rotateWorld(0, D3DXToRadian(-10), 0);
		}

		if (_skinnedAnim->getFactor() >= 0.98)
		{
			_skinnedAnim->Play(W_IDLE);
			_transform->setWorldPosition(_WolfPos->getWorldPosition());
			_Wolfstate = W_IDLE;
		}
	}
}


void Wolf01::animaiton()
{

	Follow();

	_playerPos->getPlayerPosition();

	_distance = _playerPos->getPlayerPosition() - _WolfPos->getWorldPosition();
	_distanceResult = D3DXVec3Length(&_distance);

	switch (_Wolfstate)
	{
		case W_NULL:
		{
			_skinnedAnim->SetPlaySpeed(1.0f);
			_skinnedAnim->Play("ATKIDLE");

			// 랜덤을 몇초있다가 해줄거임 
			if (TIMEMANAGER->getWorldTime() - _secondTime >= 5.0f)
			{
				_secondTime = TIMEMANAGER->getWorldTime();
				_ing = false;
				_changeAttack = false;
				_Wolfstate = (MONSTER_MOTION)randomIntRange(4, 8);
				pattern();
			}
		}
		break;
		case W_IDLE:
		{
			eMonsterState = 0;
			_skinnedAnim->Play("IDLE");
		}
		break;
		case W_RUN:
		{
			_skinnedAnim->Play("RUN");
		}
		break;
		case W_HIT_A:
		{
			_skinnedAnim->Play("HIT_A");

			if (_skinnedAnim->getFactor() >= 0.90f)
			{
				_skinnedAnim->Play("GUARD");
				_Wolfstate = W_WATTING;
				_ing = false;
				_Wolfstate = (MONSTER_MOTION)randomIntRange(3,7);
				_secondTime = TIMEMANAGER->getWorldTime();
				pattern();
			}
		}
		break;
		case W_HIT_B:
		{
			_skinnedAnim->Play("HIT_B");

			if (_skinnedAnim->getFactor() >= 0.90f)
			{
				_skinnedAnim->Play("GUARD");
				_Wolfstate = W_WATTING;
				_ing = false;
				_Wolfstate = (MONSTER_MOTION)randomIntRange(4, 8);
				_secondTime = TIMEMANAGER->getWorldTime();
				pattern();
			}
		}
		break;
		case W_HIT_SMASH:
		{
			if (_playerPos->getMesh()->getAniFactor() < 0.3f)
			{
				this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x,
					_transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));

				_WolfPos->lookDirection(_transform->getForward());

				_skinnedAnim->Play("BODY_SMASH");
				_skinnedAnim->Stop();
			}

			else if (_playerPos->getMesh()->getAniFactor() > 0.3f)
			{
				_skinnedAnim->Play("BODY_SMASH");
				this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x,
					_transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));

				_WolfPos->lookDirection(_transform->getForward());

				_transform->setWorldPosition(_WolfPos->getWorldPosition());

				_WolfPos->movePositionWorld(_WolfPos->getForward() * -0.015f);		//이동속도대로 이동

			}

			if (_skinnedAnim->getFactor() >= 0.80f)
			{
				_secondTime = TIMEMANAGER->getWorldTime();
				_Wolfstate = W_DEATH;
			}
		}
		break;
		case W_HIT_WINDMILL:
		{
			if (_playerPos->getMesh()->getAniFactor() < 0.3f)
			{
				this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x,
					_transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));

				_WolfPos->lookDirection(_transform->getForward());

				_skinnedAnim->Play("BODY_WINDMILL");
				_skinnedAnim->Stop();
			}

			else if (_playerPos->getMesh()->getAniFactor() > 0.3f)
			{
				_skinnedAnim->Play("BODY_WINDMILL");
				this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x,
					_transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));

				_WolfPos->lookDirection(_transform->getForward());

				_transform->setWorldPosition(_WolfPos->getWorldPosition());

				_WolfPos->movePositionWorld(_WolfPos->getForward() * -0.015f);		//이동속도대로 이동

			}

			if (_skinnedAnim->getFactor() >= 0.80f)
			{
				_secondTime = TIMEMANAGER->getWorldTime();
				_Wolfstate = W_DEATH;
			}
		}
		break;
		case W_HIT_ARROW:
			_skinnedAnim->Play("BODY_ARROW");
			break;
		case W_DEATH:
		{
			_skinnedAnim->Play("DEATH");

			if (_skinnedAnim->getFactor() >= 0.30f)
			{
				if (_wolf_hp > 0)
				{
					_skinnedAnim->Stop();
					_die = false;
					_ing = false;

					if (TIMEMANAGER->getWorldTime() - _secondTime > 3.0f)
					{
						_secondTime = TIMEMANAGER->getWorldTime();
						_Wolfstate = W_DIE_TO_UP;
					}
				}
			}
			if (_wolf_hp <= 0)
			{
				_wolf_hp = 0;
				if (!_die)
				{
					_playerPos->playerLvUp();
				}
				_die = true;
			}
		}
		break;
		case W_ATK_IDLE:
		{
			eMonsterState = 0;
			_skinnedAnim->Play("ATKIDLE");
		}
		break;
		case W_ATK_ATTACK:
		{
			_skinnedAnim->Play("BODY_ATTACK");
			_secondTime = 0;
		}
		break;
		case W_ATK_COUNTER:
		{
			_skinnedAnim->Play("COUNTER");
			eMonsterState = 4;

			//붙자
			this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x,
				_transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));

			_changeAttack = true;
			_tex = _curSkill[3];

			if (TIMEMANAGER->getWorldTime() - _secondTime >= 7)
			{
				_secondTime = TIMEMANAGER->getWorldTime();
				_ing = false;
				_changeAttack = false;
				_Wolfstate = (MONSTER_MOTION)randomIntRange(4, 8);
				pattern();
			}

		}
		break;
		case W_ATK_SMASH:
		{
			if(_distanceResult <=1.0f)

			eMonsterState = 6;
			_tex = _curSkill[1];
			_ing = true;

			if (_distanceResult >= 1.1f && !_motionstop)
			{
				_skinnedAnim->Play("RUN");
				movePos(0.43f);
				_WolfPos->lookDirection(_transform->getForward());
			}

			else
			{
				_motionstop = true;
			}

			if (_playerPos->getPlayerSkillCount() != 3)
			{
				if (_motionstop)
				{
					_skinnedAnim->SetPlaySpeed(3.0f);
					_skinnedAnim->Play("SMASH");
					_transform->setWorldPosition(_WolfPos->getWorldPosition());

					if (!_WRldi && _skinnedAnim->getFactor() >= 0.40f)
					{
						_playerPos->playerHit(6, _wolf_attack);
						_WRldi = true;
					}
					if (_skinnedAnim->getFactor() >= 0.75f)
					{
						_Wolfstate = W_IDLE;
						_changeAttack = false;
						_ing = false;
						_motionstop = false;
						_attackCount = 1;
						_Wolfstate = (MONSTER_MOTION)randomIntRange(4, 8);
						SOUNDMANAGER->play("pWolfSmash", 0.3f);
					}
				}
			}
			//카운터라면
			if (_playerPos->getPlayerSkillCount() == 3)
			{
				_skinnedAnim->Play("SMASH");
				_transform->setWorldPosition(_WolfPos->getWorldPosition());

				if (_skinnedAnim->getFactor() >= 0.70f)
				{
					_playerPos->playerHit(6, _wolf_attack);
					_Wolfstate = W_FLY;
				}
			}
		}
		break;
		case W_GUARD:
		{
			_skinnedAnim->SetPlaySpeed(2.0f);
			_skinnedAnim->Play("GUARD");
			eMonsterState = 5;

			// 가까이 붙을떄까지는 팔로우로 좀 붙이고
			this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x,
				_transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));

			// 안치면 그냥 있을거고 치면 endureA에서 처리할거임 
			_changeAttack = true;
			_ing = true;

			if (TIMEMANAGER->getWorldTime() - _secondTime >= 7.0f)
			{
				_secondTime = TIMEMANAGER->getWorldTime();
				_ing = false;
				_changeAttack = false;
				_Wolfstate = (MONSTER_MOTION)randomIntRange(4 ,8);
				pattern();
			}

			_tex = _curSkill[2];
		}
		break;
		case W_WALK:
		{
			_skinnedAnim->Play("WALK");
		}
		break;
		case W_FOLLOW:
		{
			_skinnedAnim->Play("WALK");

			if (!_distanceOverCheck)
			{
				this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x,
					_transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));
			}

			_WolfPos->lookDirection(_transform->getForward());
			movePos(0.260f);	//이동속도대로 이동

			if (_distanceResult < 1.0f)
			{
				_skinnedAnim->Play(W_IDLE);
				_transform->setWorldPosition(_WolfPos->getWorldPosition());
				_changeAttack = true;
				_ing = false;

				if (_changeAttack && !_ing)
				{
					_Wolfstate = (MONSTER_MOTION)randomIntRange(4, 8);
					pattern();
				}
			}

			//가까워지면
			if (_distanceResult < 0.5f && _changeAttack && !_ing)
			{
				_changeAttack = false;
			}

		}
		break;
		case W_WATTING:
		{
			_skinnedAnim->Play("ATKIDLE");

			if (TIMEMANAGER->getWorldTime() - _secondTime >= 5.0f)
			{
				_secondTime = TIMEMANAGER->getWorldTime();
				_ing = false;
				_changeAttack = false;
				_Wolfstate = (MONSTER_MOTION)randomIntRange(4, 8);
				pattern();
			}
		}
		break;
		case W_DIE_TO_UP:
		{
			_skinnedAnim->Play("IDLE");

			if (_skinnedAnim->getFactor() > 0.90f)
			{
				//if문으로 윈드밀로 맞았는지 스매시에 맞았는지 구분하자
				_skinnedAnim->Play("BODY_SMASH");
				_Wolfstate = W_NULL;

				_Wolfstate = (MONSTER_MOTION)randomIntRange(4, 8);
				_secondTime = TIMEMANAGER->getWorldTime();
				pattern();
			}
		}
		break;
		case W_ATK_COUNTERATTACK:
		{
			_skinnedAnim->Play("COUNTER");
			_ing = true;

			if (!_distanceOverCheck)
			{
				this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x,
					_transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));
			}

			// 끝났으니 다른거 모션 돌릴거임 
			if (_skinnedAnim->getFactor() >= 0.80f)
			{
				// 상태를 잠깐 디펜스 모션만 보여주고 
				_Wolfstate = W_NULL;
				_secondTime = TIMEMANAGER->getWorldTime();
			}
		}
		break;
		case W_FLY:
		{
			this->_transform->lookPosition(D3DXVECTOR3(_playerPos->getPlayerPosition().x,
				_transform->getWorldPosition().y, _playerPos->getPlayerPosition().z));

			_WolfPos->lookDirection(_transform->getForward());

			_transform->setWorldPosition(_WolfPos->getWorldPosition());

			_skinnedAnim->Play("BODY_SMASH");

			_WolfPos->movePositionWorld(_WolfPos->getForward() * -0.015f);		//이동속도대로 이동

			if (_skinnedAnim->getFactor() > 0.94f)
			{
				_skinnedAnim->Play("HIT_B");
				_Wolfstate = W_DEATH;
			}
		}
		break;
		case W_GROGGY:
		{
			_skinnedAnim->Play("DEATH");

			if (_skinnedAnim->getFactor() >= 0.30f)
			{
				_skinnedAnim->Stop();
			}
		}
			break;
		default:
			break;
	}

	if (_Wolfstate != W_ATK_SMASH && _Wolfstate != W_GUARD && _Wolfstate != W_ATK_COUNTER)
	{
		_tex = _curSkill[0];

	}

	if (_Wolfstate == W_ATK_SMASH && _Wolfstate == W_GUARD && _Wolfstate == W_ATK_COUNTER)
	{
		_secondTime = TIMEMANAGER->getWorldTime();
	}
}

void Wolf01::hit()
{
	_changeAttack = true;

	// 디펜스나 카운터가 아닐떄 하셈 
	if (_playerPos->getPlayerSkillCount() != 2 && _playerPos->getPlayerSkillCount() != 3)
	{
		if (_skinnedAnim->getFactor() > 0.23f && _skinnedAnim->getFactor() < 0.24f)
		{
			if (_attackCount == 1)
			{
				eMonsterState = 1;	// 플레이어한테 전달해줘야하는값
				_playerPos->playerHit(_attackCount, _wolf_attack);
				SOUNDMANAGER->play("pWolfAttack", 0.3f);
			}

			if (_attackCount == 2.0f)
			{
				eMonsterState = 2;
				_playerPos->playerHit(_attackCount, _wolf_attack);
				SOUNDMANAGER->play("pWolfAttack", 0.3f);
			}

			if (_attackCount >= 3.0f)
			{
				eMonsterState = 3;
				_playerPos->playerHit(_attackCount, _wolf_attack);
				SOUNDMANAGER->play("pWolfAttack", 0.3f);
			}
		}
	}

	if (_playerPos->getPlayerSkillCount() == 2)
	{
		if (_attackCount < 3.0f)
		{
			_Wolfstate = W_WATTING;
		}
	}

	if (_playerPos->getPlayerSkillCount() == 3)
	{
		if (_attackCount < 3.0f)
		{
			if (_skinnedAnim->getFactor() > 0.50f)
			{
				_Wolfstate = W_FLY;
				_playerPos->playerHit(_attackCount, 0);
			}
		}
	}

	if (_skinnedAnim->getFactor() >= 0.99f && _skinnedAnim->getFactor() <= 0.998f)
	{
		_attackCount++;
	}

	if (_attackCount > 3.0f)
	{
		_Wolfstate = (MONSTER_MOTION)randomIntRange(4, 8);
		_changeAttack = false;
		_ing = false;
	}
}

void Wolf01::endureA(int playerstate, int playerAtk)
{
	_temp = true;
	_appreciate = true;
	_attackCount = 1;

	// 경우의 수 // 
	// 윈드밀 , 스매시 , 어택 , 
	// 1) 플레이어가 평상시일떄 
	if (_Wolfstate == W_IDLE)
	{
		// 평타로 첫타를 쳤을떄 + 윈드밀로 칠떄
		if (playerstate == 0 && playerAtk == 0)
		{
			_Wolfstate = W_HIT_A;
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
			_Wolfstate = W_FLY;
		}
	}

	//공격대기일때
	if (_Wolfstate == W_ATK_IDLE)
	{
		// 평타로 첫타를 쳤을떄 + 윈드밀로 칠떄
		if (playerstate == 0 && playerAtk == 0)
		{
			_Wolfstate = W_HIT_A;
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
			_Wolfstate = W_FLY;
		}
	}

	// 1-1 ) 런일떄 
	if (_Wolfstate == W_RUN)
	{
		// 평타로 첫타를 쳤을떄 + 윈드밀로 칠떄
		if (playerstate == 0 && playerAtk == 0)
		{
			_Wolfstate = W_HIT_A;
		}

		if (playerstate == 4 && playerAtk == 0)
		{
			_Wolfstate = W_FLY;
		}

		// 평타로 첫타를 쳤을떄 + 윈드밀로 칠떄
		if (playerstate == 1 && playerAtk == 0)
		{
			Smash_endure();
		}
	}

	if (_Wolfstate == W_HIT_A)
	{
		// 2번째 타
		if (playerstate == 0 && playerAtk == 1)
		{
			_Wolfstate = W_HIT_B;
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
			_Wolfstate = W_FLY;
		}
	}

	if (_Wolfstate == W_HIT_B)
	{
		if (playerstate == 0 && playerAtk == 2 || playerstate == 1 && playerAtk == 0 || playerstate == 4 && playerAtk == 0)
		{
			_Wolfstate = W_FLY;
			endureC();
		}
	}


	// 2) 내가 디펜스 상태이고 쟤들이 평타를 칠때 + 윈드밀 칠떄
	if (_Wolfstate == W_GUARD && playerstate == 0 && playerAtk == 0 ||
		_Wolfstate == W_GUARD && playerstate == 4 && playerAtk == 0)
	{
		_attackCount = 1;
		_skinnedAnim->Play("IDLE");
		_Wolfstate = W_ATK_ATTACK; // 평타로 반격할것임 
	}

	// 2 ) 내가 디펜스 상태이고, 쟤들이 스매시를 칠경우 
	if (_Wolfstate == W_GUARD && playerstate == 1 && playerAtk == 0) // 맨마지막 숫자는 뭐가 들어가야해 ? 
	{
		Smash_endure();
	}

	// 3)  내가 카운터 쓸떄 플레이어가 어택 일떄 + 스매시
	if (_Wolfstate == W_ATK_COUNTER && playerstate == 0 && playerAtk == 0 ||
		_Wolfstate == W_ATK_COUNTER && playerstate == 1 && playerAtk == 0)
	{
		// 날쳤으니 이새끼를 조져야함  카운터 모션 재생하고 
		_Wolfstate = W_ATK_COUNTER;
		_playerPos->playerHit(4, _wolf_attack);
	}

	// 4) 내가 카운터 쓸떄 플레이어가 윈드밀 칠떄
	if (_Wolfstate == W_ATK_COUNTER && playerstate == 4 && playerAtk == 0)
	{
		_Wolfstate = W_FLY;
	}

	// 5) 내가 스매시 일떄 플레이어가 평타를 칠떄 
	if (_Wolfstate == W_ATK_SMASH && playerstate == 0 && playerAtk == 0)
	{
		// 날쳤으니 이새끼를 조져야함  카운터 모션 재생하고 
		_Wolfstate = W_HIT_A;
	}

	if (_Wolfstate == W_ATK_SMASH && playerstate == 4 && playerAtk == 0)
	{
		// 날쳤으니 이새끼를 조져야함  카운터 모션 재생하고 
		_Wolfstate = W_HIT_A;
	}

	// 6) 몬스터가 웨이팅 상태일떄 플레이어가 첫타를 쳤을떄 
	if (_Wolfstate == W_WATTING)
	{
		if (playerstate == 0 && playerAtk == 0)
		{
			_Wolfstate = W_HIT_A;
		}

		if (playerstate == 4 && playerAtk == 0)
		{
			_Wolfstate = W_FLY;
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

void Wolf01::endureB(int playerstate, int playerAtk)
{
	if (_skinnedAnim->getFactor() >= 0.80) this->pattern();

	if (playerstate == 0 && playerAtk == 2)
	{
		endureC();
	}
}

void Wolf01::endureC()
{
	_Wolfstate = W_FLY;

	// 끝나고 die -> 죽었냐 : die 그대로 / 살았냐 : die to up

	_secondTime = TIMEMANAGER->getWorldTime();
}

void Wolf01::Smash_endure()
{
	_Wolfstate = W_HIT_SMASH;
}

void Wolf01::movePos(float moveSpeed)
{
	_WolfPos->movePositionSelf(D3DXVECTOR3(0, 0, moveSpeed));
}

void Wolf01::moveStop()
{
	_skinnedAnim->Play("IDLE");
	_transform->setWorldPosition(_WolfPos->getWorldPosition());
	eMonsterState == 0;
}

void Wolf01::Follow()
{
	_heightRay.origin = _WolfPos->getWorldPosition() + D3DXVECTOR3(0, 2, 0);
	_heightRay.direction = D3DXVECTOR3(0, -1, 0);

	D3DXVECTOR3 heightRayHitPos;

	PHYSICSMANAGER->isRayHitStaticMeshObject(&_heightRay, _scene, &heightRayHitPos, NULL);

	this->WolfPosition(heightRayHitPos.y);
}

void Wolf01::WolfPosition(float Ypos)
{
	D3DXVECTOR3 _framePos;
	D3DXVECTOR3 _charPos;

	_framePos = _transform->getWorldPosition();
	_framePos.y = Ypos;
	_transform->setWorldPosition(_framePos);

	_charPos = _WolfPos->getWorldPosition();
	_charPos.y = Ypos;
	_WolfPos->setWorldPosition(_charPos);
}

void Wolf01::SpawnPosBack()
{
	_appreciate = false;
	_ing = true;

	_SpawnDir = _SpawnPos - _WolfPos->getWorldPosition(); //스폰위치와의 거리 체크
	_SpawnDirResult = D3DXVec3Length(&_SpawnDir);

	if (_SpawnDirResult > 10.0f && !_distanceOverCheck)
	{
		WolfCheck();
	}

	if (_distanceOverCheck)
	{
		_Wolfstate = W_RUN;
		_WolfPos->movePositionSelf(0.0,0.0,0.043f);

		if (_skinnedAnim->getFactor() >= 0.85f)
		{
			_skinnedAnim->Play("IDLE");
			_transform->setWorldPosition(_WolfPos->getWorldPosition());
		}
	}

	if (_SpawnDirResult < 1.1f && _distanceOverCheck)
	{
		_skinnedAnim->Play("IDLE");
		_transform->setWorldPosition(_WolfPos->getWorldPosition());
		_Wolfstate = W_IDLE;
		_distanceOverCheck = false;
		_appreciate = false;
		_ing = false;
		this->noPlayer();
	}
}

void Wolf01::WolfCheck()
{
	_skinnedAnim->Play("GUARD");
	_transform->setWorldPosition(_WolfPos->getWorldPosition());
	_WolfPos->lookPosition(_SpawnPos);
	_transform->lookDirection(_WolfPos->getForward());
	_transform->rotateWorld(0, D3DXToRadian(-10), 0);

	_distanceOverCheck = true;
}


