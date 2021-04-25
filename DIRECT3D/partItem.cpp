#include "stdafx.h"
#include "partItem.h"


void partItem::dyeingTex(DWORD color)
{
	dyeing dye; // 염색 클래스(염색할때만 사용할테니 굳이 동적할당 해줄 필요는 없다.
	
	// 메쉬의 텍스처 벡터를 받아온다.(원본텍스처벡터랑, 염색되어 나갈 텍스처 벡터)
	vector<LPDIRECT3DTEXTURE9>* pBaseTextureVector = this->getSkinnedMesh()->getBaseTexVector();
	vector<LPDIRECT3DTEXTURE9>* pDiffTextureVector = this->getSkinnedMesh()->getDiffTexVector();

	for (int i = 0; i < pDiffTextureVector->size(); i++)
	{
		dye.dyeingColor((*pBaseTextureVector)[i], (*pDiffTextureVector)[i], color);
	}

}

// 염색 오버로딩
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

