#include "stdafx.h"
#include "deviceManager.h"

// 디바이스 생성
HRESULT deviceManager::CreateDevice()
{
	/*
	다이렉트3D 초기화
	1. Device를 생성할 IDirect3D9 객체를 얻는다
	2. 하드웨어 정보를 가지고 와서 자신의 정점 프로세스 타입을 정한다
	3. D3DPRESENT_PARAMETERS 구조체 정보를 생성
	4. Device 생성
	*/

	// 1. Device를 생성할 IDirect3D9 객체를 얻는다
	_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	// LPDIRECT3D9 객체얻기를 실패하면 NULL이 참조된다
	if (_d3d9 == NULL)
	{
		MessageBox(_hWnd, "Direct3DCreate9() - FAILED", "FAILED", MB_OK);
		return E_FAIL;
	}

	// 2. 하드웨어 정보를 가지고 와서 자신의 정점 프로세스 타입을 정한다
	D3DCAPS9 caps;
	// Direct3D9 객체를 통해 비디오 카드의 하드웨어 정보를 가지고 온다
	_d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,	// 주 디스플레이 그래픽카드 그냥 D3DADAPTER_DEFAULT 사용하면 됨
		D3DDEVTYPE_HAL,		// 디바이스타입 설정 그냥 D3DDEVTYPE_HAL
		&caps				// 디바이스 정보를 받아올 Direct3D9 포인터
	);

	// 정점계산 처리방식을 지정할 플래그 값
	DWORD vertexProcessing = 0;
	// 정점 처리와 정점 광원 처리를 하드웨어에서 지원하냐?
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// 3. D3DPRESENT_PARAMETERS 구조체 정보를 생성
	// 내가 이러한 디바이스를 만들겠다라는 정보
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// 뎁스버퍼와 스텐실버퍼의 크기 뎁스24비트, 스텐실8비트
	d3dpp.BackBufferCount = 1;									// 백버퍼는 한개
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;					// 백버퍼 색상포맷 알파8비트,레드8비트,그린8비트,블루8비트
	d3dpp.BackBufferWidth = WINSIZEX;							// 백버퍼 픽셀 가로크기
	d3dpp.BackBufferHeight = WINSIZEY;							// 백버퍼 픽셀 세로크기
	d3dpp.EnableAutoDepthStencil = true;						// 자동 깊이버퍼 사용여부 (그냥true)
	d3dpp.Flags = 0;											// 일단 0
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // 화면 주사율 (그냥 D3DPRESENT_RATE_DEFAULT 모니터 주사율과 동일)
	d3dpp.hDeviceWindow = _hWnd;								// 디바이스가 출력될 윈도우핸들
	d3dpp.MultiSampleQuality = 0;								// 멀티 샘플링 퀄러티
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;				// 멀티 샘플링 타입 
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;		// 화면 전송 간격
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// 화면 스왑 체인 방식
	d3dpp.Windowed = true;

	// 4. Device 생성
	// Direct3d9 객체를 통해서 디바이스를 생성한다

	HRESULT result = _d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,		// 주 디스플레이 그래픽카드 그냥 디폴트
		D3DDEVTYPE_HAL,			// 디바이스 타입 설정 그냥 "할"
		_hWnd,					// 디바이스 사용할 윈도우 핸들
		vertexProcessing,		// 정점 처리방싱에 대한 플래그
		&d3dpp,					// 앞에서 정의한 d3dpp 구조체 포인터
		&_device);				// 얻어올 디바이스 포인터

	if (FAILED(result))
	{
		// 디바이스 생성실패시 d3d9 객체 날려야 한다
		SAFE_RELEASE(_d3d9);

		MessageBox(_hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}

// 디바이스 해제
void deviceManager::ReleaseDevice()
{
	//SAFE_RELEASE(_d3d9);
	//SAFE_RELEASE(_device);
}
