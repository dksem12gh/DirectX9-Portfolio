
#include "baseProgressBar.h"
class billboardProgress :
	public baseProgressBar
{
private:
	int							_textureWidth;
	int							_textureHeight;
	float						_upValue;
	transform*					_transform;
public:
	virtual void release(void) override;
	virtual void update(void) override {};
	void render(camera* pCamera);
	void skillRender(camera* pCamera, LPDIRECT3DTEXTURE9 skillTex);

	billboardProgress(transform* pTransform, int* maxValue, int* curValue, int width, int height, DWORD downColor, DWORD upColor, float upValue = 1.0f);

	~billboardProgress() {};
};
