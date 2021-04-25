#include "stdafx.h"
#include "terrain.h"
#include "quadTree.h"

HRESULT terrain::init(int vertexNum, float cellSize)
{
	// 정점수
	_verNumX = vertexNum;
	_verNumZ = vertexNum;
	_totalVerNum = _verNumX * _verNumZ;

	// 셀수
	_cellNumX = _verNumX - 1;
	_cellNumZ = _verNumZ - 1;
	_totalCellNum = _cellNumX * _cellNumZ;

	// 셀사이즈
	_cellScale = cellSize;

	// 폴리곤 개수
	_totalTriangle = _totalCellNum * 2;

	// 지형 사이즈
	_terrainSizeX = _cellNumX * cellSize;
	_terrainSizeZ = _cellNumZ * cellSize;

	// 지형 정보 생성(정점 수대로 정점 위치 정보를 만든다)
	_vertexPosArray = new D3DXVECTOR3[_totalVerNum];

	// 시작 지점은 0,0,0
	D3DXVECTOR3 startPos(0, 0, 0);

	for (int z = 0; z < _verNumZ; z++)
	{
		for (int x = 0; x < _verNumX; x++)
		{
			// 정점 인덱스
			int idx = z * _verNumX + x;
			// 정점 위치
			_vertexPosArray[idx].x = startPos.x + (x * _cellScale);
			_vertexPosArray[idx].z = startPos.z + (z * _cellScale);
			// _vertexPosArray[idx].y = 0;
			_vertexPosArray[idx].y = sinf(x * D3DXToRadian(20)) * 3.0f;
		}
	}

	// 정점 인덱스
	_indices = new TERRAININDEX[_totalTriangle];

	for (int z = 0; z < _cellNumZ; z++)
	{
		// 수평라인
		int nowZ = z;
		int nextZ = z + 1;

		for (int x = 0; x < _cellNumX; x++)
		{
			//수직라인
			int nowX = x;
			int nextX = x + 1;

			// 사각형 모서리(lt, rt, lb, rb)
			//	lt---rt
			//	|	/ |
			//	|  /  |
			//	| /	  |
			//	lb---rb

			// 모서리 정점 인덱스
			DWORD lt = nextZ * _verNumX + nowX;
			DWORD rt = nextZ * _verNumX + nextX;
			DWORD lb = nowZ * _verNumX + nowX;
			DWORD rb = nowZ * _verNumX + nextX;

			// 삼각형 배열인덱스
			int idx = (z * _cellNumX + x) * 2;
			// 셀폴리곤 1
			// lb, lt, rt
			_indices[idx]._0 = lb;
			_indices[idx]._1 = lt;
			_indices[idx]._2 = rt;
			// 셀폴리곤 2
			// lb, rt, rb
			_indices[idx + 1]._0 = lb;
			_indices[idx + 1]._1 = rt;
			_indices[idx + 1]._2 = rb;
		}
	}

	// 정점의 형태를 알려주는 배열
	D3DVERTEXELEMENT9	vertElement[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END() };

	device->CreateVertexDeclaration(
		vertElement,
		&_terrainDecl);

	// 정점 버퍼를 생성
	if (FAILED(
		device->CreateVertexBuffer(
			sizeof(TERRAINVERTEX) * _totalVerNum,
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_DEFAULT,
			&_terrainVb,
			NULL))) return E_FAIL;

	LPTERRAINVERTEX	pVertices = NULL;
	if (SUCCEEDED(_terrainVb->Lock(0, 0, (void**)&pVertices, 0)))
	{
		for (int i = 0; i < _totalVerNum; i++)
		{
			//정점 위치 대입
			pVertices[i].pos = _vertexPosArray[i];
		}
		_terrainVb->Unlock();
	}

	// 인덱스 버퍼 생성
	if (FAILED(device->CreateIndexBuffer(
		sizeof(TERRAININDEX) * _totalTriangle,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&_terrainIb,
		NULL))) return E_FAIL;

	void* pIndices = NULL;

	if (SUCCEEDED(_terrainIb->Lock(0, 0, &pIndices, 0)))
	{
		memcpy(pIndices, _indices, sizeof(TERRAININDEX) * _totalTriangle);
		_terrainIb->Unlock();
	}

	return S_OK;
}

