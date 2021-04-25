#pragma once
#include "Monster.h"
#include "player.h"

class Golem :
	public Monster
{
private:
	baseObject* _scene;							//맵 받아와야지
	transform*  _GolemPos;						//골렘 실제 좌표
	player*		_playerPos;						//플레이어 받기
	boundBox*	_GolemBox;						
	Ray			_heightRay;
	camera*		_mainCamera;

	billboardProgress* _hpBar;

	D3DXVECTOR3 _SpawnPos;						//몹 이동 최대 좌표
	D3DXVECTOR3 _SpawnDir;
	D3DXVECTOR3 _randomAngle;
	D3DXVECTOR3 _distance;

	LPDIRECT3DCUBETEXTURE9 _curSkill[5];
	LPDIRECT3DCUBETEXTURE9 _tex;

	float _distanceResult;
	float _SpawnDirResult;

	int _golem_hp;
	int _golem_maxhp;
	int _golem_gold;
	int _golem_attack;
	float _secondTime;
	int _randNum;

public:
	HRESULT init(baseObject* _map, player* character, D3DXVECTOR3 randPos, camera* pCamera, string filePath, string mtrlPath = "", D3DXMATRIXA16* matCorrection = NULL);
	void release(void);
	void update(void);
	void render(void);

	void hit();
	void endureA(int playerstate, int playerAtk = 0);
	void endureB(int playerstate, int playerAtk = 0);
	void endureC();
	void Smash_endure();

	boundBox* monsterBoundBox() { return _GolemBox; }		//몬스터 실시간 바운드박스 리턴
	transform* monsterTr() { return _GolemPos; }	//몬스터 실시간 좌표값 리턴

	Golem();
	~Golem();
};

