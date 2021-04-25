#pragma once
#include "singletonBase.h"
// ====================================================================
// 			## deviceManager ## (디바이스 매니져)
// ====================================================================

class deviceManager : public singletonBase <deviceManager>
{
private:
	// 진짜 중요함, 이 디바이스로 모든걸 다 진행한다고 생각하면됨!!!
	// 이놈의 파워는 HDC와는 비교불가
	LPDIRECT3DDEVICE9 _device;		// 디바이스
	LPDIRECT3D9	_d3d9;				// 디바이스를 생성할 IDirect3D9 객체	

public:
	HRESULT CreateDevice(void);		// 디바이스 생성
	void ReleaseDevice(void);		// 디바이스 해제

									// 디바이스 얻기
	LPDIRECT3DDEVICE9 getDevice(void) { return _device; }

	deviceManager() {}
	~deviceManager() {}
};