HRESULT terrain::init(char * heightMapName, char * tile_0, char * tile_1, char * tile_2, char * tile_3, char * tileSplat, float cellSize, float heightScale, int smoothLevel, int tileNum)
{
	// 스케일값 대입
	_heightScale = heightScale;
	_cellScale = cellSize;

	// 높이맵 불러온다
	_heightMap = RM_TEXTURE->addResource(heightMapName);
	// 불러오는데 실패하면 바로 종료
	if (_heightMap == NULL) return E_FAIL;

	// 높이 맵에 대한 이미지 정보를 불러온다. (로딩된 Texture 정보를 얻는다)
	D3DSURFACE_DESC sd;
	// GetLevelDesc(밉맵체인 레벨, D3DSURFACE_DESC 구조체 포인터)
	_heightMap->GetLevelDesc(0, &sd);

	// 가로세로 정점 수를 구한다.
	_verNumX = sd.Width + 1;	// 가로 정점갯수는 높이 맵에 가로 해상도 + 1 과 같다. (이유는 쿼드트리쓰려면 정점갯수가 2의N승 + 1 이여야 하기 때문에)
	_verNumZ = sd.Height + 1;	// 세로 정점잿수는 높이 맵에 세로 해상도 + 1 과 같다. (이유는 쿼드트리쓰려면 정점갯수가 2의N승 + 1 이여야 하기 때문에)
	_totalVerNum = _verNumX * _verNumZ;	// 총 정점 갯수

	// 가로세로 셀수
	_cellNumX = _verNumX - 1;
	_cellNumZ = _verNumZ - 1;
	_totalCellNum = _cellNumX * _cellNumZ;

	// 총 삼각형수는
	_totalTriangle = _totalCellNum * 2;

	// 터레인을 만든다.
	// 스무싱 레벨 (이값이 클수록 지형이 부드러워진다)
	// 타일 Texture 가 몇개로 나누어질건지 갯수
	if (FAILED(createTerrain(smoothLevel, tileNum)))
	{
		release();
		return E_FAIL;
	}

	// 터레인 크기
	_terrainSizeX = _cellNumX * _cellScale;
	_terrainSizeZ = _cellNumZ * _cellScale;

	// 터레인 범위
	_terrainStartX = _terrainVertices[0].pos.x;
	_terrainStartZ = _terrainVertices[0].pos.z;
	_terrainEndX = _terrainVertices[_totalVerNum - 1].pos.x;
	_terrainEndZ = _terrainVertices[_totalVerNum - 1].pos.z;

	// 쿼드트리를 만든다.
	_quadTree = new quadTree;
	_quadTree->init(_terrainVertices, _verNumX);

	// 지형 텍스쳐 로딩
	_texTile_0 = RM_TEXTURE->addResource(tile_0);
	_texTile_1 = RM_TEXTURE->addResource(tile_1);
	_texTile_2 = RM_TEXTURE->addResource(tile_2);
	_texTile_3 = RM_TEXTURE->addResource(tile_3);
	_texSlat = RM_TEXTURE->addResource(tileSplat);

	// 지형 셰이더이펙트 로딩
	_terrainEffect = RM_SHADER->addResource("Resources/Shader/TerrainBase.fx");

	return S_OK;
}

void terrain::release(void)
{
	SAFE_RELEASE(_terrainVb);
	SAFE_RELEASE(_terrainIb);
	SAFE_DELETE_ARRAY(_vertexPosArray);
	SAFE_DELETE_ARRAY(_indices);
}

void terrain::update(void)
{
}

