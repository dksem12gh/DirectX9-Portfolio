#include "stdafx.h"
#include "MYUTIL.h"

LPDIRECT3DVERTEXBUFFER9 MYUTIL::_vb;				//정점 버퍼
LPDIRECT3DINDEXBUFFER9 MYUTIL::_ib;					//인덱스 버퍼

void MYUTIL::createCube()
{
	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |  *   | |     ==> 중점(0, 0, 0)
	//    | 1------| 2
	//    |/       |/
	//    0--------3
	//정점 버퍼 만들기

	device->CreateVertexBuffer(sizeof(MYVERTEX) * 24, 0, MYVERTEX::FVF, D3DPOOL_MANAGED, &_vb, NULL);

	//쿼드 플랜 정점( 정점 4개를 돌려서 찌글 예정)
	MYVERTEX quad[4];

	quad[0].pos = D3DXVECTOR3(-1, -1, -1);
	quad[1].pos = D3DXVECTOR3(-1, 1, -1);
	quad[2].pos = D3DXVECTOR3(1, -1, -1);
	quad[3].pos = D3DXVECTOR3(1, 1, -1);


	//4개면을 회전하면서 돌리기 위한 매트릭스

	//정점배열 준비
	MYVERTEX vertices[24];

	//앞면 복사
	{
		memcpy(vertices, quad, sizeof(MYVERTEX) * 4);
		vertices[0].color = 0xffff0000;
		vertices[1].color = 0xffff0000;
		vertices[2].color = 0xffff0000;
		vertices[3].color = 0xffff0000;
	}

	D3DXMATRIXA16 matRot;

	//좌측면 복사
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationY(&matRot, D3DXToRadian(90));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[4], quad, sizeof(MYVERTEX) * 4);
		vertices[4].color = 0xff00ffff;
		vertices[5].color = 0xff00ff00;
		vertices[6].color = 0xff00ff00;
		vertices[7].color = 0xff00ff00;
	}



	//우측 복사
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationY(&matRot, D3DXToRadian(180));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[8], quad, sizeof(MYVERTEX) * 4);
		vertices[8].color = 0xff00ff00;
		vertices[9].color = 0xff00ff00;
		vertices[10].color = 0xff00ff00;
		vertices[11].color = 0xff00ff00;
	}



	//뒷면 복사	
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationY(&matRot, D3DXToRadian(270));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[12], quad, sizeof(MYVERTEX) * 4);
		vertices[12].color = 0xffff0000;
		vertices[13].color = 0xffff0000;
		vertices[14].color = 0xffff0000;
		vertices[15].color = 0xffff0000;
	}


	//윗면 복사
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationX(&matRot, D3DXToRadian(-90));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[16], quad, sizeof(MYVERTEX) * 4);
		vertices[16].color = 0xff0000ff;
		vertices[17].color = 0xff0000ff;
		vertices[18].color = 0xff0000ff;
		vertices[19].color = 0xff0000ff;
	}

	//밑면 복사
	{
		D3DXMatrixIdentity(&matRot);
		D3DXMatrixRotationX(&matRot, D3DXToRadian(180));

		for (int i = 0; i < 4; i++)
		{
			D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
		}
		memcpy(&vertices[20], quad, sizeof(MYVERTEX) * 4);
		vertices[20].color = 0xff0000ff;
		vertices[21].color = 0xff0000ff;
		vertices[22].color = 0xff0000ff;
		vertices[23].color = 0xff0000ff;
	}


	//정점 정보 버텍스버퍼에 넣어주기
	void* pVertices;

	_vb->Lock(0, 0, (void**)&pVertices, 0);
	memcpy(pVertices, vertices, sizeof(MYVERTEX) * 24);
	_vb->Unlock();

	//인덱스 버퍼 생성
	device->CreateIndexBuffer(sizeof(DWORD), 0, D3DFMT_INDEX32, D3DPOOL_MANAGED, &_ib, NULL);

	//인덱스 정보

	_ib->Lock(0, 0, (void**)&pVertices, 0);
	_ib->Unlock();
}

void MYUTIL::renderCube()
{
	device->SetFVF(MYVERTEX::FVF);
	device->SetStreamSource(0, _vb, 0, sizeof(MYVERTEX));
	for (int i = 0; i < 6; i++)
	{
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}
}

