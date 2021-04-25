#pragma once
#include "player.h"
#include "Monster.h"

enum MONSTER_MOTION
{
	W_NULL,
	W_IDLE,
	W_WALK,
	W_RUN,

	W_ATK_ATTACK,
	W_ATK_SMASH,
	W_GUARD,
	W_ATK_COUNTER,
	W_ATK_IDLE,

	W_HIT_A,
	W_HIT_B,
	W_HIT_SMASH,
	W_HIT_WINDMILL,
	W_HIT_ARROW,

	W_GROGGY,
	W_DEATH,

	W_FOLLOW,
	W_FLY,
	W_WATTING,
	W_DIE_TO_UP,
	W_ATK_COUNTERATTACK
};

class Wolf01 :
	public Monster
{
public:
	bool _incountPlayer;

private:
	baseObject * _Wolf; // �÷��̾� �޽���ǥ
	baseObject *_scene; // �� �޾ƿ������� ���� 06.15 �߰�
	transform* _WolfPos; // �÷��̾� ������ǥ
	D3DXVECTOR3 _SpawnPos; // �� �̵� �ִ� ��ǥ

	boundBox* _WolfBox;

	player* _playerPos;
	MONSTER_MOTION _Wolfstate;

	D3DXVECTOR3 _distance;
	D3DXVECTOR3 _SpawnDir;
	D3DXVECTOR3 _randomAngle;

	Ray _heightRay; // ���� ����

	camera* _mainCamera;
	billboardProgress* _hpBar; // ����ü�¹�
	LPDIRECT3DTEXTURE9 _curSkill[4];//��ų�Ⱦ��� ��
	LPDIRECT3DTEXTURE9 _tex;

	float _distanceResult;
	float _SpawnDirResult;
	float temp;
	float endTime;

	int _wolf_hp;
	int _wolf_maxhp;
	int _wolf_gold;
	int _wolf_attack;
	float _secondTime;
	int _randNum;
	int rotateAngle;

	bool _Check;
	bool _CheckPos;
	bool _attackChk;


	bool _changeAttack;
	bool _WolfMove;

	//���� �ٲ㺸��
	int a;//����
	int _attackCount;
	int _skillCount;
	int _loockCount;

	bool _battle; // �ο���
	bool _ing; //~�ϴ���
	bool _attacking;//����?
	bool _isDown;//���ư���
	bool _isStiffen;//����
	bool _isWindmill;//����� �¾�����
	bool _Die; //�׾���
	bool _isFollow;//���밡 ���Ž��� ���ȳ�?
	bool _counter; //ī���� �����߳�?
	bool _isIdle; //���ݳѾ���� ���
	bool _isFlyDown;//���Ž� �ٿ�
	bool _isWindMillDown;//����дٿ�
	bool _appreciate; // �÷��̾����� ������ �ޱ�����߳� ? 
	bool _distanceOverCheck;
	bool _motionstop;
	bool _WRldi;

	bool _animation;
	bool _lookPlay;

	bool _ATK;
	bool _die;
	bool _dietrue;
	bool _temp;

public:
	HRESULT init(baseObject* _map, player* character, D3DXVECTOR3 randPos, camera* pCamera, string filePath, string mtrlPath = "", D3DXMATRIXA16* matCorrection = NULL);
	void release(void);
	void update(void);
	void render(void);
	void pattern();
	void noPlayer();

	void hit();
	void endureA(int playerstate, int playerAtk = 0);
	void endureB(int playerstate, int playerAtk = 0);
	void endureC();
	void Smash_endure();

	void movePos(float moveSpeed);
	void moveStop();
	void Follow();

	void WolfPosition(float Ypos);

	void SpawnPosBack();

	void WolfCheck();

	void animaiton();

	void setMonsterHp(int atk) { _wolf_hp -= atk; }

	boundBox* monsterBoundBox() { return _WolfBox; }		//���� �ǽð� �ٿ��ڽ� ����
	transform* monsterTr() { return _WolfPos; }	//���� �ǽð� ��ǥ�� ����

	void setstate(MONSTER_MOTION state) { state = _Wolfstate; }
	MONSTER_MOTION getState() { return getState(); }

	int getWolfHp() { return _wolf_hp; }
	void setWolfHp(int hp) { _wolf_hp = hp; }

	int getWolfMaxHp() { return _wolf_maxhp; }
	void setWolfMaxHp(int maxhp) { _wolf_maxhp = maxhp; }

	int getWolfAtk() { return _wolf_attack; }
	void setWolfAtk(int atk) { _wolf_attack = atk; }

	void setScene(baseObject* scene) { _scene = scene; }	// �ʼ���		

	float getWolfDis() { return _distanceResult; }


	/*bool getIncountPlayer(void) { return _incountPlayer; }
	void setIncountPlayer(bool value) { _incountPlayer = value; }*/

	Wolf01();
	~Wolf01();
};

