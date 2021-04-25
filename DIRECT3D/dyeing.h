#pragma once
#include "equip.h"
class equip;

#define ACTIVEFONT		0xffffffff
#define INACTIVEFONT	0xffb0b0b0

class dyeing
{
private:
	LPDIRECT3DTEXTURE9  _dyeingPallet;			// 염색 팔레트
	LPDIRECT3DTEXTURE9  _ui;					// UI 텍스처
	LPDIRECT3DTEXTURE9  _spoidTex;
	POINT			    _windowPos;				// 염색UI 위치


	BOOL			    _isShow;				// UI를 현재 화면에 표시하는가?
	int					_texNumber;				// 염색할 텍스처 번호
	bool				_openDyeingInterface;

	equip*				_dyeingItem;			// 염색할 장비
	equip*				_sampleItem[5];			// 염색 인터페이스에 보여질 샘플 이미지

	LPDIRECT3DTEXTURE9	_button;				// 버튼 이미지
	LPDIRECT3DTEXTURE9	_inButton;				// 눌러진 버튼 이미지

	D3DXCOLOR			_glitterColor;			// 염색 파트를 시각적으로 표시하기 위한 알파값
	float				_glitterValue;			// 알파값을 증감시키는 값

	RECT			    _palletWindowRC;		// 염색UI 영역
	RECT			    _baseWindowRC;			// 염색 팔레트 영역
	RECT				_windowSourRC;			// 나인패치용 렉트
	RECT				_windowSourCenterRC;	// 나인패치용 렉트
	RECT				_slotRC;				// 슬롯 이미지 렉트
	RECT				_slotSourRC;			// 슬롯 이미지 나인패치용 렉트
	RECT				_sampleSlotRC[5];		// 샘플이 보여질 슬롯 렉트 영역
	RECT				_slotSourCenterRC;		// 나인패치용
	RECT				_partButton[3];			// 파트를 선택할 버튼용 렉트
	RECT				_startButtonRC;			// 염색 시작 버튼용 렉트
	RECT				_cancelButtonRC;		// 염색 취소 버튼용 렉트

	string				_partName[3];			// 파트 버튼 출력용

	DWORD				_selectColor[5];		// 선택된 컬러
	bool				_pickSelectColor;		// 선택이 완료 되었는가?
	bool				_clickPallet;			// 팔레트를 클릭하여 염색을 시작하였는가?
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void showWindows();	// 인터페이스 출력

						// 텍스처를 선택한 컬러로 염색한다.
	void dyeingColor(LPDIRECT3DTEXTURE9 originTexture, LPDIRECT3DTEXTURE9 diffuseTexutre, DWORD color, float alpha = 1.0f);
	// 인벤토리 이미지를 선택한 컬러로 염색한다.
	void dyeingImage(LPDIRECT3DTEXTURE9 originTexture, LPDIRECT3DTEXTURE9 diffuseTexutre, int partIndex, int width, DWORD color);

	DWORD getPixelColor(POINT point);		// 포인트상 픽셀의 컬러를 얻어온다.

	BOOL getShow(void) { return _isShow; }			// 화면 표시 여부
	void setShow(BOOL show) { _isShow = show; }

	LPRECT getPallectRC(void) { return &_palletWindowRC; };		// 염색팔레트 영역을 리턴

																// 염색할 아이템을 세팅한다.
	void setItem(equip* item);
	// 염색을 종료한다.
	void endDyeing(void);

	// 염색 파트를 선택한다.
	void setDyeingPart(int partNumber);

	void EventMouseOnPallet();			// 염색팔레트에 마우스가 위치하였을 때의 이벤트
	void EventPickColor();				// 컬러 선택을 완료할때의 이벤트
	void EventClickStartButton();		// 염색 시작 버튼을 눌렸을때의 이벤트
	void EventClickCancelButton();		// 염색 종료 버튼을 눌렸을때의 이벤트
	void EventClickPartButton();		// 염색 파트 선택버튼을 눌렸을때의 이벤트

										// 현재 마우스가 염색 인터페이스 안에 있는지 체크
	bool mouseOnInterface(void) { return PtInRect(&_baseWindowRC, MYUTIL::getMousePos()); }
	// 현재 마우스가 염색 팔레트 안에 있는지 체크
	bool mouseOnPalette(void) { return PtInRect(&_palletWindowRC, MYUTIL::getMousePos()); }
	// 현재 마우스가 파트 선택 버튼 안에 있는지 체크
	bool mouseOnPartButton(void);
	// 현재 마우스가 시작 버튼 안에 있는지 체크
	bool mouseOnStartButton(void) { return PtInRect(&_startButtonRC, MYUTIL::getMousePos()); }
	// 현재 마우스가 종료 버튼 안에 있는지 체크
	bool mouseOnCancelButton(void) { return PtInRect(&_cancelButtonRC, MYUTIL::getMousePos()); }
	// 색 선택이 시작되었는가?
	bool selectColoring(void) { return _clickPallet; }
	bool readyDyeing(void) { return _selectColor; }

	dyeing() : _dyeingPallet(NULL), _isShow(FALSE), _texNumber(0), _dyeingItem(NULL) {};
	~dyeing() {};
};

