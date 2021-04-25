#pragma once
#include "meshInfo.h"
#include "Wolf01.h"
#include "Monster_fox.h"
#include "Golem.h"

//#include "Orge.h"

class MonsterManager
{
private:
	map<string, Monster*> _Monster_map;//ㅇㅋ 베이스 오브제트 몬스터로
	map<string, Monster*>::iterator _Monster_Imap;

	Wolf01* _wolf;
	Golem* _Golem;

	Monster_fox* _fox;
	//Orge* _orge;

	vector<Monster*> _vMonster;//몬스터
	vector<vector<Monster*>> _vVecMonster;

	D3DXVECTOR3 _random;
	int _count;

	transform*	_playerTransform;
	camera* _mainCamera;
public:
	Monster * inBattleMonster;

	HRESULT init(player* player, baseObject* mapSet, camera* pCamera);//06.15 베이스 오브젝트 추가 (몹들이 언덕을 올라가기 위해서)
	void release(void);
	void update(void);
	void render(void);

	Monster* MonsterRayHit(LPRay ray);

	void WolfSpawn(baseObject* mapSet, player* player, int count, float minX, float minZ);//06.15 베이스 오브젝트 추가 (몹들이 언덕을 올라가기 위해서)
	void FoxSpawn(baseObject* mapSet, player* player, int count, float minX, float minZ);
	void GolemSpawn(baseObject* mapSet, player* player, int count, float minX, float minZ);

	vector<Monster*> getMonsterVec() { return _vMonster; }

	MonsterManager() {};
	~MonsterManager() {};
};

