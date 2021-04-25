#include "stdafx.h"
#include "baseObject.h"

void baseObject::update()
{
	if (_isActive)
	{
		baseObjectUpdate();
		if (_skinnedAnim != NULL)
		{
			_skinnedAnim->update();
			_skinnedAnim->SetPlaySpeed(4.0f);
		}
	}
	else
	{
		baseObjectNoActiveUpdate();
	}
}

void baseObject::render()
{
	if (_isActive)
	{
		baseObjectRender();
	}
}

void baseObject::setMesh(xMesh * pMesh)
{
	_pMesh = pMesh;
	if (_pMesh == NULL)
	{
		LOGMANAGER->addLog("Mesh Read Failed!!");
	}
	else
	{
		LOGMANAGER->addLog("Mesh Read Success!!");
	}
	xMeshSkinned* pSkin = dynamic_cast<xMeshSkinned*>(pMesh);
	this->computeBoundBox();

	// staticMesh ? skinnedMesh?
	if (pSkin == NULL)
	{
		if (_skinnedAnim != NULL)
		{
			_skinnedAnim->release();
			SAFE_DELETE(_skinnedAnim);
		}

	}
	else
	{
		_skinnedAnim = new skinnedAnimation;
		_skinnedAnim->init(pSkin);
	}
}

void baseObject::setActive(bool isActive)
{
	//활성화 되어있는 상태에서 비활성화 셋팅시 
	if (isActive == false && _isActive)
	{
		_isActive = false;
		baseObjectDisable();
	}
	//비활성화 되어있는 상태에서 활성화 셋팅시
	else if (isActive && _isActive == false)
	{
		_isActive = true;
		baseObjectEnable();
	}
}

void baseObject::computeBoundBox()
{
	if (_pMesh != NULL)
	{
		/*
		dynamic_cast : 상속관계 안에서 포인터나 참조자의 타입을
		기본클래스에서 파생클래스로 다운 캐스팅과
		다중상속에서 기본 클래스간의 안전한 타입캐스팅에 사용한다.
		*/
		xMeshStatic* pStaticMesh = dynamic_cast<xMeshStatic*>(_pMesh);
		//세팅된 메쉬가 xMeshStatic 라면
		if (pStaticMesh != NULL)
		{
			_boundBox._localCenter = pStaticMesh->_boundCenter;
			_boundBox._halfSize = pStaticMesh->_boundHalfSize;
			_boundBox._localMinPos = pStaticMesh->_boundMin;
			_boundBox._localMaxPos = pStaticMesh->_boundMax;
			_boundBox._radius = pStaticMesh->_boundRadius;
		}
		// 셋팅 된 메쉬가 xMeshSinned 라면 임시로 바운드 박스 만들기
		else
		{	 
			_boundBox.setBound(&D3DXVECTOR3(0, 0, 0),
				&D3DXVECTOR3(10.0f, 10.0f, 10.0f));
		}
	}
}

void baseObject::baseObjectRender()
{
	if (_pMesh != NULL)
	{
		if (_skinnedAnim)
		{
			D3DXVECTOR3 transPos = _transform->getWorldPosition();

			D3DXVECTOR3 shadowPos = _transform->getWorldPosition();
			_skinnedAnim->render(_transform);
		}
		else
		{
			_pMesh->render(_transform);
		}
	}	
	//_boundBox.renderGizmo(_transform);
}

baseObject::baseObject() :  
	_isActive(FALSE), _pMesh(NULL), _skinnedAnim(NULL) 
{ 
	_transform = new transform;
	_ignoreCreateShadow = false;
}

baseObject::~baseObject()
{
	SAFE_DELETE(_transform);

	if (_skinnedAnim != NULL) {
		_skinnedAnim->release();
		SAFE_DELETE(_skinnedAnim);
	}
}

