#include "stdafx.h"
#include "dyeing.h"

HRESULT dyeing::init(void)
{
	// 염색 파레트 로드
	_dyeingPallet = RM_TEXTURE->addResource("Resources/mabinogi/UI/pallete.dds");
	// UI 이미지 로드
	_ui = RM_TEXTURE->addResource("Resources/mabinogi/UI/dollpink.dds");
	// 버튼 이미지 로드
	_button = RM_TEXTURE->addResource("Resources/mabinogi/UI/button.dds");
	// 눌러진 버튼 이미지 로드
	_inButton = RM_TEXTURE->addResource("Resources/mabinogi/UI/inButton.dds");
	// 스포이드 컬러 출력용 텍스처 블록 로드
	_spoidTex = RM_TEXTURE->addResource("Resources/mabinogi/UI/whiteBlock.dds");

	// 윈도우 출력 위치
	_windowPos = { WINSIZEX - 520, 50 };
	// 윈도우 영역렉트
	_baseWindowRC = MYUTIL::rectMake(_windowPos.x, _windowPos.y, 480, 540);

	D3DSURFACE_DESC desc;
	_dyeingPallet->GetLevelDesc(0, &desc); // 팔레트 이미지 정보를 생성.
										   // 팔레트 영역렉트
	_palletWindowRC = MYUTIL::rectMake(_baseWindowRC.right - 22 - desc.Width, _baseWindowRC.top + 60, desc.Width, desc.Height);

	// UI 이미지에서 필요한 부분만을 자르기 위한 렉트들
	_windowSourRC = { 0, 0, 14, 41 };
	_windowSourCenterRC = { 4, 3, 12, 31 };
	_slotRC = MYUTIL::rectMake(_baseWindowRC.left + 20, _palletWindowRC.top, 80, 128);
	_slotSourRC = { 378, 122, 399, 143 };
	_slotSourCenterRC = { 379, 123, 398, 142 };
	for (int i = 0; i < 5; i++)
	{
		_sampleItem[i] = NULL;
		_sampleSlotRC[i] = MYUTIL::rectMake(_slotRC.left + (i * (MYUTIL::getRectSize(&_slotRC).x + 8)), _palletWindowRC.bottom + 20, 80, 128);
	}

	// 파트버튼 세팅
	_partButton[0] = MYUTIL::rectMake(_sampleSlotRC[0].left, _sampleSlotRC[0].top - 135, 90, 40);
	_partButton[1] = MYUTIL::rectMake(_sampleSlotRC[0].left, _sampleSlotRC[0].top - 90, 90, 40);
	_partButton[2] = MYUTIL::rectMake(_sampleSlotRC[0].left, _sampleSlotRC[0].top - 45, 90, 40);
	_partName[0] = "파트 A";
	_partName[1] = "파트 B";
	_partName[2] = "파트 C";

	// 시작 버튼과 종료 버튼 영역 세팅
	_startButtonRC = MYUTIL::rectMake(_baseWindowRC.right - 180 - 40, _baseWindowRC.bottom - 60, 90, 40);
	_cancelButtonRC = MYUTIL::rectMake(_baseWindowRC.right - 90 - 20, _baseWindowRC.bottom - 60, 90, 40);

	// 염색 파트 표시용 정보 세팅
	_glitterColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	_glitterValue = -0.05f;


	_pickSelectColor = false;
	_clickPallet = false;
	return S_OK;
}

void dyeing::release(void)
{
	this->endDyeing();
}

void dyeing::update(void)
{
	// 염색 파트 표시용 추가컬러값의 알파를 증가값만큼 더해준다.
	_glitterColor.a += _glitterValue;

	// 만약 알파가 1.0을 넘거나 0.0보다 작아질 경우 증가값의 부호를 반전
	// (이로써 염색 파트 표시부가 반짝인다.)
	if (_glitterColor.a > 1.0f || _glitterColor.a < 0.0f)
	{
		_glitterValue *= -1;
	}
}

