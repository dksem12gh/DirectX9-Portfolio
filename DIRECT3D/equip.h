#pragma once
#include "baseItem.h"
#include "partItem.h"

/*
	��� Ŭ����
	��Ʈ���� �������� �޽��� �κ��丮 �̹���, ����� �ɷ�ġ ���� ����
	partitem Ŭ������ �̿��Ͽ� ��Ʈ�� �����(�ִ� 4�� ��Ʈ)
	part A ~ C�� ������Ʈ
	part D�� �񿰻���Ʈ(�� �Ǻλ��� ������ ����� ��)
*/

enum EQUIP_PARTS	// ����� �������Ǹ� �����ϴ� ENUM. �κ��丮 ���� ������ ����.
{
	HEAD = 0,
	BODY,
	GLOVE,
	SHOES,
	MANTLE,
	WEAPON,
	GOLD
};

// ������ ���� ����ü 2018. 06. 16. ����ȣ
typedef struct tagItemInfo
{
	string		itemName;				// ������ �̸�
	string		filePath;				// �޽� ���
	string		texturePath;			// �ؽ��� ���
	string		invenImagePath;			// �κ��丮 �̹��� ���
	int			invenImageWidth;		// �κ��丮 �̹��� ���� ������(������)
	int			invenImageHeight;		// �κ��丮 �̹��� ���� ������(������)
	int			invenImagePartCount;	// �κ��丮 �̹����� �� ���������� �����Ǿ� �ִ°�?
	EQUIP_PARTS equipPart;				// ��������
	POINT		invensize;				// �κ��丮 ������
	DWORD		dyeingColor[3];			// ���� �÷��� ����
	int			price;					// ������ ����
	int			stack;					// ����(��ġ�� ��)
} ITEMINFO;

#define		INVENFRAMEWIDTH			48

class equip : public baseItem
{
private:
	string					_itemName;				// ������ �̸�
private:
	ITEMINFO				_itemInfo;
	vector<partItem*>		_vecMesh;				// ��Ʈ �޽��� ����� ���� �����̳�
	EQUIP_PARTS				_equipParts;			// ���� ��Ʈ
	LPDIRECT3DTEXTURE9		_originInvenImage;		// �κ��丮 �̹���(����)
	LPDIRECT3DTEXTURE9		_invenImage;			// �κ��丮 �̹���(����)
	LPDIRECT3DTEXTURE9		_itemInfoPop;			// ������ ���� �˾�
	int						_invenImagePartCount;	// �κ��丮 �̹����� ����Ʈ�� ���������ֳ�
	float					_invenImageWidth;		// �κ��丮 �̹��� ������ ����
	float					_invenImageHeight;
	POINT					_invenSize;				// �κ��丮 ������
	D3DXVECTOR2				_imagePositon;			// �κ��丮 �̹��� ��ġ

	RECT					_itemRC;
	DWORD					_partColor[3];			// �����÷�

	int						_price;					// ������ ����
	int						_stack;					// ������ ����(��ġ�� ��)
public:
	/*
		name     : ������ �̸�
		itemPath : �������� ������ �ҷ��� ���(���� : �ҷ����� x�޽��� ���ϸ������ �ƴ϶� ��α������� ����
					(�� : "Resources/mabinogi/mesh/wear/tanuki/")
		mtrlPath : ���׸��� ���
		equipParts : ���� ����(�� ENUM ����)
	*/
	void release(void);
	void update(void);
	void render(void);

	void renderImage(void);

	void renderImage(int destX, int destY, int passIndex = -1, DWORD glitterColor = 0xffffffff);

	void setName(string name) { _itemName = name; }
	string getName(void) { return _itemName; }

	// �������� ����
	void setEquipParts(EQUIP_PARTS equipParts) { _equipParts = equipParts; }

	// �������� Ȯ��
	EQUIP_PARTS getEquipParts(void) { return _equipParts; }

	// �κ��丮�� ������ ����
	void setInvenSize(POINT size) { _invenSize = size; }
	POINT getInvenSize(void) { return _invenSize; }

	// �κ��丮 �̹����� ��ġ�� ������(����� ��ǥ��)
	void setImagePosition(D3DXVECTOR2 pos) { _imagePositon = pos; }
	void setImagePosition(float x, float y) { _imagePositon = D3DXVECTOR2(x, y); }
	D3DXVECTOR2 getImagePosition(void) { return _imagePositon; }

	// ��Ų�� �޽ÿ� ��� ����
	void setMesh(baseObject* pObject);
	// ���������� ���Ͽ� ��Ȱ��ȭ
	void releaseMesh(void);

	// ����
	void dyeingColor(int part, DWORD color);

	void setFrameWidth(int width) { _invenImageWidth = width; }
	int getFrameWidth(void) { return _invenImageWidth; }

	void setFrameHeight(int height) { _invenImageHeight = height; }
	int getFrameHeight(void) { return _invenImageHeight; }

	LPRECT getRect(void) { return &_itemRC; };

	// ������ ���� ���� 2018. 06. 16. ����ȣ
	ITEMINFO getItemInfo(void) { return _itemInfo; };

	// ���� get
	int getPrice(void) { return _price; }
	equip* cloneItem(void);

	int PartCount(void) { return _invenImagePartCount > 3 ? 3 : _invenImagePartCount; }

	void createItemInfoPop(LPDIRECT3DTEXTURE9* pPopTex);
	void renderItemInfoPop(int destX, int destY);

	equip(string name, string itemPath, string mtrlPath, string invenImage, int invenSizeWidth, int invenSizeHeight, int invenImageFrameCount, int invenImageWidth, int invenImageHeight, EQUIP_PARTS equipParts, int price, int stack, D3DXMATRIXA16 * pMatCorrection);
	equip() : _invenSize({ 1, 1 }), _originInvenImage(NULL), _imagePositon(D3DXVECTOR2(0, 0)) {};
	~equip() {};
};

