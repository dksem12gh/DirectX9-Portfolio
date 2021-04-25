// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <Windows.h>
// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
// C++ ��Ÿ�� ��� �����Դϴ�.
#include <iostream>
// STL �����̳� �������
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;
// DIRECT3D ��� �� ���̺귯�� �߰�
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
/*D3D ��� �� ���̺귯�� �߰��� �������*/
//�ʿ��� ��� (DirectSDK ��ġ����/Include)
//d3dx9.h
//d3d9.h
//#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
//#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"
//(DirectSDK ��ġ����/Include) ��ΰ� ������Ʈ�� System ������ ����� �Ǿ��ִٸ� < > �ε� ���԰���
//System ���� (DirectSDK ��ġ����/Include) ��� ���
//�ַ��Ž���⿡�� ������Ʈ ��Ŭ��
//�Ӽ�
//�����Ӽ��� VC++ ���͸� �׸���
//â�� ����������.
//���Ե��丮ĭ�� �����ӹ��ڿ������� �����ϸ� ������ �Ʒ�ȭ��ǥ Ŭ��
//<����..> ���ڸ�Ŭ��
//���Ե��丮 â�� �����鼭 ���� ����� �����߰� ������Ŭ��
//Ŭ���ؼ� ������ �� ������ ... ��ư ����
//Ž���� �����鼭 (DirectSDK ��ġ����/Include) ����
//�ʿ���̺귯�� (DirectSDK ��ġ����/Lib/x86)
//d3dx9.lib
//d3d9.lib
//#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3d9.lib" )
//#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3dx9.lib" )
//System ���� (DirectSDK ��ġ����/Lib/x86) ��� ���
//�ַ��Ž���⿡�� ������Ʈ ��Ŭ��
//�Ӽ�
//�����Ӽ��� VC++ ���͸� �׸���
//â�� ����������.
//���̺귯�� ���丮ĭ�� �����ӹ��ڿ������� �����ϸ� ������ �Ʒ�ȭ��ǥ Ŭ��
//<����..> ���ڸ�Ŭ��
//���Ե��丮 â�� �����鼭 ���� ����� �����߰� ������Ŭ��
//Ŭ���ؼ� ������ �� ������ ... ��ư ����
//Ž���� �����鼭 (DirectSDK ��ġ����/Lib/x86) ����

//====================================================================
//			## ���� ���� ��������� �̰��� �߰��Ѵ� ##
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
#include "logManager.h"				// 2018.05.14. ����ȣ
#include "equipManager.h"
#include "optionManager.h"
#include "ShadowObjectManager.h"
#include "soundManager.h"
#include "effectManager.h"

//====================================================================
//			## �̱����� �̰��� �߰��Ѵ� ##
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
#define LOGMANAGER logManager::getSingleton()			// 2018.05.14. ����ȣ
#define EQUIP equipManager::getSingleton()
#define OPTION optionManager::getSingleton()			// 2018.06.12. ����ȣ
#define SHADOWMANAGER ShadowObjectManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define EFFECTMANAGER effectManager::getSingleton()

//====================================================================
//			## �����ι� - ��ũ�� ## (������â �ʱ�ȭ)
//====================================================================
#define WINNAME			(LPTSTR)(TEXT("DirectX 3D FrameWork!!"))
#define WINSTARTX		100
#define WINSTARTY		100
#define WINSIZEX		1420
#define WINSIZEY		800
#define WINSTYLE		WS_OVERLAPPEDWINDOW

//====================================================================
//			## ��ũ���Լ� ## (Ŭ���� �����Ҵ�� �κ� ����)
//====================================================================
#define SAFE_DELETE(p)			{if(p) {delete(p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)		{if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p)			{if(p) {(p)->Release(); (p) = NULL;}}

//====================================================================
//			## �������� ## 
//====================================================================
extern HWND _hWnd;
extern HINSTANCE _hInstance;
extern float _deltaTime;
extern D3DXMATRIXA16 _matCorrection;
//====================================================================
//			## FVF ##  (���� �Ѱ��� ���� ���� ����)
//====================================================================
typedef struct tagVertex
{
	D3DXVECTOR3 pos;		// ������ ��ġ.
	DWORD color;			// ������ �÷�.
	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };

}MYVERTEX, *LPMYVERTEX;

typedef struct tagVertexPU
{
	D3DXVECTOR3	pos;		// ������ ��ġ
	D3DXVECTOR2	uv;			// ������ UV��ǥ
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
}MYVERTEX_PU, *LPMYVERTEX_PU;

typedef struct tagVertexPN
{
	D3DXVECTOR3	pos;		// ������ ��ġ
	D3DXVECTOR3 normal;		// ������ �븻����.
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