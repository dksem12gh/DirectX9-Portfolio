#pragma once
#include "singletonBase.h"

#define SET_OUTLINE_RS(x) OPTION->setOutlineRenderState(x)
#define GET_OUTLINE_RS	  OPTION->getOutlineRenderState()

#define SET_HPVIEW(x)		OPTION->setHpBarView(x)
#define GET_HPVIEW			OPTION->getHpBarView()

#define MAINCAMERA	OPTION->getCamera()

enum MABI_CURSOR
{
	defaultCursor = 0,
	spoidCursor
};

class optionManager :
	public singletonBase<optionManager>
{
private:
	bool				_outline;
	bool				_hpBarView;
	float				_lightPower;
	camera* _pCamera;	//메인 카메라를 저장할 camera 변수

	LPDIRECT3DTEXTURE9	_defaultCursor;
	LPDIRECT3DTEXTURE9  _spoidCursor;
	LPDIRECT3DTEXTURE9	_curCursor;
public:
	HRESULT init(camera* pCamera);

	void setOutlineRenderState(bool outline) { _outline = outline; };
	bool getOutlineRenderState(void) { return _outline; };

	void setHpBarView(bool view) { _hpBarView = view; }
	bool getHpBarView(void) { return _hpBarView; }

	void renderCursor();
	void renderCursor(POINT mousePos);
	void setCursor(MABI_CURSOR cursor);
	camera*	getCamera(void) { return _pCamera; }

	optionManager();
	~optionManager();
};

