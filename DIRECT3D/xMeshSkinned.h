#pragma once
#include "xMesh.h"
#include "boneHierarchy.h"
#include "skinnedAnimation.h"

class xMeshSkinned : public xMesh
{
private:
	friend class BONE_HIERARCHY;		//본하이어 라키는 내친구....
	friend class skinnedAnimation;		//skinnedAnimation 는 내친구...
	typedef map<string, LPD3DXANIMATIONSET>				MAP_ANIMSET;
	typedef vector<LPD3DXANIMATIONSET>					VEC_ANIMSET;
	typedef vector<LPDIRECT3DTEXTURE9>					VEC_TEXTURE;

public:
	static LPD3DXEFFECT				_sSkinnedMeshEffect;		//StaticMesh 를 그리는 Effect
private:
	LPD3DXEFFECT					_pSkinnedEffect;			//Skinned 메쉬 Effect;
	LPD3DXFRAME						_pRootbone;					//Skinned 메쉬의 루트 본
	D3DXMATRIXA16					_matCorrection;				//보정행렬
	DWORD							_dwWorkingPaletteSize;		//행렬 작업 팔래트수
	D3DXMATRIX *					_pmWorkingPalette;			//행렬 작업 팔래트 
	
																/////////////////////////////////////////////////////////////////////////////////

	LPD3DXANIMATIONCONTROLLER		_pAnimController;			//Animation 컨트롤러
	UINT							_AnimNum;					//Animation 갯수
	MAP_ANIMSET						_mapAnimSet;				//Map 으로 되어있는 AnimationSet
	VEC_ANIMSET						_vecAnimSet;				//Vector 로 되어있는 AnimationSet
	VEC_TEXTURE						_vecBaseTexture;			//원본 텍스처
	VEC_TEXTURE						_vecDiffTexture;			//염색된 텍스처

	LPD3DXANIMATIONSET				_pNowPlayAnimationSet;		//현제 플레이되고있는 AnimationSet
	D3DXTRACK_DESC					_Track_Desc_0;				//0번 Track_Desc ( 0 번 Track 의 정보 )

	bool							_bPlay;						//지금 플레이 중이니?
	bool							_bLoop;						//루프 플레이니?
	LPD3DXANIMATIONSET				_pPrevPlayAnimationSet;		//OneShot 플레이시 한번 Animation 플레이되고 다시 되돌아갈 Animaiton

	float							_fCrossFadeTime;			//크로스페이드 타임
	float							_fLeftCrossFadeTime;		//남은크로스페이드 타임
	float							_fOutCrossFadeTime;			//OneShot Animation 끝나고 되돌아갈때 CrossFade 타임
	double							_AnimationPlayFactor;		//Animation 진행 Factor ( 0 이면 처음 1 일수록 마지막 )

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

	//Animation 정보를 출력한다.
	void ShowAnimationName(int x, int y);

	//해당 인덱스의 Animation Set 을 얻는다.
	LPD3DXANIMATIONSET GetAnimSet(int index)
	{
		return _vecAnimSet[index];
	}

	// 현재 애니메이션 진행 펙터를 얻는다.
	double getAnimationPlayFactor(void) { return _AnimationPlayFactor; };
	// 현재 애니메이션 진행 펙터를 세팅한다.
	void setAnimationPlayFactor(double playFactor) { _AnimationPlayFactor = playFactor; };

	//본이름으로 본찾는다.
	BONE* GetFineBONE(std::string boneName);

private:
	//본 행렬을 셋팅한다.
	void InitBoneMatrixPointer(BONE* pBone);
	//본행렬 을 업데이트한다.
	void UpdateMatrices(BONE* pBone, D3DXMATRIXA16* pParentMatrix);

	//본을 그린다.
	void RenderBone(BONE* pBone);
	void outlineRenderBone(BONE* pBone);

	//본이름을 출력한다.
	void RenderChildBoneName(camera* pCam, BONE* pBone, D3DXVECTOR3* parentPos);
	//Animation 을 바꾼다.
	void SetAnimation(LPD3DXANIMATIONSET animSet);

public:
	// 카메라 셋팅
	static void setCamera(camera* camera);
	//베이스 광원 세팅 (베이스 광원은 방향성 광원이다)
	static void setBaseLight(lightDirection* pDirLight);
	//광원 세팅
	static void setLighting(light** pLights, int lightNum);
	static void setLighting(vector<light*>* pLights);
	static void setBaseLighting(light** ppLights);
	//테크닉 세팅
	static void setTechniqueName(string name);

public:
	VEC_TEXTURE* getBaseTexVector(void) { return &_vecBaseTexture; }
	VEC_TEXTURE* getDiffTexVector(void) { return &_vecDiffTexture; }

	void changeMesh(BONE* pRootBone) { this->InitBoneMatrixPointer(pRootBone); }

	BONE* getRootBone(void) { return (BONE*)_pRootbone; }

};