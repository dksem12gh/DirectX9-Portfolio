#include "stdafx.h"
#include "meshInfo.h"


HRESULT meshInfo::init(void)
{
	this->playerInit();		//플레이어를 초기화
	this->wearInit();		//옷을 초기화

	_lHand = _lHandPalm;		// 기본 손을 편손으로
	_frame->getSkinnedMesh()->changeMesh((BONE*)_lHand->getSkinnedMesh()->getRootBone());
	_lHand->setActive(true);

	_rHand = _rHandPalm;		// 기본 손을 편손으로
	_frame->getSkinnedMesh()->changeMesh((BONE*)_rHand->getSkinnedMesh()->getRootBone());
	_rHand->setActive(true);

	//머리
	_frame->getSkinnedMesh()->changeMesh((BONE*)_hair->getSkinnedMesh()->getRootBone());
	_hair->setActive(true);

	_characterPosition = new transform;


	_characterPosition->setWorldPosition(2, 0, 2);
	_frame->_transform->setWorldPosition(_characterPosition->getWorldPosition());

	_wear = EQUIP->addSampleItem("기본 바디",									// 이름
		"Resources/mabinogi/char/Meshs/wear/defaultBody/",		// 메쉬 폴더 경로
		"Resources/mabinogi/char/materials/",					// 마테리얼 경로
		"",
		0,
		0,
		0,
		0,
		0,
		BODY,													// 파트 타입
		0,
		1,
		&_matCorrection);
	_wear->setMesh(_frame);

	_shoes = NULL;

	return S_OK;
}

void meshInfo::release(void)
{
	SAFE_DELETE(_frame);
	SAFE_DELETE(_lHandPalm);
	SAFE_DELETE(_lHandFist);
	SAFE_DELETE(_rHandPalm);
	SAFE_DELETE(_rHandFist);
	SAFE_DELETE(_hair);
}

void meshInfo::update(void)
{
	_frame->update();

	if (_frame->_skinnedAnim->getFactor() >= 0.98f)
	{
		float delta = _frame->_skinnedAnim->getFactor();
		_frame->_transform->setWorldPosition(_characterPosition->getWorldPosition());
	}

	if (_wear != NULL) _wear->update();
	if (_shoes != NULL) _shoes->update();
	if (_weapon != NULL) _weapon->update();
}

void meshInfo::render(void)
{
	_frame->render();
	_lHand->render();
	_rHand->render();
	_hair->render();

	//_frame->_boundBox.renderGizmo(_frame->_transform);
	//_characterPosition->renderGizmo();
	//_frame->_transform->renderGizmo();

	if (_wear != NULL) _wear->render();
	if (_shoes != NULL) _shoes->render();
	if (_weapon != NULL) _weapon->render();
}

//플레이어 초기화
void meshInfo::playerInit()
{
	//메인 프레임
	_frame = new baseObject;
	_frame->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/maleFrame.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));
	_frame->setActive(true);

	//왼손 초기화(주먹, 손바닥)
	_lHandPalm = new baseObject;
	_lHandPalm->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/lHand1.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));
	_lHandFist = new baseObject;
	_lHandFist->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/lHand2.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));

	// 오른손 초기화
	_rHandPalm = new baseObject;
	_rHandPalm->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/rHand1.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));

	_rHandFist = new baseObject;
	_rHandFist->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/rHand2.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));

	//머리
	_hair = new baseObject;
	_hair->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/hair.x", "Resources/mabinogi/char/materials/", &_matCorrection));
}

void meshInfo::wearInit()
{
	
}

void meshInfo::changeBody(equip * pEquip)
{
	if (_wear != NULL) _wear->releaseMesh();

	if (pEquip == NULL)
	{
		_wear = EQUIP->findSampleItem("기본 바디");
	}
	else
	{
		_wear = pEquip;
	}

	_wear->setMesh(_frame);
}

void meshInfo::changeShoes(equip * pEquip)
{
	if (_shoes != NULL) _shoes->releaseMesh();

	if (pEquip != NULL)
	{
		_shoes = pEquip;

		_shoes->setMesh(_frame);
	}
}