void terrain::render(void)
{
	//와이어프레임 모드
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//월드행렬 초기화
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	device->SetTransform(D3DTS_WORLD, &matWorld);

	//정점 그리기
	device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
	device->SetIndices(_terrainIb);
	device->SetVertexDeclaration(_terrainDecl);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);

	//솔리드모드로 전환(정점을 그리고 난 이후 복원)
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void terrain::render(camera * cam, lightDirection * directionLight)
{
	//월드 행렬셋팅
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	//셰이더이펙트 월드행렬 세팅
	_terrainEffect->SetMatrix("matWorld", &matWorld);
	//셰이더이펙트 뷰행렬 세팅
	_terrainEffect->SetMatrix("matViewProjection", &cam->getViewProjectionMatrix());
	//셰이더이펙트 텍스쳐 세팅
	_terrainEffect->SetTexture("Terrain0_Tex", _texTile_0);
	_terrainEffect->SetTexture("Terrain1_Tex", _texTile_1);
	_terrainEffect->SetTexture("Terrain2_Tex", _texTile_2);
	_terrainEffect->SetTexture("Terrain3_Tex", _texTile_3);
	_terrainEffect->SetTexture("TerrainControl_Tex", _texSlat);
	//셰이더이펙트 광원 세팅
	D3DXVECTOR3 dirLight = directionLight->_transform.getForward();
	_terrainEffect->SetVector("worldLightDir", &D3DXVECTOR4(dirLight, 1));

	//셰이더 렌더
	UINT passNum = 0;
	_terrainEffect->Begin(&passNum, 0);
	for (UINT i = 0; i < passNum; i++)
	{
		_terrainEffect->BeginPass(i);
		device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
		device->SetVertexDeclaration(_terrainDecl);
		device->SetIndices(_terrainIb);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);
		_terrainEffect->EndPass();
	}
	_terrainEffect->End();

	/*
	//정점렌더
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
	device->SetIndices(_terrainIb);
	device->SetVertexDeclaration(_terrainDecl);
	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	*/
}

void terrain::render(camera * cam, lightDirection * directionLight, camera * directionLightCamera)
{
	//ShadowMap 셋팅
	_terrainEffect->SetTexture("Shadow_Tex", directionLightCamera->getRenderTexture());

	//ShadowViewProjection 셋팅 
	_terrainEffect->SetMatrix("matLightViewProjection", &directionLightCamera->getViewProjectionMatrix());

	//월드 행렬셋팅
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	_terrainEffect->SetMatrix("matWorld", &matInd);

	//뷰 행렬셋팅
	_terrainEffect->SetMatrix("matViewProjection", &cam->getViewProjectionMatrix());

	//Texture 셋팅
	_terrainEffect->SetTexture("Terrain0_Tex", _texTile_0);
	_terrainEffect->SetTexture("Terrain1_Tex", _texTile_1);
	_terrainEffect->SetTexture("Terrain2_Tex", _texTile_2);
	_terrainEffect->SetTexture("Terrain3_Tex", _texTile_3);
	_terrainEffect->SetTexture("TerrainControl_Tex", _texSlat);

	//광원 셋팅
	D3DXVECTOR3 dirLight = directionLight->_transform.getForward();
	_terrainEffect->SetVector("worldLightDir", &D3DXVECTOR4(dirLight, 1));

	_terrainEffect->SetTechnique("ReciveShadow");

	UINT passNum = 0;
	_terrainEffect->Begin(&passNum, 0);
	for (UINT i = 0; i < passNum; i++)
	{
		_terrainEffect->BeginPass(i);
		device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
		device->SetVertexDeclaration(this->_terrainDecl);
		device->SetIndices(_terrainIb);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);
		_terrainEffect->EndPass();
	}
	_terrainEffect->End();
}

