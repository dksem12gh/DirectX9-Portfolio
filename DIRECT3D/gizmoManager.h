#pragma once
#include "singletonBase.h"

#define CIRCLEGIZMOMAX 36

class gizmoManager : public singletonBase <gizmoManager>
{
private :
	typedef struct tagGizmoVertex
	{
		D3DXVECTOR3 pos;	//  정점의 위치
		DWORD		color;	//  정점의 컬러
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	}GIZMOVERTEX, *LPGIZMOVERTEX;

private :
	D3DXVECTOR3		_circlePos[CIRCLEGIZMOMAX + 1];		//  원의 정점 위치
	


public:
	HRESULT		init(void);
	void		release(void);
	
	
	
	//  선을 그리는 함수
	void lineMake(D3DXVECTOR3& startPos, D3DXVECTOR3& endPos, DWORD color = 0xff808080);

	//  그리드 그리는 함수
	void gridMake(float cellSize, int cellNum);

	//  원을 그리는 함수
	void circleMake(D3DXVECTOR3& centerPos, float radius, D3DXVECTOR3& axis = D3DXVECTOR3(0, 0, 1), DWORD color = 0xffff00ff);

	//  와이어 구를 그리는 함수
	void sphereMake(D3DXVECTOR3& centerPos, float radius, DWORD color = 0xffffff00);

	//  월드 AABB를 그리는 함수
	void aabbMake(D3DXVECTOR3& minPos, D3DXVECTOR3& maxPos, DWORD color = 0xffff0000);

	gizmoManager() {};
	~gizmoManager() {};



};

