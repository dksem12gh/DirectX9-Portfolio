#include "stdafx.h"
#include "Orge.h"

Orge::Orge()
{
}

Orge::~Orge()
{
}

HRESULT Orge::init(player* character, baseObject* _map, D3DXVECTOR3 randPos, camera* pCamera)
{
	D3DXMATRIXA16 matScale, matRotateX, matRotateY, matCorrection;

	D3DXMatrixScaling(&matScale, 0.0001f, 0.0001f, 0.0001f);
	D3DXMatrixRotationX(&matRotateX, D3DXToRadian(-90));
	D3DXMatrixRotationY(&matRotateY, D3DXToRadian(10));
	matCorrection = matRotateX * matRotateY * matScale;

	this->setMesh(RM_SKINNED->addResource("Resources/mabinogi/Monster/Orge/Orge_smoosh.x", &matCorrection));
	this->setActive(true);

	_char = character;
	initForRandomSeed();

	_OrgePosition = new transform;
	_transform->setWorldPosition(randPos);
	_SpawnPos = _transform->getWorldPosition();
	_OrgePosition->setWorldPosition(_transform->getWorldPosition());

	_scene = _map;
	_mainCamera = pCamera;

	_OrgeBox = new boundBox;
	_OrgeBox->setBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0.2f, 0.2f, 0.2f));

	// 혹시 피안줄면 포인터 체력부분 바꾸셈 ㅎㅂㅎ 
	_hpBar = new billboardProgress(_OrgePosition, &_Orge_maxHp, &_Orge_hp, 0, 0,
		0xffc0c000, 0xffff0000, 0.55f);

	// 스톰프 지금 없음 
	_curSkill[0] = NULL;
	_curSkill[1] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Smash.png");
	_curSkill[2] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Defence.png");
	_curSkill[3] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Counter.png");
	_curSkill[4] = RM_TEXTURE->addResource("Resources/mabinogi/UI/ActiveSkill_Windmill.png");
	

	return S_OK;
}

void Orge::release(void)
{
}

void Orge::update(void)
{
	baseObject::update();
	baseObject::_transform->setWorldPosition(_transform->getWorldPosition());

}

void Orge::render(void)
{
	baseObject::render();

	_OrgePosition->renderGizmo();
	_transform->renderGizmo();
	_OrgeBox->renderGizmo(_OrgePosition);

}

void Orge::state()
{
	switch (_O_state)
	{
	case O_idle:
		break;
	case O_attack_idle:
		break;
	case O_walk:
		break;
	case O_run:
		break;
	case O_attack:
		break;
	case O_defense:
		break;
	case O_counter:
		break;
	case O_smash:
		break;
	case O_windmill:
		break;
	case O_stomp_wating:
		break;
	case O_stomp:
		break;
	case O_endureA:
		break;
	case O_endureB:
		break;
	case O_endure:
		break;
	case O_fly:
		break;
	case O_die:
		break;
	case O_die_to_up:
		break;
	default:
		break;
	}
}

void Orge::hit()
{
}

void Orge::endureA()
{
}

void Orge::Smash_endure()
{
}
