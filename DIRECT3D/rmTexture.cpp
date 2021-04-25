#include "stdafx.h"
#include "rmTexture.h"

HRESULT rmTexture::init(void)
{
	return S_OK;
}

void rmTexture::release(void)
{
	this->clearResource();
}

// 리소스 로딩방식만 재정의한다(텍스쳐)
LPDIRECT3DTEXTURE9 rmTexture::loadResource(string fileName, void * param)
{
	LPDIRECT3DTEXTURE9 texture = NULL;
	if (FAILED(D3DXCreateTextureFromFile(device, fileName.c_str(), &texture))) return NULL;
	return texture;
}

LPDIRECT3DTEXTURE9 rmTexture::loadResource(string fileName, string mtrlPath, void * param)
{
	return LPDIRECT3DTEXTURE9();
}

// 리소스 해제방식만 재정의한다
void rmTexture::releaseResource(LPDIRECT3DTEXTURE9 data)
{
	SAFE_RELEASE(data);
}