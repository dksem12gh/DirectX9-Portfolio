#include "stdafx.h"
#include "partItem.h"


void partItem::dyeingTex(DWORD color)
{
	dyeing dye; // ���� Ŭ����(�����Ҷ��� ������״� ���� �����Ҵ� ���� �ʿ�� ����.
	
	// �޽��� �ؽ�ó ���͸� �޾ƿ´�.(�����ؽ�ó���Ͷ�, �����Ǿ� ���� �ؽ�ó ����)
	vector<LPDIRECT3DTEXTURE9>* pBaseTextureVector = this->getSkinnedMesh()->getBaseTexVector();
	vector<LPDIRECT3DTEXTURE9>* pDiffTextureVector = this->getSkinnedMesh()->getDiffTexVector();

	for (int i = 0; i < pDiffTextureVector->size(); i++)
	{
		dye.dyeingColor((*pBaseTextureVector)[i], (*pDiffTextureVector)[i], color);
	}

}

// ���� �����ε�
void partItem::dyeingTex(D3DXCOLOR color)
{
	dyeing dye;

	vector<LPDIRECT3DTEXTURE9>* pBaseTextureVector = this->getSkinnedMesh()->getBaseTexVector();
	vector<LPDIRECT3DTEXTURE9>* pDiffTextureVector = this->getSkinnedMesh()->getDiffTexVector();

	for (int i = 0; i < pDiffTextureVector->size(); i++)
	{
		dye.dyeingColor((*pBaseTextureVector)[i], (*pDiffTextureVector)[i], DWORD(color));
	}
}

