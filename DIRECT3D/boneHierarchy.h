#pragma once

class xMeshSkinned;
/*
//Hierachy ������ �Ǿ��ִ� �� ������ ����
typedef struct _D3DXFRAME
{
LPSTR                   Name;						//������ �̸�
D3DXMATRIX              TransformationMatrix;		//��ȯ ��� ( ���ϸ��̼��� ����Ǹ� ���ϸ��̼ǿ� ���� ���� ��������� ���ŵȴ� )
LPD3DXMESHCONTAINER     pMeshContainer;				//�ش� ���� ������ �ް� �Ǵ� �޽� ������ ( ���⿡ BONE_MESH�� ������ ���� )
struct _D3DXFRAME       *pFrameSibling;				//�̿� ���
struct _D3DXFRAME       *pFrameFirstChild;			//ù��° �ڽĳ�� ( ù��° �ڽĳ��� ���� ����ؼ� �������� ��ũ�� ����Ʈ ������ �Ǿ��ִ� )
}D3DXFRAME, *LPD3DXFRAME;
*/

struct BONE : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;			//���������� �θ� ����� ����Ǿ� ��ȯ�� �Ϸ�� ���
	transform*	pBoneTransform;
	transform*	pApplyTransform;
};


/*
//�� �����ӿ� �����ִ� �޽õ����� ����
typedef struct _D3DXMESHCONTAINER
{
LPSTR                   Name;						//�޽��� ������ �̸�
D3DXMESHDATA            MeshData;					//�޽� ������
LPD3DXMATERIAL          pMaterials;					//�޽��� ���� �迭 ������
LPD3DXEFFECTINSTANCE    pEffects;					//�޽��� Effect
DWORD                   NumMaterials;				//�޽��� ������
DWORD                  *pAdjacency;					//�޽��� ���� ����
LPD3DXSKININFO          pSkinInfo;					//�޽��� ��Ű�� ����
struct _D3DXMESHCONTAINER *pNextMeshContainer;		//�ϳ��� �����ӿ� �ټ��� �޽õ����Ͱ� �����ִ� ��� ���� �޽õ������� ����
}D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;
*/

//
// D3DXMESHCONTAINER �� ��ӹ��� BONE_MESH ����ü (���� ������ �޽� ����)
//
struct BONE_MESH : public D3DXMESHCONTAINER
{
	//LPD3DXMESH					OriginalMesh;			//��ȯ�� �޽� ������
	LPD3DXMESH						WorkingMesh;			//�������̴��� ����ϱ����� �ִ� �ȷ�Ʈ�� MATRIX_PALETTE_SIZE ��� ��������� ������ �޽� 
	vector<D3DMATERIAL9>			Materials;				//�޽��� ���� ������
	vector<LPDIRECT3DTEXTURE9>		DiffuseTexs;			//�ε�� XFile ���� ���Ǵ� Texture ��
	vector<LPDIRECT3DTEXTURE9>		NormalTexs;
	vector<LPDIRECT3DTEXTURE9>		SpecularTexs;
	vector<LPDIRECT3DTEXTURE9>		EmissionTexs;
	vector<LPDIRECT3DTEXTURE9>		GlossTex;
	D3DXMATRIXA16**					ppBoneMatrixPtrs;		//������ ��� [ ������ �迭 ]
	D3DXMATRIXA16*					pBoneOffsetMatices;		//�ڽ��� �⺻ ��� �迭
															//D3DXMATRIXA16*				pCurrentBoneMatrices;	//���� Animation �� ����� ���

	DWORD							NumPaletteEntries;		//�����޽��� ����ȷ�Ʈ ��
	DWORD							MaxNumFaceInfls;			//�ش� �޽ÿ� ����Ǵ� �����ϳ��� �ִ� ����ġ ����
	DWORD							NumAttributesGroup;		//�޽��� �Ӽ� �׷�� (�ش� �޽ÿ� ���͸��������� ��ִ�? )
	LPD3DXBUFFER					BufBoneCombos;			//���ĺ���̼� (�޽ÿ� ����Ǵ� �� ID ������ ���͸��� ���� )
};



