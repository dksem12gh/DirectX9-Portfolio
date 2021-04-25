#pragma once
#include "singletonBase.h"

class boundBox;
class baseObject;

class physicsManager : public singletonBase <physicsManager>
{
public:
	HRESULT init(void);
	void release(void);

	//레이가 바운드에 충돌했는지 확인
	bool isRayHitBound(
		LPRay ray,					// 레이
		boundSphere* bound,			// 바운드구
		transform* boundTrans,		// 바운드트렌스폼
		D3DXVECTOR3* hitPos,		// 히트 위치 (NULL이면 대입안됨)
		D3DXVECTOR3* hitNormal);	// 히트 노말 (NULL이면 대입안됨)

	// 레이가 바운드에 충돌했는지 확인
	bool isRayHitBound(
		LPRay ray,					// 레이
		boundBox* bound,			// 바운드박스
		transform* boundTrans,		// 바운드트렌스폼
		D3DXVECTOR3* hitPos,		// 히트 위치 (NULL이면 대입안됨)
		D3DXVECTOR3* hitNormal);	// 히트 노말 (NULL이면 대입안됨)

	// 레이가 오브젝트와 충돌했는지 확인
	bool isRayHitStaticMeshObject(
		LPRay ray,					// 레이
		baseObject* object,			// 오브젝트
		D3DXVECTOR3* hitPos,		// 히트 위치 (NULL이면 대입안됨)
		D3DXVECTOR3* hitNormal);	// 히트 노말 (NULL이면 대입안됨)

	// 레이가 구와 충돌했는지 확인
	bool isRayHitSphere(
		LPRay ray,
		D3DXVECTOR3* pCenter,
		float radius,
		D3DXVECTOR3* hitPos,		//히트 위치 (NULL이면 대입안됨)
		D3DXVECTOR3* hitNormal		//히트 노말 (NULL이면 대입안됨)
	);

	// 두개의 오브젝트가 겹쳤는지 확인
	bool isOverlap(baseObject* objA, baseObject* objB);

	// 두개의 바운드에 대한 충돌정보를 확인
	bool isOverlap(transform* transA, boundSphere* boundA, transform* transB, boundSphere* boundB);
	bool isOverlap(transform* transA, boundBox* boundA, transform* transB, boundBox* boundB);
	bool isOverlap(transform* transA, boundSphere* boundA, transform* transB, boundBox* boundB);
	bool isOverlap(transform* transA, boundBox* boundA, transform* transB, boundSphere* boundB);

	//
	bool isBlocking(baseObject* objA, baseObject* objB, float moveFactor = 0.0f);
	bool isBlocking(
		transform* transA, boundSphere* boundA,
		transform* transB, boundSphere* boundB,
		float moveFactor = 0.0f);
	bool isBlocking(
		transform* transA, boundBox* boundA,
		transform* transB, boundBox* boundB,
		float moveFactor = 0.0f);

	// 점 3개로 무한 평면 만들기
	void createPlane(LPD3DXPLANE pOutPlane, D3DXVECTOR3* p0, D3DXVECTOR3* p1, D3DXVECTOR3* p2);
	// 임의에 한점에서 평면까지의 최단거리
	float planeDot(LPD3DXPLANE plane, D3DXVECTOR3* point);
	// 반직선과 평면의 충돌위치
	bool intersectRayToPlane(D3DXVECTOR3* pOut, LPRay ray, LPD3DXPLANE plane, bool checkToSide = false);

	physicsManager() {};
	~physicsManager() {};
};