void dyeing::showWindows()
{
	if (_dyeingItem == NULL) return;

	// 아이템 이미지의 사이즈를 받아놓는다.
	D3DXVECTOR2			sampleItemSize = { (FLOAT)_dyeingItem->getFrameWidth(), (FLOAT)_dyeingItem->getFrameHeight() };

	// 인터페이스 윈도우를 나인패치렌더로 그려준다.
	SPRITEMANAGER->ninePatchRender(_ui, &_windowSourRC, &_baseWindowRC, &_windowSourCenterRC, 0xf000ffff);
	SPRITEMANAGER->ninePatchRender(_ui, &_slotSourRC, &_slotRC, &_slotSourCenterRC, 0xf000ffff);

	// 샘플 장비 이미지를 슬롯의 정중앙에 맞추기 위하여...
	D3DXVECTOR2 sampleItemPos;
	sampleItemPos.x = (_slotRC.left + (MYUTIL::getRectSize(&_slotRC).x / 2))
		- (sampleItemSize.x / 2);
	sampleItemPos.y = (_slotRC.top + (MYUTIL::getRectSize(&_slotRC).y / 2))
		- (sampleItemSize.y / 2);

	// 염색 팔레트 렌더
	SPRITEMANAGER->spriteRender(_dyeingPallet, NULL, _palletWindowRC.left, _palletWindowRC.top);

	// 스프라이트 렌더 자체의 버그로 실제 이미지보다 크게 그려지기 때문에 
	// (원하는 버튼 크기 / 소스로 불러온 이미지의 크기)로 계산하여 비율을 구한다.
	int buttonRCWidth = MYUTIL::getRectSize(&_partButton[0]).x;
	int buttonRCHeight = MYUTIL::getRectSize(&_partButton[0]).y;

	D3DSURFACE_DESC desc;
	_inButton->GetLevelDesc(0, &desc);
	float sizeWidth = (float)buttonRCWidth / (float)desc.Width;
	float sizeHeight = (float)buttonRCHeight / (float)desc.Height;


	for (int i = 0; i < 3; i++)
	{
		// 염색할 파트와 현재 버튼이 일치하면 들어간 이미지로 렌더
		// 이 때 이미지의 비율값에 위에서 구해준 비율을 대입한다.
		if (_texNumber == i)
		{
			SPRITEMANAGER->spriteRender(_inButton, NULL, _partButton[i].left, _partButton[i].top, sizeWidth, sizeHeight, 0.0f, 0xf000ffff);
		}
		else
		{
			SPRITEMANAGER->spriteRender(_button, NULL, _partButton[i].left, _partButton[i].top, sizeWidth, sizeHeight, 0.0f, 0xf000ffff);
		}

		// 폰트 위치를 맞춰서 폰트 렌더
		D3DXVECTOR2 fontPos;
		fontPos.x = _partButton[i].left + (MYUTIL::getRectSize(&_partButton[i]).x / 2) - ((int)(_partName[i].length() * 6.5f) / 2);
		fontPos.y = _partButton[i].top + (MYUTIL::getRectSize(&_partButton[i]).y / 2) - 6.5f;

		if (_dyeingItem->PartCount() - 1 >= i)
		{
			FONTMANAGER->fontOut("tap1", _partName[i], fontPos.x, fontPos.y, ACTIVEFONT);
		}
		else
		{
			FONTMANAGER->fontOut("tap1", _partName[i], fontPos.x, fontPos.y, INACTIVEFONT);
		}

	}
	// 염색될 장비 이미지를 렌더
	_dyeingItem->renderImage(sampleItemPos.x, sampleItemPos.y);
	// 반짝이는 효과를 위하여 장비 이미지의 원본이미지(염색때 쓰이는 염색되지 않은 오리지널 이미지)를 위에 덧씌운다.
	_dyeingItem->renderImage(sampleItemPos.x, sampleItemPos.y, _texNumber, _glitterColor);

	// 5개의 샘플 슬롯과 이미지를 렌더
	for (int i = 0; i < 5; i++)
	{
		SPRITEMANAGER->ninePatchRender(_ui, &_slotSourRC, &_sampleSlotRC[i], &_slotSourCenterRC, 0xf000ffff);
		D3DXVECTOR2 pos;
		pos.x = (_sampleSlotRC[i].left + (MYUTIL::getRectSize(&_sampleSlotRC[i]).x / 2))
			- (sampleItemSize.x / 2);
		pos.y = (_sampleSlotRC[i].top + (MYUTIL::getRectSize(&_sampleSlotRC[i]).y / 2))
			- (sampleItemSize.y / 2);
		_sampleItem[i]->renderImage(pos.x, pos.y);
	}

	// 시작 버튼
	SPRITEMANAGER->spriteRender(_button, NULL, _startButtonRC.left, _startButtonRC.top, sizeWidth, sizeHeight, 0.0f, 0xf000ffff);
	FONTMANAGER->fontOut("tap1", "염색 시작",
		_startButtonRC.left + (MYUTIL::getRectSize(&_startButtonRC).x / 2) - ((9 * 6.5f) / 2),
		_startButtonRC.top + (MYUTIL::getRectSize(&_startButtonRC).y / 2) - 6.5f, ACTIVEFONT);

	// 종료 버튼
	SPRITEMANAGER->spriteRender(_button, NULL, _cancelButtonRC.left, _cancelButtonRC.top, sizeWidth, sizeHeight, 0.0f, 0xf000ffff);
	FONTMANAGER->fontOut("tap1", "취소",
		_cancelButtonRC.left + (MYUTIL::getRectSize(&_cancelButtonRC).x / 2) - ((4 * 6.5f) / 2),
		_cancelButtonRC.top + (MYUTIL::getRectSize(&_cancelButtonRC).y / 2) - 6.5f, ACTIVEFONT);

	if (_isShow && _clickPallet == false)
	{
		string str = "염색을 하실려면 팔레트를 클릭하세요.";
		int fontPosX = (WINSIZEX / 2) - ((str.length() * 14) / 2);
		FONTMANAGER->fontOutLine("systemFont", str, fontPosX, 300, 0xffffffff, 0xff000000);
	}
}


