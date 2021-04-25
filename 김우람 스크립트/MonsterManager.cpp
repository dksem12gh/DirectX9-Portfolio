#include "stdafx.h"
#include "MonsterManager.h"

HRESULT MonsterManager::init(player* player, baseObject* mapSet, camera* pCamera) //06.15 인자값 베이스 오브젝트 추가 (몹들이 언덕을 올라가기 위해서)
{
	inBattleMonster = new Monster;
	inBattleMonster = NULL;

	initForRandomSeed();

	_mainCamera = pCamera;
	this->WolfSpawn(mapSet, player, 7, 11, 11); // 플레이어 메쉬인포,몇마리스폰할지,x최소좌표,z최대좌표(ex)-1,1 이면 2차원 그래프에서 1크기의 사각형 영역에 랜덤배치)
	this->FoxSpawn(mapSet, player, 10, 50.5f, 50.5f);
	this->GolemSpawn(mapSet, player, 1, -4.5, 5); // 플레이어 메쉬인포,몇마리스폰할지,x최소좌표,z최대좌표(ex)-1,1 이면 2차원 그래프에서 1크기의 사각형 영역에 랜덤배치)

	return S_OK;

}

void MonsterManager::release(void)
{

	/*for (_miAppearChar = _mAppearChar.begin(); _miAppearChar != _mAppearChar.end(); _miAppearChar++)*/

	//_miAppearChar = _mAppearChar.begin();

	//for (; _miAppearChar != _mAppearChar.end();)
	//{
	//	_miAppearChar->second.release();
	//	_miAppearChar = _mAppearChar.erase(_miAppearChar);
	//}


	//for (_Monster_Imap = _Monster_map.begin(); _Monster_Imap != _Monster_map.end();_Monster_Imap++)
	//{
	//	SAFE_DELETE(_monster[i]);
	//}

	for (int i = 0; i < _vMonster.size(); i++)
	{
		SAFE_DELETE(_vMonster[i]);
	}
}

void MonsterManager::update(void)
{
	if (inBattleMonster == NULL)
	{
		for (int i = 0; i < _vMonster.size(); i++)
		{
			Wolf01* pWolf = dynamic_cast<Wolf01*>(_vMonster[i]);

			if (pWolf == NULL) continue;

			if (pWolf->getWolfDis() <= 1.0)
			{
				inBattleMonster = dynamic_cast<Monster*>(pWolf);
				inBattleMonster->setWolfCheck(2);
			}
		}
	}
	else
	{
		for (int i = 0; i < _vMonster.size(); i++)
		{
			Wolf01* pWolf = dynamic_cast<Wolf01*>(_vMonster[i]);

			if (pWolf == NULL || pWolf == inBattleMonster) continue;

			if (pWolf->getWolfDis() <= 1.0)
			{
				pWolf->setWolfCheck(1);
			}
		}

		Wolf01* pWolf = dynamic_cast<Wolf01*>(inBattleMonster);

		if (pWolf->getWolfDis() > 1.0f)
		{
			pWolf->setWolfCheck(0);
			inBattleMonster = NULL;
		}
	}
	for (int i = 0; i < _vMonster.size(); i++)
	{
		_vMonster[i]->update();
	}
}

void MonsterManager::render(void)
{
	for (int i = 0; i < _vMonster.size(); i++)
	{
		_vMonster[i]->render();
	}
}

Monster* MonsterManager::MonsterRayHit(LPRay ray)
{
	for (int i = 0; i < _vMonster.size(); i++)
	{
		if (PHYSICSMANAGER->isRayHitBound(ray, _vMonster[i]->monsterBoundBox(), _vMonster[i]->_transform, NULL, NULL))
		{
			return _vMonster[i];
		}
	}

	return NULL;
}

void MonsterManager::WolfSpawn(baseObject* mapSet, player* player, int count, float minX, float minZ)
{
	_count = count;

	D3DXMATRIXA16 matScale, matRotateX, matRotateY, matCorrection;

	D3DXMatrixScaling(&matScale, 0.0001f, 0.0001f, 0.0001f);
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(-90));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(10));
	matCorrection = matRotateX * matRotateY * matScale;

	for (int i = 0; i < _count; i++)
	{
		_random = D3DXVECTOR3(MYUTIL::randomFloatRange(minX, minZ), 0, MYUTIL::randomFloatRange(minX, minZ));

		Wolf01* newWolf = new Wolf01;
		newWolf->init(mapSet, player, _random, _mainCamera, "Resources/mabinogi/Monster/WooMonster/wolf.x", "Resources/mabinogi/Monster/WooMonster/", &matCorrection);
		_vMonster.push_back(newWolf);
	}
}

void MonsterManager::FoxSpawn(baseObject* mapSet, player * player, int count, float minX, float minZ)
{
	_count = count;

	for (int i = 0; i < _count; i++)
	{
		_random = D3DXVECTOR3(MYUTIL::randomFloatRange(minX, minZ), 0, MYUTIL::randomFloatRange(minX, minZ));

		_fox = new Monster_fox;
		_fox->init(player, mapSet, _random, _mainCamera);
		_fox->_transform->setLocalPosition(_random);

		_vMonster.push_back(_fox);
	}
}

//void MonsterManager::OrgeSpawn(baseObject * mapSet, player * player, int count, float minX, float minZ)
//{
//	_count = count;	_wolfCheck = 0;
//
//	for (int i = 0; i < _count; i++)
//	{
//		_random = D3DXVECTOR3(MYUTIL::randomFloatRange(minX, minZ), 0, MYUTIL::randomFloatRange(minX, minZ));
//
//		_orge = new Orge;
//		_orge->init(player, mapSet, _random, _mainCamera);
//		_orge->_transform->setLocalPosition(_random);
//
//		_monster.push_back(_orge);
//	}
//}



void MonsterManager::GolemSpawn(baseObject * mapSet, player * player, int count, float minX, float minZ)
{
	_count = count;

	D3DXMATRIXA16 matScale, matRotateX, matRotateY, matCorrection;

	D3DXMatrixScaling(&matScale, 0.00005f, 0.00005f, 0.00005f);
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(-90));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(10));
	matCorrection = matRotateX * matRotateY * matScale;

	for (int i = 0; i < _count; i++)
	{
		_random = D3DXVECTOR3(MYUTIL::randomFloatRange(minX, minZ), 0, MYUTIL::randomFloatRange(minX, minZ));

		_Golem = new Golem;
		_Golem->init(mapSet, player, _random, _mainCamera, "Resources/mabinogi/Monster/WooMonster/Golem.x", "Resources/mabinogi/Monster/WooMonster/", &matCorrection);
		_Golem->_transform->setLocalPosition(_random);

		_vMonster.push_back(_Golem);
	}
}

