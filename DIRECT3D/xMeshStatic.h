// xMeshStatic.h
#pragma once
#include "xMesh.h"

class xMeshStatic : public xMesh
{
public:
	friend class physicManager;						// 물리매니져는 내친구
	static LPD3DXEFFECT			_xMeshEffect;		// 고정 메쉬를 그리는 이팩트
	static LPDIRECT3DTEXTURE9	_defDiffuseTex;		// 디퓨즈 텍스쳐
	static LPDIRECT3DTEXTURE9	_defNormalTex;		// 노말 텍스쳐
	static LPDIRECT3DTEXTURE9	_defSpecularTex;	// 스펙큘라 텍스쳐
	static LPDIRECT3DTEXTURE9	_defEmssionTex;		// 이미션 텍스쳐

public:
	LPD3DXMESH					_mesh;				// xMesh
	DWORD						_materialsNum;		// 로딩된 메쉬의 재질 갯수
	vector<LPDIRECT3DTEXTURE9>	_vDiffuseTex;		// 로딩된 메쉬에서 사용되는 디퓨즈 텍스쳐
	vector<LPDIRECT3DTEXTURE9>	_vNormalTex;
	vector<LPDIRECT3DTEXTURE9>	_vSpecularTex;
	vector<LPDIRECT3DTEXTURE9>	_vEmissionTex;
	vector<LPDIRECT3DTEXTURE9>	_vGlossTex;
	vector<D3DMATERIAL9>		_vMaterials;		// 로딩된 메쉬에서 사용되는 마테리얼

	vector<D3DXVECTOR3>			_vertices;
	vector<D3DXVECTOR3>			_normals;
	vector<DWORD>				_indices;
	DWORD						_triNum;

public:
	D3DXVECTOR3					_boundMin;			// 바운드의 최소 위치
	D3DXVECTOR3					_boundMax;			// 바운드의 최대 위치
	D3DXVECTOR3					_boundCenter;		// 바운드의 센터
	D3DXVECTOR3					_boundSize;			// 바운드의 크기
	D3DXVECTOR3					_boundHalfSize;		// 바운드의 절반 크기
	float						_boundRadius;		// 바운드의 외접구 반지름

public:
	virtual HRESULT init(string filePath, D3DXMATRIXA16* matCorrection = NULL) override;
	virtual HRESULT init(string filePath, string mtrlPath, D3DXMATRIXA16* matCorrection = NULL) override;

	virtual void release(void) override;
	virtual void render(transform* trans) override;

	// 보정행렬 대로 메쉬 수정
	void meshCorrection(D3DXMATRIXA16* matCorrection);
	// 카메라 셋팅
	static void setCamera(camera* cam);
	//베이스 라이팅 세팅 ( 베이스 라이팅은 방향성 광원이다 )
	static void setBaseLight(lightDirection* pDirLight);
	// 라이트 셋팅
	static void setLighting(vector<light*>* pLights);
	static void setLighting(light** pLights, int lightNum);
	//테크닉 세팅
	static void setTechniqueName(string name);
	
	// 마테리얼 갯수 리턴
	DWORD getMaterialsNumber(void) { return _materialsNum; }
	LPD3DXMESH getMesh(void) { return _mesh; }
	// 디퓨즈 텍스쳐 리턴
	vector<LPDIRECT3DTEXTURE9>* getTexture(void) {return &_vDiffuseTex; }
	xMeshStatic() {};
	~xMeshStatic() {};

};