void MYUTIL::releaseVI()
{
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_ib);
}

void MYUTIL::createNormalCube()
{
	//정점 버퍼 만들기
	device->CreateVertexBuffer(
		sizeof(MYVERTEX_PN) * 24,	//만들어질 정점버퍼의 크기(삼각형은 정점 3개 필요)
		D3DUSAGE_WRITEONLY,			//만들어지는 정점버퍼의 속성(일단 writeonly가 접근이 빠르다, 후에 정점버퍼에 접근이 자주 일어난다면 다이내믹으로 변경하자)
		MYVERTEX_PN::FVF,			//FVF정보
		D3DPOOL_DEFAULT,			//메모리풀 보관방식
		&_vb,						//생성된 버텍버퍼
		NULL);						//그냥 묻지도 따지지도 말고 NULL

									//쿼드 플랜 정점
	MYVERTEX_PN quad[4];
	quad[0].pos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	quad[1].pos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	quad[2].pos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	quad[3].pos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);

	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, D3DXToRadian(-90));

	//정점 배열 준비
	MYVERTEX_PN vertices[24];

	//앞면
	memcpy(vertices, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[0].normal = D3DXVECTOR3(0, 0, -1);
	vertices[1].normal = D3DXVECTOR3(0, 0, -1);
	vertices[2].normal = D3DXVECTOR3(0, 0, -1);
	vertices[3].normal = D3DXVECTOR3(0, 0, -1);

	//4개 정점 돌려기
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//우측면 복사
	memcpy(vertices + 4, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[4].normal = D3DXVECTOR3(1, 0, 0);
	vertices[5].normal = D3DXVECTOR3(1, 0, 0);
	vertices[6].normal = D3DXVECTOR3(1, 0, 0);
	vertices[7].normal = D3DXVECTOR3(1, 0, 0);

	//4개 정점 돌려기
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//뒷면 복사
	memcpy(vertices + 8, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[8].normal = D3DXVECTOR3(0, 0, 1);
	vertices[9].normal = D3DXVECTOR3(0, 0, 1);
	vertices[10].normal = D3DXVECTOR3(0, 0, 1);
	vertices[11].normal = D3DXVECTOR3(0, 0, 1);

	//4개 정점 돌려기
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//좌측면 복사
	memcpy(vertices + 12, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[12].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[13].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[14].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[15].normal = D3DXVECTOR3(-1, 0, 0);

	D3DXMatrixRotationZ(&matRot, D3DXToRadian(-90));

	//4개 정점 돌려기
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//윗면 복사
	memcpy(vertices + 16, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[16].normal = D3DXVECTOR3(0, 1, 0);
	vertices[17].normal = D3DXVECTOR3(0, 1, 0);
	vertices[18].normal = D3DXVECTOR3(0, 1, 0);
	vertices[19].normal = D3DXVECTOR3(0, 1, 0);

	D3DXMatrixRotationZ(&matRot, D3DXToRadian(-180));

	//4개 정점 돌려기
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);
	}

	//아래면 복사
	memcpy(vertices + 20, quad, sizeof(MYVERTEX_PN) * 4);
	vertices[20].normal = D3DXVECTOR3(0, -1, 0);
	vertices[21].normal = D3DXVECTOR3(0, -1, 0);
	vertices[22].normal = D3DXVECTOR3(0, -1, 0);
	vertices[23].normal = D3DXVECTOR3(0, -1, 0);

	//정점 락
	void* pVertices = NULL;
	_vb->Lock(0, 0, &pVertices, 0);
	//메모리 복사
	memcpy(pVertices, vertices, sizeof(MYVERTEX_PN) * 24);
	_vb->Unlock();

	//인덱스 버퍼 생성
	device->CreateIndexBuffer(
		sizeof(WORD) * 36,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&_ib,
		NULL);

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	WORD indices[36];
	for (int i = 0; i < 6; i++)
	{
		//정점 스타트 인덱스
		WORD vertexIndex = i * 4;
		//인덱스 배열 스타트 인덱스
		WORD indicesIndex = i * 6;

		indices[indicesIndex + 0] = vertexIndex + 0;
		indices[indicesIndex + 1] = vertexIndex + 1;
		indices[indicesIndex + 2] = vertexIndex + 2;
		indices[indicesIndex + 3] = vertexIndex + 0;
		indices[indicesIndex + 4] = vertexIndex + 2;
		indices[indicesIndex + 5] = vertexIndex + 3;
	}

	//인덱스 락
	void* pIndices = NULL;
	_ib->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, indices, sizeof(WORD) * 36);
	_ib->Unlock();
}

void MYUTIL::renderNormalCube()
{
	//버텍스버퍼 세팅
	device->SetStreamSource(0, _vb, 0, sizeof(MYVERTEX_PN));
	//인덱스버퍼 세팅
	device->SetIndices(_ib);
	//지금 그리는 정점에 대한 정보플래그 세팅
	device->SetFVF(MYVERTEX_PN::FVF);
	//그리기
	device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,	//그릴 도형 방식
		0,					//버텍스배열 시작
		0,					//0일때 처음부터 끝까지
		24,					//정점 개수
		0,					//인덱스배열 시작
		12					//그리는 개체수(삼각형 12개)
	);
}

