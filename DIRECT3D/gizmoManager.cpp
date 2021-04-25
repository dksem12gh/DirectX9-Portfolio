#include "stdafx.h"
#include "gizmoManager.h"

HRESULT gizmoManager::init(void)
{
	//  구를 위한 기본 준비
	float intervalAngle = (D3DX_PI * 2.0f) / CIRCLEGIZMOMAX;

	//  원을 그리기 위한 정점 위치 셋팅
	for (int i = 0; i <= CIRCLEGIZMOMAX; i++)
	{
		float angle = i * intervalAngle;
		_circlePos[i] = D3DXVECTOR3(cosf(angle), sinf(angle), 0.0f);
	}

	return S_OK;
}

void gizmoManager::release(void)
{
}

void gizmoManager::lineMake(D3DXVECTOR3 & startPos, D3DXVECTOR3 & endPos, DWORD color)
{
	// 이전의 월드행렬 기억하기
	D3DXMATRIXA16 matPrevWorld;
	device->GetTransform(D3DTS_WORLD, &matPrevWorld);

	// 월드행렬 초기화
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	device->SetTransform(D3DTS_WORLD, &matWorld);

	// 이전의 라이트 모드 기억하기
	DWORD prevLight = 0;
	device->GetRenderState(D3DRS_LIGHTING, &prevLight);

	// 라이트 기능 오프
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 임시 정점 배열 2개
	GIZMOVERTEX vertices[2];
	vertices[0].pos = startPos;
	vertices[1].pos = endPos;
	vertices[0].color = color;
	vertices[1].color = color;

	// 정점 그리기
	device->SetFVF(GIZMOVERTEX::FVF);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertices, sizeof(GIZMOVERTEX));

	// 이부분은 조금있다 시험해보기
	// 월드 다시 원상복구
	device->SetTransform(D3DTS_WORLD, &matPrevWorld);
	// 라이트 모드 원상복구
	device->SetRenderState(D3DRS_LIGHTING, prevLight);
}

void gizmoManager::gridMake(float cellSize, int cellNum)
{
	// 축의 절반 길이
	float axisLineLength = cellSize * cellNum;

	// x축 라인
	D3DXVECTOR3 xStart(-axisLineLength, 0, 0);
	D3DXVECTOR3 xEnd(axisLineLength, 0, 0);

	// y축 라인
	D3DXVECTOR3 yStart(0, -axisLineLength, 0);
	D3DXVECTOR3 yEnd(0, axisLineLength, 0);

	// z축 라인
	D3DXVECTOR3 zStart(0, 0, -axisLineLength);
	D3DXVECTOR3 zEnd(0, 0, axisLineLength);

	// 축그린다
	this->lineMake(xStart, xEnd, 0xffff0000); // 빨
	this->lineMake(yStart, yEnd, 0xff00ff00); // 녹
	this->lineMake(zStart, zEnd, 0xff0000ff); // 파

											  // 축화살표
	D3DXVECTOR3 arrowPos[4];
	arrowPos[0] = D3DXVECTOR3(-1.0f, 0.0f, -1.0f);
	arrowPos[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	arrowPos[2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	arrowPos[3] = D3DXVECTOR3(1.0f, 0.0f, -1.0f);

	D3DXVECTOR3 arrowVertices[4];

	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, zEnd.x, zEnd.y, zEnd.z);
	D3DXMATRIXA16 matRotation;
	D3DXMATRIXA16 matFinal = matTrans;

	// z변환
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matTrans);
	}
	// 그린다
	this->lineMake(arrowVertices[0], arrowVertices[1], 0xff0000ff);
	this->lineMake(arrowVertices[2], arrowVertices[3], 0xff0000ff);

	// x축
	D3DXMatrixTranslation(&matTrans, xEnd.x, xEnd.y, xEnd.z);
	D3DXMatrixRotationY(&matRotation, D3DXToRadian(90));
	matFinal = matRotation * matTrans;

	// x변환
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);
	}
	// 그린다
	this->lineMake(arrowVertices[0], arrowVertices[1], 0xffff0000);
	this->lineMake(arrowVertices[2], arrowVertices[3], 0xffff0000);

	// y축
	D3DXMatrixTranslation(&matTrans, yEnd.x, yEnd.y, yEnd.z);
	D3DXMatrixRotationX(&matRotation, D3DXToRadian(-90));
	matFinal = matRotation * matTrans;
	// y변환
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);
	}
	// 그린다
	this->lineMake(arrowVertices[0], arrowVertices[1], 0xff00ff00);
	this->lineMake(arrowVertices[2], arrowVertices[3], 0xff00ff00);

	// 월드 그리드 그린다
	for (int i = -cellNum; i <= cellNum; i++)
	{
		// 0은 넘긴다
		if (i == 0) continue;

		D3DXVECTOR3 posStartX(i * cellSize, 0, -axisLineLength);
		D3DXVECTOR3 posEndX(i * cellSize, 0, axisLineLength);

		this->lineMake(posStartX, posEndX);

		D3DXVECTOR3 posStartZ(-axisLineLength, 0, i * cellSize);
		D3DXVECTOR3 posEndZ(axisLineLength, 0, i * cellSize);

		this->lineMake(posStartZ, posEndZ);
	}
}

