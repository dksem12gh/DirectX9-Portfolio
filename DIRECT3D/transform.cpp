#include "stdafx.h"
#include "transform.h"

// 생성자로 초기화
transform::transform()
{
	// 부모자식 초기화
	_parent = NULL;
	_firstChild = NULL;
	_nextSibling = NULL;

	// 방향벡터 초기화
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);
	// 위치 초기화
	_position = D3DXVECTOR3(0, 0, 0);
	// 크기 초기화
	_scale = D3DXVECTOR3(1, 1, 1);

	// 자동 업데이트 초기화
	_isAutoUpdate = TRUE;

	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 특정 Child 를 내자식으로 붙인다
void transform::addChild(transform * pNewChild)
{
	// 이미 내새끼면 할필요 없다
	if (pNewChild->_parent == this) return;

	// 너이새끼 내밑으로 들어올려면 부모랑 연을 끊어라...
	pNewChild->releaseParent();

	// 부모의 상대적인 좌표값으로 갱신하기위해 
	// 부모의 final 역행렬을 구한다.
	D3DXMATRIXA16 matInvFinal;
	D3DXMatrixInverse(&matInvFinal, NULL, &this->_matFinal);

	// 자식의 Position 과 Axis 및 Scale 갱신
	D3DXVec3TransformCoord(&pNewChild->_position, &pNewChild->_position, &matInvFinal);

	// 축3개 변환하고 
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(pNewChild->axis + i, pNewChild->axis + i, &matInvFinal);
	}

	// 3축에 대한 길이값을 얻는다.
	pNewChild->_scale.x = D3DXVec3Length(&pNewChild->right);
	pNewChild->_scale.y = D3DXVec3Length(&pNewChild->up);
	pNewChild->_scale.z = D3DXVec3Length(&pNewChild->forward);

	// 정규화
	D3DXVec3Normalize(&pNewChild->right, &pNewChild->right);
	D3DXVec3Normalize(&pNewChild->up, &pNewChild->up);
	D3DXVec3Normalize(&pNewChild->forward, &pNewChild->forward);

	// 새로운 놈의 부모는 내가 된다.
	pNewChild->_parent = this;

	// 나의 자식놈 포인터
	transform* pChild = this->_firstChild;

	// 자식이 없는 쓸쓸한 독거노인이라면...
	if (pChild == NULL)
	{
		// 안심하고 추가
		this->_firstChild = pNewChild;
		pNewChild->_parent = this;
	}
	else // 대가족에 들어간다.
	{
		while (pChild != NULL)
		{
			// 내가 들어갈 자리를 찾았다면...
			if (pChild->_nextSibling == NULL)
			{
				pChild->_nextSibling = pNewChild;
				pNewChild->_parent = this;
				break;
			}
			pChild = pChild->_nextSibling;
		}
	}

	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	this->updateTransform();
}

// 특정 Transform 에 붙는다
void transform::attachTo(transform * pParent)
{
	pParent->addChild(this);
	// 내가 누군가에 붙었을때 로컬포지션을 000으로 만들어 주자
	this->setLocalPosition(0, 0, 0);
}

// 부모와 안녕
void transform::releaseParent()
{
	// 부모가 없니? 없으면 부모와 안녕할 일도 없다
	if (_parent == NULL) return;

	// 부모랑 연을 끊기 전에 부모부터 자식연을 끊어라...
	transform* pChild = _parent->_firstChild;

	// 내가 부모의 첫째자식이니?
	if (pChild == this)
	{
		// 내다음 자식이 첫번째 자식이 된다.
		_parent->_firstChild = this->_nextSibling;

		// 형제들과의 연도 끊는다.
		this->_nextSibling = NULL;
	}
	else
	{
		while (pChild != NULL)
		{
			// 현재 자식의 다음이 나니?
			if (pChild->_nextSibling == this)
			{
				pChild->_nextSibling = this->_nextSibling;
				// 형재들과의 연도 끊는다.
				this->_nextSibling = NULL;
				break;
			}
			// 다음 자식본다.
			pChild = pChild->_nextSibling;
		}
	}

	// 부모랑 연을 끊어라...
	_parent = NULL;

	// 자신의 현재 월드 위치에 대한 갱신이 필요하다.
	// 진짜월드 위치는 matFinal 이 다 가지고 있다.

	// 월드 위치 갱신
	this->_position.x = this->_matFinal._41;
	this->_position.y = this->_matFinal._42;
	this->_position.z = this->_matFinal._43;

	// 3축 얻어온다.
	D3DXVECTOR3 forwardScaled(this->_matFinal._31, this->_matFinal._32, this->_matFinal._33);
	D3DXVECTOR3 upScaled(this->_matFinal._21, this->_matFinal._22, this->_matFinal._23);
	D3DXVECTOR3 rightScaled(this->_matFinal._11, this->_matFinal._12, this->_matFinal._13);

	// 3축에서 스케일 뺀다
	float scaleX = D3DXVec3Length(&rightScaled);
	float scaleY = D3DXVec3Length(&upScaled);
	float scaleZ = D3DXVec3Length(&forwardScaled);

	// 정규화
	D3DXVECTOR3 forwardUnit;
	D3DXVECTOR3 upUnit;
	D3DXVECTOR3 rightUnit;
	D3DXVec3Normalize(&rightUnit, &rightScaled);
	D3DXVec3Normalize(&upUnit, &upScaled);
	D3DXVec3Normalize(&forwardUnit, &forwardScaled);

	// 정규화된 3축 대입
	this->forward = forwardUnit;
	this->right = rightUnit;
	this->up = upUnit;

	// 스케일 대입
	this->_scale.x = scaleX;
	this->_scale.y = scaleY;
	this->_scale.z = scaleZ;

	// 나만의 세상이 되었다
	// 부모랑 사라진 기념으로 Update 한번 콜
	this->updateTransform();
}

