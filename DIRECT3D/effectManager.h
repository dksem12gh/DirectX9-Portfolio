#pragma once
#include "singletonBase.h"
#include "damageEffect.h"
class effectManager :
	public singletonBase<effectManager>
{
private:
	list<damageEffect> _lDamageEffect;
	LPDIRECT3DTEXTURE9	_criticalTex;
	string				_systemMsg;

	D3DXCOLOR			_msgColor;
	D3DXVECTOR2			_msgPos;

	bool				_isCritical;
	D3DXCOLOR			_criAlpha;
public:
	HRESULT init(void);
	void release(void);
	void update(void);
	void render(void);

	void addMonsterHit(D3DXVECTOR3* pos, float damage);
	void addPlayerHit(D3DXVECTOR3* pos, float damage);

	void changeSystemMsg(string msg);

	effectManager();
	~effectManager();
};

