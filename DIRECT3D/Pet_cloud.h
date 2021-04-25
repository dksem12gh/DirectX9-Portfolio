#pragma once
#include "meshInfo.h"

//���� ���� ������ ��ũ�� �߰�
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
	cloudState _cloudState; // ������ ����ǥ�� 
	meshInfo* _char; // ������ �÷��̾ �˾ƾ��� ������ 

	baseObject*	_scene;

	D3DXVECTOR3 _distance; // ������ �÷��̾��� �Ÿ�
	float _distanceResult;

	Ray _heightRay; // ���� �Ǻ��뷹��

	float _random, _random1; // ���� ���¹���! 
	float _speed;
	int _range;
	
	bool _riding;

	//06.18�κ���
	bool _flying;			//������ ����
	float _flyHeight;		//������̿� ����

public:
	HRESULT init(meshInfo* character);
	void release(void);
	void update(void);
	void render(void);
	void state();
	void summon(); // ������ȯ �Լ��� ������ 
	void Unsummon(); // ������ȯ ����!
	void ride();
	void setYPosition(float YPosition);

	cloudState getState() { return _cloudState; }
	void setScene(baseObject* scene) { _scene = scene; }	// �ʼ���					

	bool getRiding() { return _riding; }
	void setRiding(bool ride) { _riding = ride; }
	void setFly(bool fly) { _flying = fly; }

	Pet_cloud();
	~Pet_cloud();
};

