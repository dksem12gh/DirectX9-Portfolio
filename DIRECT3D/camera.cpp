#include "stdafx.h"
#include "camera.h"

camera::camera() : _renderTexture(NULL), _renderSurface(NULL),
_deviceDepthAndStencilSurface(NULL)
{
	setFov(D3DXToRadian(60));
	setCamNear(0.1f);
	setCamFar(1000.0f);
	
}

void camera::update()
{
	// 카메라 디폴트 컨트롤
	//this->defaultControl();

	// 화각에 의한 투영행렬 업데이트
	D3DXMatrixPerspectiveFovLH(&_matProjection, _fov,
		static_cast<float>(WINSIZEX) / static_cast<float>(WINSIZEY),
		_camNear, _camFar
	);

	// 뷰행렬 카메라 월드 위치에 대한 역행렬 D3DXMatrixInverse()
	D3DXMatrixInverse(&_matView, NULL, &_matFinal);

	// 뷰 투영 행렬 
	_matViewProjection = _matView * _matProjection;

	// 디바이스를 통한 뷰, 투영행렬 변환
	device->SetTransform(D3DTS_VIEW, &_matView);
	device->SetTransform(D3DTS_PROJECTION, &_matProjection);

}

void camera::updateFrustum()
{
	_frustum.updateFrustum(&_matViewProjection);
}

void camera::updateMatrix()
{
	//원근투영
	if (_isOrtho == FALSE)
	{
		//화각에 의한 투영행렬 업데이트(FOV세팅 하기(원근투영))
		D3DXMatrixPerspectiveFovLH(&_matProjection, _fov, _aspect, _camNear, _camFar);
	}
	//직교투영
	else
	{
		//화각에 의한 투영행렬 업데이트(직교투영)
		D3DXMatrixOrthoLH(&_matProjection, _aspect * _orthoSize, _orthoSize, _camNear, _camFar);
	}

	//뷰행렬 카메라 월드위치에 대한 역행렬
	D3DXMatrixInverse(&_matView, NULL, &_matFinal);

	//뷰 * 투영행렬
	_matViewProjection = _matView * _matProjection;
}

void camera::computeRay(LPRay pOutRay, D3DXVECTOR2 * screenPos)
{
	//디바이스를 통해 뷰포트를 얻기
	D3DVIEWPORT9 viewPort;
	device->GetViewport(&viewPort);
	
	//스크린의 위치 비율을 얻기
	float factorX = screenPos->x / viewPort.Width;
	float factorY = (1.0f - (screenPos->y / viewPort.Height));

	//0 ~ 1 => -1 ~ 1
	factorX = factorX * 2.0f - 1.0f;
	factorY = factorY * 2.0f - 1.0f;

	//투영행렬을 얻기
	D3DXMATRIXA16 matProj = this->getProjectionMatrix();

	//[1][][][]
	//[][1][][]
	//[][][1][]
	//[0][0][0][]
	//화각 스케일량을 나눈다
	D3DXVECTOR3 direction(
		factorX / matProj._11,
		factorY / matProj._22,
		1.0f);

	//카메라 월드 행렬
	D3DXMATRIXA16 matCamWorld = this->getFinalMatrix();

	//레이의 방향을 카메라 월드기준으로 계산한다
	D3DXVec3TransformNormal(&direction, &direction, &matCamWorld);

	//방향벡터 정규화
	D3DXVec3Normalize(&direction, &direction);

	//Ray의 방향
	pOutRay->direction = direction;
	//Ray의 오리진은 카메라의 위치로
	pOutRay->origin = this->getWorldPosition();
}

bool camera::getWorldPosToScreenPos(D3DXVECTOR2 * pScreenPos, const D3DXVECTOR3 * pWorldPos)
{
	//카메라의 ViewProjection 행렬을 얻는다.
	D3DXMATRIXA16 matViewProj = this->getViewProjectionMatrix();

	//월드 위치를 동차로...
	D3DXVECTOR3 pos;
	D3DXVec3TransformCoord(&pos, pWorldPos, &matViewProj);

	//동차공간
	//				*-------*<-( 1, 1, 1 )
	//			   /|      /|
	//			  *-------* |
	//            | *-----|-*
	//		      |/      |/
	//(-1,-1, 0)->*-------* 

	//0 ~ 1 사이 값에 없으면 이면 카메라 뒤에 있다는예기
	if (pos.z < 0.0f || pos.z > 1.0f) return false;

	//화면 중앙에 있다면 pos xy 값은 다음과 같다.
	//pos.x == 0  
	//pos.y == 0

	//화면 절반 사이즈
	float halfScreenW = WINSIZEX * 0.5f;
	float halfScreenH = WINSIZEY * 0.5f;


	//screenX = (int)((viewX / viewZ + 1.0f) * fSCREEN_HALF_WIDTH);
	//screenY = (int)((-viewY / viewZ + 1.0f) * fSCREEN_HALF_HEIGHT);
	pScreenPos->x = (int)((pos.x / pos.z + 1.0f) * halfScreenW);
	pScreenPos->y = (int)((-pos.y / pos.z + 1.0f) * halfScreenH);
	//최종 화면 위치
	//pScreenPos->x = halfScreenW + (halfScreenW * pos.x);
	//pScreenPos->y = halfScreenH - (halfScreenH * pos.y);	//Y 값은 반전

	return true;
}

