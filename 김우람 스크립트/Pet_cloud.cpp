#include "stdafx.h"
#include "Pet_cloud.h"

// 펫 모두 완료 되었습니다 06.05(완) 어색해지면 이동 속도 조절해보렴

HRESULT Pet_cloud::init(meshInfo* character)
{
	D3DXMATRIXA16 matRotateX, matRotateY;
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(-90));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(0));

	D3DXMATRIXA16 matCorration = matRotateX * matRotateY;
	// 구름에 대한 메쉬셋팅 :) 
	this->setMesh(RM_SKINNED->addResource("Resources/mabinogi/Monster/pet_cloud/pet_cloud.x", &matCorration));

	_char = character;
	this->_transform->setRotateWorld(matRotateX * matRotateY);
	this->_transform->setScale(0.0001f, 0.0001f, 0.0001f);

	initForRandomSeed();
	return S_OK;
}

void Pet_cloud::release(void)
{
}

void Pet_cloud::update(void)
{
	baseObject::update();
	state();
}

void Pet_cloud::render(void)
{
	if (_cloudState != NONE) // 소환아닐떄만 렌더시킴
	{
		baseObject::render();
	}

	//_transform->renderGizmo();
/*
	char info[128];
	sprintf(info, "소환됨? : %f", _cloudState);
	FONTMANAGER->fontOut(info, 10, 330, 0xffffff00);*/

}

void Pet_cloud::state()
{
	_char->getPlayerPosition();
	// 기존의 바운드박스가 조오오오온나 커서 줄여야함 . ( 소스 나중에 고쳐야지 ) 
	//this->_boundBox.setBound(&D3DXVECTOR3(_transform->getWorldPosition().x /2,
	//	_transform->getWorldPosition().y / 2, _transform->getWorldPosition().z / 2),
	//	&D3DXVECTOR3(0, 0.2f, 0));

	// 머리가 플레이어 쪽을 바라보고 플레이어방향으로 움직인다
	if (!_riding)
	{
		this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
			_transform->getWorldPosition().y,
			_char->getPlayerPosition().z));
	}

	switch (_cloudState)
	{
	case NONE:
	{
		this->setActive(false);
	}
	break;
	case IDLE:
	{
		this->setActive(true);
		_skinnedAnim->Play("IDLE");
		_speed = 0;

		// 만약에 플레이어와 펫의 거리가 1보다 멀어질경우 플레이어를 따라가야한다. 상태 : 팔로우 -> 평상시)
		_distance = _char->getPlayerPosition() - _transform->getWorldPosition();
		_distanceResult = D3DXVec3Length(&_distance);

		if (!_riding)
		{
			if (_distanceResult > 2.0f)
			{
				_cloudState = Follow;
			}
		}

		if (_riding) this->ride();
		
	}
	break;

	case Follow:
	{
		this->setActive(true);
		_skinnedAnim->Play("RUN");

		_distance = _char->getPlayerPosition() - _transform->getWorldPosition();
		_distanceResult = D3DXVec3Length(&_distance);

		if (_distanceResult <= 1.0f)
		{
			_cloudState = IDLE;
		}

		_heightRay.origin = _transform->getWorldPosition() + D3DXVECTOR3(0, 2, 0);
		_heightRay.direction = D3DXVECTOR3(0, -1, 0);

		D3DXVECTOR3 heightRayHitPos;
		PHYSICSMANAGER->isRayHitStaticMeshObject(&_heightRay, _scene, &heightRayHitPos, NULL);

		this->setYPosition(heightRayHitPos.y);

		_transform->movePositionSelf(0.0f, 0.0f, 0.1f);		//이동속도대로 이동

	}
	break;

	default:
		break;
	}

}

void Pet_cloud::summon()
{
	if (_cloudState == NONE)
	{
		_random = randomFloatRange(-1.0f, 1.0f);
		_random1 = randomFloatRange(-1.0f, 1.0f);

		this->_transform->setWorldPosition(
			_char->getPlayerPosition().x + _random, 0,
			_char->getPlayerPosition().z + _random1);

		this->_transform->lookPosition(D3DXVECTOR3(_char->getPlayerPosition().x,
			0, _char->getPlayerPosition().z));

		_cloudState = IDLE;
	}

}

void Pet_cloud::Unsummon()
{

	_cloudState = NONE;

}

void Pet_cloud::ride()
{
	_cloudState = IDLE;

	_transform->setWorldPosition(_char->getPlayerPosition());
	_transform->lookDirection(_char->getCharacterPosition()->getForward());

	_heightRay.origin = _transform->getWorldPosition() + D3DXVECTOR3(0, 2, 0);
	_heightRay.direction = D3DXVECTOR3(0, -1, 0);

	D3DXVECTOR3 heightRayHitPos;
	PHYSICSMANAGER->isRayHitStaticMeshObject(&_heightRay, _scene, &heightRayHitPos, NULL);

	//비행상태일때와 아닐때 높이 변화
	if (_flying && _flyHeight > FLYHEIGHT)
	{
		_flyHeight = FLYHEIGHT;
	}
	else if (!_flying && _flyHeight < 0.0f)
	{
		_flyHeight = 0.0f;
	}

	if (_flying && _flyHeight < FLYHEIGHT)
	{
		_flyHeight += 0.1f;
	}
	else if (!_flying && _flyHeight > 0.0f)
	{
		_flyHeight -= 0.1f;
	}

	//06.18민병덕
	// + flyheight 추가
	this->setYPosition(heightRayHitPos.y + _flyHeight);
}

void Pet_cloud::setYPosition(float YPosition)
{
	D3DXVECTOR3 framePosition;		//애니메이션 높이조절을 위한 vec3
	D3DXVECTOR3 charPosition;		//구름 높이조절을 위한 vec3
									//애니메이션 높이조절ing

	framePosition = _transform->getWorldPosition();
	framePosition.y = YPosition;
	_transform->setWorldPosition(framePosition);
	//플레이어 높이조절ing
	charPosition = _transform->getWorldPosition();
	charPosition.y = YPosition;
	_transform->setWorldPosition(charPosition);
}

Pet_cloud::Pet_cloud()
{
	// 처음에는 소환해제 상태이다.
	_cloudState = NONE;

	_distanceResult = 0;
	_range = 0;
	_random = 0;
	_random1 = 0;

	_speed = 0;
	_riding = false;
	_flying = false;
}


Pet_cloud::~Pet_cloud()
{
}