void dyeing::dyeingColor(LPDIRECT3DTEXTURE9 originTexture, LPDIRECT3DTEXTURE9 diffuseTexutre, DWORD color, float alpha)
{
	DWORD levelCount = originTexture->GetLevelCount(); // 텍스처의 레벨의 수

	LPDIRECT3DSURFACE9 originSurface = NULL;		// 원본 텍스처의 표면 정보
	LPDIRECT3DSURFACE9 diffuseSurface = NULL;		// 염색될 텍스처의 표면 정보

	D3DSURFACE_DESC desc;	// 텍스처의 이미지 정보

	for (int i = 0; i < (int)levelCount; i++)				// 텍스처의 레벨 수만큼 반복
	{
		originTexture->GetLevelDesc(i, &desc);				// 현재 레벨의 원본 텍스처 이미지 정보를 얻어온다.
		originTexture->GetSurfaceLevel(i, &originSurface);	// 현재 레벨의 원본 텍스처의 표면을 얻어온다.
		diffuseTexutre->GetSurfaceLevel(i, &diffuseSurface);	// 현재 레벨의 염색될 텍스처 포면을 얻어온다.
		D3DLOCKED_RECT originLockRect, diffuseLockRect;			// 표면을 잠그는 구조체

		originSurface->LockRect(&originLockRect, NULL, D3DLOCK_READONLY);	// 텍스처를 잠근다. 원본은 정보를 얻어오기만 하기에 READONLY 플래그를 사용
		DWORD* originImgData = (DWORD*)(originLockRect.pBits);				// 텍스처의 비트맵 정보를 DWORD 배열로 변환한다.

		diffuseSurface->LockRect(&diffuseLockRect, NULL, D3DLOCK_DISCARD);
		DWORD* diffuseImgData = (DWORD*)(diffuseLockRect.pBits);

		// 알파값 보간
		if (alpha > 1.0f) alpha = 1.0f;
		if (alpha < 0.0f) alpha = 0.0f;

		for (int y = 0; y < desc.Height; y++)			// 텍스처 세로 사이즈만큼 반복
		{
			for (int x = 0; x < desc.Width; x++)		// 텍스처 가로 사이즈만큼 반복 
			{
				int index = (y * desc.Width) + x;		// 픽셀 인덱스

														/*
														픽셀의 정보를 D3DXCOLOR롷 변환한다.
														텍스처의 포멧이 32bit RGBA가 아니면 에러가 발생한다. (!! 중요 !!)
														*/
				D3DXCOLOR targetColor = originImgData[index];
				D3DXCOLOR dyeingColor = color;
				if (targetColor.r == targetColor.b) //R, B가 동일하면 염색되는 픽셀이다.
				{
					/*
					D3DXCOLOR의 각 컬러값은 0 ~ 1 사이로 보간되므로 원하는 색을 곱해주면 자연스럽게 설정된다.
					*/

					D3DXCOLOR newColor;
					newColor.r = dyeingColor.r * targetColor.r;
					newColor.g = dyeingColor.g * targetColor.g;
					newColor.b = dyeingColor.b * targetColor.b;
					newColor.a = 1.0f;

					// 새롭게 구해진 컬러값을 염색될 텍스처의 픽셀값에 대입한다.
					diffuseImgData[index] = newColor;
				}
			}
		}

		originSurface->UnlockRect();		// 표면 정보 잠금을 해제한다.
		originSurface->Release();			// 표면 정보 릴리즈
		diffuseSurface->UnlockRect();
		diffuseSurface->Release();
	}
}