// 위치를 월드 좌표계로 세팅
void transform::setWorldPosition(float x, float y, float z)
{
	D3DXVECTOR3 pos(x, y, z);

	// 부모가 있다면 부모의 상태적인 위치로 바꿔라...
	if (_parent != NULL)
	{
		// 부모의 최종 행렬의 역행렬
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		// 그 역행열에 Pos 적용
		D3DXVec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	_position = pos;

	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setWorldPosition(D3DXVECTOR3 pos)
{
	// 부모가 있다면 부모의 상태적인 위치로 바꿔라...
	if (_parent != NULL)
	{
		// 부모의 최종 행렬의 역행렬
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		// 그 역행열에 Pos 적용
		D3DXVec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}

	_position = pos;

	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 위치를 로컬 좌표계로 셋팅(부모가 있는 경우 부모의 상태적인 위치)
void transform::setLocalPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setLocalPosition(D3DXVECTOR3 pos)
{
	_position = pos;
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 자기자신 기준으로 이동하기(상대좌표)
void transform::movePositionSelf(float dx, float dy, float dz)
{
	// 이동 벡터
	D3DXVECTOR3 move(0, 0, 0);

	// 자신의 이동 축 얻는다.
	D3DXVECTOR3 moveAxis[3];
	this->getUnitAxies(moveAxis);
	move += moveAxis[0] * dx;
	move += moveAxis[1] * dy;
	move += moveAxis[2] * dz;

	// 월드 이동
	D3DXVECTOR3 nowWorldPos = this->getWorldPosition();

	// 아래의 함수에서 TransformUpdate 가 일어남
	this->setWorldPosition(nowWorldPos + move);
}

void transform::movePositionSelf(D3DXVECTOR3 delta)
{
	// 이동 벡터
	D3DXVECTOR3 move(0, 0, 0);

	// 자신의 이동 축 얻는다.
	D3DXVECTOR3 moveAxis[3];
	this->getUnitAxies(moveAxis);
	move += moveAxis[0] * delta.x;
	move += moveAxis[1] * delta.y;
	move += moveAxis[2] * delta.z;

	// 월드 이동
	D3DXVECTOR3 nowWorldPos = this->getWorldPosition();

	// 아래의 함수에서 TransformUpdate 가 일어남
	this->setWorldPosition(nowWorldPos + move);
}

// 월드 기준으로 이동하기(절대좌표)
void transform::movePositionWorld(float dx, float dy, float dz)
{
	// 이동 벡터
	D3DXVECTOR3 move(dx, dy, dz);

	// 월드 이동
	D3DXVECTOR3 nowWorldPos = this->getWorldPosition();

	// 아래의 함수에서 TransformUpdate 가 일어남
	this->setWorldPosition(nowWorldPos + move);
}

void transform::movePositionWorld(D3DXVECTOR3 delta)
{
	// 월드 이동
	D3DXVECTOR3 nowWorldPos = this->getWorldPosition();

	// 아래의 함수에서 TransformUpdate 가 일어남
	this->setWorldPosition(nowWorldPos + delta);
}

void transform::movePositionLocal(float dx, float dy, float dz)
{
	_position.x += dx;
	_position.y += dy;
	_position.z += dz;
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

void transform::movePositionLocal(D3DXVECTOR3 delta)
{
	_position += delta;
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 스케일 세팅
void transform::setScale(float x, float y, float z)
{
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setScale(D3DXVECTOR3 scale)
{
	_scale = scale;
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 스케일링
void transform::scaling(float dx, float dy, float dz)
{
	_scale.x += dx;
	_scale.y += dy;
	_scale.z += dz;
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

void transform::scaling(D3DXVECTOR3 delta)
{
	_scale += delta;
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 월드 기준으로 회전
void transform::rotateWorld(float angleX, float angleY, float angleZ)
{
	// 부모가 있는 경우
	if (_parent)
	{
		// 진짜로월드 축
		D3DXVECTOR3 worldAxis[3];
		this->getUnitAxies(worldAxis);

		// 각 축에 대한 회전 행렬
		D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
		D3DXMatrixRotationX(&matRotateX, angleX);
		D3DXMatrixRotationY(&matRotateY, angleY);
		D3DXMatrixRotationZ(&matRotateZ, angleZ);

		/*
		// 다음과 같이 하면 회전 행렬을 2번 돌리니깐.....

		// 최종 회전 행렬
		D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

		// 최종 회전 행렬 대로 회전 시킨다.
		for( int i = 0 ; i < 3 ; i++ )
		D3DXVec3TransformNormal( worldAxis + i, worldAxis + i, &matRotate );

		// 부모의 역행렬로 다시 회전
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse( &matInvParentFinal, NULL, &_parent->_matFinal );


		// 최종 회전 행렬 대로 회전 시킨다.
		for( int i = 0 ; i < 3 ; i++ )
		D3DXVec3TransformNormal( worldAxis + i, worldAxis + i, &matInvParentFinal );
		*/

		// 최종 회전 행렬만들때 미리 역행렬성분을 곱해놓으면 1 번만 회전시키면 된다.

		// 최종 회전 행렬
		D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

		// 부모의 역행렬로 다시 회전
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		// 최종 회전 행렬 대로 회전 시킨다.
		for (int i = 0; i < 3; i++)
		{
			D3DXVec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);
		}
		// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
		if (_isAutoUpdate) this->updateTransform();
	}
	// 부모가 없는 경우
	else
	{
		rotateLocal(angleX, angleY, angleZ);
	}
}

void transform::rotateWorld(D3DXVECTOR3 angle)
{
	// 부모가 있는 경우
	if (_parent)
	{
		// 진짜로월드 축
		D3DXVECTOR3 worldAxis[3];
		this->getUnitAxies(worldAxis);

		// 각 축에 대한 회전 행렬
		D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
		D3DXMatrixRotationX(&matRotateX, angle.x);
		D3DXMatrixRotationY(&matRotateY, angle.y);
		D3DXMatrixRotationZ(&matRotateZ, angle.z);

		// 최종 회전 행렬만들때 미리 역행렬성분을 곱해놓으면 1 번만 회전시키면 된다.

		// 최종 회전 행렬
		D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

		// 부모의 역행렬로 다시 회전
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		matRotate = matRotate * matInvParentFinal;

		// 최종 회전 행렬 대로 회전 시킨다.
		for (int i = 0; i < 3; i++)
		{
			D3DXVec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);
		}

		// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
		if (_isAutoUpdate) this->updateTransform();
	}
	// 부모가 없는 경우
	else
	{
		rotateLocal(angle.x, angle.y, angle.z);
	}
}

// 자신의 축 기준으로 회전
void transform::rotateSelf(float angleX, float angleY, float angleZ)
{
	// 각 축에 대한 회전행렬
	D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
	D3DXMatrixRotationAxis(&matRotateX, &this->right, angleX);
	D3DXMatrixRotationAxis(&matRotateY, &this->up, angleY);
	D3DXMatrixRotationAxis(&matRotateZ, &this->forward, angleZ);

	// 최종 회전행렬(각 매트릭스끼리 곱하면 됨)
	// 주의사항 x, y, z 각축 3개의 곱은 순서를 지켜줘야 한다 (Yaw->Pitch->Roll) Y->X->Z순서
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

	// 최종 회전 행렬대로 회전시키기
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

void transform::rotateSelf(D3DXVECTOR3 angle)
{
	// 각 축에 대한 회전행렬
	D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
	D3DXMatrixRotationAxis(&matRotateX, &this->right, angle.x);
	D3DXMatrixRotationAxis(&matRotateY, &this->up, angle.y);
	D3DXMatrixRotationAxis(&matRotateZ, &this->forward, angle.z);

	// 최종 회전행렬(각 매트릭스끼리 곱하면 됨)
	// 주의사항 x, y, z 각축 3개의 곱은 순서를 지켜줘야 한다 (Yaw->Pitch->Roll) Y->X->Z순서
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

	// 최종 회전 행렬대로 회전시키기
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 부모가 있는 경우 부모 로컬의 축기준으로 회전
void transform::rotateLocal(float angleX, float angleY, float angleZ)
{
	// 각 축에 대한 회전행렬
	D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
	D3DXMatrixRotationX(&matRotateX, angleX);
	D3DXMatrixRotationY(&matRotateY, angleY);
	D3DXMatrixRotationZ(&matRotateZ, angleZ);

	// 최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	// 최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

void transform::rotateLocal(D3DXVECTOR3 angle)
{
	// 각 축에 대한 회전 행렬
	D3DXMATRIXA16 matRotateX, matRotateY, matRotateZ;
	D3DXMatrixRotationX(&matRotateX, angle.x);
	D3DXMatrixRotationY(&matRotateY, angle.y);
	D3DXMatrixRotationZ(&matRotateZ, angle.z);

	// 최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

	// 최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	this->updateTransform();
}

/*여기는 과제임*/
// 특정 방향을 바라보게 회전해라
void transform::lookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up)
{
	// 정면 벡터
	D3DXVECTOR3 newForward = dir;

	// 오른쪽벡터 ( 매개변수로 들어온 Up 을 가지고 외적 )
	D3DXVECTOR3 newRight;
	D3DXVec3Cross(&newRight, &up, &newForward);
	D3DXVec3Normalize(&newRight, &newRight);

	// 업 
	D3DXVECTOR3 newUp;
	D3DXVec3Cross(&newUp, &newForward, &newRight);
	D3DXVec3Normalize(&newUp, &newUp);

	// 만약 부모가 있다면...
	if (_parent)
	{
		// 새로운 축 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);

		D3DXVec3TransformNormal(&this->forward, &newForward, &matInvParentFinal);
		D3DXVec3TransformNormal(&this->right, &newRight, &matInvParentFinal);
		D3DXVec3TransformNormal(&this->up, &newUp, &matInvParentFinal);
	}
	else
	{
		this->forward = newForward;
		this->right = newRight;
		this->up = newUp;
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 특정 방향을 바라보는데 angle 만큼 회전해라
void transform::lookDirection(D3DXVECTOR3 dir, float angle)
{
	// 진짜로월드 축
	D3DXVECTOR3 worldAxis[3];
	this->getUnitAxies(worldAxis);

	// 정면 벡터와 바라볼 방향의 각도차를 얻자...
	float distRadian = acos(clampMinusPlusOne(D3DXVec3Dot(worldAxis + AXIS_Z, &dir)));

	// 각도차가 거의 없다면 하지마...
	if (FLOATZERO(distRadian)) return;

	// 외적 ( 내정면과 타겟까지의 방향을 외적 )
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, worldAxis + AXIS_Z, &dir);
	D3DXVec3Normalize(&cross, &cross);

	// 외적축으로 각차만큼 회전 시키는 행렬
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationAxis(&matRotate, &cross, min(angle, distRadian));

	// 만약 부모가 있다면...
	if (_parent)
	{
		// 회전 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}
	// 적용
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 특정 위치를 바라보게 회전해라
void transform::lookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up)
{
	// 위치에 대한 방향벡터를 얻는다.
	D3DXVECTOR3 worldPos = this->getWorldPosition();
	D3DXVECTOR3 dir = pos - worldPos;

	D3DXVec3Normalize(&dir, &dir);
	this->lookDirection(dir, up);
}

// 특정 위치를 바라보는데 angle 만큼 회전해라
void transform::lookPosition(D3DXVECTOR3 pos, float angle)
{
	// 위치에 대한 방향벡터를 얻는다.
	D3DXVECTOR3 worldPos = this->getWorldPosition();
	D3DXVECTOR3 dir = pos - worldPos;

	D3DXVec3Normalize(&dir, &dir);
	this->lookDirection(dir, angle);
}

// 사원수를 이용한 특정 회전값으로 회전량 세팅
void transform::setRotateWorld(float eAngleX, float eAngleY, float aAngleZ)
{
	// 사원수 준비
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	// 사원수에 의한 행렬준비
	D3DXMATRIXA16 matRotate;
	// 사원수에 의한 회전값으로 회전행렬이 만들어진다.
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	// 만약 부모가 있다면...
	if (_parent)
	{
		// 회전 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	// 축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// 최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}

	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setRotateLocal(float eAngleX, float eAngleY, float aAngleZ)
{
	// 사원수 준비
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	// 사원수에 의한 행렬준비
	D3DXMATRIXA16 matRotate;
	// 사원수에 의한 회전값으로 회전행렬이 만들어진다.
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	// 축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// 최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}
void transform::setRotateWorld(const D3DXMATRIXA16 & matWorldRotate)
{
	D3DXMATRIXA16 matRotate = matWorldRotate;

	// 만약 부모가 있다면...
	if (_parent)
	{
		// 회전 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	// 축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// 최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

void transform::setRotateLocal(const D3DXMATRIXA16 & matWorldRotate)
{
	D3DXMATRIXA16 matRotate = matWorldRotate;

	// 축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// 최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 회전 사원수를 넣어주면 그 회전값 대로 회전한다.
void transform::setRotateWorld(const D3DXQUATERNION & matWorldRotate)
{
	// 사원수 준비
	D3DXQUATERNION quatRot = matWorldRotate;

	// 사원수에 의한 행렬준비
	D3DXMATRIXA16 matRotate;
	// 사원수에 의한 회전값으로 회전행렬이 만들어진다.
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	// 만약 부모가 있다면...
	if (_parent)
	{
		// 회전 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &_parent->_matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	// 축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// 최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}
void transform::setRotateLocal(const D3DXQUATERNION & matWorldRotate)
{
	// 사원수 준비
	D3DXQUATERNION quatRot = matWorldRotate;

	// 사원수에 의한 행렬준비
	D3DXMATRIXA16 matRotate;
	// 사원수에 의한 회전값으로 회전행렬이 만들어진다.
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);

	// 축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	// 최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);
	}
	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 자신의 회전 값을 from 과 to 사이의 회전량만큼 회전보간(구면보간) 하여 적용
void transform::rotateSlerp(transform & from, transform & to, float t)
{
	t = clamp01(t);

	D3DXQUATERNION fromQuat = from.getWorldRotateQuaternion();
	D3DXQUATERNION toQuat = to.getWorldRotateQuaternion();

	// t 가 0 과 같다면...
	if (FLOATZERO(t))
	{
		this->setRotateWorld(fromQuat);
	}
	// t 가 1 과 같다면...
	else if (FLOATEQUAL(t, 1.0f))
	{
		this->setRotateWorld(toQuat);
	}
	// 0 과 1 사이의 값일때만 보간
	else
	{
		D3DXQUATERNION result;
		// from 과 to 사원수간의 t 만큼의 회전보간을 하여
		// result 사원수에 대입
		D3DXQuaternionSlerp(&result, &fromQuat, &toQuat, t);

		// 보간된 사원수 정보로 회전 셋팅
		this->setRotateWorld(result);
	}
}

//  자신의 위치 값을 from 과 to 사이의 위치만큼 선형보간 하여 적용
void transform::positionLerp(transform & from, transform & to, float t)
{
	t = clamp01(t);

	D3DXVECTOR3 fromWorldPos = from.getWorldPosition();
	D3DXVECTOR3 toWorldPos = to.getWorldPosition();

	// t 가 0 과 같다면...
	if (FLOATZERO(t))
	{
		this->setWorldPosition(fromWorldPos);
	}
	// t 가 1 과 같다면...
	else if (FLOATEQUAL(t, 1.0f))
	{
		this->setWorldPosition(toWorldPos);
	}
	// 0 과 1 사이의 값일때만 보간
	else
	{
		D3DXVECTOR3 result;
		D3DXVec3Lerp(&result, &fromWorldPos, &toWorldPos, t);
		this->setWorldPosition(result);
	}
}

//  자신의 모든 정보를 from 과 to 사이의 정보만큼 보간 하여 적용
void transform::interpolate(transform & from, transform & to, float t)
{
	t = clamp01(t);

	D3DXVECTOR3 resultScale;
	D3DXVECTOR3 resultPosition;
	D3DXQUATERNION resultRotate;

	// t 가 0 과 같다면...
	if (FLOATZERO(t))
	{
		resultScale = from._scale;
		resultPosition = from.getWorldPosition();
		resultRotate = from.getWorldRotateQuaternion();
	}
	// t 가 1 과 같다면...
	else if (FLOATEQUAL(t, 1.0f))
	{
		resultScale = to._scale;
		resultPosition = to.getWorldPosition();
		resultRotate = to.getWorldRotateQuaternion();
	}
	// 0 과 1 사이의 값일때만 보간
	else
	{
		D3DXVECTOR3 fromScale = from._scale;
		D3DXVECTOR3 toScale = to._scale;

		D3DXVECTOR3 fromPosition = from.getWorldPosition();
		D3DXVECTOR3 toPosition = to.getWorldPosition();

		D3DXQUATERNION fromQuat = from.getWorldRotateQuaternion();
		D3DXQUATERNION toQuat = to.getWorldRotateQuaternion();

		// 스케일 보간
		D3DXVec3Lerp(&resultScale, &fromScale, &toScale, t);

		// 위치 보간
		D3DXVec3Lerp(&resultPosition, &fromPosition, &toPosition, t);

		// 회전 보간
		D3DXQuaternionSlerp(&resultRotate, &fromQuat, &toQuat, t);
	}

	// 일단 오토업데이트 잠시 막는다
	_isAutoUpdate = false;

	this->setScale(resultScale);
	this->setWorldPosition(resultPosition);
	this->setRotateWorld(resultRotate);

	// 원상복구
	_isAutoUpdate = true;

	// 값이 변경되었으니 최종행렬을 업데이트 해줘야 한다
	if (_isAutoUpdate) this->updateTransform();
}

// 트렌스폼 업데이트 (트렌스폼에 정보들이 변경되었을때 사용한다)
void transform::updateTransform()
{
	// 자신의 정보로 matLocal 행렬을 갱신한다.
	D3DXMatrixIdentity(&_matLocal);

	// 스케일변화로 인한 축정보 갱신
	D3DXVECTOR3 scaleRight = this->right * _scale.x;
	D3DXVECTOR3 scaleUp = this->up * _scale.y;
	D3DXVECTOR3 scaleForward = this->forward * _scale.z;

	memcpy(&_matLocal._11, &scaleRight, sizeof(D3DXVECTOR3));
	memcpy(&_matLocal._21, &scaleUp, sizeof(D3DXVECTOR3));
	memcpy(&_matLocal._31, &scaleForward, sizeof(D3DXVECTOR3));
	memcpy(&_matLocal._41, &_position, sizeof(D3DXVECTOR3));

	// 나의 최종 행렬
	if (_parent == NULL)
	{
		this->_matFinal = _matLocal;
	}
	// 내가 부모가 있다면...
	else
	{
		this->_matFinal = _matLocal * _parent->_matFinal;
	}

	// 자식 업데이트
	transform* pChild = this->_firstChild;

	// 자식들아 너희들도 업데이트
	while (pChild != NULL)
	{
		pChild->updateTransform();
		pChild = pChild->_nextSibling;
	}
}

// 디바이스에 자신의 트렌스폼을 월드로 세팅
void transform::setDeviceWorld()
{
	device->SetTransform(D3DTS_WORLD, &_matFinal);
}
// 디바이스에 자신의 트렌스폼을 뷰로 세팅
void transform::setDeviceView()
{
	/*
	이동이나 회전 등 변환을 하나의 행렬로 표현하는데,
	그 행렬의 역행렬을 구해서 변환된 점에 곱하면 원위치가 됩니다.

	만약 직접 3d그래픽 라이브러리를 만드시려면 뷰 변환도 직접 해 줘야겠지만,
	그냥 어떤 점이나 도형을 그리면, DX의 렌더링 과정에서 뷰 변환 행렬은 원래 곱해집니다.
	뷰 변환을 두 번 하는 셈이라 제대로 안 나오다가, 한 번을 취소하니 제대로 나온거죠.

	참고로 화면상의 좌표로 3D공간 상의 물체를 선택할 때, 언프로젝션을 하는데
	화면 크기 행렬, 원근행렬, 뷰 변환 행렬의 역을 구해서 곱하면 공간상의 좌표가 나옵니다.
	*/

	D3DXMATRIXA16 matView;
	D3DXMatrixInverse(&matView, NULL, &_matFinal);
	device->SetTransform(D3DTS_VIEW, &matView);
}

void transform::setWorldMatrix(const D3DXMATRIXA16 & matWorld)
{
	// 월드 행렬로 셋팅
	this->_matFinal = matWorld;
	this->_matLocal = matWorld;

	// 월드 위치 업데이트 
	_position = D3DXVECTOR3(
		_matFinal._41,
		_matFinal._42,
		_matFinal._43);

	// 방위 업데이트 
	D3DXVECTOR3 front = D3DXVECTOR3(
		_matFinal._31,
		_matFinal._32,
		_matFinal._33);
	D3DXVECTOR3 up = D3DXVECTOR3(
		_matFinal._21,
		_matFinal._22,
		_matFinal._23);
	D3DXVECTOR3 right = D3DXVECTOR3(
		_matFinal._11,
		_matFinal._12,
		_matFinal._13);
	D3DXVec3Normalize(&this->forward, &front);
	D3DXVec3Normalize(&this->up, &up);
	D3DXVec3Normalize(&this->right, &right);

	// 길이로 스케일 준다.
	_scale = D3DXVECTOR3(
		D3DXVec3Length(&right),
		D3DXVec3Length(&up),
		D3DXVec3Length(&front));

	// 만약 부모가 있었다면..
	if (_parent)
	{
		// 위에서 대입된 정보를 부모기준의 로컬로 

		// 부모 역행렬
		D3DXMATRIXA16 matParentINVWorld = _parent->getFinalMatrix();
		D3DXMatrixInverse(&matParentINVWorld, NULL, &matParentINVWorld);

		// 로컬 행렬
		this->_matLocal = _matFinal * matParentINVWorld;

		// 나의 로컬 위치
		memcpy(&this->_position, &_matLocal._41, sizeof(D3DXVECTOR3));

		// 나의 Front, Up, Right
		D3DXVECTOR3 front;
		memcpy(&front, &_matLocal._31, sizeof(D3DXVECTOR3));
		D3DXVECTOR3 up;
		memcpy(&up, &_matLocal._21, sizeof(D3DXVECTOR3));
		D3DXVECTOR3 right;
		memcpy(&right, &_matLocal._11, sizeof(D3DXVECTOR3));

		// 각 스케일
		float scaleX = D3DXVec3Length(&right);
		float scaleY = D3DXVec3Length(&up);
		float scaleZ = D3DXVec3Length(&front);
		this->_scale = D3DXVECTOR3(scaleX, scaleY, scaleZ);

		// 벡터의 길이를 나누면 정규화
		// front /= scaleX;
		front *= (1.0f) / scaleZ;
		up *= (1.0f) / scaleY;
		right *= (1.0f) / scaleX;

		this->forward = front;
		this->up = up;
		this->right = right;
	}

	// 내가 자식이 있다면....
	transform* pChild = this->_firstChild;
	while (pChild != NULL)
	{
		pChild->updateTransform();
		pChild = pChild->_nextSibling;
	}
}

D3DXMATRIXA16 transform::getWorldRotateMatrix()
{
	// 자신의 축으로 회전 행렬을 만들어 재낀다
	D3DXMATRIXA16 matRotate;
	D3DXMatrixIdentity(&matRotate);

	// 3축을 얻는다.
	D3DXVECTOR3 axis[3];
	this->getUnitAxies(axis);

	// 행렬에 적용
	memcpy(&matRotate._11, axis + 0, sizeof(D3DXVECTOR3));
	memcpy(&matRotate._21, axis + 1, sizeof(D3DXVECTOR3));
	memcpy(&matRotate._31, axis + 2, sizeof(D3DXVECTOR3));

	return matRotate;
}

D3DXQUATERNION transform::getWorldRotateQuaternion()
{
	D3DXQUATERNION quat;

	D3DXMATRIXA16 matRotate = this->getWorldRotateMatrix();

	// 회전 행렬로 사원수를 만든다.
	D3DXQuaternionRotationMatrix(&quat, &matRotate);

	return quat;
}

D3DXVECTOR3 transform::getWorldPosition()
{
	D3DXVECTOR3 pos = _position;

	if (_parent)
	{
		D3DXVec3TransformCoord(&pos, &pos, &_parent->_matFinal);
	}

	return pos;
}

D3DXVECTOR3 transform::getLocalPosition()
{
	return _position;
}

// 축을 얻는다 (월드기준)
void transform::getScaledAxies(D3DXVECTOR3 * pVecArr)
{
	for (int i = 0; i < 3; i++)
	{
		pVecArr[i] = this->axis[i];
	}

	// 부모가 있다면..
	if (_parent)
	{
		D3DXMATRIXA16 matParentFinal = _parent->_matFinal;
		for (int i = 0; i < 3; i++)
		{
			D3DXVec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}
}

void transform::getUnitAxies(D3DXVECTOR3 * pVecArr)
{
	for (int i = 0; i < 3; i++)
	{
		D3DXVec3Normalize(pVecArr + i, this->axis + i);
	}

	// 부모가 있다면..
	if (_parent)
	{
		D3DXMATRIXA16 matParentFinal = _parent->_matFinal;
		for (int i = 0; i < 3; i++)
		{
			D3DXVec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}
}

D3DXVECTOR3 transform::getScaledAxis(int axisNum)
{
	D3DXVECTOR3 result = this->axis[axisNum];

	// 부모가 있다면..
	if (_parent)
	{
		D3DXMATRIXA16 matParentFinal = _parent->_matFinal;
		D3DXVec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}

D3DXVECTOR3 transform::getUnitAxis(int axisNum)
{
	D3DXVECTOR3 result;
	D3DXVec3Normalize(&result, this->axis + axisNum);

	// 부모가 있다면..
	if (_parent)
	{
		D3DXMATRIXA16 matParentFinal = _parent->_matFinal;
		D3DXVec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}

D3DXVECTOR3 transform::getForward(bool isNormalize)
{
	if (isNormalize)
	{
		return this->getUnitAxis(AXIS_Z);
	}
	return this->getScaledAxis(AXIS_Z);
}

D3DXVECTOR3 transform::getUp(bool isNormalize)
{
	if (isNormalize)
	{
		return this->getUnitAxis(AXIS_Y);
	}
	return this->getScaledAxis(AXIS_Y);
}

D3DXVECTOR3 transform::getRight(bool isNormalize)
{
	if (isNormalize)
	{
		return this->getUnitAxis(AXIS_X);
	}
	return this->getScaledAxis(AXIS_X);
}

// 카메라 디폴트 컨트롤
void transform::defaultControl()
{
	// 디폴트컨트롤을 위한 카메라 앵글값
	static float nowAngleH = 0.0f;				// 수평각
	static float nowAngleV = 0.0f;				// 수직각
	static float maxAngleV = 85.0f;				// 수직 최대각
	static float minAngleV = -85.0f;			// 수직 최저각
	static float sensitivityH = 1.0f;			// 가로 민감도
	static float sensitivityV = 1.0f;			// 세로 민감도
	static D3DXVECTOR3 nowVelocity(0, 0, 0);	// 현재 방향과 속도를 가진 벡터

	static float accelate = 30.0f;				// 초당 이동 증가값
	static float nowSpeed = 3.0f;				// 현재 속도
	static float maxSpeed = 10.0f;				// 최고 속도

												// 최초 누를때는 마우스 위치를 화면 가운데에 놓고 시작하기
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON))
	{
		// 화면의 중심위치
		int screenCenterX = WINSIZEX / 2;
		int screenCenterY = WINSIZEY / 2;
		// 마우스 위치를 센터로 변경하기
		setMousePos(screenCenterX, screenCenterY);
	}
	// 우클릭을 할때만 디폴트컨트롤을 한다
	else if (KEYMANAGER->isStayKeyDown(VK_RBUTTON))
	{
		// 입력 방향벡터
		D3DXVECTOR3 inputVector(0, 0, 0);
		if (KEYMANAGER->isStayKeyDown('W'))
		{
			inputVector.z = 1.0f;
		}
		else if (KEYMANAGER->isStayKeyDown('S'))
		{
			inputVector.z = -1.0f;
		}
		if (KEYMANAGER->isStayKeyDown('A'))
		{
			inputVector.x = -1.0f;
		}
		else if (KEYMANAGER->isStayKeyDown('D'))
		{
			inputVector.x = 1.0f;
		}
		if (KEYMANAGER->isStayKeyDown('R'))
		{
			inputVector.y = 1.0f;
		}
		else if (KEYMANAGER->isStayKeyDown('F'))
		{
			inputVector.y = -1.0f;
		}

		// 제로 벡터가 아닐때
		if (VECTORZERO(inputVector) == false)
		{
			// 정규화
			D3DXVec3Normalize(&inputVector, &inputVector);
		}

		/*
		// ====================================================================
		// 			## 가속개념 있는 이동 ##
		// ====================================================================

		// 타겟벡터
		D3DXVECTOR3 target = inputVector * maxSpeed;
		D3DXVec3TransformNormal( &target, &target, &this->matFinal );

		// 타겟벡터와 현제Velocity 크기차
		float targetLength = D3DXVec3Length( &target ); // D3DXVec3Length 벡터의 크기를 얻는다.
		float nowVelocityLength = D3DXVec3Length( &nowVelocity );
		float dist = abs( targetLength - nowVelocityLength );

		// 타겟과 차이가 존재한다면..
		if(FLOATZERO( dist ) == false)
		{
		// 변화 허용량
		float delta = accelate * timeDelta;

		// 보간값
		float t = clamp01(delta / dist);

		// 이번프레임에 타겟까지 변화 하는데 충분하다
		if(FLOATEQUAL( t, 1.0f ))
		{
		nowVelocity = target;
		}
		else
		{
		nowVelocity = VecLerp( nowVelocity, target, t);
		}
		}

		// 이동 벡터는
		D3DXVECTOR3 moveDeltaVector = nowVelocity * timeDelta;
		// 이동 해라
		this->MovePositionWorld( moveDeltaVector );
		*/

		// 이동하기
		// 타겟벡터
		D3DXVECTOR3 target = inputVector * nowSpeed;
		this->movePositionSelf(target * _deltaTime);

		// 회전처리
		// 화면의 중심위치
		int screenCenterX = WINSIZEX / 2;
		int screenCenterY = WINSIZEY / 2;

		// 현재 마우스 위치
		POINT mousePos = getMousePos();

		// 이동량(중심에서 멀어진 량)
		float deltaX = mousePos.x - screenCenterX;
		float deltaY = mousePos.y - screenCenterY;

		// 앵글 추가
		nowAngleH += deltaX * sensitivityH;
		nowAngleV += deltaY * sensitivityV;

		// 앵글값을 Min Max 범위 안으로
		nowAngleV = clamp(nowAngleV, minAngleV, maxAngleV);

		// 다시 마우스 위치로 세팅
		setMousePos(screenCenterX, screenCenterY);

		this->setRotateWorld(D3DXToRadian(nowAngleV), D3DXToRadian(nowAngleH), 0.0f);
	}
}

void transform::defaultMyControl()
{
	// 초당 3씩, 초당 90도씩 움직여라
	float deltaMove = 3.0f * _deltaTime;
	float deltaAngle = D3DXToRadian(90) * _deltaTime;

	if (KEYMANAGER->isStayKeyDown(VK_RBUTTON) == FALSE)
	{
		if (KEYMANAGER->isStayKeyDown('A'))
		{
			this->movePositionSelf(-deltaMove, 0.0f, 0.0f);
		}
		else if (KEYMANAGER->isStayKeyDown('D'))
		{
			this->movePositionSelf(deltaMove, 0.0f, 0.0f);
		}
		if (KEYMANAGER->isStayKeyDown('W'))
		{
			this->movePositionSelf(0.0f, 0.0f, deltaMove);
		}
		else if (KEYMANAGER->isStayKeyDown('S'))
		{
			this->movePositionSelf(0.0f, 0.0f, -deltaMove);
		}
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			this->rotateSelf(0.0f, -deltaAngle, 0.0f);
		}
		else if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			this->rotateSelf(0.0f, deltaAngle, 0.0f);
		}
		if (KEYMANAGER->isStayKeyDown(VK_UP))
		{
			this->rotateSelf(-deltaAngle, 0.0f, 0.0f);
		}
		else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
		{
			this->rotateSelf(deltaAngle, 0.0f, 0.0f);
		}
	}
}

// 트렌스폼에 대한 기즈모 그리기
void transform::renderGizmo(float scale)
{
	this->setDeviceWorld();

	GIZMOMANAGER->lineMake(_position, _position + this->right * scale, 0xffff0000);
	GIZMOMANAGER->lineMake(_position, _position + this->up * scale, 0xff00ff00);
	GIZMOMANAGER->lineMake(_position, _position + this->forward * scale, 0xff0000ff);
}