void meshInfo::changeWeapon(equip * pEquip)
{
	if (_weapon != NULL) _weapon->releaseMesh();

	if (pEquip != NULL)
	{
		_weapon = pEquip;

		_weapon->setMesh(_frame);
	}
}

void meshInfo::play(string anim)
{
	_frame->_skinnedAnim->Play(anim);
}

void meshInfo::play(string anim, string nextAnim)
{
	_frame->_skinnedAnim->Play(anim, nextAnim);
}

void meshInfo::PlayOneShotAfterHold(string anim)
{
	_frame->_skinnedAnim->PlayOneShotAfterHold(anim);
}

void meshInfo::leftHandChange(void)
{
	_lHand->setActive(false);		// 현재 손을 비활성화

	if (_lHand == _lHandPalm)		// 현재 손이 편 손이라면
	{
		_lHand = _lHandFist;		// 현재손을 주먹손으로
	}
	else							// 주먹손이라면
	{
		_lHand = _lHandPalm;		// 편손으로
	}

	// 프레임에 붙이고 활성화
	_frame->getSkinnedMesh()->changeMesh((BONE*)_lHand->getSkinnedMesh()->getRootBone());
	_lHand->setActive(true);
}

void meshInfo::rightHandChange(void)
{
	_rHand->setActive(false);		// 현재 손을 비활성화

	if (_rHand == _rHandPalm)		// 현재 손이 편 손이라면
	{
		_rHand = _rHandFist;		// 현재손을 주먹손으로
	}
	else							// 주먹손이라면
	{
		_rHand = _rHandPalm;		// 편손으로
	}

	// 프레임에 붙이고 활성화
	_frame->getSkinnedMesh()->changeMesh((BONE*)_rHand->getSkinnedMesh()->getRootBone());
	_rHand->setActive(true);
}

void meshInfo::lookPosition(D3DXVECTOR3 look)
{
	_characterPosition->lookPosition(look);							//목표지점을 바라보게함
	_frame->_transform->lookPosition(look);
	_frame->_transform->rotateWorld(0, D3DXToRadian(180), 0);
}

void meshInfo::movePosition(float moveSpeed)
{
	_characterPosition->movePositionSelf(0.0f, 0.0f, moveSpeed);		//이동속도대로 이동
	
	//float moveSet = _frame->_skinnedAnim->getFactor() * -1.18f;
}

void meshInfo::moveStop()
{
	_frame->_transform->setWorldPosition(_characterPosition->getWorldPosition());
}

void meshInfo::setYPosition(float YPosition)
{
	D3DXVECTOR3 framePosition;		//애니메이션 높이조절을 위한 vec3
	D3DXVECTOR3 charPosition;		//케릭터 높이조절을 위한 vec3
	//애니메이션 높이조절ing
	framePosition = _frame->_transform->getWorldPosition();
	framePosition.y = YPosition;
	_frame->_transform->setWorldPosition(framePosition);
	//플레이어 높이조절ing
	charPosition = _characterPosition->getWorldPosition();
	charPosition.y = YPosition;
	_characterPosition->setWorldPosition(charPosition);
}

void meshInfo::ridingMeshPosition()
{
	D3DXVECTOR3 adjustPosition(_characterPosition->getWorldPosition());		//플레이어 위치를 받아옴
	adjustPosition += _characterPosition->getForward() * 0.1f;				//플레이어위치의 앞으로 약간조정
	adjustPosition += _characterPosition->getRight() * 0.1f;				//플레이어위치의 오른쪽으로 약간조정

	_frame->_transform->setWorldPosition(adjustPosition);		//애니메이션 위치를 조정한 위치에 붙임
}

void meshInfo::AniMeshPosition()
{
	_frame->_transform->setWorldPosition(_characterPosition->getWorldPosition());		//애니메이션 위치를 조정한 위치에 붙임
}

void meshInfo::characterRotateY(float rotateY)
{
	_characterPosition->rotateSelf(0, rotateY, 0);
	_frame->_transform->lookDirection(_characterPosition->getForward());
	_frame->_transform->rotateWorld(0, D3DXToRadian(180), 0);
}

