#pragma once
#include "baseObject.h"
#include "dyeing.h"

/*
	염색파트별 메쉬 클래스
*/
class partItem :
	public baseObject
{
private:
	DWORD			_color;						// 염색될 컬러
public:
	// 들어온 컬러로 텍스처를 염색한다. DWORD, D3DXCOLOR 사용
	void dyeingTex(DWORD color);				
	void dyeingTex(D3DXCOLOR color);

	// 염색컬러를 반환한다.
	DWORD getColor(void) { return _color; }
	D3DXCOLOR getColorFormD3DXCOLOR(void) { return D3DXCOLOR(_color); }

	partItem() {};
	~partItem() {};
};

