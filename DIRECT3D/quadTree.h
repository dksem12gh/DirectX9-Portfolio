#pragma once
#include "terrain.h"

class quadTree
{
private:
	//각 코너 별 인덱스 상수
	enum CORNER { CORNER_LT = 0, CORNER_RT, CORNER_LB, CORNER_RB };
	terrain::LPTERRAINVERTEX _terrainVertices;	// 자신이 물고있는 터레인의 정점 정보

	DWORD		_corners[4];	// 자신의 쿼드 트리의 각코너 정점 인덱스
	DWORD		_center;		// 자신의 쿼드 트리의 중앙 정점 인덱스	(이게 -1 이라면 자식없다)
	quadTree*	_childs[4];		// 자신의 자식 쿼드 트리	(자신이 마지막 노드라면 다 NULL)

	D3DXVECTOR3 _centerPos;		// 자신의 쿼드트리 중심 위치
	float		_radius;		// 자신의 쿼드트리 영역 반지름

public:
	quadTree();
	~quadTree();

	// 쿼드 트리 초기화(Terrain 정점 배열,QuadTree 의 정점 한면의 갯수) 
	HRESULT init(terrain::LPTERRAINVERTEX vertices, DWORD verNumEdge);
	// 자식 트리 만든다
	void createChildTree();
	// 레이 충돌지점을 얻는다
	void getRayHits(vector<D3DXVECTOR3>* pHits, const LPRay pRay);
};

