#pragma once
#include "singletonBase.h"
//=============================================================
//	## spriteManager ## (스프라이트 매니져)
//=============================================================

class spriteManager : public singletonBase <spriteManager>
{
private:
	LPD3DXSPRITE	_sprite;

public:
	HRESULT init(void);
	void release(void);

	//스프라이트 렌더 시작시 호출
	void beginSpriteRender();
	//스프라이트 렌더 종료시 호출
	void endSpriteRender();

	//윈도우좌표계 사용됨
	//일반렌더(텍스쳐, 그릴렉트영역, x, y, 섞이는 색상, 이미지중심점)
	void spriteRender(
		LPDIRECT3DTEXTURE9 texture,
		LPRECT rcSour,
		float posX, float posY,
		DWORD color = 0xffffffff,
		D3DXVECTOR3* center = NULL);
	//일반렌더(이미지 크기, 회전가능)
	void spriteRender(
		LPDIRECT3DTEXTURE9 texture,
		LPRECT rcSour,
		float posX, float posY,
		float scaleX, float scaleY,
		float rotate,
		DWORD color = 0xffffffff,
		D3DXVECTOR3* center = NULL);

	//렉트의 영역만큼 렌더
	void renderArea(
		LPDIRECT3DTEXTURE9 texture,
		LPRECT rcSour,
		LPRECT rcDest,
		DWORD color = 0xffffffff);

	//나인패치 렌더
	void ninePatchRender(
		LPDIRECT3DTEXTURE9 texture,
		LPRECT rcSour,
		LPRECT rcDest,
		LPRECT rcCenter,
		DWORD color = 0xffffffff,
		bool isDrawCenter = true);

	spriteManager() {}
	~spriteManager() {}
};