void dyeing::dyeingImage(LPDIRECT3DTEXTURE9 originTexture, LPDIRECT3DTEXTURE9 diffuseTexutre, int partIndex, int width, DWORD color)
{
	if (originTexture == NULL || diffuseTexutre == NULL) return;
	LPDIRECT3DSURFACE9 originSurface = NULL;		// 원본 텍스처의 표면 정보
	LPDIRECT3DSURFACE9 diffuseSurface = NULL;		// 염색될 텍스처의 표면 정보
	D3DSURFACE_DESC desc;	// 텍스처의 이미지 정보
	originTexture->GetSurfaceLevel(0, &originSurface);	// 원본 텍스처의 표면을 얻어온다.
	originTexture->GetLevelDesc(0, &desc);				// 원본 텍스처의 이미지 정보를 얻어온다.
	diffuseTexutre->GetSurfaceLevel(0, &diffuseSurface);	// 염색될 텍스처의 포면을 얻어온다.
	D3DLOCKED_RECT originLockRect, diffuseLockRect;			// 표면을 잠그는 구조체

	originSurface->LockRect(&originLockRect, NULL, D3DLOCK_READONLY);	// 텍스처를 잠근다. 원본은 정보를 얻어오기만 하기에 READONLY 플래그를 사용
	DWORD* originImgData = (DWORD*)(originLockRect.pBits);				// 텍스처의 비트맵 정보를 DWORD 배열로 변환한다.

	diffuseSurface->LockRect(&diffuseLockRect, NULL, D3DUSAGE_WRITEONLY);
	DWORD* diffuseImgData = (DWORD*)(diffuseLockRect.pBits);

	for (int y = 0; y < desc.Height; ++y)			// 텍스처 세로 사이즈만큼 반복
	{
		for (int x = partIndex * width; x < partIndex * width + width; ++x)		// 텍스처 가로 사이즈만큼 반복 
		{
			int index = (y * desc.Width) + x;		// 픽셀 인덱스

													/*
													픽셀의 정보를 D3DXCOLOR롷 변환한다.
													텍스처의 포멧이 32bit RGBA가 아니면 에러가 발생한다. (!! 중요 !!)
													*/
			D3DXCOLOR targetColor = D3DXCOLOR(originImgData[index]);
			D3DXCOLOR dyeingColor = color;
			/*
			D3DXCOLOR의 각 컬러값은 0 ~ 1 사이로 보간되므로 원하는 색을 곱해주면 자연스럽게 설정된다.
			*/

			D3DXCOLOR newColor;
			newColor.r = dyeingColor.r * targetColor.r;
			newColor.g = dyeingColor.g * targetColor.g;
			newColor.b = dyeingColor.b * targetColor.b;
			newColor.a = targetColor.a;

			// 새롭게 구해진 컬러값을 염색될 텍스처의 픽셀값에 대입한다.
			diffuseImgData[index] = newColor;
		}
	}

	originSurface->UnlockRect();		// 표면 정보 잠금을 해제한다.
	originSurface->Release();			// 표면 정보 릴리즈
	diffuseSurface->UnlockRect();
	diffuseSurface->Release();
}

