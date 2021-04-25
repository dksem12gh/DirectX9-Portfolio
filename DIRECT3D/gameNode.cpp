#include "stdafx.h"
#include "gameNode.h"

HRESULT gameNode::init(void)
{
	DEVICEMANAGER->CreateDevice();		// ����̽� ����
	KEYMANAGER->init();					// Ű�Ŵ��� �ʱ�ȭ
	TIMEMANAGER->init();				// Ÿ�ӸŴ��� �ʱ�ȭ
	SCENEMANAGER->init();				// ���Ŵ��� �ʱ�ȭ
	FONTMANAGER->init();				// ��Ʈ�Ŵ��� �ʱ�ȭ
	GIZMOMANAGER->init();
	SPRITEMANAGER->init();				// �̱��� �Ŵ��� �ʱ�ȭ
	LOGMANAGER->init();					// �α׸Ŵ��� �ʱ�ȭ
	SOUNDMANAGER->init();				// ����Ŵ��� �ʱ�ȭ

										//ȯ�汸 ����Ʈ �ʱ�ȭ
	_environmentEffect = RM_SHADER->addResource("Resources/Shader/EnvironmentCube.fx");

	//����ī�޶� �ʱ�ȭ
	_mainCamera = new camera;

	//����ī�޶� RenderToTexture �غ�
	_mainCamera->readyRenderToTexture(WINSIZEX, WINSIZEY);

	//�⺻ ���� ����
	_sceneBaseDirectionLight = new lightDirection;

	//���⼺ ������ ī�޶� �� �����Ѵ�.
	_directionLightCamera = new camera;

	//���⼺���� �ڽĿ� ���δ�.
	//_directionLightCamera->attachTo(_sceneBaseDirectionLight->_transform );

	//�⺻ ��
	_sceneBaseDirectionLight->_color = D3DXCOLOR(1, 1, 1, 1);
	_sceneBaseDirectionLight->_intensity = 1.0f;

	//�׸��� �Ÿ�
	_shadowDistance = 100.0f;

	//ī�޶��� ��������� �ٲ۴�...	
	_directionLightCamera->setOrtho(true);
	_directionLightCamera->setCamNear(0.1f);
	_directionLightCamera->setCamFar(_shadowDistance * 2.0f);
	_directionLightCamera->setAspect(1);
	_directionLightCamera->setOrthoSize(_shadowDistance * 1.5f);	//����ũ��� �׸���ũ���

	//���⼺���� ī�޶��� RenderToTexture �غ�
	_directionLightCamera->readyShadowTexture(4096);
	

	_environmentTexture = NULL;

	_hdc = GetDC(_hWnd);

	return S_OK;
}

void gameNode::release(void)
{
	// ����̽� �� �̱��� ����
	DEVICEMANAGER->ReleaseDevice();
	DEVICEMANAGER->releaseSingleton();
	// Ű�Ŵ��� �̱��� ����
	KEYMANAGER->release();
	KEYMANAGER->releaseSingleton();
	// Ÿ�ӸŴ��� �̱��� ����
	TIMEMANAGER->release();
	TIMEMANAGER->releaseSingleton();
	// ���Ŵ��� �̱��� ����
	SCENEMANAGER->release();
	SCENEMANAGER->releaseSingleton();
	// ��Ʈ�Ŵ��� �̱�������
	FONTMANAGER->release();
	FONTMANAGER->releaseSingleton();
	// �����Ŵ��� �̱�������
	GIZMOMANAGER->release();
	GIZMOMANAGER->releaseSingleton();
	// ��������Ʈ �Ŵ��� �̱��� ����
	SPRITEMANAGER->release();
	SPRITEMANAGER->releaseSingleton();
	LOGMANAGER->release();
	LOGMANAGER->releaseSingleton();

	SOUNDMANAGER->release();
	SOUNDMANAGER->releaseSingleton();
}

void gameNode::update(void)
{
	//_mainCamera->defaultControl();
}

void gameNode::render(void)
{
	//����ī�޶� RTT�غ�
	_mainCamera->renderTextureBegin(0x00101010);

	//ȯ�� ����
	this->renderEnvironment();

	/*�����Լ� ����*/
	//this->render();

	//����ī�޶� RTT����
	_mainCamera->renderTextureEnd();

	////����Ʈ ����Ʈ ����
	//_postEffect->SetTechnique("Base");
	//_postEffect->SetTexture("screenTex", _mainCamera->getRenderTexture());
	//float pixelU = 1.0f / WINSIZEX;
	//float pixelV = 1.0f / WINSIZEY;
	//_postEffect->SetFloat("pixelSizeU", pixelU);
	//_postEffect->SetFloat("pixelSizeV", pixelV);
	//_postEffect->SetFloat("pixelHalfSizeU", pixelU * 0.5f);
	//_postEffect->SetFloat("pixelHalfSizeV", pixelV * 0.5f);

	////����Ʈ ����Ʈ ����
	//UINT iPass;
	//_postEffect->Begin(&iPass, 0);
	//for (UINT i = 0; i < iPass; i++)
	//{
	//	_postEffect->BeginPass(i);
	//	device->SetFVF(SCENE_VERTEX::FVF);
	//	device->DrawIndexedPrimitiveUP(
	//		D3DPT_TRIANGLELIST,
	//		0,
	//		4,
	//		2,
	//		_scenePlaneIndex,
	//		D3DFMT_INDEX16,
	//		_scenePlaneVertex,
	//		sizeof(SCENE_VERTEX));
	//	_postEffect->EndPass();
	//}
	//_postEffect->End();

	//SPRITEMANAGER->beginSpriteRender();
	//this->renderSprite();
	//SPRITEMANAGER->endSpriteRender();
}

