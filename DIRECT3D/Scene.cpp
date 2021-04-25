#include "stdafx.h"
#include "Scene.h"


HRESULT Scene::init(string filePath, string texturePath, D3DXMATRIXA16 * pMatCorrection)
{
	_scene = new baseObject;
	_scene->setMesh(RM_XMESH->addResource(filePath, texturePath, pMatCorrection));
	_scene->setActive(true);
	return S_OK;
}

HRESULT Scene::init(string filePaht, D3DXMATRIXA16* pmatCorrection)
{
	_scene = new baseObject;
	_scene->setMesh(RM_XMESH->addResource(filePaht, pmatCorrection));
	_scene->setActive(true);
	return S_OK;
}

void Scene::release(void)
{
	SAFE_DELETE(_scene);
}

void Scene::update(void)
{
	_scene->update();
}

void Scene::render(void)
{
	_scene->render();
}

Scene::Scene()
{
}


Scene::~Scene()
{
}
