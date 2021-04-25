#pragma once
#include "baseObject.h"

enum MONSTERSTATE
{
	M_idle,
	M_attack1,  //��Ÿ
	M_attack2,  //��Ÿ �ι�°
	M_attack3, // ��Ÿ 3��°
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

	int _wolfCheck;  //���� �Ŵ��������� ���� ���°� üũ 0:idle 1 : atk_idle 2:attack


	virtual void hit() {}; // ���� ���� �̰� ������ �������� 
	virtual void endureA(int playerstate, int playerAtk = 0) {};  // 1Ÿ������ ���
	virtual void endureB() {};	// 2Ÿ ������ ��� 
	virtual void endureC() {};  // 3Ÿ������ ���ư��¸�� || ī���� ������ (����)
	virtual void Smash_endure() {}; // ���Ž� �ĸ����� ���
	virtual void die() {};

	virtual boundBox* monsterBoundBox() { return NULL; }; //���� �ǽð� �ٿ�� �ڽ� �����Լ�
	virtual transform* monsterTr() { return NULL; };	//���� �ǽð� ��ǥ�� �����Լ�

	//���� �Ǳ�
	virtual void setMonsterHp(int atk) {};

	int getMonsterState() { return eMonsterState; }

	int getWolfCheck() { return _wolfCheck; }
	void setWolfCheck(int check) { _wolfCheck = check; }
};

