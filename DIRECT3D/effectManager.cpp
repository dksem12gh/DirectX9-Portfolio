#include "stdafx.h"
#include "effectManager.h"


HRESULT effectManager::init(void)
{
	FONTMANAGER->addFont("damageFont", "³ª´®°íµñ ExtraBold ", 20);
	FONTMANAGER->addFont("systemFont", "³ª´®°íµñ ExtraBold ", 36);
	_criticalTex = RM_TEXTURE->addResource("Resources/mabinogi/UI/whiteBlock.dds");

	_systemMsg = "";
	_isCritical = false;
	return S_OK;
}

void effectManager::release(void)
{
	_lDamageEffect.clear();
}

void effectManager::update(void)
{
	list<damageEffect>::iterator dmgIter = _lDamageEffect.begin();

	for (; dmgIter != _lDamageEffect.end(); ++dmgIter)
	{
		if (dmgIter->update() == false)
		{
			_lDamageEffect.erase(dmgIter);
			return;
		}
	}

	if (_criAlpha.a > 0.0f)
	{
		_criAlpha.a -= 0.05f;
	}
	else
	{
		_isCritical = false;
	}

	if (_msgColor.a > 0.0f)
	{
		_msgColor.a -= 0.005f;
	}
	else
	{
		_systemMsg = "";
	}
}

void effectManager::render(void)
{
	list<damageEffect>::iterator dmgIter = _lDamageEffect.begin();

	for (; dmgIter != _lDamageEffect.end(); ++dmgIter)
	{
		dmgIter->render();
	}

	if (_systemMsg != "" && _msgColor > 0.0f)
	{
		FONTMANAGER->fontOutLine("systemFont", _systemMsg, (int)_msgPos.x, (int)_msgPos.y, _msgColor, 0xff000000);
	}

	RECT rc = MYUTIL::rectMake(0, 0, WINSIZEX, WINSIZEY);
	if (_isCritical)
	{
		SPRITEMANAGER->spriteRender(_criticalTex, &rc, 0, 0, _criAlpha);
	}
}

void effectManager::addMonsterHit(D3DXVECTOR3* pos, float damage)
{
	damageEffect newEffect(damage, pos, 0xffff0000);

	_lDamageEffect.push_back(newEffect);
}

void effectManager::addPlayerHit(D3DXVECTOR3 * pos, float damage)
{
	damageEffect newEffect(damage, pos, 0xffffffff);

	_lDamageEffect.push_back(newEffect);
}

effectManager::effectManager()
{
}


effectManager::~effectManager()
{
}

void effectManager::changeSystemMsg(string msg)
{
	_systemMsg = msg;

	_msgPos.x = WINSIZEX / 2 - ((_systemMsg.length() * 14) / 2);
	_msgPos.y = 200;

	_isCritical = true;
	_msgColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	_criAlpha = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}