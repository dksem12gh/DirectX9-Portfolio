#pragma once
#include "resourceManager.h"
// =============================================================
// 	## rmShader ## (리소스매니져 셰이더)
// =============================================================
class rmShader : public resourceManager <LPD3DXEFFECT, rmShader>
{
public:
	HRESULT init(void);
	void release(void);

	// 리소스 로딩방식만 재정의한다(셰이더)
	LPD3DXEFFECT loadResource(string fileName, void* param = NULL) override;
	LPD3DXEFFECT loadResource(string fileName, string mtrlPath, void* param = NULL) override;
	// 리소스 해제방식만 재정의한다
	void releaseResource(LPD3DXEFFECT data) override;

	rmShader() {}
	~rmShader() {}
};