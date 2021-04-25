#include "stdafx.h"
#include "tirchonailField.h"





HRESULT tirchonailField::init(string filePath, D3DXMATRIXA16 * matCorrection)
{	
	/*오브젝트 일단 여기까지만 띄웠음 추후 필요시 더 띄울계획*/
	//바운드박스 on/off F10

	// 지형 x파일 
	_terrain = new baseObject;
	_terrain->setMesh(RM_XMESH->addResource(filePath, matCorrection));
	_terrain->setActive(true);
	_well.resize(4);
	//벽 바운드 박스
	for (int i = 0; i < 4; i++)
	{
		_well[i] = new baseObject;
		_well[i]->setActive(true);
	}
	//마을 기준
	// 뒤
	_well[0]->_transform->setWorldPosition(0,4, 37);
	_well[0]->_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(40, 5.5, 2));
	//왼쪽
	_well[1]->_transform->setWorldPosition(-33.75, 4, 0);
	_well[1]->_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(2, 5.5, 40));
	//앞
	_well[2]->_transform->setWorldPosition(0, 4, -36);
	_well[2]->_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(40, 5.5, 2));
	//오른쪽
	_well[3]->_transform->setWorldPosition(38.95, 4, 0);
	_well[3]->_boundBox.setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(2, 5.5, 40));

	

	_obj.resize(OBJMAX);

	//오브젝트
	for (int i = 0; i < OBJMAX; i++)
	{
		_obj[i] = new baseObject;	
	}
	//오브젝트 초기화
	objInit();
	for (int i = 0; i < OBJMAX; i++)
	{
		_obj[i]->setActive(true);
	}

	return S_OK;
}

void tirchonailField::release(void)
{
	
}

void tirchonailField::rander(void)
{
	_terrain->render();
	for (int i = 0; i < OBJMAX; i++)
	{
		_obj[i]->render();
	}
	for (int i = 0; i < 4; i++)
	{
		_well[i]->render();
	}
	
}

