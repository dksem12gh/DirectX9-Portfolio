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
	// ���� ����
	baseObject* _Orge; // ���� �޽���ǥ
	transform* _OrgePosition; // ���� ������ǥ

	player* _char;

	D3DXVECTOR3 _distance; // ���ſ� �÷��̾��� �Ÿ�
	float _distanceResult;

	Ray _heightRay; // ���� �Ǻ��뷹��
	baseObject* _scene;

	D3DXVECTOR3				_SpawnPos; // �� �̵� �ִ� ��ǥ
	D3DXVECTOR3				_SpawnDir; // �� �ִ� �̰� �Ÿ���

	boundBox*				_OrgeBox; //  ���� �ٿ�� �ڽ�
	camera*					_mainCamera; // ī�޶� ������
	billboardProgress*		_hpBar; // �� �������� 

	Orge_state _O_state;

	LPDIRECT3DTEXTURE9		_curSkill[6]; // ��ų�Ⱦ��������� NULL
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

