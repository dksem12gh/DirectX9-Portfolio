#pragma once
#include "baseObject.h"
#include "equip.h"

class meshInfo
{
private:
	baseObject *	_frame;				// �÷��̾� �Ӹ�(������)
	baseObject*		_lHand;				// ���� ���� ��
	baseObject*		_lHandFist;			// ���� �ָԼ�
	baseObject*		_lHandPalm;			// ���� ���
	baseObject*		_rHand;				// ���� �����ʼ�
	baseObject*		_rHandPalm;			// ������ �ָԼ�
	baseObject*		_rHandFist;			// ������ ���
	baseObject*		_hair;				// �Ӹ�~

	equip*			_wear;				// ��
	equip*			_shoes;				// �Ź�
	equip*			_weapon;			// ����

	transform * _characterPosition;		//�÷��̾� ������ǥ

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void playerInit();					//�÷��̾� �޽� �ʱ�ȭ
	void wearInit();					//�� �޽� �ʱ�ȭ

	void changeBody(equip* pEquip);
	void changeShoes(equip* pEquip);
	void changeWeapon(equip* pEquip);

	void play(string anim);						//�����ؼ� �� �ִϸ��̼Ǹ� ����
	void play(string anim, string nextAnim);	//anim�� �����ϰ� �ִϸ��̼��� ������ nextAnim�� ����
	void PlayOneShotAfterHold(string anim);		//�ѹ� ����ϰ� �����

	void leftHandChange(void);					// �� �ٲٱ�
	void rightHandChange(void);

	void lookPosition(D3DXVECTOR3 look);
	void movePosition(float moveSpeed);
	void moveStop();

	D3DXVECTOR3 getPlayerPosition() { return _characterPosition->getWorldPosition(); }	//�÷��̾� ��ġ���� get

	void setYPosition(float YPosition);			//�������� �����Լ�

	//ī�޶� ���� get,set
	transform* getCharacterPosition() { return _characterPosition; }
	void setCharacterPosition(transform* _cPosition) { _characterPosition = _cPosition; }

	//�ִϸ��̼� ��ġ������ ���� �Լ�
	void ridingMeshPosition();

	//�ִϸ��̼ǰ� ������ǥ�� ��ġ��Ű������ �Լ�
	void AniMeshPosition();

	//�ɸ��� ȸ���� ���� �Լ�
	void characterRotateY(float rotateY);

	//�ִϸ��̼� ���������� �޾ƿ��� ���� get
	float getAniFactor() { return _frame->_skinnedAnim->getFactor(); }

	//�浹 ���̸� ��������� �ɸ����� �չ����� �޾ƿ�
	D3DXVECTOR3 getForward() { return _frame->_transform->getForward(); }

	meshInfo() : _wear(NULL), _shoes(NULL), _weapon(NULL){};
	~meshInfo() {};
};