void tirchonailField::objInit(void)
{	
	//0621 변경  찾기 힘들꺼라생각함 ㅋㅋㅋㅋㅋㅋ 그냥 기존에 있는 거 지우고 이거 복붙 ㄱㄱ 
	//matCorrectionF << 팬스에 들어감 
	D3DXMATRIXA16 matScale[3], matRotateY[3], matRotateY1, matSmithyRotateY, matCorrection[4];
	D3DXMATRIXA16 matCorrectionF;
	D3DXMatrixScaling(&matScale[0], 0.1f, 0.1f, 0.1f);
	D3DXMatrixScaling(&matScale[1], 0.25, 0.25, 0.25);
	D3DXMatrixScaling(&matScale[2], 0.1, 0.05, 0.1);
	D3DXMatrixRotationY(&matRotateY[0], D3DXToRadian(180));
	D3DXMatrixRotationY(&matRotateY[1], D3DXToRadian(150));
	D3DXMatrixRotationY(&matSmithyRotateY, D3DXToRadian(240));
	D3DXMatrixRotationY(&matRotateY1, D3DXToRadian(0));
	matCorrection[0] = matScale[0] * matRotateY[0];
	matCorrection[1] = matScale[0] * matSmithyRotateY;
	matCorrection[2] = matScale[1] * matSmithyRotateY;
	matCorrection[3] = matScale[0] * matRotateY[1];
	matCorrectionF = matRotateY1 * matScale[2];


	_obj[0]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/tirchoTree.x", &matCorrection[0]));
	_obj[0]->_transform->setWorldPosition(0, 1.85f, 12.5f);
	_objBoundBoxSize[0] = { 1.0f,2.5f,0.75f };
	_obj[0]->_boundBox.setBound(&_obj[0]->_transform->getUp(), &_objBoundBoxSize[0]);

	_obj[1]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/tirchoSmithy.X", &matCorrection[1]));
	_obj[1]->_transform->setWorldPosition(8, 0.13f, 3.5f);
	_objBoundBoxSize[1] = { 1.5f,2.0f,3.0f };
	_obj[1]->_boundBox.setBound(&_obj[1]->_transform->getUp(), &_objBoundBoxSize[1]);

	_obj[2]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/tirchoChair.X", &matCorrection[2]));
	_obj[2]->_transform->setWorldPosition(6.5f, 0, -3);
	// 팬스 //0621 팬스 스케일링 변경 
	for (int i = 3; i < 12; i++)
	{
		_obj[i]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/tirchoFence.X", &matCorrectionF));
	}
	_obj[3]->_transform->setWorldPosition(-2.5, 0, 6.0);
	_obj[4]->_transform->setWorldPosition(-6.0, 0, 6.0);
	_obj[5]->_transform->setWorldPosition(-9.5, 0, 6.0);
	_obj[6]->_transform->setWorldPosition(3.0, 0, 6.0);
	_obj[7]->_transform->setWorldPosition(-2.5, 0, -10.5);
	_obj[8]->_transform->setWorldPosition(-6.0, 0, -10.5);
	_obj[9]->_transform->setWorldPosition(-9.5, 0, -10.5);
	_obj[10]->_transform->setWorldPosition(3, 0, -10.5);
	_obj[11]->_transform->setWorldPosition(6.5, 0, -10.5);

	for (int i = 12; i < 18; i++)
	{
		_obj[i]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/tirchoFence1.X", &matCorrectionF));
	}
	_obj[12]->_transform->setWorldPosition(-11, 0, 4);
	_obj[13]->_transform->setWorldPosition(-11, 0, 0.5);
	_obj[14]->_transform->setWorldPosition(-11, 0, -5);
	_obj[15]->_transform->setWorldPosition(-11, 0, -8.5);
	_obj[16]->_transform->setWorldPosition(8.5, 0, -8.5);
	_obj[17]->_transform->setWorldPosition(8.5, 0, -5);
	// 집 ,타워
	_obj[18]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/tirchoHouse.X", &matCorrection[1]));
	_obj[18]->_transform->setWorldPosition(-8, 0, -7.5);
	_objBoundBoxSize[2] = { 1.85f , 1 , 1.5f };
	_obj[18]->_boundBox.setBound(&_obj[6]->_transform->getUp(), &_objBoundBoxSize[2]);

	_obj[19]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/tirchoTop.X", &matCorrection[3]));
	_obj[19]->_transform->setWorldPosition(6, 0, -8);


	//돌
	_obj[20]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/tirchoSton.X", &matCorrection[3]));
	_obj[20]->_transform->setWorldPosition(30, 2.5, -3);


	//캠프 트리
	_obj[21]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/tirchoCampTree.X", &matCorrection[0]));
	_obj[21]->_transform->setWorldPosition(-15, 0, 5);
	_objBoundBoxSize[3] = { 1.25,5,1.25 };
	_obj[21]->_boundBox.setBound(&_obj[9]->_transform->getUp(), &_objBoundBoxSize[3]);

	_objBoundBoxSize[4] = { 0.5,1.5,0.5 };
	for (int i = 22; i < 30; i++)
	{
		_obj[i]->setMesh(RM_XMESH->addResource("Resources/mabinogi/obj/appleTree.X", &matCorrection[0]));
		_obj[i]->_boundBox.setBound(&_obj[i]->_transform->getUp(), &_objBoundBoxSize[4]);
	}
	_obj[22]->_transform->setWorldPosition(23, 1.25, 5);
	_obj[23]->_transform->setWorldPosition(27, 2, 5);
	_obj[24]->_transform->setWorldPosition(31, 2.25, 5);
	_obj[25]->_transform->setWorldPosition(27, 2.25, 1);
	_obj[26]->_transform->setWorldPosition(27, 2.25, 9);
	_obj[27]->_transform->setWorldPosition(24, 1.75, -17);
	_obj[28]->_transform->setWorldPosition(28, 1.75, -17);
	_obj[29]->_transform->setWorldPosition(28, 2, -21);
	_obj[30]->_transform->setWorldPosition(28, 2, -13);
}


