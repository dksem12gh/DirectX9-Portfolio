// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <Windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
// C++ 런타임 헤더 파일입니다.
#include <iostream>
// STL 컨테이너 헤더파일
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;
// DIRECT3D 헤더 및 라이브러리 추가
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
/*D3D 헤더 및 라이브러리 추가시 참고사항*/
//필요한 헤더 (DirectSDK 설치폴더/Include)
//d3dx9.h
//d3d9.h
//#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
//#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"
//(DirectSDK 설치폴더/Include) 경로가 프로젝트의 System 폴더로 등록이 되어있다면 < > 로도 포함가능
//System 폴더 (DirectSDK 설치폴더/Include) 등록 방법
//솔루션탐색기에서 프로젝트 우클릭
//속성
//구성속성의 VC++ 디렉터리 항목선택
//창의 우측을봐라.
//포함디렉토리칸의 오른속문자열라인을 선택하면 나오는 아래화살표 클릭
//<편집..> 문자를클릭
//포함디렉토리 창이 열리면서 우측 상단의 폴더추가 아이콘클릭
//클릭해서 나오는 빈간 오른쪽 ... 버튼 눌러
//탐색기 나오면서 (DirectSDK 설치폴더/Include) 선택
//필요라이브러리 (DirectSDK 설치폴더/Lib/x86)
//d3dx9.lib
//d3d9.lib
//#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3d9.lib" )
//#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3dx9.lib" )
//System 폴더 (DirectSDK 설치폴더/Lib/x86) 등록 방법
//솔루션탐색기에서 프로젝트 우클릭
//속성
//구성속성의 VC++ 디렉터리 항목선택
//창의 우측을봐라.
//라이브러리 디렉토리칸의 오른속문자열라인을 선택하면 나오는 아래화살표 클릭
//<편집..> 문자를클릭
//포함디렉토리 창이 열리면서 우측 상단의 폴더추가 아이콘클릭
//클릭해서 나오는 빈간 오른쪽 ... 버튼 눌러
//탐색기 나오면서 (DirectSDK 설치폴더/Lib/x86) 선택

//====================================================================
//			## 내가 만든 헤더파일을 이곳에 추가한다 ##
//====================================================================
#include "deviceManager.h"
#include "keyManager.h"
#include "timeManager.h"
#include "sceneManager.h"
#include "fontManager.h"			// 2018.03.30
#include "gizmoManager.h"			// 2018.04.02
#include "MYUTIL.h"					// 2018.04.06
using namespace MYUTIL;
#include "transform.h"
#include "lightDirection.h"	
#include "camera.h"
#include "rmShader.h"				// 2018.04.20
#include "rmTexture.h"				// 2018.04.20
#include "rmStaticMesh.h"			// 2018.04.23
#include "rmSkinnedMesh.h"			// 2018.05.08
#include "boundBox.h"
#include "baseObject.h"
#include "lightPoint.h"		
#include "physicsManager.h"
#include "spriteManager.h"
#include "terrain.h"
#include "logManager.h"				// 2018.05.14. 박인호
#include "equipManager.h"
#include "optionManager.h"
#include "ShadowObjectManager.h"
#include "soundManager.h"
#include "effectManager.h"

//====================================================================
//			## 싱글톤을 이곳에 추가한다 ##
//====================================================================
#define DEVICEMANAGER deviceManager::getSingleton()
#define device DEVICEMANAGER->getDevice()
#define KEYMANAGER keyManager::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define	FONTMANAGER	fontManager::getSingleton()			// 2018.03.30
#define	GIZMOMANAGER gizmoManager::getSingleton()		// 2018.04.02
#define RM_SHADER	rmShader::getSingleton()			// 2018.04.20
#define RM_TEXTURE	rmTexture::getSingleton()			// 2018.04.20
#define RM_XMESH	rmStaticMesh::getSingleton()		// 2018.04.23
#define RM_SKINNED	rmSkinnedMesh::getSingleton()		// 2018.05.08
#define PHYSICSMANAGER physicsManager::getSingleton()	// 2018.04.26
#define SPRITEMANAGER spriteManager::getSingleton()		// 2018.05.09
#define LOGMANAGER logManager::getSingleton()			// 2018.05.14. 박인호
#define EQUIP equipManager::getSingleton()
#define OPTION optionManager::getSingleton()			// 2018.06.12. 박인호
#define SHADOWMANAGER ShadowObjectManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define EFFECTMANAGER effectManager::getSingleton()

//====================================================================
//			## 디파인문 - 메크로 ## (윈도우창 초기화)
//====================================================================
#define WINNAME			(LPTSTR)(TEXT("DirectX 3D FrameWork!!"))
#define WINSTARTX		100
#define WINSTARTY		100
#define WINSIZEX		1420
#define WINSIZEY		800
#define WINSTYLE		WS_OVERLAPPEDWINDOW

//====================================================================
//			## 메크로함수 ## (클래스 동적할당된 부분 해제)
//====================================================================
#define SAFE_DELETE(p)			{if(p) {delete(p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)		{if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p)			{if(p) {(p)->Release(); (p) = NULL;}}

//====================================================================
//			## 전역변수 ## 
//====================================================================
extern HWND _hWnd;
extern HINSTANCE _hInstance;
extern float _deltaTime;
extern D3DXMATRIXA16 _matCorrection;
//====================================================================
//			## FVF ##  (정점 한개에 대한 정보 정의)
//====================================================================
typedef struct tagVertex
{
	D3DXVECTOR3 pos;		// 정점의 위치.
	DWORD color;			// 정점의 컬러.
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

}MYVERTEX, *LPMYVERTEX;

typedef struct tagVertexPU
{
	D3DXVECTOR3	pos;		// 정점의 위치
	D3DXVECTOR2	uv;			// 정점의 UV좌표
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
}MYVERTEX_PU, *LPMYVERTEX_PU;

typedef struct tagVertexPN
{
	D3DXVECTOR3	pos;		// 정점의 위치
	D3DXVECTOR3 normal;		// 정점의 노말벡터.
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };
}MYVERTEX_PN, *LPMYVERTEX_PN;

typedef struct tagVertexA
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	DWORD		color;
	D3DXVECTOR2 uv;
	enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
} MYVERTEX_A, *LPMYVERTEX_A;