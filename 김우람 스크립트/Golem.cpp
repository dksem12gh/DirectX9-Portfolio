#include "stdafx.h"
#include "Golem.h"


HRESULT Golem::init(baseObject * _map, player * character, D3DXVECTOR3 randPos, camera * pCamera, string filePath, string mtrlPath, D3DXMATRIXA16 * matCorrection)
{
	_GolemPos = new transform;
	_scene = _map;

	_transform->setWorldPosition(randPos);
	_SpawnPos = _transform->getWorldPosition();
	_GolemPos->setWorldPosition(_transform->getWorldPosition());

	//카메라에 렌더위해
	_mainCamera = pCamera;

	this->setMesh(RM_SKINNED->addResource(filePath, mtrlPath, matCorrection));
	this->setActive(TRUE);


	_GolemBox = new boundBox;
	_GolemBox->setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	return S_OK;
}

void Golem::release(void)
{
}

void Golem::update(void)
{
	baseObject::update();
	baseObject::_transform->setWorldPosition(_transform->getWorldPosition());

}

void Golem::render(void)
{
	baseObject::render();
	_GolemPos->renderGizmo();
	_GolemBox->renderGizmo(_GolemPos);
}

void Golem::hit()
{
}

void Golem::endureA(int playerstate, int playerAtk)
{
}

void Golem::endureB(int playerstate, int playerAtk)
{
}

void Golem::endureC()
{
}

void Golem::Smash_endure()
{
}

Golem::Golem()
{
}


Golem::~Golem()
{
}
