#include "stdafx.h"
#include "lightPoint.h"

D3DXMATRIXA16 lightPoint::getLightMatrix()
{
	//  라이트 행렬
	//  Pos.x,      Pos.y,      Pos.z,     lightType (0 Direction, 1 Point, 2 Spot)
	//  Dir.x,      Dir.y,      Dir.z,     DistancePow;
	//  Color.r,    Color.g,    Color.b,   strength
	//  MaxRange,   MinRange,   InAngle    OutAngle 

	D3DXMATRIXA16 matLight;
	D3DXMatrixIdentity(&matLight);
	matLight._14 = 1;	// Point Light Type
	D3DXVECTOR3 pos = _transform.getWorldPosition();
	memcpy(&matLight._11, &pos, sizeof(D3DXVECTOR3));
	memcpy(&matLight._31, &_color, sizeof(FLOAT) * 3);
	matLight._24 = _distancePow;
	matLight._41 = _maxRange;
	matLight._42 = _minRange;
	matLight._34 = _intensity;

	return matLight;
}

void lightPoint::renderGizmo()
{
	this->_transform.renderGizmo();
}