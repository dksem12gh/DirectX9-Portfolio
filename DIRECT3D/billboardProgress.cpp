#include "stdafx.h"
#include "billboardProgress.h"

void billboardProgress::release(void)
{
	SAFE_RELEASE(_barTexture);
}

void billboardProgress::render(camera* pCamera)
{	
	D3DXVECTOR3	progressPos = _transform->getWorldPosition();
	progressPos.y += _upValue;

	D3DXVECTOR2 screenPos;
	pCamera->getWorldPosToScreenPos(&screenPos, &progressPos); // 3D ��ǥ�� 2D�� �ٲ�

	int barPosX = screenPos.x - _barWidth / 2;
	int barPosY = screenPos.y - _barHeight / 2;

	SPRITEMANAGER->spriteRender(_barTexture, NULL, barPosX, barPosY, _downColor);

	float compute = this->computeProgress();

	RECT upRect = MYUTIL::rectMake(0, 0, _barWidth * compute, _barHeight);

	SPRITEMANAGER->spriteRender(_barTexture, &upRect, barPosX, barPosY, _upColor);
}

void billboardProgress::skillRender(camera * pCamera, LPDIRECT3DTEXTURE9 skillTex)
{
	if (skillTex == NULL) return;

	D3DXVECTOR3	iconVec3Pos = _transform->getWorldPosition();
	iconVec3Pos.y += 1.0f;

	D3DXVECTOR2 screenPos;
	pCamera->getWorldPosToScreenPos(&screenPos, &iconVec3Pos); // 3D ��ǥ�� 2D�� �ٲ�

	D3DSURFACE_DESC		desc;
	skillTex->GetLevelDesc(0, &desc);

	int iconPosX = screenPos.x - desc.Width / 2;
	int iconPosY = screenPos.y - desc.Height / 2;
	
	SPRITEMANAGER->spriteRender(skillTex, NULL, iconPosX, iconPosY);

}

billboardProgress::billboardProgress(transform* pTransform, int* maxValue, int* curValue, int width, int height, DWORD downColor, DWORD upColor, float upValue)
	: _transform(NULL)
{
	// �������� ������ 
	_transform = pTransform;

	D3DXCreateTextureFromFile(device,
		"Resources/mabinogi/Ui/progress.dds",
		&_barTexture);

	// �ִ�ü�� �ּҰ�
	_maxValue = maxValue;

	// ����ü��
	_curValue = curValue;

	D3DSURFACE_DESC desc;
	_barTexture->GetLevelDesc(0, &desc);

	// ���� 00 ��õ ��0�� 
	if (width == 0) _barWidth = (int)desc.Width;
	else _barWidth = width;

	// 00 ��õ
	if (height == 0) _barHeight = (int)desc.Height;
	else _barHeight = height;

	_downColor = downColor;
	_upColor = upColor;
	_upValue = upValue;
}
