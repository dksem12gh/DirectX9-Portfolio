#include "stdafx.h"
#include "rmShader.h"


HRESULT rmShader::init(void)
{
	return S_OK;
}

void rmShader::release(void)
{
	this->clearResource();
}

LPD3DXEFFECT rmShader::loadResource(string fileName, void * param)
{
	// 셰이더 이펙트 선언
	LPD3DXEFFECT	effect;
	// 셰이더 로딩시 컴파일 에러가 나면 에러의 내용을 받아올 error버퍼
	LPD3DXBUFFER	error = NULL;
	DWORD			shaderFlag = 0;

	// fx파일로 부터 이펙트 객체에 로딩하기
	D3DXCreateEffectFromFile(
		device,
		fileName.c_str(),
		NULL, NULL, shaderFlag, NULL,
		&effect, &error);

	// 에러 예외처리
	if (error != NULL && effect == NULL)
	{
		SAFE_RELEASE(error);
		return NULL;
	}
	return effect;
}

LPD3DXEFFECT rmShader::loadResource(string fileName, string mtrlPath, void * param)
{
	return LPD3DXEFFECT();
}

void rmShader::releaseResource(LPD3DXEFFECT data)
{
	SAFE_RELEASE(data);
}