void terrain::renderShadow(camera * directionLightCam)
{
	//월드 행렬셋팅
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	_terrainEffect->SetMatrix("matWorld", &matInd);

	//뷰 행렬셋팅
	_terrainEffect->SetMatrix("matViewProjection", &directionLightCam->getViewProjectionMatrix());

	_terrainEffect->SetTechnique("CreateShadow");

	UINT passNum = 0;
	_terrainEffect->Begin(&passNum, 0);
	for (UINT i = 0; i < passNum; i++)
	{
		_terrainEffect->BeginPass(i);
		device->SetStreamSource(0, _terrainVb, 0, sizeof(TERRAINVERTEX));
		device->SetVertexDeclaration(this->_terrainDecl);
		device->SetIndices(_terrainIb);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _totalVerNum, 0, _totalTriangle);
		_terrainEffect->EndPass();
	}
	_terrainEffect->End();
}

bool terrain::isIntersectRay(D3DXVECTOR3 * pOut, LPRay ray)
{
	vector<D3DXVECTOR3> hits;

	// 최상단 쿼드 트리부터 Ray Check 들어간다.
	_quadTree->getRayHits(&hits, ray);

	// 히트된게 없다
	if (hits.size() == 0) return false;

	// 먼저 처음 위치와 처음 위치에대한 거리 대입
	D3DXVECTOR3 hitPos = hits[0];
	D3DXVECTOR3 dir = hitPos - ray->origin;
	float distSq = D3DXVec3LengthSq(&dir);

	for (int i = 1; i < hits.size(); ++i)
	{
		dir = hits[i] - ray->origin;
		float newDist = D3DXVec3LengthSq(&dir);

		if (newDist < distSq)
		{
			distSq = newDist;
			hitPos = hits[i];
		}
	}

	// 여기까지온다면 제일 거리가 가까운 Hit 위치가 나온다
	*pOut = hitPos;

	return true;
}

float terrain::getHeight(float x, float z)
{
	// 터레인 범위을 넘어가면 0.0 값을 리턴한다
	if (x < _terrainStartX || x > _terrainEndX ||
		z > _terrainStartZ || z < _terrainEndZ)
	{
		return 0.0f;
	}

	// Terrain 의 좌상단 0 을 기준으로 월드 Terrain 의 상태적 위치를 찾자
	float pX = x - _terrainStartX;
	float pZ = -(z + _terrainEndZ);

	// 해당 위치가 어느 셀에 포함되는지 파악
	float invCell = 1.0f / _cellScale;
	pX *= invCell;
	pZ *= invCell;

	// 해당 위치의 셀 인덱스
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	// 셀의 네군데 정점을 얻는다.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb

	D3DXVECTOR3 lt = _terrainVertices[idxZ * _verNumX + idxX].pos;
	D3DXVECTOR3 rt = _terrainVertices[idxZ * _verNumX + idxX + 1].pos;
	D3DXVECTOR3 lb = _terrainVertices[(idxZ + 1) * _verNumX + idxX].pos;
	D3DXVECTOR3 rb = _terrainVertices[(idxZ + 1) * _verNumX + idxX + 1].pos;

	// 해당 셀에서의 delta 량을 구한다
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float height = 0.0f;

	// 해당 점이 좌상단에 있니?
	if (dX < 1.0f - dZ)
	{
		float deltaU = rt.y - lt.y;		// lt 정점에서 rt 정점까지의 y 위치의 변위량
		float deltaV = lb.y - lt.y;		// lt 정점에서 lb 정점까지의 y 위치의 변위량

		height = lt.y + (deltaU * dX) + (deltaV * dZ);
	}
	// 해당 점이 우하단이 있니?
	else
	{
		float deltaU = lb.y - rb.y;
		float deltaV = rt.y - rb.y;

		// 우에서 좌로 하에서 상으로 보간방향이 바뀌었기 때문에
		// delta 량을 역수로 취한다.
		dX = 1.0f - dX;
		dZ = 1.0f - dZ;

		height = rb.y + (deltaU * dX) + (deltaV * dZ);
	}

	return height;
}

