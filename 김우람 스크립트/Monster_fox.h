#pragma once
#include "player.h"
#include "Monster.h"

enum Foxstate
{
	f_IDLE,
	f_run,
	f_attack, // 1타
	f_smash,
	f_defense,
	f_counter,
	f_wating,
	f_endureA,
	f_endureB,
	f_smashEndure,
	f_fly,
	f_die,
	f_dieToup,
	f_follow,
	f_CounterAttack,
	f_null
};

// die 모션 오류 

class Monster_fox : public Monster
{
private:
	baseObject*				_fox; // 플레이어 메쉬좌표
	transform*				_foxPosition; // 플레이어 실제좌표

	player*					_char;
	Foxstate				_foxstate;

	D3DXVECTOR3				_distance; // 여우와 플레이어의 거리
	float					 _distanceResult;

	Ray						_heightRay; // 높이 판별용레이
	baseObject*				_scene;

	D3DXVECTOR3				_SpawnPos; // 몹 이동 최대 좌표
	D3DXVECTOR3				_SpawnDir; // 몹 최대 이공 거리값

	boundBox*				_FoxBox; //  폭스 바운드 박스
	camera*					_mainCamera; // 카메라 ㅇㅅㅇ
	billboardProgress*		_hpBar; // 폭스 피바

	LPDIRECT3DTEXTURE9		_curSkill[4]; // 스킬안쓰고있으면 NULL
	LPDIRECT3DTEXTURE9		_tex;

private:
	int _fox_hp;
	int _fox_maxHp;

	int _fox_gold;
	int _fox_atk;
	float _fox_atkCount;

	float _secondTime;	// 몇초마다 실행시키기 위해 쓰는 변수 
	int _randNum;
	int rotateAngle;

	int temp1, temp2;

	int _count;

	float _SpawnDirResult; // 거리 결과값pat

	bool _Check; // 플레이어 없을떄 상태값 변환시킬려고
	bool _changeAttack; // 공격시작할때 패턴 랜덤으로 돌릴려고 + 모션이 끝나야 돌림 
	bool _die; // 몬스터 피가 많을경우 die_to_up실행용 
	bool _atkcheck; // 3타연속공격을 하기위해서 
	bool _appreciate; // 플레이어한테 공격을 받기시작했나 ? 

	bool _rotate;
	bool _distanceOverCheck;
	bool _ing;

	bool _motionstop;

	bool dpfkdlrotRldi;

public:
	HRESULT init(player* character, baseObject* _map, D3DXVECTOR3 randPos, camera* pCamera);
	void release(void);
	void update(void);
	void render(void);
	void state(void);
	void noPlayer();
	void pattern(void);

	void setYPosition(float YPosition);
	void distanceOver();
	void hit();
	void returnback();

	void endureA(int playerstate, int playerAtk = 0);
	void endureB(int playerstate, int playerAtk = 0);
	void endureC();
	void Smash_endure();

	//hp -
	void setMonsterHp(int atk) { _fox_hp -= atk; }

	void setstate(Foxstate state) { state = _foxstate; }
	Foxstate getState() { return getState(); }

	int getfoxHp() { return _fox_hp; }
	void setfoxHp(int hp) { _fox_hp = hp; }

	int getfoxMaxHp() { return _fox_maxHp; }
	void setfoxMaxHp(int maxhp) { _fox_maxHp = maxhp; }

	int getfoxAtk() { return _fox_atk; }
	void setfoxAtk(int atk) { _fox_atk = atk; }

	boundBox* monsterBoundBox() { return _FoxBox; }		//몬스터 실시간 바운드박스 리턴
	transform* monsterTr() { return _foxPosition; }	//몬스터 실시간 좌표값 리턴

	Monster_fox();
	~Monster_fox();
};

