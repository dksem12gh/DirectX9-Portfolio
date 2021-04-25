#pragma once
#include "stdafx.h"

class boundBox;
class baseObject;

class xMesh
{
public:
	virtual HRESULT init(string filePath, D3DXMATRIXA16* matCorrection = NULL) PURE;
	virtual HRESULT init(string filePath, string mtrlPath, D3DXMATRIXA16* matCorrection = NULL) PURE;
	virtual void release(void) PURE;
	virtual void render(transform* trans) PURE;

	xMesh() {};
	~xMesh() {};
};