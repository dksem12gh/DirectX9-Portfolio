#pragma once
#include "resourceManager.h"
// =============================================================
// 	## rmTexture ## (리소스매니져 텍스쳐)
// =============================================================
class rmTexture : public resourceManager <LPDIRECT3DTEXTURE9, rmTexture>
{
public:
	HRESULT init(void);
	void release(void);

	// 리소스 로딩방식만 재정의한다(텍스쳐)
	LPDIRECT3DTEXTURE9 loadResource(string fileName, void* param = NULL) override;
	LPDIRECT3DTEXTURE9 loadResource(string fileName, string mtrlPath, void* param = NULL) override;
	// 리소스 해제방식만 재정의한다
	void releaseResource(LPDIRECT3DTEXTURE9 data) override;

	rmTexture() {}
	~rmTexture() {}
};

