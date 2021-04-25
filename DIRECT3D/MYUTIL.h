//MYYUTIL.h
//2017 - 12 - 20
#pragma once

#define EPSILON 0.0001f
//플롯트 값끼리 비교할 때 엡실론 이하의 오차는 같다
//a = 10.00001f
//b = 10.00008f
//abs(a - b) < c  => a ,b 는 같다

#define FLOATEQUAL(a,b)			( abs( (a) - (b) ) < EPSILON )									// a와 b 가 같은가
#define FLOATZERO(a)			(abs(a)  < EPSILON)												// a의 값이 0 과 같은지
#define VECTORZERO(v)			(FLOATZERO((v).x) && FLOATZERO((v).y) && FLOATZERO((v).z))		//

//레이 구조체
typedef struct tagRay
{
	D3DXVECTOR3 origin;		//직선의 시작위치
	D3DXVECTOR3	direction;	//직선의 방향(정규화 되어 있어야 함)
}Ray, *LPRay;

namespace MYUTIL
{
	//==================================================================
	//		## 큐브 만들기 ##
	//==================================================================
	extern LPDIRECT3DVERTEXBUFFER9 _vb;				//정점 버퍼
	extern LPDIRECT3DINDEXBUFFER9 _ib;				//인덱스 버퍼
	void createCube();								//큐브 생성	
	void renderCube();								//큐브 그리기
	void releaseVI();								//큐브 해제

	void createNormalCube();						//노말큐브 생성
	void renderNormalCube();						//큐브 그리기

	void createQuadCube();							//쿼드큐브 생성
	void renderQuadCube();							//쿼드큐브 그리기

	//==================================================================
	//		## 마우스 ##
	//==================================================================
	POINT getMousePos();							//마우스 위치 얻기
	void setMousePos(int x, int y);					//마우스 위치 설정


	//==================================================================
	//		## 선형 보간 ##
	//==================================================================

	float clamp(float value, float min, float max);		//value 값을 min ~ max 사이의 값으로 맞춤		
	float clamp01(float value);							//value 값을 0~1 사이의 값으로 맞춤	
	float clampMinusPlusOne(float value);				//value 값을 -1 ~ 1 사이의 값으로 맞춤

	//float 선형보간
	float lerp(float from, float to, float time);

	//벡터 선형 보간
	D3DXVECTOR3 vecLerp(D3DXVECTOR3 from, D3DXVECTOR3 to, float time);
	float inverseLerp(float head, float rear, float value);


	//==================================================================
	//		## 랜덤 함수 ##
	//==================================================================

	void initForRandomSeed();						//랜덤 시드 생성
	int randomIntRange(int min, int max);			//min ~ max 사이의 랜덤 정수 생성
	float randomFloatRange(float min, float max);	//min ~ max 사이의 랜덤 실수 생성

	//==================================================================
	//		## 기타 함수 ##
	//==================================================================
	string intToString(int number);
	string floatToString(float number);
	RECT   rectMake(int destX, int destY, int width, int height);
	POINT  getRectSize(LPRECT rect);
	//==================================================================
	//		## 노말계산 ##
	//==================================================================
	//정점의 노말을 계산해주는 함수
	void ComputeNormal(
		D3DXVECTOR3* pNorams,			//결과로 받을 노말백터 배열 포인터 ( 노말벡터의 배열의 수는 정점 배열의 수와 같다 )
		const D3DXVECTOR3* pVertices,	//계산에 사용된 정점위치 배열 포인터
		int verticesNum,				//정점 갯수
		const DWORD* pIndices,			//인덱스 배열포인터 ( 일단 인덱스는 DWORD 로 대동단결 )
		int indicesNum					//인덱스 배열포인터 갯수
	);

	//탄젠트와 바이노말 계산
	void ComputeTangentBinormal(
		D3DXVECTOR3* outTangets,			//결과 Tangent 배열
		D3DXVECTOR3* outBinormals,			//결과 Binormal 배열
		const D3DXVECTOR3* pPositions,		//정점 위치 배열
		const D3DXVECTOR3* pNormals,		//정점 노말 배열
		const D3DXVECTOR2* pUVs,			//정점의 UV 배열
		const DWORD* pIndices,				//인덱스 배열
		DWORD NumTris,						//삼각형 갯수		
		DWORD NumVertices 					//정점 갯수
	);

}



