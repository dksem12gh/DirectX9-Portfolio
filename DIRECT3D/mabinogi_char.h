#pragma once
#include "equip_Sheet.h"

class mabinogi_char
{
private:
	baseObject* _body;
	equip_Sheet* _equip[5];

	typedef vector<LPDIRECT3DTEXTURE9>* pTexture;
private:
	pTexture _diffuseTexs;	// ���� �ؽ���
	pTexture _dyeingTexs;	//������ �ؽ���	
public:
	HRESULT init(string filePath, string mtrlPath, D3DXMATRIXA16* matCorrection = NULL);
	void release(void);
	void update(void);
	void render();
	void outlineRender(camera* pCamera);
	baseObject* getBody() { return _body; };

	mabinogi_char() :_body(NULL) 
	{
		for (int i = 0; i < 5; i++)
		{
			_equip[i] = NULL;
		}
	};

	~mabinogi_char() {};
};

