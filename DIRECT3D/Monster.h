#pragma once
#include "baseObject.h"

enum MONSTERSTATE
{
	M_idle,
	M_attack1,  //평타
	M_attack2,  //평타 두번째
	M_attack3, // 평타 3번째
	M_counter,
	M_guard,
	M_smash,
	M_stompe,
	M_windmill
};

class Monster : public baseObject
{
protected:
	int eMonsterState;

public:
	Monster();
	~Monster();

	int _wolfCheck;  //몬스터 매니저에서의 늑대 상태값 체크 0:idle 1 : atk_idle 2:attack


	virtual void hit() {}; // 너희가 딱히 이걸 쓸일은 없을거임 
	virtual void endureA(int playerstate, int playerAtk = 0) {};  // 1타맞을떄 모션
	virtual void endureB() {};	// 2타 맞을떄 모션 
	virtual void endureC() {};  // 3타맞을떄 날아가는모션 || 카운터 맞을떄 (여우)
	virtual void Smash_endure() {}; // 스매시 쳐맞을떄 모션
	virtual void die() {};

	virtual boundBox* monsterBoundBox() { return NULL; }; //몬스터 실시간 바운드 박스 리턴함수
	virtual transform* monsterTr() { return NULL; };	//몬스터 실시간 좌표값 리턴함수

	//몬스터 피깍
	virtual void setMonsterHp(int atk) {};

	int getMonsterState() { return eMonsterState; }

	int getWolfCheck() { return _wolfCheck; }
	void setWolfCheck(int check) { _wolfCheck = check; }
};

