#pragma once
#include "gameNode.h"

#define OBJECTNUM 30

class todayTest : public gameNode
{
private:
	vector<light*>			_lights;		//����
	vector<baseObject*>		_renderObject;	//���� ��ġ�� ���� ������Ʈ
	vector<baseObject*>		_cullObject;	//�ø��� ������Ʈ

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	todayTest() {}
	~todayTest() {}
};

