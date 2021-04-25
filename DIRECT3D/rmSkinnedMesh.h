#pragma once
#include "resourceManager.h"
#include "xMeshSkinned.h"

class rmSkinnedMesh :
	public resourceManager 
	< xMeshSkinned*, rmSkinnedMesh>
{
public :
	HRESULT init(void);
	void release(void);

	//  로소스 로딩 ( 로딩 방식만 재정의)
	xMeshSkinned* loadResource(string filePath, void* pParam = NULL) override;
	xMeshSkinned* loadResource(string fileName, string mtrlPath, void* param = NULL) override;
	//  해제
	void releaseResource(xMeshSkinned* data) override;


	rmSkinnedMesh() {}
	~rmSkinnedMesh() {}
};