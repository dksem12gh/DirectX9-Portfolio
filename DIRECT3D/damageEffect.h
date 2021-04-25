#pragma once

class damageEffect
{
private:
	LPDIRECT3DTEXTURE9	_hitEffect;
	D3DXVECTOR2			_effectPos;

	float				_damage;
	float				_maxUpPos;
	float				_upSpeed;

	D3DXVECTOR2			_effectTexPos;
	D3DXCOLOR			_effectTexAlpha;
	float				_effectTexSize;
	D3DXCOLOR			_fontColor;

public:

	bool update(void);
	void render(void);

	damageEffect(float damage, D3DXVECTOR3* effectPos, DWORD color);
	~damageEffect();
};

