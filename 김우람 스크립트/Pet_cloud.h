#pragma once
#include "meshInfo.h"

//비행 높이 설정용 매크로 추가
#define FLYHEIGHT 6.0f

enum cloudState
{
	NONE,
	IDLE,
	Follow
};

class Pet_cloud : public baseObject
{
private:
	cloudState _cloudState; // 구름의 상태표기 
	meshInfo* _char; // 구름은 플레이어를 알아야함 ㅇㅅㅇ 

	baseObject*	_scene;

	D3DXVECTOR3 _distance; // 구름과 플레이어의 거리
	float _distanceResult;

	Ray _heightRay; // 높이 판별용레이

	float _random, _random1; // 랜덤 보는방향! 
	float _speed;
	int _range;
	
	bool _riding;

	//06.18민병덕
	bool _flying;			//비행모드 구별
	float _flyHeight;		//비행높이용 변수

public:
	HRESULT init(meshInfo* character);
	void release(void);
	void update(void);
	void render(void);
	void state();
	void summon(); // 구름소환 함수임 ㅇㅅㅇ 
	void Unsummon(); // 구름소환 해제!
	void ride();
	void setYPosition(float YPosition);

	cloudState getState() { return _cloudState; }
	void setScene(baseObject* scene) { _scene = scene; }	// 맵셋팅					

	bool getRiding() { return _riding; }
	void setRiding(bool ride) { _riding = ride; }
	void setFly(bool fly) { _flying = fly; }

	Pet_cloud();
	~Pet_cloud();
};

