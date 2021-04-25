#pragma once
#include "player.h"
#include "Monster.h"

enum Foxstate
{
	f_IDLE,
	f_run,
	f_attack, // 1Ÿ
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

// die ��� ���� 

class Monster_fox : public Monster
{
private:
	baseObject*				_fox; // �÷��̾� �޽���ǥ
	transform*				_foxPosition; // �÷��̾� ������ǥ

	player*					_char;
	Foxstate				_foxstate;

	D3DXVECTOR3				_distance; // ����� �÷��̾��� �Ÿ�
	float					 _distanceResult;

	Ray						_heightRay; // ���� �Ǻ��뷹��
	baseObject*				_scene;

	D3DXVECTOR3				_SpawnPos; // �� �̵� �ִ� ��ǥ
	D3DXVECTOR3				_SpawnDir; // �� �ִ� �̰� �Ÿ���

	boundBox*				_FoxBox; //  ���� �ٿ�� �ڽ�
	camera*					_mainCamera; // ī�޶� ������
	billboardProgress*		_hpBar; // ���� �ǹ�

	LPDIRECT3DTEXTURE9		_curSkill[4]; // ��ų�Ⱦ��������� NULL
	LPDIRECT3DTEXTURE9		_tex;

private:
	int _fox_hp;
	int _fox_maxHp;

	int _fox_gold;
	int _fox_atk;
	float _fox_atkCount;

	float _secondTime;	// ���ʸ��� �����Ű�� ���� ���� ���� 
	int _randNum;
	int rotateAngle;

	int temp1, temp2;

	int _count;

	float _SpawnDirResult; // �Ÿ� �����pat

	bool _Check; // �÷��̾� ������ ���°� ��ȯ��ų����
	bool _changeAttack; // ���ݽ����Ҷ� ���� �������� �������� + ����� ������ ���� 
	bool _die; // ���� �ǰ� ������� die_to_up����� 
	bool _atkcheck; // 3Ÿ���Ӱ����� �ϱ����ؼ� 
	bool _appreciate; // �÷��̾����� ������ �ޱ�����߳� ? 

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

	boundBox* monsterBoundBox() { return _FoxBox; }		//���� �ǽð� �ٿ��ڽ� ����
	transform* monsterTr() { return _foxPosition; }	//���� �ǽð� ��ǥ�� ����

	Monster_fox();
	~Monster_fox();
};

