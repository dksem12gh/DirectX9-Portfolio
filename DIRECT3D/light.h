#pragma once

class light
{
public :
	transform	_transform;			//  라이트 트랜스폼
	D3DXCOLOR	_color;				//  라이트 색깔
	float		_intensity;			//  라이트 강도

public :
	//  라이트 행렬
	//  Pos.x,      Pos.y,      Pos.z,     lightType (0 Direction, 1 Point, 2 Spot)
	//  Dir.x,      Dir.y,      Dir.z,     DistancePow;
	//  Color.r,    Color.g,    Color.b,   strength
	//  MaxRange,   MinRange,   InAngle    OutAngle 

	//  라이트 행렬 얻기
	virtual D3DXMATRIXA16 getLightMatrix() = 0;
	//  라이트 기즈모 그리기
	virtual void renderGizmo() = 0;

	light() : _intensity(1.0f) {};
	~light() {};

};