//
// ID3DXAllocateHierarchy Ŭ����( �������̽�) �� ��ӹ޴� BONE_HIRERARCHY Ŭ����
// ID3DXAllocateHierarchy �� �������� ����Լ��� �Ǿ��־�
// �ش� �ɹ� �Լ��� �۵��ϴ� �Ϳ� ���� �����Ǹ� ��ǥ�� �Ѵ�
//
class BONE_HIERARCHY : public ID3DXAllocateHierarchy
{
private:
	string			_fileName;		//�ε��Ǵ� ���� ��
	string			_filePath;		//�ε��Ǵ� ���� ���
	string			_texturePath;	// �ε��Ǵ� �ؽ��� ���
	xMeshSkinned*	_pSkinnedMesh;	//�ڽ��� �ε��ϴ� ��Ų�� �޽�

public:
	void SetLoadPath(string filePath);
	void SetLoadPath(string filePath, string texturePath);
	void SetSkinnedMesh(xMeshSkinned* pSkinnedMesh);

public:
	//
	// �� ������ ( ����� ) �Ҵ�
	//
	//virtual HRESULT CreateFrame( THIS_ LPCSTR Name,LPD3DXFRAME* ppNewFrame )	override	
	STDMETHOD(CreateFrame)(THIS_
		LPCSTR Name,					//������ �̸� ( �ҷ��� ���� �� �÷����� �̸��� ���´� )
		LPD3DXFRAME* ppNewFrame			//���Ӱ� �Ҵ�Ǿ� ������ ������ ( �츮�� ���⿡�� BONE �Ҵ��Ͽ� �Ű������� ���� �̸��� �����Ͽ� �Ҵ�����־�� �Ѵ� )
		) override;


	//
	// ��������( ����� ) �� �޽ø� �Ҵ�
	//
	//virtual HRESULT CreateMeshContainer( THIS_ LPCSTR Name,LPD3DXFRAME* ppNewFrame )	override
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,										//�Ҵ� �Ǵ� �޽��� �̸�
		CONST D3DXMESHDATA* pMeshData,						//�Ҵ� �Ǵ� �޽��� ������ 
		CONST D3DXMATERIAL* pMaterials,						//�Ҵ� �Ǵ� �޽��� ���͸���
		CONST D3DXEFFECTINSTANCE* pEffectInstance,			//Effect �� ����ϴ� �޽��� ��� Effect �ν��Ͻ� 
		DWORD NumMaterials,									//�Ҵ� �Ǵ� �޽��� ������ 
		CONST DWORD* pAdjacency,							//�Ҵ� �Ǵ� �޽��� ���� ���� ����
		LPD3DXSKININFO pSkinInfo,							//�Ҵ� �Ǵ� �޽��� ��Ű�� ���� ( ���� �����ӿ� ���� ��Ű�� ���� )
		LPD3DXMESHCONTAINER* ppNewMeshContainer			//���Ӱ� �Ҵ�Ǿ� ������ �޽� ( �츮�� ���⿡�� ���� �Ű������� ���� ������� BONE_MESH �� �Ҵ��Ͽ� ���� �� �־�� �Ѵ� )
		) override;


	//
	// ������ ( ����� ) �� ����
	//
	STDMETHOD(DestroyFrame)(THIS_
		LPD3DXFRAME pFrameToFree							//������ ���������� �ּ�( ���� CreateFrame ���� �Ҵ�� BONE ��ü�� �ּҰ� ���´� ) 
		) override;


	//
	// ������ ( ����� ) �޽õ����͸� ����
	//
	STDMETHOD(DestroyMeshContainer)(THIS_
		LPD3DXMESHCONTAINER pMeshContainerToFree			//������ ���޽� �������� �ּ� ( ���� CreateMeshContainer ���� �Ҵ�� BONE_MESH ��ü�� �ּҰ� ���´� ) 
		) override;
};