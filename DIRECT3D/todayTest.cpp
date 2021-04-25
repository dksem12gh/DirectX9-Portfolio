#include "stdafx.h"
#include "todayTest.h"

HRESULT todayTest::init(void)
{
	//�Ϲ������� ���� �޽����� ��� ũ�� ������� �ִ�
	//���� ���� �ִ� �޽����� ������ ���� ��ķ� ����������� �Ѵ�
	D3DXMATRIXA16	matScale, matRotate, matCorrection;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&matRotate, D3DXToRadian(180));
	matCorrection = matScale * matRotate;

	//���� �޽� ����
	xMeshStatic* mesh = RM_XMESH->addResource("Resources/Mesh/Fighter/Fighter.x", &matCorrection);

	//������Ʈ ����
	for (int i = 0; i < OBJECTNUM; i++)
	{
		for (int j = 0; j < OBJECTNUM; j++)
		{
			baseObject* pNewObject = new baseObject;
			pNewObject->setMesh(mesh);
			pNewObject->setActive(true);

			//��ġ ����
			pNewObject->_transform->setWorldPosition(i * 20, 0.0f, j * 20);

			//���� ������Ʈ �Է�
			_renderObject.push_back(pNewObject);
		}
	}

	//��������
	//�¾籤(���⼺ ����)
	lightDirection* light1 = new lightDirection;
	light1->_color = D3DXCOLOR(1, 1, 1, 0);
	light1->_intensity = 1.0f;

	//���� ���Ϳ� ���
	_lights.push_back(light1);

	return S_OK;
}

void todayTest::release(void)
{
}

void todayTest::update(void)
{
}

void todayTest::render(void)
{
}
