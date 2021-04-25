#pragma once

#define OBJMAX 50

class tirchonailField
{
private:
	baseObject* _terrain;
	
	vector<baseObject*> _well;            //���� �� ��ü x���Ͽ� ���� �Ǿ����� �� ���� �浹 �ڽ� ����
	vector<baseObject*> _obj;			  //������Ʈ
	D3DXVECTOR3 _objBoundBoxSize[OBJMAX]; //������Ʈ �ٿ�� �ڽ� ũ�� ����
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

