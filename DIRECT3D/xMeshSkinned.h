#pragma once
#include "xMesh.h"
#include "boneHierarchy.h"
#include "skinnedAnimation.h"

class xMeshSkinned : public xMesh
{
private:
	friend class BONE_HIERARCHY;		//�����̾� ��Ű�� ��ģ��....
	friend class skinnedAnimation;		//skinnedAnimation �� ��ģ��...
	typedef map<string, LPD3DXANIMATIONSET>				MAP_ANIMSET;
	typedef vector<LPD3DXANIMATIONSET>					VEC_ANIMSET;
	typedef vector<LPDIRECT3DTEXTURE9>					VEC_TEXTURE;

public:
	static LPD3DXEFFECT				_sSkinnedMeshEffect;		//StaticMesh �� �׸��� Effect
private:
	LPD3DXEFFECT					_pSkinnedEffect;			//Skinned �޽� Effect;
	LPD3DXFRAME						_pRootbone;					//Skinned �޽��� ��Ʈ ��
	D3DXMATRIXA16					_matCorrection;				//�������
	DWORD							_dwWorkingPaletteSize;		//��� �۾� �ȷ�Ʈ��
	D3DXMATRIX *					_pmWorkingPalette;			//��� �۾� �ȷ�Ʈ 
	
																/////////////////////////////////////////////////////////////////////////////////

	LPD3DXANIMATIONCONTROLLER		_pAnimController;			//Animation ��Ʈ�ѷ�
	UINT							_AnimNum;					//Animation ����
	MAP_ANIMSET						_mapAnimSet;				//Map ���� �Ǿ��ִ� AnimationSet
	VEC_ANIMSET						_vecAnimSet;				//Vector �� �Ǿ��ִ� AnimationSet
	VEC_TEXTURE						_vecBaseTexture;			//���� �ؽ�ó
	VEC_TEXTURE						_vecDiffTexture;			//������ �ؽ�ó

	LPD3DXANIMATIONSET				_pNowPlayAnimationSet;		//���� �÷��̵ǰ��ִ� AnimationSet
	D3DXTRACK_DESC					_Track_Desc_0;				//0�� Track_Desc ( 0 �� Track �� ���� )

	bool							_bPlay;						//���� �÷��� ���̴�?
	bool							_bLoop;						//���� �÷��̴�?
	LPD3DXANIMATIONSET				_pPrevPlayAnimationSet;		//OneShot �÷��̽� �ѹ� Animation �÷��̵ǰ� �ٽ� �ǵ��ư� Animaiton

	float							_fCrossFadeTime;			//ũ�ν����̵� Ÿ��
	float							_fLeftCrossFadeTime;		//����ũ�ν����̵� Ÿ��
	float							_fOutCrossFadeTime;			//OneShot Animation ������ �ǵ��ư��� CrossFade Ÿ��
	double							_AnimationPlayFactor;		//Animation ���� Factor ( 0 �̸� ó�� 1 �ϼ��� ������ )

	LPDIRECT3DTEXTURE9				_normalTex;
	LPDIRECT3DTEXTURE9				_emissionTex;

public:
	xMeshSkinned();
	~xMeshSkinned() {}

	virtual HRESULT init(string filePath, D3DXMATRIXA16* pmatCorrection = NULL);
	virtual HRESULT init(string filePath, string mtrlPath, D3DXMATRIXA16* matCorrection = NULL) override;
	virtual void release();
	void update();
	virtual void render(transform* pTrans);

	void renderBoneName(camera* pCam, transform* transform = NULL);

	void Play(string animName, float crossFadeTime = 0.0);
	void Play(int animIndex, float crossFadeTime = 0.0);

	void PlayOneShot(string animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(std::string animName, float crossFadeTime = 0.0);

	void Stop() { _bPlay = false; }

	void SetPlaySpeed(float speed);

	//Animation ������ ����Ѵ�.
	void ShowAnimationName(int x, int y);

	//�ش� �ε����� Animation Set �� ��´�.
	LPD3DXANIMATIONSET GetAnimSet(int index)
	{
		return _vecAnimSet[index];
	}

	// ���� �ִϸ��̼� ���� ���͸� ��´�.
	double getAnimationPlayFactor(void) { return _AnimationPlayFactor; };
	// ���� �ִϸ��̼� ���� ���͸� �����Ѵ�.
	void setAnimationPlayFactor(double playFactor) { _AnimationPlayFactor = playFactor; };

	//���̸����� ��ã�´�.
	BONE* GetFineBONE(std::string boneName);

private:
	//�� ����� �����Ѵ�.
	void InitBoneMatrixPointer(BONE* pBone);
	//����� �� ������Ʈ�Ѵ�.
	void UpdateMatrices(BONE* pBone, D3DXMATRIXA16* pParentMatrix);

	//���� �׸���.
	void RenderBone(BONE* pBone);
	void outlineRenderBone(BONE* pBone);

	//���̸��� ����Ѵ�.
	void RenderChildBoneName(camera* pCam, BONE* pBone, D3DXVECTOR3* parentPos);
	//Animation �� �ٲ۴�.
	void SetAnimation(LPD3DXANIMATIONSET animSet);

public:
	// ī�޶� ����
	static void setCamera(camera* camera);
	//���̽� ���� ���� (���̽� ������ ���⼺ �����̴�)
	static void setBaseLight(lightDirection* pDirLight);
	//���� ����
	static void setLighting(light** pLights, int lightNum);
	static void setLighting(vector<light*>* pLights);
	static void setBaseLighting(light** ppLights);
	//��ũ�� ����
	static void setTechniqueName(string name);

public:
	VEC_TEXTURE* getBaseTexVector(void) { return &_vecBaseTexture; }
	VEC_TEXTURE* getDiffTexVector(void) { return &_vecDiffTexture; }

	void changeMesh(BONE* pRootBone) { this->InitBoneMatrixPointer(pRootBone); }

	BONE* getRootBone(void) { return (BONE*)_pRootbone; }

};