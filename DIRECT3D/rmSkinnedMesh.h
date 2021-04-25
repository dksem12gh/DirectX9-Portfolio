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

	//  �μҽ� �ε� ( �ε� ��ĸ� ������)
	xMeshSkinned* loadResource(string filePath, void* pParam = NULL) override;
	xMeshSkinned* loadResource(string fileName, string mtrlPath, void* param = NULL) override;
	//  ����
	void releaseResource(xMeshSkinned* data) override;


	rmSkinnedMesh() {}
	~rmSkinnedMesh() {}
};