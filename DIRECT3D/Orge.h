#pragma once
#include "Monster.h"
#include "player.h"

enum Orge_state
{
	O_idle,
	O_attack_idle,
	O_walk,
	O_run,
	O_attack,
	O_defense,
	O_counter,
	O_smash,
	O_windmill,
	O_stomp_wating,
	O_stomp,
	O_endureA,
	O_endureB,
	O_endure,
	O_fly,
	O_die,
	O_die_to_up
};

class Orge : public Monster
{
private :
	// 뼈대 정보
	baseObject* _Orge; // 오거 메쉬좌표
	transform* _OrgePosition; // 오거 실제좌표

	player* _char;

	D3DXVECTOR3 _distance; // 오거와 플레이어의 거리
	float _distanceResult;

	Ray _heightRay; // 높이 판별용레이
	baseObject* _scene;

	D3DXVECTOR3				_SpawnPos; // 몹 이동 최대 좌표
	D3DXVECTOR3				_SpawnDir; // 몹 최대 이공 거리값

	boundBox*				_OrgeBox; //  오거 바운드 박스
	camera*					_mainCamera; // 카메라 ㅇㅅㅇ
	billboardProgress*		_hpBar; // 피 게이지바 

	Orge_state _O_state;

	LPDIRECT3DTEXTURE9		_curSkill[6]; // 스킬안쓰고있으면 NULL
	LPDIRECT3DTEXTURE9		_tex;

private:
	int _Orge_hp;
	int _Orge_maxHp;

	int _Orge_gold;
	int _Orge_atk;
	int _Orge_atkCount;

public:
	HRESULT init(player* character, baseObject* _map, D3DXVECTOR3 randPos, camera* pCamera);
	void release(void);
	void update(void);
	void render(void);

	void state();

	void hit(); // 
	void endureA();
	void Smash_endure();
	Orge();
	~Orge();
};

