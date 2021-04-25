#pragma once
#include "singletonBase.h"
// ====================================================================
// 			## keyManager ## (키 매니져)
// ====================================================================

#define KEYMAX 256

class keyManager : public singletonBase <keyManager>
{
private:
	bool _keyUp[KEYMAX];
	bool _keyDown[KEYMAX];

public:
	// 키매니져 초기화
	HRESULT init();
	void release(void);

	// 키가 한번만 눌렸냐?
	bool isOnceKeyDown(int key);
	// 키가 한번 눌렸다 띄었냐?
	bool isOnceKeyUp(int key);
	// 키가 계속 눌려 있냐?
	bool isStayKeyDown(int key);
	// 토글키냐?
	bool isToggleKey(int key);

	keyManager() {}
	~keyManager() {}
};