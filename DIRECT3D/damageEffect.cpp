#include "stdafx.h"
#include "damageEffect.h"


bool damageEffect::update(void)
{
	if (_maxUpPos > 0.0f)
	{
		_effectPos.y -= _upSpeed;
		_maxUpPos -= _upSpeed;
		_effectTexSize += 0.05f;
	}
	
	_fontColor.a -= 0.01f;
	_effectTexAlpha.a -= 0.05f;

	if (_fontColor.a <= 0.1f)
	{
		return false;
	}
		
	return true;
}

void damageEffect::render(void)
{
	D3DSURFACE_DESC	desc;
	_hitEffect->GetLevelDesc(0, &desc);
	FONTMANAGER->fontOutLine("damageFont" ,to_string((int)_damage), _effectPos.x, _effectPos.y, DWORD(_fontColor), 0xff000000);
	SPRITEMANAGER->spriteRender(_hitEffect, NULL, _effectTexPos.x - ((int)(desc.Width /2) * _effectTexSize), _effectTexPos.y - ((int)(desc.Height / 2) * _effectTexSize), _effectTexSize, _effectTexSize, 0, DWORD(_effectTexAlpha));
}

damageEffect::damageEffect(float damage, D3DXVECTOR3* effectPos, DWORD color)
{
	_damage = damage;
	string strDmg = to_string((int)damage);

	D3DXVECTOR2 fontPos;

	MAINCAMERA->getWorldPosToScreenPos(&fontPos, effectPos);

	_effectPos.x = fontPos.x - (((int)strDmg.length() * 10) / 2) + MYUTIL::randomIntRange(-5, 5);
	_effectPos.y = fontPos.y - 120.0f;
	_maxUpPos = 50.0f;
	_upSpeed = 2.0f;
	_fontColor = D3DXCOLOR(color);

	
	_hitEffect = RM_TEXTURE->addResource("Resources/mabinogi/effect/hitEffect.dds");
	_effectTexPos = fontPos;
	_effectTexPos.y += MYUTIL::randomIntRange(-5, 5);
	_effectTexPos.y -= (60.0f + MYUTIL::randomIntRange(-5, 5));
	_effectTexAlpha = D3DXCOLOR(0xffffffff);
	_effectTexSize = 0.1f;
}


damageEffect::~damageEffect()
{
}
