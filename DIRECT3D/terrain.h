#pragma once

class quadTree;

class terrain
{
public:
	//지형 정점 구조체
	typedef struct tagTerrainVertex
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR2 baseUV;
		D3DXVECTOR2 tileUV;
	}TERRAINVERTEX, *LPTERRAINVERTEX;
	//지형 인덱스 구조체
	typedef struct tagTerrainIndex
	{
		DWORD _0;
		DWORD _1;
		DWORD _2;
	}TERRAININDEX, *LPTERRAININDEX;

private:
	quadTree*						_quadTree;
	LPDIRECT3DVERTEXBUFFER9			_terrainVb;		//지형 정점버퍼
	LPDIRECT3DINDEXBUFFER9			_terrainIb;		//지형 인덱스버퍼
	LPDIRECT3DVERTEXDECLARATION9	_terrainDecl;	//정점 정보선언

	LPTERRAINVERTEX					_terrainVertices;	//지형 정점정보
	LPD3DXEFFECT					_terrainEffect;		//지형 Effect
	LPDIRECT3DTEXTURE9				_heightMap;			//지형 높이맵 텍스쳐
	LPDIRECT3DTEXTURE9				_texTile_0;			//지형 0 층 타일 텍스쳐
	LPDIRECT3DTEXTURE9				_texTile_1;			//지형 1 층 타일 텍스쳐
	LPDIRECT3DTEXTURE9				_texTile_2;			//지형 2 층 타일 텍스쳐
	LPDIRECT3DTEXTURE9				_texTile_3;			//지형 3 층 타일 텍스쳐
	LPDIRECT3DTEXTURE9				_texSlat;			//스플렛팅 텍스쳐

	float _cellScale;				//셀 간격
	float _heightScale;				// 높이 스케일(픽셀컬러가 255일 때 높이)
	int _verNumX;					//가로 정점수
	int _verNumZ;					//깊이 정점수
	int _totalVerNum;				//전체 정점수
	int _cellNumX;					//가로 셀수
	int _cellNumZ;					//깊이 셀수
	int _totalCellNum;				//전체 셀수
	int _totalTriangle;				//전체 삼각형수
	float _terrainSizeX;			//지형 가로 사이즈
	float _terrainSizeZ;			//지형 세로 사이즈

									//정확한 높이 계산을 위해 필요한 변수들
	float _terrainStartX;			//지형 시작 위치X
	float _terrainStartZ;			//지형 시작 위치Z
	float _terrainEndX;				//지형 종료 위치X
	float _terrainEndZ;				//지형 종료 위치Z

	D3DXVECTOR3* _vertexPosArray;	//정점 위치 배열
	LPTERRAININDEX	_indices;		//인덱스 구조체
		

public:
	HRESULT init(int vertexNum, float cellSize);
	HRESULT init(
		char * heightMapName,		// 높이맵 이름
		char * tile_0,				// 타일 이미지 0
		char * tile_1,				// 타일 이미지 1
		char * tile_2,				// 타일 이미지 2
		char * tile_3,				// 타일 이미지 3
		char * tileSplat,			// 타일 이미지 콘트롤(RGB BLACK)
		float cellSize,				// 하나의 셀크기
		float heightScale,			// 높이 스케일
		int smoothLevel,			// 스무싱 처리 횟수
		int tileNum);				// 타일링 갯수
	void release(void);
	void update(void);
	void render(void);
	void render(camera* cam, lightDirection* directionLight);
	void render(camera* cam, lightDirection* directionLight, camera* directionLightCamera);
	void renderShadow(camera* directionLightCam);

	// get set terrainEffect
	LPD3DXEFFECT getTerrainEffect() { return _terrainEffect; }
	void setTerrainEffect(LPD3DXEFFECT terrainEffect) { _terrainEffect = terrainEffect; }

	// Ray 히트 위치를 구함.
	bool isIntersectRay(D3DXVECTOR3 * pOut, LPRay ray);

	// X, Z 위치의 지형의 높이를 구함.
	float getHeight(float x, float z);

	// 해당 X, Z 위치의 경사 벡터
	bool getSlant(D3DXVECTOR3* pOut, float gravityPower, float x, float z);

	terrain() {}
	~terrain() {}

private:
	// 지형클래스 내부에서만 사용가능한 함수
	// 지형 정점 만들기
	HRESULT createTerrain(int smooth, int tileNum);
	// 지형 스무싱
	void smoothTerrain(int passed);
};

