#pragma once
#include "light.h"
class lightDirection : public light
{
public:
	// 라이트 행렬 얻기
	virtual D3DXMATRIXA16 getLightMatrix();
	// 라이트 기즈모를 그린다.
	virtual void renderGizmo();

	lightDirection() {}
	~lightDirection() {}
};