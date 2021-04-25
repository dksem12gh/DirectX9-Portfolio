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
	baseObject * _Wolf; // 플레이어 메쉬좌표
	baseObject *_scene; // 맵 받아오기위한 변수 06.15 추가
	transform* _WolfPos; // 플레이어 실제좌표
	D3DXVECTOR3 _SpawnPos; // 몹 이동 최대 좌표

	boundBox* _WolfBox;

	player* _playerPos;
	MONSTER_MOTION _Wolfstate;

	D3DXVECTOR3 _distance;
	D3DXVECTOR3 _SpawnDir;
	D3DXVECTOR3 _randomAngle;

	Ray _heightRay; // 높이 레이

	camera* _mainCamera;
	billboardProgress* _hpBar; // 늑대체력바
	LPDIRECT3DTEXTURE9 _curSkill[4];//스킬안쓰면 널
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

	//구조 바꿔보자
	int a;//랜덤
	int _attackCount;
	int _skillCount;
	int _loockCount;

	bool _battle; // 싸움모드
	bool _ing; //~하는중
	bool _attacking;//맞음?
	bool _isDown;//날아가나
	bool _isStiffen;//경직
	bool _isWindmill;//윈드밀 맞았을때
	bool _Die; //죽었냐
	bool _isFollow;//늑대가 스매쉬로 날렸냐?
	bool _counter; //카운터 성공했냐?
	bool _isIdle; //공격넘어가기전 대기
	bool _isFlyDown;//스매쉬 다운
	bool _isWindMillDown;//윈드밀다운
	bool _appreciate; // 플레이어한테 공격을 받기시작했나 ? 
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

	boundBox* monsterBoundBox() { return _WolfBox; }		//몬스터 실시간 바운드박스 리턴
	transform* monsterTr() { return _WolfPos; }	//몬스터 실시간 좌표값 리턴

	void setstate(MONSTER_MOTION state) { state = _Wolfstate; }
	MONSTER_MOTION getState() { return getState(); }

	int getWolfHp() { return _wolf_hp; }
	void setWolfHp(int hp) { _wolf_hp = hp; }

	int getWolfMaxHp() { return _wolf_maxhp; }
	void setWolfMaxHp(int maxhp) { _wolf_maxhp = maxhp; }

	int getWolfAtk() { return _wolf_attack; }
	void setWolfAtk(int atk) { _wolf_attack = atk; }

	void setScene(baseObject* scene) { _scene = scene; }	// 맵셋팅		

	float getWolfDis() { return _distanceResult; }


	/*bool getIncountPlayer(void) { return _incountPlayer; }
	void setIncountPlayer(bool value) { _incountPlayer = value; }*/

	Wolf01();
	~Wolf01();
};