void MYUTIL::createQuadCube()
{
	//정점버퍼 생성
	device->CreateVertexBuffer(
		sizeof(MYVERTEX_PU) * 4,
		D3DUSAGE_WRITEONLY,
		MYVERTEX_PU::FVF,
		D3DPOOL_DEFAULT,
		&_vb,
		NULL);

	// 1---2
	// |  /|
	// | / |
	// 0---3
	LPMYVERTEX_PU pVertices = NULL;
	_vb->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0].pos = D3DXVECTOR3(-1, -1, 0);
	pVertices[1].pos = D3DXVECTOR3(-1, 1, 0);
	pVertices[2].pos = D3DXVECTOR3(1, 1, 0);
	pVertices[3].pos = D3DXVECTOR3(1, -1, 0);
	//UV좌표는 0.0f ~ 1.0f (윈도우좌표계와 동일 lt(0,0), rt(1,0), lb(0,1), rb(1,1))
	//최대값이 1인데 2로하면 총 4개의 이미지, 3으로하면 총 9개의 이미지가 나온다
	pVertices[0].uv = D3DXVECTOR2(0, 1);
	pVertices[1].uv = D3DXVECTOR2(0, 0);
	pVertices[2].uv = D3DXVECTOR2(1, 0);
	pVertices[3].uv = D3DXVECTOR2(1, 1);
	_vb->Unlock();

	// 1---2
	// |  /|
	// | / |
	// 0---3
	//인덱스버퍼 생성
	device->CreateIndexBuffer(
		sizeof(DWORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&_ib,
		NULL);

	DWORD idx[] = { 0, 1, 2, 0, 2, 3 };

	void* pIndices = NULL;
	_ib->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, idx, sizeof(DWORD) * 6);
	_ib->Unlock();
}

void MYUTIL::renderQuadCube()
{
	//버텍스버퍼 세팅
	device->SetStreamSource(0, _vb, 0, sizeof(MYVERTEX_PU));
	//인덱스버퍼 세팅
	device->SetIndices(_ib);
	//지금 그리는 정점에 대한 정보플래그 세팅
	device->SetFVF(MYVERTEX_PU::FVF);
	//그리기 (4개 정점 삼각형 2개)
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

POINT MYUTIL::getMousePos()
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(_hWnd, &pt);
	return pt;
}

