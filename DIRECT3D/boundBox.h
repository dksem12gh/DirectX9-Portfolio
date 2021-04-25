#pragma once
#include "boundSphere.h"

class boundBox : public boundSphere
{
public:
	D3DXVECTOR3		_localMinPos;
	D3DXVECTOR3		_localMaxPos;


public:
	void getWorldBox(transform* trans, D3DXVECTOR3* outBoxPos);
	void getWorldAABBMinMax(transform* trans, D3DXVECTOR3* min, D3DXVECTOR3* max);

	// 월드 단위로 그리기
	virtual void renderGizmo(transform* trans) override;

	virtual void setBound(D3DXVECTOR3* center, D3DXVECTOR3* halfSize) override;

	void setLocalMinPos(D3DXVECTOR3 localMinPos) { _localMinPos = localMinPos; }
	void setLocalMaxPos(D3DXVECTOR3 localMaxPos) { _localMaxPos = localMaxPos; }

	D3DXVECTOR3 getLocalMinPos() { return _localMinPos; }
	D3DXVECTOR3 getLocalMaxPos() { return _localMaxPos; }


	boundBox() {}
	~boundBox() {}
};

