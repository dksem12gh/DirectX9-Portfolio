#pragma once
#include "gameNode.h"

#define OBJECTNUM 30

class todayTest : public gameNode
{
private:
	vector<light*>			_lights;		//광원
	vector<baseObject*>		_renderObject;	//씬에 배치된 렌더 오브젝트
	vector<baseObject*>		_cullObject;	//컬링된 오브젝트

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	todayTest() {}
	~todayTest() {}
};