void MYUTIL::setMousePos(int x, int y)
{
	POINT pt = { x,y };
	ClientToScreen(_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

float MYUTIL::clamp(float value, float min, float max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
	return 0.0f;
}

float MYUTIL::clamp01(float value)
{
	if (value < 0)
	{
		return 0;
	}
	else if (value > 1)
	{
		return 1;
	}
	else
	{
		return value;
	}
	return 0.0f;
}

float MYUTIL::clampMinusPlusOne(float value)
{
	if (value < -1)
	{
		return -1;
	}
	else if (value > 1)
	{
		return 1;
	}
	else
	{
		return value;
	}
	return 0.0f;
}

float MYUTIL::lerp(float from, float to, float time)
{
	time = clamp01(time);
	float delta = to - from;

	return (delta * time + from);
}

D3DXVECTOR3 MYUTIL::vecLerp(D3DXVECTOR3 from, D3DXVECTOR3 to, float time)
{
	D3DXVECTOR3 result;
	result.x = lerp(from.x, to.x, time);
	result.y = lerp(from.y, to.y, time);
	result.z = lerp(from.z, to.z, time);

	//D3DXVec3Lerp(&result, &from, &to, time);
	return result;
}

float MYUTIL::inverseLerp(float head, float rear, float value)
{
	float delta = rear - head;
	float inValue = value - head;

	if (inValue < 0)
	{
		inValue = 0;
	}

	return inValue / delta;
}

void MYUTIL::initForRandomSeed()
{
	srand(GetTickCount());
}

int MYUTIL::randomIntRange(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

float MYUTIL::randomFloatRange(float min, float max)
{
	//float rnd = (float)rand() / (float)RAND_MAX;
	//return (rnd * (max - min) + min);

	float delta = max - min;
	float factor = (rand() % 1001) * 0.001f;
	return (delta * factor) + min;
}

string MYUTIL::intToString(int number)
{
	char buffer[256];

	_itoa(number, buffer, 10);
	return string(buffer);
}

string MYUTIL::floatToString(float number)
{
	char buffer[128];
	sprintf(buffer, "%f", number);

	return string(buffer);
}

void MYUTIL::ComputeNormal(D3DXVECTOR3 * pNorams, const D3DXVECTOR3 * pVertices, int verticesNum, const DWORD * pIndices, int indicesNum)
{
	//노말벡터 초기화
	ZeroMemory(pNorams, sizeof(D3DXVECTOR3) * verticesNum);

	//삼각형 갯수
	DWORD triNum = indicesNum / 3;
	for (DWORD i = 0; i < triNum; i++)
	{
		//해당 삼각형의 정점 인덱스
		DWORD i0 = pIndices[(i * 3) + 0];
		DWORD i1 = pIndices[(i * 3) + 1];
		DWORD i2 = pIndices[(i * 3) + 2];

		//정점 3개
		D3DXVECTOR3 v0 = pVertices[i0];
		D3DXVECTOR3 v1 = pVertices[i1];
		D3DXVECTOR3 v2 = pVertices[i2];

		//Edge
		D3DXVECTOR3 edge1 = v1 - v0;
		D3DXVECTOR3 edge2 = v2 - v0;

		//Cross
		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &edge1, &edge2);

		//Normal
		D3DXVECTOR3 normal;
		D3DXVec3Normalize(&normal, &cross);

		//증가 시킨다.
		pNorams[i0] += normal;
		pNorams[i1] += normal;
		pNorams[i2] += normal;
	}

	//최종 적으로 Normalvector 정규화한다.
	for (DWORD i = 0; i < verticesNum; i++)
	{
		D3DXVec3Normalize(&pNorams[i], &pNorams[i]);
	}
}

void MYUTIL::ComputeTangentBinormal(D3DXVECTOR3 * outTangets, D3DXVECTOR3 * outBinormals, const D3DXVECTOR3 * pPositions, const D3DXVECTOR3 * pNormals, const D3DXVECTOR2 * pUVs, const DWORD * pIndices, DWORD NumTris, DWORD NumVertices)
{
	//탄젠트 바이노말 초기화
	ZeroMemory(outTangets, sizeof(D3DXVECTOR3) * NumVertices);
	ZeroMemory(outBinormals, sizeof(D3DXVECTOR3) * NumVertices);

	//임시 Tangent Binormal 배열
	D3DXVECTOR3* pTangents = new D3DXVECTOR3[NumVertices];
	D3DXVECTOR3* pBinormals = new D3DXVECTOR3[NumVertices];

	//임시 Tangent Binormal 배열 초기화
	ZeroMemory(pTangents, sizeof(D3DXVECTOR3) * NumVertices);
	ZeroMemory(pBinormals, sizeof(D3DXVECTOR3) * NumVertices);

	//일단 삼각형 수대로....
	for (DWORD a = 0; a < NumTris; a++)
	{
		//해당 삼각형의 인덱스
		DWORD i0 = pIndices[a * 3 + 0];
		DWORD i1 = pIndices[a * 3 + 1];
		DWORD i2 = pIndices[a * 3 + 2];

		//해당 삼각형의 정점위치
		D3DXVECTOR3 p0 = pPositions[i0];
		D3DXVECTOR3 p1 = pPositions[i1];
		D3DXVECTOR3 p2 = pPositions[i2];

		//해당 삼각형의 UV
		D3DXVECTOR2 uv0 = pUVs[i0];
		D3DXVECTOR2 uv1 = pUVs[i1];
		D3DXVECTOR2 uv2 = pUVs[i2];

		//각변의 Edge
		D3DXVECTOR3 edge1 = p1 - p0;
		D3DXVECTOR3 edge2 = p2 - p0;

		//UV Edge
		D3DXVECTOR2 uvEdge1 = uv1 - uv0;
		D3DXVECTOR2 uvEdge2 = uv2 - uv0;

		// 위의 정보로 다음과 같은 공식이 성립
		// edge1 = ( uvEdge1.x ) * Tangent + ( uvEdge1.y ) * Binormal;
		// edge2 = ( uvEdge2.x ) * Tangent + ( uvEdge2.y ) * Binormal;

		// 다음과 같이 치환
		// E1 = edge1;
		// E2 = edge2;
		// U1 = uvEdge1.x;
		// V1 = uvEdge1.y;
		// U2 = uvEdge2.x;
		// V2 = uvEdge2.y;
		// T = Tangent;
		// B = Binormal;

		// E1 = U1 * T + V1 * B;
		// E2 = U2 * T + V2 * B;

		// | E1 |   | U1 , V1 | | T |
		// |    | = |         | |   |
		// | E2 |   | U2 , V2 | | B |

		// | T |          1        |  V2 , -V1 | | E1 |
		// |   | = --------------- |           | |    |
		// | B |    U1*V2 - V1*U2  | -U2 ,  U1 | | E2 |

		// R = 1 / U1*V2 - V1*U2;

		// T = ( ( E1 * V2 ) + ( E2 * -V1 ) ) * R
		// B = ( ( E1 * -U2 ) + ( E2 * U1 ) ) * R

		float r = 1.0f / ((uvEdge1.x * uvEdge2.y) - (uvEdge1.y * uvEdge2.x));

		//Tangent
		D3DXVECTOR3 t = ((edge1 * uvEdge2.y) + (edge2 * -uvEdge1.y)) * r;
		D3DXVec3Normalize(&t, &t);

		//Binormal 
		D3DXVECTOR3 b = ((edge1 * -uvEdge2.x) + (edge2 * uvEdge1.x)) * r;
		D3DXVec3Normalize(&b, &b);

		//탄젠트 바이노말 임시 배열에 추가
		pTangents[i0] += t;
		pTangents[i1] += t;
		pTangents[i2] += t;
		pBinormals[i0] += b;
		pBinormals[i1] += b;
		pBinormals[i2] += b;
	}

	//Binormal Tangent 노말에 직교화
	for (int i = 0; i < NumVertices; i++)
	{
		D3DXVECTOR3 n = pNormals[i];

		//Tangent 그람슈미트 직교
		D3DXVECTOR3 t = pTangents[i] - (D3DXVec3Dot(&pTangents[i], &n) * n);
		D3DXVec3Normalize(&t, &t);

		//노말과 직교화된 T 와 외적하여 B
		D3DXVECTOR3 b;
		D3DXVec3Cross(&b, &n, &t);
		D3DXVec3Normalize(&b, &b);

		outTangets[i] = t;
		outBinormals[i] = b;
	}

	//임시 배열 삭제
	SAFE_DELETE_ARRAY(pTangents);
	SAFE_DELETE_ARRAY(pBinormals);
}

RECT MYUTIL::rectMake(int destX, int destY, int width, int height)
{
	RECT newRect = {
		destX,
		destY,
		destX + width,
		destY + height
	};

	return newRect;
}

POINT MYUTIL::getRectSize(LPRECT rect)
{
	POINT size;

	size.x = rect->right - rect->left;
	size.y = rect->bottom - rect->top;

	return size;
}
