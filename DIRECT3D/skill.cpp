#include "stdafx.h"
#include "skill.h"


skill::skill()
{
}

skill::~skill()
{
}

int skill::baseAttack(int minAtk, int maxAtk, int critical)
{
	int temp = randomIntRange(0, 100);
	
	if (temp < critical)
	{
		EFFECTMANAGER->changeSystemMsg("ũ��Ƽ�� ��Ʈ!");
		return randomIntRange(minAtk * 2, maxAtk * 2);
	}

	return randomIntRange(minAtk, maxAtk);
}