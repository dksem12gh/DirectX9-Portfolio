#pragma once

//쿼드트리 클래스
class quadTree;

class gameNode
{
private:
	typedef struct SCENE_VERTEX
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR2 uv;
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	}SCENE_VERTEX, *LPSCENE_VETEX;

	HDC _hdc;
public :
	camera* _mainCamera;

protected:
	LPDIRECT3DCUBETEXTURE9	_environmentTexture;		//환경 텍스쳐
	LPD3DXMESH				_environmemtSphereMesh;		//환경 구
	LPD3DXEFFECT			_environmentEffect;			//환경 이펙트

	SCENE_VERTEX			_scenePlaneVertex[4];		//씬평면 버텍스
	WORD					_scenePlaneIndex[6];		//씬평면 인덱스
	LPD3DXEFFECT			_postEffect;				//포스트 이펙트

	lightDirection*			_sceneBaseDirectionLight;	//씬에 배치된 기본라이팅
	camera*					_directionLightCamera;		//방향성 광원에 따른 카메라
	float					_shadowDistance;			//그림자 거리

public:
	virtual HRESULT init(void);
	virtual void release(void);
	virtual void update(void);
	virtual void render(void);
	virtual void renderSprite(void) {};
	void renderEnvironment(void);
	void setEnvironment(string cubeFilePath);
	void readyShadowMap(vector<baseObject*>* renderObjects, terrain* pTerrain = NULL);



	// 메인 프로시져
	LRESULT CALLBACK MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	gameNode() { _mainCamera = new camera; };
	HDC getHDC(void) { return _hdc; };
	virtual ~gameNode() {}
};