#include "stdafx.h"
#include "mabinogi_char.h"


HRESULT mabinogi_char::init(string filePath, string mtrlPath, D3DXMATRIXA16 * matCorrection)
{
	_body = new baseObject;

	_body->setMesh(RM_SKINNED->addResource(filePath, mtrlPath, matCorrection));
	_body->setActive(TRUE);
	LOGMANAGER->addLog("LOAD TEXTURE : " + intToString(_body->getSkinnedMesh()->getDiffuseTexSet()->size()));
	return S_OK;
}

void mabinogi_char::release(void)
{
	
}

void mabinogi_char::update(void)
{
	_body->update();
}

void mabinogi_char::render()
{
	_body->render();
}

void mabinogi_char::outlineRender(camera* pCamera)
{

	D3DXVECTOR3 prev = _body->_transform->getScale();
	
	D3DXVECTOR3 line = pCamera->getRight();
	
	_body->_transform->setScale(prev.x * line.x, prev.y * line.y, prev.z * line.z);

	xMeshSkinned::setTechniqueName("Toon");

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	_body->render();

	xMeshSkinned::setTechniqueName("ReciveShadowToon");
	_body->_transform->setScale(prev);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
