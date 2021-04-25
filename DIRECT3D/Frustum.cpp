#include "stdafx.h"
#include "Frustum.h"

bool Frustum::isInFrustum(baseObject * pObject)
{
	D3DXVECTOR3 worldCenter;
	float radius;

	pObject->_boundBox.getWorldCenterRadius(
		pObject->_transform,
		&worldCenter,
		&radius
	);

	// 월드 센터가 절두체의 각 6면 안쪽에 있는지 확인
	for (int i = 0; i < 6; i++)
	{
		// 면과의 거리
		float dist =
			PHYSICSMANAGER->planeDot(&_plane[i], &worldCenter);

		// 반지름 이상 벗어나면 절두체 안에 없음.
		if (dist > radius)
		{
			return false;
		}

	}

	return true;
}

void Frustum::updateFrustum(D3DXMATRIXA16 * matViewProj)
{
	// ViewProjection 의 역행렬
	D3DXMATRIXA16 matInv;
	D3DXMatrixInverse(&matInv, NULL, matViewProj);

	// 동차 좌표의 정점을 역행렬에 적용
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(_pos + i, _RHWPos + i, &matInv);
	}

	//		4--------5
	//     /|       /|
	//    0--------1 |
	//    | |  *   | |     ==> 중점(0, 0, 0)
	//    | 6------| 7
	//    |/       |/
	//    2--------3
	// 정면
	PHYSICSMANAGER->createPlane(&_plane[0], &_pos[0], &_pos[1], &_pos[2]);
	// 후면
	PHYSICSMANAGER->createPlane(&_plane[1], &_pos[5], &_pos[4], &_pos[7]);
	// 우측
	PHYSICSMANAGER->createPlane(&_plane[2], &_pos[1], &_pos[5], &_pos[3]);
	// 좌측
	PHYSICSMANAGER->createPlane(&_plane[3], &_pos[4], &_pos[0], &_pos[6]);
	// 상단
	PHYSICSMANAGER->createPlane(&_plane[4], &_pos[4], &_pos[5], &_pos[0]);
	// 하단
	PHYSICSMANAGER->createPlane(&_plane[5], &_pos[2], &_pos[3], &_pos[6]);

}

void Frustum::renderGizmo()
{
	//동차 좌표 정보 저장
	//					  4-------5  <-( 1, 1, 1 )
	//					 /|		 /|
	//					0-------1 |
	//					| 6-----|-7
	//					|/      |/
	//( -1, -1, 0 )->	2-------3

	DWORD color = 0xff00ffff;
	GIZMOMANAGER->lineMake(_pos[0], _pos[1], color);
	GIZMOMANAGER->lineMake(_pos[1], _pos[3], color);
	GIZMOMANAGER->lineMake(_pos[3], _pos[2], color);
	GIZMOMANAGER->lineMake(_pos[2], _pos[0], color);

	GIZMOMANAGER->lineMake(_pos[4], _pos[5], color);
	GIZMOMANAGER->lineMake(_pos[5], _pos[7], color);
	GIZMOMANAGER->lineMake(_pos[7], _pos[6], color);
	GIZMOMANAGER->lineMake(_pos[6], _pos[4], color);

	GIZMOMANAGER->lineMake(_pos[0], _pos[4], color);
	GIZMOMANAGER->lineMake(_pos[1], _pos[5], color);
	GIZMOMANAGER->lineMake(_pos[3], _pos[7], color);
	GIZMOMANAGER->lineMake(_pos[2], _pos[6], color);


}

Frustum::Frustum()
{
	//		4--------5
	//     /|       /|
	//    0--------1 |
	//    | |  *   | |     ==> 중점(0, 0, 0)
	//    | 6------| 7
	//    |/       |/
	//    2--------3

	_RHWPos[0] = D3DXVECTOR3(-1, 1, 0);
	_RHWPos[1] = D3DXVECTOR3(1, 1, 0);
	_RHWPos[2] = D3DXVECTOR3(-1, -1, 0);
	_RHWPos[3] = D3DXVECTOR3(1, -1, 0);
	_RHWPos[4] = D3DXVECTOR3(-1, 1, 1);
	_RHWPos[5] = D3DXVECTOR3(1, 1, 1);
	_RHWPos[6] = D3DXVECTOR3(-1, -1, 1);
	_RHWPos[7] = D3DXVECTOR3(1, -1, 1);
}

Frustum::~Frustum()
{
}
