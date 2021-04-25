#pragma once

class boundSphere
{
public:
	D3DXVECTOR3		_localCenter;	// 로컬 센터좌표
	D3DXVECTOR3		_halfSize;		// 절반 사이즈
	float			_radius;		//  반지름


public:
	// 월드단위의 센터와 반지름 얻기
	void getWorldCenterRadius(transform* trans, D3DXVECTOR3* center, float* radius);
	// 월드 단위로 그리기
	virtual void renderGizmo(transform* trans);

	//  바운드 박스 셋팅
	virtual void setBound(D3DXVECTOR3* center, D3DXVECTOR3* halfSize);

	void setLocalCenter(D3DXVECTOR3 localCenter) { _localCenter = localCenter; }
	D3DXVECTOR3 getLocalCEnter(void) { return _localCenter; }
	void setHalfSize(D3DXVECTOR3 halfSize) { _halfSize = halfSize; }


	boundSphere() {}
	~boundSphere() {}
};

