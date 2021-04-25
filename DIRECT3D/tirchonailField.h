#pragma once

#define OBJMAX 50

class tirchonailField
{
private:
	baseObject* _terrain;
	
	vector<baseObject*> _well;            //벽이 맵 전체 x파일에 포함 되어있음 벽 전용 충돌 박스 셋팅
	vector<baseObject*> _obj;			  //오브젝트
	D3DXVECTOR3 _objBoundBoxSize[OBJMAX]; //오브젝트 바운드 박스 크기 벡터
public:
	HRESULT init(string filePath, D3DXMATRIXA16* matCorrection = NULL);
	void release(void);
	void rander(void);
	void objInit(void);

	vector<baseObject*>* getObj(){ return &_obj; }
	vector<baseObject*>* getWellBound() { return &_well; }
	
	baseObject* getField() { return _terrain; }

	tirchonailField() {}
	~tirchonailField() {}
};

