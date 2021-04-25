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

// ���ҽ� �ε���ĸ� �������Ѵ�(�ؽ���)
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

// ���ҽ� ������ĸ� �������Ѵ�
void rmTexture::releaseResource(LPDIRECT3DTEXTURE9 data)
{
	SAFE_RELEASE(data);
}