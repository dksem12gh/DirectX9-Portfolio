#pragma once
#include "baseObject.h"
#include "dyeing.h"

/*
	������Ʈ�� �޽� Ŭ����
*/
class partItem :
	public baseObject
{
private:
	DWORD			_color;						// ������ �÷�
public:
	// ���� �÷��� �ؽ�ó�� �����Ѵ�. DWORD, D3DXCOLOR ���
	void dyeingTex(DWORD color);				
	void dyeingTex(D3DXCOLOR color);

	// �����÷��� ��ȯ�Ѵ�.
	DWORD getColor(void) { return _color; }
	D3DXCOLOR getColorFormD3DXCOLOR(void) { return D3DXCOLOR(_color); }

	partItem() {};
	~partItem() {};
};

