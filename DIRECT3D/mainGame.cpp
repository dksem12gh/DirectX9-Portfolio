#include "stdafx.h"
#include "mainGame.h"
//==================================================================
//		## 초기화 ## init(void)
//==================================================================
HRESULT mainGame::init(void)
{
	gameNode::init();

	//씬추가
	SCENEMANAGER->addScene("인게임", new mabinogi);

	//씬초기화
	SCENEMANAGER->changeScene("인게임");

	return S_OK;
}

//==================================================================
//		## 해제 ## release(void)
//==================================================================
void mainGame::release(void)
{
	gameNode::release();
}

//==================================================================
//		## 업데이트 ## update(void)
//==================================================================
void mainGame::update(void)
{
	//씬매니져 업데이트
	SCENEMANAGER->update();
}

//==================================================================
//		## 렌더 ## render(void)
//==================================================================
void mainGame::render(void)
{
	//디바이스 버퍼를 청소하기
	HRESULT result = device->Clear(
		0,				//청소할 영역의 D3D렉트 배열 개수 (전체 클리어 0)
		NULL,			//청소할 영역의 D3D렉트 배열 포인터 (전체 클리어 NULL)
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, //청소할 버퍼 플래그(컬러, 스텐실, 깊이버퍼)
		0xff202020,		//청소한 후 채워질 색상 (0xAARRGGBB)
		1.0f,			//깊이버퍼를 청소할 값 (0 ~ 1, 0이 카메라에서 젤 가깝고, 1이 카메라에서 제일 멀다)
		0);				//스텐실 버퍼를 채울값

						//디바이스 버퍼를 깨끗이 청소했다면
	if (SUCCEEDED(result))
	{
		//디바이스 렌더링 시작
		device->BeginScene();
		//씬매니져 렌더
		SCENEMANAGER->render();
		//타임매니져 렌더
		//TIMEMANAGER->render();
		//기즈모 매니져 렌더
		//GIZMOMANAGER->gridMake(1, 10);
		// 로그매니저 렌더
		//LOGMANAGER->renderLog();
		//디바이스 렌더링 종료
		device->EndScene();
		//렌더링된 버퍼내욜을 화면에 전송
		device->Present(NULL, NULL, NULL, NULL);
	}
}