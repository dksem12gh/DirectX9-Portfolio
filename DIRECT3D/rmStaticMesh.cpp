#include "stdafx.h"
#include "rmStaticMesh.h"

HRESULT rmStaticMesh::init(void)
{
	return S_OK;
}

void rmStaticMesh::release(void)
{
	this->clearResource();
}

xMeshStatic * rmStaticMesh::loadResource(string filePath, void * pParam)
{
	xMeshStatic* newMesh = new xMeshStatic;
	
	if (FAILED(newMesh->init(filePath, (D3DXMATRIXA16*)pParam)))
	{
		SAFE_DELETE(newMesh);
		return NULL;
	}

	return newMesh;
}

xMeshStatic * rmStaticMesh::loadResource(string filePath, string mtrlPath, void * pParam)
{
	xMeshStatic* newMesh = new xMeshStatic;

	if (FAILED(newMesh->init(filePath, mtrlPath, (D3DXMATRIXA16*)pParam)))
	{
		SAFE_DELETE(newMesh);
		return NULL;
	}

	return newMesh;
}

void rmStaticMesh::releaseResource(xMeshStatic * data)
{
	data->release();
	SAFE_DELETE(data);
}
