// camera.h
// 17.12.22
#pragma once
#include "transform.h"
#include "Frustum.h"

class camera : public transform
{
private:
	// 화각 : 카메라로 포착하는 장면의 시야
	float			_fov;				// 카메라 화각
	float			_camNear;			// 카메라 근거리
	float			_camFar;			// 카메라 원거리
	float			_aspect;			// 카메라 종횡비
	bool			_isOrtho;			// 직교냐? (직교투영)
	float			_orthoSize;			// 직교시 사이즈
	Frustum			_frustum;			// 프러스텀
	

	LPDIRECT3DTEXTURE9 _renderTexture;	// 렌더링 될 texture
	LPDIRECT3DSURFACE9 _renderSurface;	// Texture Depth 버퍼와 
	// Stencil 버퍼가 있는 Surface
	LPDIRECT3DSURFACE9 _deviceTargetSurface;
	LPDIRECT3DSURFACE9 _deviceDepthAndStencilSurface;

protected:
	D3DXMATRIXA16	_matView;			// 뷰 행렬
	D3DXMATRIXA16	_matProjection;		// 투영 행렬
	D3DXMATRIXA16	_matViewProjection;	// 뷰 투영 행렬	

public:
	camera();
	~camera() {};

	// 카메라 update()
	void update();	
	void updateFrustum();
	//투영행렬과 뷰행렬 업데이트
	void updateMatrix();

	// 카메라 관련 행렬 가져오기
	D3DXMATRIXA16 getViewMatrix() { return _matView; };
	D3DXMATRIXA16 getProjectionMatrix() { return _matProjection; };
	D3DXMATRIXA16 getViewProjectionMatrix() { return _matViewProjection; };

	// frustrum get, set
	Frustum getFrustum() { return _frustum; }
	void setFrustum(Frustum frustum) { _frustum = frustum; }

	// aspect get, set
	float getAspect() { return _aspect; }
	void setAspect(float aspect) { _aspect = aspect; }

	// 화각 get, set
	float getFov() { return _fov; }
	void setFov(float fov) { _fov = fov; };

	// cma near get, set
	float getCamNear() { return _camNear; }
	void setCamNear(float camNear) { _camNear = camNear; }

	// cam far get , set
	float getCamFar() { return _camFar; }
	void setCamFar(float camFar) { _camFar = camFar; }

	// 직교냐? (직교투영)
	bool getOrtho() { return _isOrtho; }
	void setOrtho(bool ortho) { _isOrtho = ortho; }

	// 직교시 사이즈 _orthoSize
	float getOrthoSize() { return _orthoSize; }
	void setOrthoSize(float orthosize) { _orthoSize = orthosize; }
		
	//화면의 위치를 가지고 카메라의 투영레이를 얻는 함수
	void computeRay(LPRay pOutRay, D3DXVECTOR2* screenPos);
	//3차원 좌표를 2차원 좌표로 변환

	//월드 위치로  화면의 위치를 얻는다.
	bool getWorldPosToScreenPos(D3DXVECTOR2* pScreenPos, const D3DXVECTOR3* pWorldPos);

	// 렌더 Texture를 준비 함수
	void readyRenderToTexture(int width, int height);

	//Shadow Map Texture를 준비한다.
	void readyShadowTexture(int size);

	// 렌더 texture로 렌더링 시작
	void renderTextureBegin(DWORD backColor);

	// 렌더 texture로 렌더링 종료
	void renderTextureEnd();

	// 렌더 texture get 함수
	LPDIRECT3DTEXTURE9 getRenderTexture() { return _renderTexture; }

};