#include "stdafx.h"
#include "enemyPlane.h"
//
//void enemyPlane::baseObjectEnable()
//{
//	//// 위치 랜덤으로 생성
//	this->_transform.setWorldPosition(randomFloatRange(-15.0f, 15.0f), 0, 50);
//
//	//// 속도 랜덤
//	_speed = randomFloatRange(1.0f, 5.0f);
//
//	//// 뒤를 보도록 방향설정
//	this->_transform.lookDirection(D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 1, 0));
//}
//
//void enemyPlane::baseObjectUpdate()
//{
//	if (this->_transform.getWorldPosition().z < -5.0f)
//	{
//	_nextGenTime = randomFloatRange(5.0f, 10.0f);
//	this->setActive(false);
//	}
//	else
//	{
//	this->_transform.movePositionSelf(0, 0, _speed * _deltaTime);
//	}
//}
//
//void enemyPlane::baseObjectNoActiveUpdate()
//{
//	_nextGenTime -= _deltaTime;
//	if (_nextGenTime <= 0.0f)
//	{
//	this->setActive(true);
//	}
//}