void gizmoManager::circleMake(D3DXVECTOR3 & centerPos, float radius, D3DXVECTOR3 & axis, DWORD color)
{

	// 이전의 월드행렬 기억하기
	D3DXMATRIXA16 matPrevWorld;
	device->GetTransform(D3DTS_WORLD, &matPrevWorld);

	// 이전의 라이트 모드 기억하기
	DWORD prevLight = 0;
	device->GetRenderState(D3DRS_LIGHTING, &prevLight);

	// 라이트 기능 오프
	device->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 임시 정점 배열 2개
	GIZMOVERTEX vertices[CIRCLEGIZMOMAX + 1];
	for (int i = 0; i < CIRCLEGIZMOMAX + 1; i++)
	{
		vertices[i].pos = _circlePos[i];
		vertices[i].color = color;
	}

	// 원에 대한 행렬 준비
	// 회전준비
	D3DXVECTOR3 forward = axis;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up(0, 1, 0);
	if (forward == up) up = D3DXVECTOR3(0, 0, -1);
	else if (forward == D3DXVECTOR3(0, -1, 0)) up = D3DXVECTOR3(0, 0, 1);

	// 벡터의 외적을 통해 두벡터의 수직인 법선벡터를 알수 있다
	D3DXVec3Cross(&right, &up, &forward);
	D3DXVec3Cross(&up, &forward, &right);

	// 축 스케일
	forward = forward * radius;
	right = right * radius;
	up = up * radius;

	// 월드행렬 초기화
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	memcpy(&matWorld._11, right, sizeof(D3DXVECTOR3));
	memcpy(&matWorld._21, up, sizeof(D3DXVECTOR3));
	memcpy(&matWorld._31, forward, sizeof(D3DXVECTOR3));
	memcpy(&matWorld._41, centerPos, sizeof(D3DXVECTOR3));

	device->SetTransform(D3DTS_WORLD, &matWorld);

	// 정점 그리기
	device->SetFVF(GIZMOVERTEX::FVF);
	device->DrawPrimitiveUP(D3DPT_LINESTRIP, CIRCLEGIZMOMAX, vertices, sizeof(GIZMOVERTEX));

	// 월드 다시 원상복구
	device->SetTransform(D3DTS_WORLD, &matPrevWorld);
	// 라이트 모드 원상복구
	device->SetRenderState(D3DRS_LIGHTING, prevLight);
}

void gizmoManager::sphereMake(D3DXVECTOR3 & centerPos, float radius, DWORD color)
{
	circleMake(centerPos, radius, D3DXVECTOR3(1, 0, 0), color);
	circleMake(centerPos, radius, D3DXVECTOR3(0, 1, 0), color);
	circleMake(centerPos, radius, D3DXVECTOR3(0, 0, 1), color);
}

void gizmoManager::aabbMake(D3DXVECTOR3 & minPos, D3DXVECTOR3 & maxPos, DWORD color)
{

	// 		5--------6
	//      /|       /|
	//     4--------7 |
	//     | |  *   | |     ==> 중점(0, 0, 0)
	//     | 1------| 2
	//     |/       |/
	//     0--------3
	lineMake(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), color);
	lineMake(D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), D3DXVECTOR3(minPos.x, minPos.y, minPos.z), color);

	lineMake(D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), color);

	lineMake(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	lineMake(D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
}