DWORD dyeing::getPixelColor(POINT point)
{
	if (!PtInRect(&_palletWindowRC, point)) return NULL;

	LPDIRECT3DSURFACE9 surface = NULL;
	D3DSURFACE_DESC desc;

	_dyeingPallet->GetSurfaceLevel(0, &surface);
	_dyeingPallet->GetLevelDesc(0, &desc);

	// 마우스가 클릭된 포인트를 팔레트 출력 위치에 맞춰 보간한다.
	D3DLOCKED_RECT lockRect;

	POINT palletePos;
	palletePos.x = point.x - _palletWindowRC.left;
	palletePos.y = point.y - _palletWindowRC.top;
	surface->LockRect(&lockRect, NULL, D3DLOCK_READONLY);
	DWORD* screenData = (DWORD*)lockRect.pBits;
	DWORD color = screenData[palletePos.y * desc.Width + palletePos.x];

	surface->UnlockRect();
	surface->Release();

	return color;
}

void dyeing::setItem(equip * item)
{
	if (item == NULL) return;

	_dyeingItem = item;
	this->setDyeingPart(0);
	for (int i = 0; i < 5; i++)
	{
		_sampleItem[i] = _dyeingItem->cloneItem();
	}
}

void dyeing::endDyeing(void)
{
	_dyeingItem = NULL;
	for (int i = 0; i < 5; i++)
	{
		if (_sampleItem[i] != NULL)
		{
			_sampleItem[i]->release();
			SAFE_DELETE(_sampleItem[i]);
		}

	}

	_pickSelectColor = false;
	_clickPallet = false;
}

void dyeing::setDyeingPart(int partNumber)
{
	_texNumber = partNumber;
}

