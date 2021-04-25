#pragma once
#include "resourceManager.h"
#include "xMeshStatic.h"

class rmStaticMesh : public resourceManager <xMeshStatic*, rmStaticMesh>
{
public :
	HRESULT init(void);
	void release(void);

	//  로소스 로딩 ( 로딩 방식만 재정의)
	xMeshStatic* loadResource(string filePath, void* pParam = NULL) override;
	xMeshStatic* loadResource(string filePath, string mtrlPath, void* pParam = NULL) override;
	//  해제
	void releaseResource(xMeshStatic* data) override;

	rmStaticMesh() {};
	~rmStaticMesh() {};

};