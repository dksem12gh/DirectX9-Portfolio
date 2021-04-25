#include "stdafx.h"
#include "todayTest.h"

HRESULT todayTest::init(void)
{
	//일반적으로 원본 메쉬들은 댓다 크게 만들어져 있다
	//따라서 원래 있던 메쉬에서 다음과 같은 행렬로 보정시켜줘야 한다
	D3DXMATRIXA16	matScale, matRotate, matCorrection;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMatrixRotationY(&matRotate, D3DXToRadian(180));
	matCorrection = matScale * matRotate;

	//원본 메쉬 렌더
	xMeshStatic* mesh = RM_XMESH->addResource("Resources/Mesh/Fighter/Fighter.x", &matCorrection);

	//오브젝트 생성
	for (int i = 0; i < OBJECTNUM; i++)
	{
		for (int j = 0; j < OBJECTNUM; j++)
		{
			baseObject* pNewObject = new baseObject;
			pNewObject->setMesh(mesh);
			pNewObject->setActive(true);

			//위치 세팅
			pNewObject->_transform->setWorldPosition(i * 20, 0.0f, j * 20);

			//렌더 오브젝트 입력
			_renderObject.push_back(pNewObject);
		}
	}

	//광원세팅
	//태양광(방향성 광원)
	lightDirection* light1 = new lightDirection;
	light1->_color = D3DXCOLOR(1, 1, 1, 0);
	light1->_intensity = 1.0f;

	//광원 벡터에 담기
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
