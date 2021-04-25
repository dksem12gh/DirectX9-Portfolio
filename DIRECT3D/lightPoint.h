#pragma once
#include "light.h"
class lightPoint : public light
{
public:
	float _minRange;		// 빛의 최소거리
	float _maxRange;		// 빛의 최대거리
	float _distancePow;	// 거리 감쇠율

public:
	// 라이트 행렬 얻기
	virtual D3DXMATRIXA16 getLightMatrix();
	// 라이트 기즈모를 그린다.
	virtual void renderGizmo();

	lightPoint() {}
	~lightPoint() {}
};