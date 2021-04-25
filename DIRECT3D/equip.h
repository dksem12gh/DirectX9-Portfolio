#pragma once
#include "baseItem.h"
#include "partItem.h"

/*
	장비 클래스
	파트별로 나뉘어진 메쉬와 인벤토리 이미지, 장비의 능력치 등을 지정
	partitem 클래스를 이용하여 파트를 만들고(최대 4개 파트)
	part A ~ C는 염색파트
	part D는 비염색파트(단 피부색은 염색을 해줘야 함)
*/

enum EQUIP_PARTS	// 장비의 장착부의를 설정하는 ENUM. 인벤토리 장착 구성을 위함.
{
	HEAD = 0,
	BODY,
	GLOVE,
	SHOES,
	MANTLE,
	WEAPON,
	GOLD
};

// 아이템 정보 구조체 2018. 06. 16. 박인호
typedef struct tagItemInfo
{
	string		itemName;				// 아이템 이름
	string		filePath;				// 메시 경로
	string		texturePath;			// 텍스쳐 경로
	string		invenImagePath;			// 인벤토리 이미지 경로
	int			invenImageWidth;		// 인벤토리 이미지 가로 사이즈(프레임)
	int			invenImageHeight;		// 인벤토리 이미지 세로 사이즈(프레임)
	int			invenImagePartCount;	// 인벤토리 이미지가 몇 프레임으로 구성되어 있는가?
	EQUIP_PARTS equipPart;				// 장착부위
	POINT		invensize;				// 인벤토리 사이즈
	DWORD		dyeingColor[3];			// 염색 컬러값 정보
	int			price;					// 아이템 가격
	int			stack;					// 스택(겹치는 수)
} ITEMINFO;

#define		INVENFRAMEWIDTH			48

class equip : public baseItem
{
private:
	string					_itemName;				// 아이템 이름
private:
	ITEMINFO				_itemInfo;
	vector<partItem*>		_vecMesh;				// 파트 메쉬가 저장될 벡터 컨테이너
	EQUIP_PARTS				_equipParts;			// 장착 파트
	LPDIRECT3DTEXTURE9		_originInvenImage;		// 인벤토리 이미지(원본)
	LPDIRECT3DTEXTURE9		_invenImage;			// 인벤토리 이미지(염색)
	LPDIRECT3DTEXTURE9		_itemInfoPop;			// 아이템 정보 팝업
	int						_invenImagePartCount;	// 인벤토리 이미지가 몇파트로 나뉘어져있나
	float					_invenImageWidth;		// 인벤토리 이미지 프레임 넓이
	float					_invenImageHeight;
	POINT					_invenSize;				// 인벤토리 사이즈
	D3DXVECTOR2				_imagePositon;			// 인벤토리 이미지 위치

	RECT					_itemRC;
	DWORD					_partColor[3];			// 염색컬러

	int						_price;					// 아이템 가격
	int						_stack;					// 아이템 스텍(겹치는 수)
public:
	/*
		name     : 아이템 이름
		itemPath : 아이템의 외형을 불러올 경로(주의 : 불러오는 x메쉬의 파일명까지가 아니라 경로까지만을 지정
					(예 : "Resources/mabinogi/mesh/wear/tanuki/")
		mtrlPath : 마테리얼 경로
		equipParts : 장착 부위(위 ENUM 참조)
	*/
	void release(void);
	void update(void);
	void render(void);

	void renderImage(void);

	void renderImage(int destX, int destY, int passIndex = -1, DWORD glitterColor = 0xffffffff);

	void setName(string name) { _itemName = name; }
	string getName(void) { return _itemName; }

	// 장착부위 세팅
	void setEquipParts(EQUIP_PARTS equipParts) { _equipParts = equipParts; }

	// 장착부위 확인
	EQUIP_PARTS getEquipParts(void) { return _equipParts; }

	// 인벤토리내 차지할 공간
	void setInvenSize(POINT size) { _invenSize = size; }
	POINT getInvenSize(void) { return _invenSize; }

	// 인벤토리 이미지가 위치할 포지션(모니터 좌표계)
	void setImagePosition(D3DXVECTOR2 pos) { _imagePositon = pos; }
	void setImagePosition(float x, float y) { _imagePositon = D3DXVECTOR2(x, y); }
	D3DXVECTOR2 getImagePosition(void) { return _imagePositon; }

	// 스킨드 메시에 장비 장착
	void setMesh(baseObject* pObject);
	// 장착헤제를 위하여 비활성화
	void releaseMesh(void);

	// 염색
	void dyeingColor(int part, DWORD color);

	void setFrameWidth(int width) { _invenImageWidth = width; }
	int getFrameWidth(void) { return _invenImageWidth; }

	void setFrameHeight(int height) { _invenImageHeight = height; }
	int getFrameHeight(void) { return _invenImageHeight; }

	LPRECT getRect(void) { return &_itemRC; };

	// 아이텀 정보 복사 2018. 06. 16. 박인호
	ITEMINFO getItemInfo(void) { return _itemInfo; };

	// 가격 get
	int getPrice(void) { return _price; }
	equip* cloneItem(void);

	int PartCount(void) { return _invenImagePartCount > 3 ? 3 : _invenImagePartCount; }

	void createItemInfoPop(LPDIRECT3DTEXTURE9* pPopTex);
	void renderItemInfoPop(int destX, int destY);

	equip(string name, string itemPath, string mtrlPath, string invenImage, int invenSizeWidth, int invenSizeHeight, int invenImageFrameCount, int invenImageWidth, int invenImageHeight, EQUIP_PARTS equipParts, int price, int stack, D3DXMATRIXA16 * pMatCorrection);
	equip() : _invenSize({ 1, 1 }), _originInvenImage(NULL), _imagePositon(D3DXVECTOR2(0, 0)) {};
	~equip() {};
};

