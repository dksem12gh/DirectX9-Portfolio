#pragma once
#include "equip.h"
class equip;

#define ACTIVEFONT		0xffffffff
#define INACTIVEFONT	0xffb0b0b0

class dyeing
{
private:
	LPDIRECT3DTEXTURE9  _dyeingPallet;			// ���� �ȷ�Ʈ
	LPDIRECT3DTEXTURE9  _ui;					// UI �ؽ�ó
	LPDIRECT3DTEXTURE9  _spoidTex;
	POINT			    _windowPos;				// ����UI ��ġ


	BOOL			    _isShow;				// UI�� ���� ȭ�鿡 ǥ���ϴ°�?
	int					_texNumber;				// ������ �ؽ�ó ��ȣ
	bool				_openDyeingInterface;

	equip*				_dyeingItem;			// ������ ���
	equip*				_sampleItem[5];			// ���� �������̽��� ������ ���� �̹���

	LPDIRECT3DTEXTURE9	_button;				// ��ư �̹���
	LPDIRECT3DTEXTURE9	_inButton;				// ������ ��ư �̹���

	D3DXCOLOR			_glitterColor;			// ���� ��Ʈ�� �ð������� ǥ���ϱ� ���� ���İ�
	float				_glitterValue;			// ���İ��� ������Ű�� ��

	RECT			    _palletWindowRC;		// ����UI ����
	RECT			    _baseWindowRC;			// ���� �ȷ�Ʈ ����
	RECT				_windowSourRC;			// ������ġ�� ��Ʈ
	RECT				_windowSourCenterRC;	// ������ġ�� ��Ʈ
	RECT				_slotRC;				// ���� �̹��� ��Ʈ
	RECT				_slotSourRC;			// ���� �̹��� ������ġ�� ��Ʈ
	RECT				_sampleSlotRC[5];		// ������ ������ ���� ��Ʈ ����
	RECT				_slotSourCenterRC;		// ������ġ��
	RECT				_partButton[3];			// ��Ʈ�� ������ ��ư�� ��Ʈ
	RECT				_startButtonRC;			// ���� ���� ��ư�� ��Ʈ
	RECT				_cancelButtonRC;		// ���� ��� ��ư�� ��Ʈ

	string				_partName[3];			// ��Ʈ ��ư ��¿�

	DWORD				_selectColor[5];		// ���õ� �÷�
	bool				_pickSelectColor;		// ������ �Ϸ� �Ǿ��°�?
	bool				_clickPallet;			// �ȷ�Ʈ�� Ŭ���Ͽ� ������ �����Ͽ��°�?
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void showWindows();	// �������̽� ���

						// �ؽ�ó�� ������ �÷��� �����Ѵ�.
	void dyeingColor(LPDIRECT3DTEXTURE9 originTexture, LPDIRECT3DTEXTURE9 diffuseTexutre, DWORD color, float alpha = 1.0f);
	// �κ��丮 �̹����� ������ �÷��� �����Ѵ�.
	void dyeingImage(LPDIRECT3DTEXTURE9 originTexture, LPDIRECT3DTEXTURE9 diffuseTexutre, int partIndex, int width, DWORD color);

	DWORD getPixelColor(POINT point);		// ����Ʈ�� �ȼ��� �÷��� ���´�.

	BOOL getShow(void) { return _isShow; }			// ȭ�� ǥ�� ����
	void setShow(BOOL show) { _isShow = show; }

	LPRECT getPallectRC(void) { return &_palletWindowRC; };		// �����ȷ�Ʈ ������ ����

																// ������ �������� �����Ѵ�.
	void setItem(equip* item);
	// ������ �����Ѵ�.
	void endDyeing(void);

	// ���� ��Ʈ�� �����Ѵ�.
	void setDyeingPart(int partNumber);

	void EventMouseOnPallet();			// �����ȷ�Ʈ�� ���콺�� ��ġ�Ͽ��� ���� �̺�Ʈ
	void EventPickColor();				// �÷� ������ �Ϸ��Ҷ��� �̺�Ʈ
	void EventClickStartButton();		// ���� ���� ��ư�� ���������� �̺�Ʈ
	void EventClickCancelButton();		// ���� ���� ��ư�� ���������� �̺�Ʈ
	void EventClickPartButton();		// ���� ��Ʈ ���ù�ư�� ���������� �̺�Ʈ

										// ���� ���콺�� ���� �������̽� �ȿ� �ִ��� üũ
	bool mouseOnInterface(void) { return PtInRect(&_baseWindowRC, MYUTIL::getMousePos()); }
	// ���� ���콺�� ���� �ȷ�Ʈ �ȿ� �ִ��� üũ
	bool mouseOnPalette(void) { return PtInRect(&_palletWindowRC, MYUTIL::getMousePos()); }
	// ���� ���콺�� ��Ʈ ���� ��ư �ȿ� �ִ��� üũ
	bool mouseOnPartButton(void);
	// ���� ���콺�� ���� ��ư �ȿ� �ִ��� üũ
	bool mouseOnStartButton(void) { return PtInRect(&_startButtonRC, MYUTIL::getMousePos()); }
	// ���� ���콺�� ���� ��ư �ȿ� �ִ��� üũ
	bool mouseOnCancelButton(void) { return PtInRect(&_cancelButtonRC, MYUTIL::getMousePos()); }
	// �� ������ ���۵Ǿ��°�?
	bool selectColoring(void) { return _clickPallet; }
	bool readyDyeing(void) { return _selectColor; }

	dyeing() : _dyeingPallet(NULL), _isShow(FALSE), _texNumber(0), _dyeingItem(NULL) {};
	~dyeing() {};
};

