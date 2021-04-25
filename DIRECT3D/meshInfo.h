#pragma once
#include "baseObject.h"
#include "equip.h"

class meshInfo
{
private:
	baseObject *	_frame;				// 플레이어 머리(프레임)
	baseObject*		_lHand;				// 현재 왼쪽 손
	baseObject*		_lHandFist;			// 왼쪽 주먹손
	baseObject*		_lHandPalm;			// 왼쪽 편손
	baseObject*		_rHand;				// 현재 오른쪽손
	baseObject*		_rHandPalm;			// 오른쪽 주먹손
	baseObject*		_rHandFist;			// 오른쪽 편손
	baseObject*		_hair;				// 머리~

	equip*			_wear;				// 옷
	equip*			_shoes;				// 신발
	equip*			_weapon;			// 무기

	transform * _characterPosition;		//플레이어 실제좌표

public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void playerInit();					//플레이어 메쉬 초기화
	void wearInit();					//옷 메쉬 초기화

	void changeBody(equip* pEquip);
	void changeShoes(equip* pEquip);
	void changeWeapon(equip* pEquip);

	void play(string anim);						//연속해서 한 애니메이션만 실행
	void play(string anim, string nextAnim);	//anim를 실행하고 애니메이션이 끝나면 nextAnim를 실행
	void PlayOneShotAfterHold(string anim);		//한번 재생하고 멈춘다

	void leftHandChange(void);					// 손 바꾸기
	void rightHandChange(void);

	void lookPosition(D3DXVECTOR3 look);
	void movePosition(float moveSpeed);
	void moveStop();

	D3DXVECTOR3 getPlayerPosition() { return _characterPosition->getWorldPosition(); }	//플레이어 위치벡터 get

	void setYPosition(float YPosition);			//높이조절 적용함수

	//카메라를 위한 get,set
	transform* getCharacterPosition() { return _characterPosition; }
	void setCharacterPosition(transform* _cPosition) { _characterPosition = _cPosition; }

	//애니메이션 위치조정을 위한 함수
	void ridingMeshPosition();

	//애니메이션과 실제좌표를 일치시키기위한 함수
	void AniMeshPosition();

	//케릭터 회전을 위한 함수
	void characterRotateY(float rotateY);

	//애니메이션 진행정도를 받아오기 위한 get
	float getAniFactor() { return _frame->_skinnedAnim->getFactor(); }

	//충돌 레이를 만들기위한 케릭터의 앞방향을 받아옴
	D3DXVECTOR3 getForward() { return _frame->_transform->getForward(); }

	meshInfo() : _wear(NULL), _shoes(NULL), _weapon(NULL){};
	~meshInfo() {};
};

