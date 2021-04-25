#include "stdafx.h"
#include "meshInfo.h"


HRESULT meshInfo::init(void)
{
	this->playerInit();		//�÷��̾ �ʱ�ȭ
	this->wearInit();		//���� �ʱ�ȭ

	_lHand = _lHandPalm;		// �⺻ ���� �������
	_frame->getSkinnedMesh()->changeMesh((BONE*)_lHand->getSkinnedMesh()->getRootBone());
	_lHand->setActive(true);

	_rHand = _rHandPalm;		// �⺻ ���� �������
	_frame->getSkinnedMesh()->changeMesh((BONE*)_rHand->getSkinnedMesh()->getRootBone());
	_rHand->setActive(true);

	//�Ӹ�
	_frame->getSkinnedMesh()->changeMesh((BONE*)_hair->getSkinnedMesh()->getRootBone());
	_hair->setActive(true);

	_characterPosition = new transform;


	_characterPosition->setWorldPosition(2, 0, 2);
	_frame->_transform->setWorldPosition(_characterPosition->getWorldPosition());

	_wear = EQUIP->addSampleItem("�⺻ �ٵ�",									// �̸�
		"Resources/mabinogi/char/Meshs/wear/defaultBody/",		// �޽� ���� ���
		"Resources/mabinogi/char/materials/",					// ���׸��� ���
		"",
		0,
		0,
		0,
		0,
		0,
		BODY,													// ��Ʈ Ÿ��
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

//�÷��̾� �ʱ�ȭ
void meshInfo::playerInit()
{
	//���� ������
	_frame = new baseObject;
	_frame->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/maleFrame.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));
	_frame->setActive(true);

	//�޼� �ʱ�ȭ(�ָ�, �չٴ�)
	_lHandPalm = new baseObject;
	_lHandPalm->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/lHand1.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));
	_lHandFist = new baseObject;
	_lHandFist->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/lHand2.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));

	// ������ �ʱ�ȭ
	_rHandPalm = new baseObject;
	_rHandPalm->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/rHand1.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));

	_rHandFist = new baseObject;
	_rHandFist->setMesh(RM_SKINNED->addResource("Resources/mabinogi/char/Meshs/rHand2.x",
		"Resources/mabinogi/char/materials/",
		&_matCorrection));

	//�Ӹ�
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
		_wear = EQUIP->findSampleItem("�⺻ �ٵ�");
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
	_lHand->setActive(false);		// ���� ���� ��Ȱ��ȭ

	if (_lHand == _lHandPalm)		// ���� ���� �� ���̶��
	{
		_lHand = _lHandFist;		// ������� �ָԼ�����
	}
	else							// �ָԼ��̶��
	{
		_lHand = _lHandPalm;		// �������
	}

	// �����ӿ� ���̰� Ȱ��ȭ
	_frame->getSkinnedMesh()->changeMesh((BONE*)_lHand->getSkinnedMesh()->getRootBone());
	_lHand->setActive(true);
}

void meshInfo::rightHandChange(void)
{
	_rHand->setActive(false);		// ���� ���� ��Ȱ��ȭ

	if (_rHand == _rHandPalm)		// ���� ���� �� ���̶��
	{
		_rHand = _rHandFist;		// ������� �ָԼ�����
	}
	else							// �ָԼ��̶��
	{
		_rHand = _rHandPalm;		// �������
	}

	// �����ӿ� ���̰� Ȱ��ȭ
	_frame->getSkinnedMesh()->changeMesh((BONE*)_rHand->getSkinnedMesh()->getRootBone());
	_rHand->setActive(true);
}

void meshInfo::lookPosition(D3DXVECTOR3 look)
{
	_characterPosition->lookPosition(look);							//��ǥ������ �ٶ󺸰���
	_frame->_transform->lookPosition(look);
	_frame->_transform->rotateWorld(0, D3DXToRadian(180), 0);
}

void meshInfo::movePosition(float moveSpeed)
{
	_characterPosition->movePositionSelf(0.0f, 0.0f, moveSpeed);		//�̵��ӵ���� �̵�
	
	//float moveSet = _frame->_skinnedAnim->getFactor() * -1.18f;
}

void meshInfo::moveStop()
{
	_frame->_transform->setWorldPosition(_characterPosition->getWorldPosition());
}

void meshInfo::setYPosition(float YPosition)
{
	D3DXVECTOR3 framePosition;		//�ִϸ��̼� ���������� ���� vec3
	D3DXVECTOR3 charPosition;		//�ɸ��� ���������� ���� vec3
	//�ִϸ��̼� ��������ing
	framePosition = _frame->_transform->getWorldPosition();
	framePosition.y = YPosition;
	_frame->_transform->setWorldPosition(framePosition);
	//�÷��̾� ��������ing
	charPosition = _characterPosition->getWorldPosition();
	charPosition.y = YPosition;
	_characterPosition->setWorldPosition(charPosition);
}

void meshInfo::ridingMeshPosition()
{
	D3DXVECTOR3 adjustPosition(_characterPosition->getWorldPosition());		//�÷��̾� ��ġ�� �޾ƿ�
	adjustPosition += _characterPosition->getForward() * 0.1f;				//�÷��̾���ġ�� ������ �ణ����
	adjustPosition += _characterPosition->getRight() * 0.1f;				//�÷��̾���ġ�� ���������� �ణ����

	_frame->_transform->setWorldPosition(adjustPosition);		//�ִϸ��̼� ��ġ�� ������ ��ġ�� ����
}

void meshInfo::AniMeshPosition()
{
	_frame->_transform->setWorldPosition(_characterPosition->getWorldPosition());		//�ִϸ��̼� ��ġ�� ������ ��ġ�� ����
}

void meshInfo::characterRotateY(float rotateY)
{
	_characterPosition->rotateSelf(0, rotateY, 0);
	_frame->_transform->lookDirection(_characterPosition->getForward());
	_frame->_transform->rotateWorld(0, D3DXToRadian(180), 0);
}

