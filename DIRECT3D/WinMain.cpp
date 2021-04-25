// ====================================================================
// 			## 헤더파일 써준다 ##
// ====================================================================
#include "stdafx.h"
#include "mainGame.h"
// ====================================================================
// 			## 전역변수 써준다 ##
// ====================================================================
HWND _hWnd;				// 윈도우 핸들(자신의 프로그램에서 돌고있는 윈도우 번호)
HINSTANCE _hInstance;	// 프로그램 인스턴스 핸들 (OS 가 부여한 프로그램 번호)
float	_deltaTime;	
D3DXMATRIXA16 _matCorrection;		// 전역 매트릭스
// 메인게임 클래스
mainGame _mg;

// ====================================================================
// 			## 함수 전방선언 ##
// ====================================================================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);

// ====================================================================
// 			## 윈도우 메인함수 ##
// ====================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	// 인스턴스를 전역변수에 담아둔다
	_hInstance = hInstance;

	// WNDCLASS : 윈도우창의 정보를 저장하기 위한 구조체
	// 윈도우클래스 구조체 선언후 초기화
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;		// 윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	wndClass.cbWndExtra = 0;		// 윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);  // 윈도우 배경색상
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					// 윈도우의 커서모양 결정
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// 윈도우아이콘모양 결정
	wndClass.hInstance = hInstance;									// 프로그램인스턴스핸들 
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						// 윈도우 프로시져 함수 포인터
	wndClass.lpszClassName = WINNAME;								// 메뉴이름 없으면 NULL
	wndClass.lpszMenuName = NULL;									// 지금 작성하고 있는 윈도우 클레스의 이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						// 윈도우 그리기 방식 설정 (사이즈가 변경될때 화면갱신 CS_HREDRAW | CS_VREDRAW)

																	// 위에서 작성한 윈도우 클래스 등록
	RegisterClass(&wndClass);

	// 윈도우 창 생성
	// 생성된 윈도우 핸들을 전역변수 _hWnd가 받는다.
	_hWnd = CreateWindow(
		WINNAME,				// 윈도우 클래스의 이름
		WINNAME,				// 윈도우 타이틀바 이름
		WS_OVERLAPPEDWINDOW,	// 윈도우 스타일
		WINSTARTX,				// 윈도우 화면 x좌표
		WINSTARTY,				// 윈도우 화면 y좌표
		WINSIZEX,				// 윈도우 화면 가로크기
		WINSIZEY,				// 윈도우 화면 세로크기
		GetDesktopWindow(),		// 부모 윈도우 핸들 (프로그램에서 최상위 윈도우면 NULL 또는 GetDesktopWindow())
		(HMENU)NULL,			// 메뉴핸들
		hInstance,				// 인스턴스
		NULL					// 우린 걍 NULL사용하면 됨 (신경끄자)
	);

	// 윈도우 클라이언트영역 정확히 세팅하기 
	setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	// 화면에 윈도우 보여주기
	ShowWindow(_hWnd, nCmdShow);

	// 메인게임 클래스 초기화 실패했다면 걍 종료시키기
	if (FAILED(_mg.init()))
	{
		return E_FAIL;
	}

	// MSG : 운영체제에서 발생하는 메세지 정보를 저장하기 위한 구조체
	MSG message;
	ZeroMemory(&message, sizeof(MSG));

	// 메시큐에서 얻어온 메시지가 종료메시지가 아니면 계속 루프
	while (message.message != WM_QUIT)
	{
		// 메시지큐의 메시지를본다 
		// 하지만 마지막의 PM_REMOVE 옵션으로인해 본메시지는
		// 지운다.
		// 만약 Message 가 없다면 false 를 리턴한다.
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);  // 눌린 키보드의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
			DispatchMessage(&message);	 // 받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
		}

		// 타임매니져 업데이트(FPS 60)
		TIMEMANAGER->update(60.0f);
		_deltaTime = TIMEMANAGER->getElapedTime();
		_mg.update();
		_mg.render();
	}

	// 메인게임 클래스 해제
	_mg.release();

	// 윈도우 클래스 등록해제 (해줘도 그만 안해줘도 그만..)
	UnregisterClass(WINNAME, hInstance);

	// 윈도우 프로그램 종료	
	return (int)message.wParam;
}

// ====================================================================
// 			## 윈도우 프로시져 ##
// ====================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _mg.MainProc(hWnd, iMessage, wParam, lParam);
}

// ====================================================================
// 			## 윈도우 크기 조정 ## (클라이언트 영역의 사이즈를 정확히 잡아준다)
// ====================================================================
void setWindowSize(int x, int y, int width, int height)
{
	RECT rc = { 0, 0, width, height };
	// 실제 윈도우 크기 조정
	AdjustWindowRect(&rc, WINSTYLE, false);
	// 위 렉트 정보로 윈도우 클라이언트 사이즈 세팅
	SetWindowPos(_hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE);
}