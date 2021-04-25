#include "stdafx.h"
#include "mainGame.h"
//==================================================================
//		## �ʱ�ȭ ## init(void)
//==================================================================
HRESULT mainGame::init(void)
{
	gameNode::init();

	//���߰�
	SCENEMANAGER->addScene("�ΰ���", new mabinogi);

	//���ʱ�ȭ
	SCENEMANAGER->changeScene("�ΰ���");

	return S_OK;
}

//==================================================================
//		## ���� ## release(void)
//==================================================================
void mainGame::release(void)
{
	gameNode::release();
}

//==================================================================
//		## ������Ʈ ## update(void)
//==================================================================
void mainGame::update(void)
{
	//���Ŵ��� ������Ʈ
	SCENEMANAGER->update();
}

//==================================================================
//		## ���� ## render(void)
//==================================================================
void mainGame::render(void)
{
	//����̽� ���۸� û���ϱ�
	HRESULT result = device->Clear(
		0,				//û���� ������ D3D��Ʈ �迭 ���� (��ü Ŭ���� 0)
		NULL,			//û���� ������ D3D��Ʈ �迭 ������ (��ü Ŭ���� NULL)
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, //û���� ���� �÷���(�÷�, ���ٽ�, ���̹���)
		0xff202020,		//û���� �� ä���� ���� (0xAARRGGBB)
		1.0f,			//���̹��۸� û���� �� (0 ~ 1, 0�� ī�޶󿡼� �� ������, 1�� ī�޶󿡼� ���� �ִ�)
		0);				//���ٽ� ���۸� ä�ﰪ

						//����̽� ���۸� ������ û���ߴٸ�
	if (SUCCEEDED(result))
	{
		//����̽� ������ ����
		device->BeginScene();
		//���Ŵ��� ����
		SCENEMANAGER->render();
		//Ÿ�ӸŴ��� ����
		//TIMEMANAGER->render();
		//����� �Ŵ��� ����
		//GIZMOMANAGER->gridMake(1, 10);
		// �α׸Ŵ��� ����
		//LOGMANAGER->renderLog();
		//����̽� ������ ����
		device->EndScene();
		//�������� ���۳����� ȭ�鿡 ����
		device->Present(NULL, NULL, NULL, NULL);
	}
}