bool terrain::getSlant(D3DXVECTOR3 * pOut, float gravityPower, float x, float z)
{
	// 터레인 범위을 넘어가면 0.0 값을 리턴한다
	if (x < _terrainStartX || x > _terrainEndX ||
		z > _terrainStartZ || z < _terrainEndZ)
	{
		return false;
	}

	// Terrain 의 좌상단 0 을 기준으로 월드 Terrain 의 상태적 위치를 찾자
	float pX = x - _terrainStartX;
	float pZ = -(z + _terrainEndZ);

	// 해당 위치가 어느 셀에 포함되는지 파악
	float invCell = 1.0f / _cellScale;
	pX *= invCell;
	pZ *= invCell;

	// 해당 위치의 셀 인덱스
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	// 셀의 네군데 정점을 얻는다.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb

	D3DXVECTOR3 lt = _terrainVertices[idxZ * _verNumX + idxX].pos;
	D3DXVECTOR3 rt = _terrainVertices[idxZ * _verNumX + idxX + 1].pos;
	D3DXVECTOR3 lb = _terrainVertices[(idxZ + 1) * _verNumX + idxX].pos;
	D3DXVECTOR3 rb = _terrainVertices[(idxZ + 1) * _verNumX + idxX + 1].pos;

	// 해당 셸에서의 delta 량을 구한다.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	// 폴리곤의 노말 벡터
	D3DXVECTOR3 normal;

	// 해당 점이 좌상단에 있니?
	if (dX < 1.0f - dZ)
	{
		//해당폴리곤의 법선 벡터를 구한다.
		D3DXVECTOR3 edge1 = rt - lt;
		D3DXVECTOR3 edge2 = lb - lt;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	// 해당 점이 우하단이 있니?
	else
	{
		// 해당폴리곤의 법선 벡터를 구한다.
		D3DXVECTOR3 edge1 = rt - lb;
		D3DXVECTOR3 edge2 = rb - lb;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	// 노말은 정규화
	D3DXVec3Normalize(&normal, &normal);

	// 중력 방향
	D3DXVECTOR3 gravity(0, -gravityPower, 0);

	// 경사면의 우측
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &normal, &gravity);

	// 우측벡터에서 법선벡터를 외적한 결과가 경사면의 경사 방향이 된다.
	D3DXVec3Cross(pOut, &right, &normal);

	return true;
}

HRESULT terrain::createTerrain(int smooth, int tileNum)
{
	// 정점 위치 구한다.

	// 타일링 갯수에 따른 간격 (정점당 uv 간격)
	float tileIntervalX = static_cast<float>(tileNum) / _cellNumX;
	float tileIntervalY = static_cast<float>(tileNum) / _cellNumZ;

	// 지형 정점 가지고 있어야 한다.
	_terrainVertices = new TERRAINVERTEX[_totalVerNum];

	// 텍스쳐의 pixel 정보를 얻기 위해 Texture 를 lock 한다.
	D3DLOCKED_RECT lockRect;
	_heightMap->LockRect(
		0,					// lock 을 할 밉맵 레벨 0
		&lockRect,			// lock 으로 얻어온 D3DLOCKED_RECT 정보 구조체
		0,					// lock 을 일부 영역만 하고 싶은 경우 사각형RECT 구조체의 포인터를 주는데 전체 할꺼면 NULL
		0					// lock 플레그 일단 0
	);

	// lockRect->Pitch	lock 을 한 영역 이미지의 가로 byte 크기 (얻어온 바이트크기는 다음을 성립한다 pitch % 4 == 0 ) < 3byte 컬러시 pitch byte 구하는 공식 ( 가로 픽셀수 * 3 + 3 ) & ~3 = pitch  >
	// lockRect->pBits	이미지데이터가 시작되는 포인터 주소

	// 정정위치와 정점 UV 를 계산하기
	for (int z = 0; z < _verNumZ; z++)
	{
		for (int x = 0; x < _verNumX; x++)
		{
			D3DXVECTOR3 pos;

			// 정점의 x, z 위치 계산
			pos.x = (x - (_cellNumX * 0.5)) * _cellScale;
			pos.z = (-z + (_cellNumZ * 0.5)) * _cellScale;

			// 가로마지막 라인이라면 (이전 왼쪽의 정점 Y 위치와 맞춘다)
			if (x == _verNumX - 1)
			{
				int idx = z * _verNumX + x - 1;
				pos.y = _terrainVertices[idx].pos.y;
			}

			// 세로 마지막 라인이라면 (이전 위쪽의 정점 Y 위치와 맞춘다)
			else if (z == _verNumZ - 1)
			{
				int idx = (z - 1) * _verNumX + x;
				pos.y = _terrainVertices[idx].pos.y;
			}
			else
			{
				// 해당 픽셀의 컬러 값을 얻는다.
				DWORD* pStart = (DWORD*)lockRect.pBits;	//(DWORD 형으로 형변환된 lock 된 이미지지의 시작 주소
				DWORD dwColor = *(pStart + (z * (lockRect.Pitch / 4) + x));

				// 각 컬러 값을 0 ~ 1 사이의 실수로 나눈다.
				float inv = 1.0f / 255.0f;
				float r = ((dwColor & 0x00ff0000) >> 16) * inv;
				float g = ((dwColor & 0x0000ff00) >> 8) * inv;
				float b = ((dwColor & 0x000000ff)) * inv;

				// 높이 맵 값
				float factor = (r + g + b) / 3.0f;

				// 높이 값
				pos.y = factor * _heightScale;
			}

			// 정점 UV 계산

			//Terrain Tile Splating UV
			D3DXVECTOR2 baseUV;
			baseUV.x = x / static_cast<float>(_verNumX - 1);
			baseUV.y = z / static_cast<float>(_verNumZ - 1);

			//Terrain Tile UV
			D3DXVECTOR2 tileUV;
			tileUV.x = x * tileIntervalX;
			tileUV.y = z * tileIntervalY;

			//버텍스 배열인덱스 계산
			int idx = z * _verNumX + x;

			_terrainVertices[idx].pos = pos;
			_terrainVertices[idx].normal = D3DXVECTOR3(0, 0, 0);	//아래에서 정점 노말 구할때 더해지니 일단 0 벡터로 초기화
			_terrainVertices[idx].baseUV = baseUV;
			_terrainVertices[idx].tileUV = tileUV;
		}
	}

	// 텍스쳐의 pixel 정보 Unlock
	_heightMap->UnlockRect(0);

	// 지형 스무싱 
	this->smoothTerrain(smooth);

	// 정점 인덱스를 구한다
	LPTERRAININDEX pIndices = new tagTerrainIndex[_totalTriangle];

	// 인덱스 배열 인덱스
	int idx = 0;

	for (DWORD z = 0; z < _cellNumZ; z++)
	{
		for (DWORD x = 0; x < _cellNumX; x++)
		{
			// lt-----rt
			//  |    /|
			//  |   / |
			//  |  /  |
			//  | /   |
			//  |/    |
			// lb-----rb

			// 해당 셀에 대한 정점 인덱스를 얻자
			DWORD lt = z * _verNumX + x;
			DWORD rt = z * _verNumX + x + 1;
			DWORD lb = ((z + 1)* _verNumX) + x;
			DWORD rb = ((z + 1)* _verNumX) + x + 1;

			// 셀의 삼각형 하나
			pIndices[idx]._0 = lt;
			pIndices[idx]._1 = rt;
			pIndices[idx]._2 = lb;
			idx++;

			// 셀의 삼각형 하나
			pIndices[idx]._0 = lb;
			pIndices[idx]._1 = rt;
			pIndices[idx]._2 = rb;
			idx++;
		}
	}

	// 노말이랑 Binormal 이랑 Tangent 계산하기
	D3DXVECTOR3* poses = new D3DXVECTOR3[_totalVerNum];		//정점위치
	D3DXVECTOR3* normals = new D3DXVECTOR3[_totalVerNum];
	D3DXVECTOR3* tangents = new D3DXVECTOR3[_totalVerNum];
	D3DXVECTOR3* binormals = new D3DXVECTOR3[_totalVerNum];
	D3DXVECTOR2* uvs = new D3DXVECTOR2[_totalVerNum];
	DWORD* indices = (DWORD*)pIndices;

	// 정점위치 및 UV 대입
	for (int i = 0; i < _totalVerNum; i++)
	{
		poses[i] = _terrainVertices[i].pos;
		uvs[i] = _terrainVertices[i].baseUV;
	}

	// 노말계산
	ComputeNormal(normals, poses, _totalVerNum, indices, _totalTriangle * 3);

	// 탄젠트 바이노말 계산
	ComputeTangentBinormal(
		tangents,
		binormals,
		poses,
		normals,
		uvs,
		indices,
		_totalTriangle,
		_totalVerNum);

	// 계산된거 대입
	for (int i = 0; i < _totalVerNum; i++)
	{
		_terrainVertices[i].normal = normals[i];
		_terrainVertices[i].binormal = binormals[i];
		_terrainVertices[i].tangent = tangents[i];
	}

	// 버퍼 생성
	// 정점버퍼 만들기
	device->CreateVertexBuffer(
		sizeof(TERRAINVERTEX) * _totalVerNum,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&_terrainVb,
		0);

	// 만들어진 정점 버퍼를 Lock 하여 지형 정점 값을 쓴다
	void* p = NULL;
	_terrainVb->Lock(0, 0, &p, 0);
	memcpy(p, _terrainVertices, sizeof(TERRAINVERTEX) * _totalVerNum);
	_terrainVb->Unlock();

	// 인덱스 버퍼 만들기
	device->CreateIndexBuffer(
		sizeof(TERRAININDEX) * _totalTriangle,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&_terrainIb,
		0);

	// 인덱스 버퍼에 값을 쓴다.
	_terrainIb->Lock(0, 0, &p, 0);
	memcpy(p, pIndices, sizeof(TERRAININDEX) * _totalTriangle);
	_terrainIb->Unlock();

	// 정점 선언부를 만든다.
	// 정점의 형태를 알려주는 배열
	D3DVERTEXELEMENT9 vertElement[7];	// 배열을 정점정보 갯수 + 1

	// Position 
	vertElement[0].Stream = 0;							// Stream 은 0
	vertElement[0].Offset = 0;							// 메모리의 시작 Byte 단위 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			// 자료의 타입
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		// 일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		// 정보 타입
	vertElement[0].UsageIndex = 0;						// UsageIndex 일단 0
														   
														// Normal
	vertElement[1].Stream = 0;							// Stream 은 0
	vertElement[1].Offset = 12;							// 메모리의 시작 Byte 단위 0
	vertElement[1].Type = D3DDECLTYPE_FLOAT3;			// 자료의 타입
	vertElement[1].Method = D3DDECLMETHOD_DEFAULT;		// 일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[1].Usage = D3DDECLUSAGE_NORMAL;			// 정보 타입
	vertElement[1].UsageIndex = 0;						// UsageIndex 일단 0
														   
														// BiNormal
	vertElement[2].Stream = 0;							// Stream 은 0
	vertElement[2].Offset = 24;							// 메모리의 시작 Byte 단위 0
	vertElement[2].Type = D3DDECLTYPE_FLOAT3;			// 자료의 타입
	vertElement[2].Method = D3DDECLMETHOD_DEFAULT;		// 일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[2].Usage = D3DDECLUSAGE_BINORMAL;		// 정보 타입
	vertElement[2].UsageIndex = 0;						// UsageIndex 일단 0

														// Tangent
	vertElement[3].Stream = 0;							// Stream 은 0
	vertElement[3].Offset = 36;							// 메모리의 시작 Byte 단위 0
	vertElement[3].Type = D3DDECLTYPE_FLOAT3;			// 자료의 타입
	vertElement[3].Method = D3DDECLMETHOD_DEFAULT;		// 일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[3].Usage = D3DDECLUSAGE_TANGENT;		// 정보 타입
	vertElement[3].UsageIndex = 0;						// UsageIndex 일단 0

														// BaseUV
	vertElement[4].Stream = 0;							// Stream 은 0
	vertElement[4].Offset = 48;							// 메모리의 시작 Byte 단위 0
	vertElement[4].Type = D3DDECLTYPE_FLOAT2;			// 자료의 타입
	vertElement[4].Method = D3DDECLMETHOD_DEFAULT;		// 일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[4].Usage = D3DDECLUSAGE_TEXCOORD;		// 정보 타입
	vertElement[4].UsageIndex = 0;						// UsageIndex 일단 0

														// tileUV
	vertElement[5].Stream = 0;							// Stream 은 0
	vertElement[5].Offset = 56;							// 메모리의 시작 Byte 단위 0
	vertElement[5].Type = D3DDECLTYPE_FLOAT2;			// 자료의 타입
	vertElement[5].Method = D3DDECLMETHOD_DEFAULT;		// 일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[5].Usage = D3DDECLUSAGE_TEXCOORD;		// 정보 타입
	vertElement[5].UsageIndex = 1;						// UsageIndex 두번째 UV 1 

														// 더이상 없으니 종료
	D3DVERTEXELEMENT9 end = D3DDECL_END(); // {0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
	vertElement[6] = end;

	// LPDIRECT3DVERTEXDECLARATION9 생성
	device->CreateVertexDeclaration(
		vertElement,			// 앞에서 만든 D3DVERTEXELEMENT9 배열 포인터
		&_terrainDecl			// 얻어올 LPDIRECT3DVERTEXDECLARATION9 포인터
	);

	SAFE_DELETE_ARRAY(poses);
	SAFE_DELETE_ARRAY(normals);
	SAFE_DELETE_ARRAY(tangents);
	SAFE_DELETE_ARRAY(binormals);
	SAFE_DELETE_ARRAY(uvs);
	SAFE_DELETE_ARRAY(indices);

	return S_OK;
}

void terrain::smoothTerrain(int passed)
{
	if (passed <= 0) return;

	float* smooth = new float[_totalVerNum];

	while (passed > 0)
	{
		passed--;
		for (int z = 0; z < _verNumZ; z++)
		{
			for (int x = 0; x < _verNumX; x++)
			{
				int adjacentSections = 0;		// 몇개의 정점과 평균값을 내니?
				float totalSections = 0.0f;		// 주변의 정점 높이 총합은 얼마니?

				// 왼쪽 체크
				if ((x - 1) > 0)
				{
					totalSections += _terrainVertices[(z * _verNumX) + (x - 1)].pos.y;
					adjacentSections++;
					// 왼쪽 상단
					if ((z - 1) > 0)
					{
						totalSections += _terrainVertices[((z - 1) * _verNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}
					// 왼쪽 하단
					if ((z + 1) < _verNumZ)
					{
						totalSections += _terrainVertices[((z + 1) * _verNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}
				}
				// 오른쪽 체크
				if ((x + 1) < _verNumX)
				{
					totalSections += _terrainVertices[(z * _verNumX) + (x + 1)].pos.y;
					adjacentSections++;

					// 오른쪽 상단
					if ((z - 1) > 0)
					{
						totalSections += _terrainVertices[((z - 1) * _verNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}
					// 오른쪽 하단 
					if ((z + 1) < _verNumZ)
					{
						totalSections += _terrainVertices[((z + 1) * _verNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}
				}
				// 상단
				if ((z - 1) > 0)
				{
					totalSections += _terrainVertices[((z - 1) * _verNumX) + x].pos.y;
					adjacentSections++;
				}
				// 하단
				if ((z + 1) < _verNumZ)
				{
					totalSections += _terrainVertices[((z + 1) * _verNumX) + x].pos.y;
					adjacentSections++;
				}
				smooth[(z * _verNumX) + x] = (
					_terrainVertices[(z * _verNumX) + x].pos.y +
					(totalSections / adjacentSections)) * 0.5f;
			}
		}

		// 위에서 계산된 y 스무싱 적용
		for (int i = 0; i < _totalVerNum; i++)
		{
			_terrainVertices[i].pos.y = smooth[i];
		}
	}

	SAFE_DELETE_ARRAY(smooth);
}
