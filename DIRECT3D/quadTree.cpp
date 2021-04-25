#include "stdafx.h"
#include "quadTree.h"

quadTree::quadTree()
{
	//차일드 포인터 NULL 처리
	ZeroMemory(_childs, sizeof(quadTree*) * 4);
	//센터인덱스 -1 로 초기화
	_center = -1;
}
quadTree::~quadTree()
{
	//자식 안전 지우기
	SAFE_DELETE(_childs[0]);
	SAFE_DELETE(_childs[1]);
	SAFE_DELETE(_childs[2]);
	SAFE_DELETE(_childs[3]);
}

//쿼드 트리 초기화(Terrain 정점 배열,QuadTree 의 정점 한면의 갯수) 
HRESULT quadTree::init(terrain::LPTERRAINVERTEX vertices, DWORD verNumEdge)
{
	//Terrain 정점 데이터 물린다
	_terrainVertices = vertices;

	//각 코너 인덱스 계산한다.
	_corners[CORNER_LT] = 0;
	_corners[CORNER_RT] = verNumEdge - 1;
	_corners[CORNER_LB] = (verNumEdge - 1) * verNumEdge;
	_corners[CORNER_RB] = verNumEdge * verNumEdge - 1;

	//자식 트리 만든다.
	createChildTree();

	return S_OK;
}
//자식 트리 만든다
void quadTree::createChildTree()
{
	//중심위치
	//각코너의 포지션 센터 값
	_centerPos = (_terrainVertices[_corners[0]].pos +
		_terrainVertices[_corners[1]].pos +
		_terrainVertices[_corners[2]].pos +
		_terrainVertices[_corners[3]].pos) * 0.25f;

	//아무점 하나의 거리가 반지름이다.
	D3DXVECTOR3 dir = _terrainVertices[_corners[0]].pos - _centerPos;
	float prevLengthSq = D3DXVec3LengthSq(&dir);
	_radius = sqrt(prevLengthSq);

	//자식이 있다면 계산해야 한다
	if ((_corners[CORNER_RT] - _corners[CORNER_LT]) > 1)
	{
		//센터 인덱스 계산
		_center = (_corners[0] + _corners[1] + _corners[2] + _corners[3]) / 4;

		DWORD topCenter = (_corners[CORNER_RT] + _corners[CORNER_LT]) / 2;		//상단 중앙
		DWORD leftCenter = (_corners[CORNER_LT] + _corners[CORNER_LB]) / 2;		//좌 중앙
		DWORD rightCenter = (_corners[CORNER_RT] + _corners[CORNER_RB]) / 2;	//우 중앙
		DWORD bottomCenter = (_corners[CORNER_RB] + _corners[CORNER_LB]) / 2;	//우 중앙

																				//좌상단 자식
		_childs[CORNER_LT] = new quadTree;
		_childs[CORNER_LT]->_corners[CORNER_LT] = _corners[CORNER_LT];
		_childs[CORNER_LT]->_corners[CORNER_RT] = topCenter;
		_childs[CORNER_LT]->_corners[CORNER_LB] = leftCenter;
		_childs[CORNER_LT]->_corners[CORNER_RB] = _center;
		_childs[CORNER_LT]->_terrainVertices = _terrainVertices;
		_childs[CORNER_LT]->createChildTree();

		//우상단 자식
		_childs[CORNER_RT] = new quadTree;
		_childs[CORNER_RT]->_corners[CORNER_LT] = topCenter;
		_childs[CORNER_RT]->_corners[CORNER_RT] = _corners[CORNER_RT];
		_childs[CORNER_RT]->_corners[CORNER_LB] = _center;
		_childs[CORNER_RT]->_corners[CORNER_RB] = rightCenter;
		_childs[CORNER_RT]->_terrainVertices = _terrainVertices;
		_childs[CORNER_RT]->createChildTree();

		//좌하단 자식
		_childs[CORNER_LB] = new quadTree;
		_childs[CORNER_LB]->_corners[CORNER_LT] = leftCenter;
		_childs[CORNER_LB]->_corners[CORNER_RT] = _center;
		_childs[CORNER_LB]->_corners[CORNER_LB] = _corners[CORNER_LB];
		_childs[CORNER_LB]->_corners[CORNER_RB] = bottomCenter;
		_childs[CORNER_LB]->_terrainVertices = _terrainVertices;
		_childs[CORNER_LB]->createChildTree();

		//우하단 자식
		_childs[CORNER_RB] = new quadTree;
		_childs[CORNER_RB]->_corners[CORNER_LT] = _center;
		_childs[CORNER_RB]->_corners[CORNER_RT] = rightCenter;
		_childs[CORNER_RB]->_corners[CORNER_LB] = bottomCenter;
		_childs[CORNER_RB]->_corners[CORNER_RB] = _corners[CORNER_RB];
		_childs[CORNER_RB]->_terrainVertices = _terrainVertices;
		_childs[CORNER_RB]->createChildTree();
	}
}
//레이 충돌지점을 얻는다
void quadTree::getRayHits(vector<D3DXVECTOR3>* pHits, const LPRay pRay)
{
	//나의 바운드와 충돌했니?
	if (PHYSICSMANAGER->isRayHitSphere(pRay, &_centerPos, _radius, NULL, NULL))
	{
		//자식이잇니?
		if ((_corners[CORNER_RT] - _corners[CORNER_LT]) > 1)
		{
			//자식 재귀
			for (int i = 0; i < 4; i++)
			{
				_childs[i]->getRayHits(pHits, pRay);
			}
		}
		//자식이 없는 맨마지막 노드니?
		else
		{
			//Tri 2개 얻는다.

			// lt---rt
			//  |  /|
			//  | / |
			//  |/  |
			// lb---rb

			//나의 폴리곤과 Ray 의 Hit 지점을 얻자
			float dist = 0.0f;

			D3DXVECTOR3 lt = _terrainVertices[_corners[CORNER_LT]].pos;
			D3DXVECTOR3 rt = _terrainVertices[_corners[CORNER_RT]].pos;
			D3DXVECTOR3 lb = _terrainVertices[_corners[CORNER_LB]].pos;
			D3DXVECTOR3 rb = _terrainVertices[_corners[CORNER_RB]].pos;

			//좌상단 폴리곤 체크
			if (D3DXIntersectTri(
				&lt, &rt, &lb,		//폴리곤 정점 3 위치 (두루기 순서 주의)
				&pRay->origin,		//레이 위치
				&pRay->direction,	//레이 방향
				NULL, NULL,
				&dist				//히트가 되었다면 Origin 에서 부터 hit 위치까지의 거리가 나온다.
			))
			{
				//히트지점
				D3DXVECTOR3 hitPos = pRay->origin + pRay->direction * dist;
				//푸쉬
				pHits->push_back(hitPos);
				return;
			}

			//우하단 폴리곤 체크
			if (D3DXIntersectTri(
				&lb, &rt, &rb,		//폴리곤 정점 3 위치
				&pRay->origin,		//레이 위치
				&pRay->direction,	//레이 방향
				NULL, NULL,
				&dist				//히트가 되었다면 Origin 에서 부터 hit 위치까지의 거리가 나온다.
			))
			{
				//히트지점
				D3DXVECTOR3 hitPos = pRay->origin + pRay->direction * dist;
				//푸쉬
				pHits->push_back(hitPos);
				return;
			}
		}
	}
}


