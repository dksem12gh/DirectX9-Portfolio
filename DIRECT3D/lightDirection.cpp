#include "stdafx.h"
#include "lightDirection.h"

D3DXMATRIXA16 lightDirection::getLightMatrix()
{
	//  라이트 행렬
	//  Pos.x,      Pos.y,      Pos.z,     lightType (0 Direction, 1 Point, 2 Spot)
	//  Dir.x,      Dir.y,      Dir.z,     DistancePow;
	//  Color.r,    Color.g,    Color.b,   strength
	//  MaxRange,   MinRange,   InAngle    OutAngle 

	D3DXMATRIXA16 matLight;
	D3DXMatrixIdentity(&matLight);
	matLight._14 = 0;	// Directional Light Type
	D3DXVECTOR3 forward = _transform.getForward();
	memcpy(&matLight._21, &forward, sizeof(D3DXVECTOR3));
	memcpy(&matLight._31, &_color, sizeof(FLOAT) * 3);
	matLight._34 = _intensity;

	return matLight;
}

void lightDirection::renderGizmo()
{
	this->_transform.renderGizmo();
}