void camera::readyRenderToTexture(int width, int height)
{
	SAFE_RELEASE(_renderTexture);
	SAFE_RELEASE(_renderSurface);

	// renderTarget 빈 Texture 만듦.
	device->CreateTexture(
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET,		// render target 용 texture
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&_renderTexture,
		NULL
	);

	// render 할 surface
	device->CreateDepthStencilSurface(
		width,
		height,
		D3DFMT_D24S8,		// Depth는 24 bit, Stencil 8 bit
		D3DMULTISAMPLE_NONE,	// 샘플링 안티얼리아싱이 없음
		0,						// 멀티 샘플링 퀄리티는 0
		true,					// 버퍼 교체시 이전 버퍼내용을 유지 하겠는가?
		&_renderSurface,
		NULL
	);
}

void camera::readyShadowTexture(int size)
{
	SAFE_RELEASE(_renderTexture);
	SAFE_RELEASE(_renderSurface);

	//RenderTarget 빈 Texture 만들기
	device->CreateTexture(
		size,						//Texture 가로 해상도 
		size,						//Texture 세로 해상도
		1,							//밉맵체인 레벨
		D3DUSAGE_RENDERTARGET,		//RenderTarget 용 Texture이다
		D3DFMT_R32F,				//모든 컬러 버퍼를 Red 로 다사용 ( 4byte float 실수로 사용 )
		D3DPOOL_DEFAULT,			//RenderTarget 용 Texture 는 Pool 을 Default
		&_renderTexture,			//생성된 Texture 받아올 포인터
		NULL
	);

	//Render 할 Surface 
	device->CreateDepthStencilSurface(
		size,						//Texture 가로 해상도 
		size,						//Texture 세로 해상도
		D3DFMT_D24S8,				//Deapth 는 24 비트 Stencil 은 8 비트	
		D3DMULTISAMPLE_NONE,		//멀티 샘플링 안티알리아싱은 존재하지 않는다, 
		0,							//멀티 샘플링 퀄리티는 0
		TRUE,						//버퍼 교체시 이전 퍼버내용을 유지하지 않니? ( TRUE 로 쓰면 버퍼교체 될때 이전에 써진 버퍼내용을 기억하지 않는다 )
		&_renderSurface,			//얻어올 포인터...
		NULL);
}


void camera::renderTextureBegin(DWORD backColor)
{
	//현 디바이스의 Target 버퍼의 표면과 DepthStencil 버퍼의 표면정보를 기억
	device->GetRenderTarget(0, &_deviceTargetSurface);
	device->GetDepthStencilSurface(&_deviceDepthAndStencilSurface);

	//RenderTexture 의 Surface 를 얻는다
	LPDIRECT3DSURFACE9 texSurface = NULL;
	if (SUCCEEDED(_renderTexture->GetSurfaceLevel(0, &texSurface)))
	{
		//Texture 표면을 Device 의 Target 버퍼로 세팅한다
		device->SetRenderTarget(0, texSurface);

		//세팅된 Surface 정보는 바로 날려주는 예의를 갖추자
		SAFE_RELEASE(texSurface);
	}

	//Depth 버퍼와 Stencil 버퍼의 Surface 로 _renderSurface 세팅
	device->SetDepthStencilSurface(_renderSurface);

	//디바이스 버퍼 클리어 (사실 위에서 세팅된 Textuer 랑 Surface 가 클리어 된다)
	device->Clear(0, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		backColor,
		1.0f,
		0);
}

void camera::renderTextureEnd()
{
	//Render Texture 에 그릴 작업이 끝났으면 다시 원상복귀하는 센스
	device->SetRenderTarget(0, _deviceTargetSurface);
	device->SetDepthStencilSurface(_deviceDepthAndStencilSurface);
	//세팅된 Surface 정보는 바로 날려주는 예의를 갖추자
	SAFE_RELEASE(_deviceTargetSurface);
	SAFE_RELEASE(_deviceDepthAndStencilSurface);
}