void gameNode::renderEnvironment(void)
{
	//ȯ�� �� �ε��Ȱ� ���ٸ� �����ض�
	if (_environmentTexture == NULL) return;

	//ȯ�� ����Ʈ ����
	_environmentEffect->SetTexture("MyCube_Tex", _environmentTexture);

	//WVP ��Ʈ����
	D3DXMATRIXA16 matWorld = _mainCamera->getFinalMatrix();
	D3DXMATRIXA16 matViewProj = _mainCamera->getViewProjectionMatrix();
	D3DXMATRIXA16 matWVP = matWorld * matViewProj;

	//����Ʈ ����
	_environmentEffect->SetMatrix("matWVP", &matWVP);
	_environmentEffect->SetMatrix("matWorld", &matWorld);

	//���̴� �׷���
	UINT numPass;
	_environmentEffect->Begin(&numPass, 0);		//���̴��� �׸��ٴ� ���� �˸��� pass ���� ��´�.
	for (UINT i = 0; i < numPass; i++)			//���� ����� ����.
	{
		_environmentEffect->BeginPass(i);		//i��° ��������
		_environmemtSphereMesh->DrawSubset(0);	//�� �׷���
		_environmentEffect->EndPass();			//i��° ������
	}
	_environmentEffect->End();					//���̴��� �׸��� ��

}

void gameNode::setEnvironment(string cubeFilePath)
{
	//������ ȯ����� ���õǾ������� �𸣴� ���� ���������ְ� ��������
	SAFE_RELEASE(_environmentTexture);

	//ȯ��� �ε�
	D3DXCreateCubeTextureFromFile(device, cubeFilePath.c_str(), &_environmentTexture);

	//ȯ�汸 �ε��Ǿ����� �ʴٸ�....
	if (_environmemtSphereMesh == NULL)
	{
		D3DXCreateSphere(device, 3.0f, 20, 20, &_environmemtSphereMesh, NULL);
	}
}

void gameNode::readyShadowMap(vector<baseObject*>* renderObjects, terrain * pTerrain)
{
	//���⼺������ ���� ī�޶��� Frustum ������Ʈ
	_directionLightCamera->updateMatrix();
	_directionLightCamera->updateFrustum();

	//���̷��Ƕ����� ī�޶� ������ �ֵ鸸 �׸���...
	static vector<baseObject*>	shadowCullObject;
	shadowCullObject.clear();

	for (int i = 0; i < renderObjects->size(); i++)
	{
		//�������� �ȿ� �ִ�?
		if (_directionLightCamera->getFrustum().isInFrustum((*renderObjects)[i]))
		{
			shadowCullObject.push_back((*renderObjects)[i]);
		}
	}

	//������ �� �׸���.
	_directionLightCamera->renderTextureBegin(0xffffffff);

	//�����޽� ����
	xMeshStatic::setCamera(this->_directionLightCamera);
	xMeshStatic::setTechniqueName("CreateShadow");
	//��Ų��޽� ����
	xMeshSkinned::setCamera(this->_directionLightCamera);
	xMeshSkinned::setTechniqueName("CreateShadow");

	for (int i = 0; i < shadowCullObject.size(); i++)
	{
		if (shadowCullObject[i]->_ignoreCreateShadow == false)
		{
			//shadowCullObject[i]->render();
		}
	}

	//���� Terrain �� ������ ���� �׷����Ѵٸ�...
	if (pTerrain != NULL)
	{
		pTerrain->renderShadow(_directionLightCamera);
	}

	_directionLightCamera->renderTextureEnd();


	//���� Terrain �� ������ ���� �����Ѵٸ�
	if (pTerrain != NULL)
	{
		pTerrain->getTerrainEffect()->SetTexture("Shadow_Tex", _directionLightCamera->getRenderTexture());
		pTerrain->getTerrainEffect()->SetMatrix("matLightViewProjection", &_directionLightCamera->getViewProjectionMatrix());
	}

	//������ �ؽ���
	xMeshStatic::_xMeshEffect->SetTexture("Shadow_Tex", _directionLightCamera->getRenderTexture());
	//������ ���
	xMeshStatic::_xMeshEffect->SetMatrix("matLightViewProjection", &_directionLightCamera->getViewProjectionMatrix());
	//������ �ؽ���
	xMeshSkinned::_sSkinnedMeshEffect->SetTexture("Shadow_Tex", _directionLightCamera->getRenderTexture());
	//������ ���
	xMeshSkinned::_sSkinnedMeshEffect->SetMatrix("matLightViewProjection", &_directionLightCamera->getViewProjectionMatrix());
}

LRESULT gameNode::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{

	case WM_KEYDOWN:
		switch (wParam)
		{
		//case VK_ESCAPE:
		//	DestroyWindow(hWnd);
		//	break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
