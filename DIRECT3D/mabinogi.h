#pragma once
#include "gameNode.h"
#include "player.h"
#include "tirchonailField.h"
#include "Pet_cloud.h"
#include "Monster_fox.h"
#include "MonsterManager.h"
#include "UI.h"

class mabinogi : public gameNode
{
private:
	vector<light*>				_lights;					//조명벡터
	tirchonailField*			_scene;						//지형
	player*						_player;					//플레이어
	UI*							_ui;						//UI
	Pet_cloud*					_petCloud;					//플레이어의 펫
	MonsterManager*			    _mob;						//몬스터

	Monster*					_clickMonster;				//타켓몬스터 변수

	vector<baseObject*>			_cullObjects;				//절두체컬링 할거임 ㅇㅅㅇ 

	Ray							_playerStop;				//플레이어가 스탑할려고 만드는 레이

	bool						_click;						//클릭이 되었니?

	dyeing*						_dyeingInterface;
public:
	HRESULT init(void);
	void lightInit(void);
	void release(void);
	void update(void);
	void render(void);

	//플레이어와 몬스터의 배틀
	void playerMonsterBattle();	
	void playerMonsterAtk();
	void playerMonsterHit();
	//플레이어가 맵과의 충돌시 멈추는 함수
	void playerMapstop();	

	mabinogi() {};
	~mabinogi() {};
};