void dyeing::EventMouseOnPallet()
{
	if (!_clickPallet)
	{
		OPTION->setCursor(defaultCursor);
		return;
	}
	if (_pickSelectColor)
	{
		OPTION->setCursor(defaultCursor);
		return; // 이미 색이 선택되었으면 리턴
	}
	if (_dyeingItem == NULL) return;
	POINT spoidPoint[5];		// 염색 포인트 5자리
	spoidPoint[0] = MYUTIL::getMousePos();
	spoidPoint[1] = { spoidPoint[0].x - 40, spoidPoint[0].y - 40 };
	spoidPoint[2] = { spoidPoint[0].x + 20, spoidPoint[0].y - 36 };
	spoidPoint[3] = { spoidPoint[0].x - 48, spoidPoint[0].y + 40 };
	spoidPoint[4] = { spoidPoint[0].x + 28, spoidPoint[0].y + 40 };

	// 중앙 포인터를 제외한 포인터들이 팔레트 영역을 넘어가면 적절하게(원작처럼) 보정
	for (int i = 1; i < 5; i++)
	{
		if (spoidPoint[i].x < _palletWindowRC.left)
		{
			spoidPoint[i].x += MYUTIL::getRectSize(&_palletWindowRC).x;
		}
		else if (spoidPoint[i].x > _palletWindowRC.right)
		{
			spoidPoint[i].x -= MYUTIL::getRectSize(&_palletWindowRC).x;
		}

		if (spoidPoint[i].y < _palletWindowRC.top)
		{
			spoidPoint[i].y += MYUTIL::getRectSize(&_palletWindowRC).y;
		}
		else if (spoidPoint[i].y > _palletWindowRC.bottom)
		{
			spoidPoint[i].y -= MYUTIL::getRectSize(&_palletWindowRC).y;
		}
	}

	// 커서를 스포이드 커서로 변환
	OPTION->setCursor(spoidCursor);

	// 스포이드는 총 5개를 그린다.
	for (int i = 0; i < 5; i++)
	{
		// 스포이드 컬러가 표시될 렉트 영역
		RECT colorRC = MYUTIL::rectMake(0, 0, 10, 11);

		// 스포이드를 그려준다.
		OPTION->renderCursor(spoidPoint[i]);
		// 현재 스포이드의 컬러값을 얻어온다.
		_selectColor[i] = this->getPixelColor(spoidPoint[i]);
		// 샘플 이미지에 색을 입힌다.
		_sampleItem[i]->dyeingColor(_texNumber, _selectColor[i]);
		// 스프라이트 렌더에서는 색 순서가 ABGR순이라서 Red와 Blue의 위치를 스왑
		D3DXCOLOR sampleColor = D3DXCOLOR(_selectColor[i]);
		float tempR = sampleColor.r;
		sampleColor.r = sampleColor.b;
		sampleColor.b = tempR;
		// 스포이드 컬러를 적절한 위치에 렌더
		SPRITEMANAGER->spriteRender(_spoidTex, &colorRC, spoidPoint[i].x + 11, spoidPoint[i].y - 5, DWORD(sampleColor));
	}

	// 커서를 기본 커서로
	OPTION->setCursor(defaultCursor);
}

void dyeing::EventPickColor()
{
	// 염색 시작이 되지 않았다면 염색 시작
	if (!_clickPallet) _clickPallet = true;
	// 이미 염색이 시작되었다면 색 결정
	else _pickSelectColor = true;
}

void dyeing::EventClickStartButton()
{
	// 염색이 시작되지 않았다면 리턴
	if (!_pickSelectColor) return;

	// 5개의 랜덤 숫자를 뽑는다.
	int pickNumber = MYUTIL::randomIntRange(0, 4);

	// 아이템을 염색하고 염색 결과를 화면에 출력
	_dyeingItem->dyeingColor(_texNumber, _selectColor[pickNumber]);

	EFFECTMANAGER->changeSystemMsg(to_string(pickNumber + 1) + " 번 색으로 염색되었습니다.");

	// 염색 정보를 리셋하고 염색창을 종료
	this->endDyeing();
	_isShow = false;
}

void dyeing::EventClickCancelButton()
{
	// 염색 정보를 종리셋하고 염색창을 종료
	this->endDyeing();
	_isShow = false;
}

void dyeing::EventClickPartButton()
{
	// 이미 색을 뽑았다면 리턴
	if (_pickSelectColor) return;

	for (int i = 0; i < _dyeingItem->PartCount(); i++)
	{
		if (PtInRect(&_partButton[i], MYUTIL::getMousePos())
			&& _dyeingItem->PartCount() > i) // 선택한 파트가 장비에 없다면 무시
		{
			// 염색 파트 번호를 세팅
			this->setDyeingPart(i);
			return;
		}
	}
}

bool dyeing::mouseOnPartButton(void)
{
	for (int i = 0; i < 3; i++)
	{
		if (PtInRect(&_partButton[i], MYUTIL::getMousePos()))
		{
			return true;
		}
	}

	return false;
}
