#pragma once
#include "xMeshSkinned.h"

class skinnedAnimation
{
private:
	typedef map<BONE*, transform*>				MAP_BONETRANSFORM;	//매핑되는 본Transform 맵
	typedef map<string, LPD3DXANIMATIONSET >	MAP_ANIMSET;
	typedef vector< LPD3DXANIMATIONSET >		VEC_ANIMSET;

private:
	xMeshSkinned*		_pSkinnedMesh;
	MAP_BONETRANSFORM	_mapApplyBoneTransform;
	MAP_BONETRANSFORM	_mapBoneTransform;

	/////////////////////////////////////////////////////////////////////////////////

	LPD3DXANIMATIONCONTROLLER	_pAnimController;			//Animation 컨트롤러
	UINT						_AnimNum;					//Animation 갯수
	MAP_ANIMSET					_mapAnimSet;				//Map 으로 되어있는 AnimationSet
	VEC_ANIMSET					_vecAnimSet;				//Vector 로 되어있는 AnimationSet
	LPD3DXANIMATIONSET			_pNowPlayAnimationSet;		//현제 플레이되고있는 AnimationSet
	D3DXTRACK_DESC				_Track_Desc_0;				//0번 Track_Desc ( 0 번 Track 의 정보 )
	bool						_bPlay;						//지금 플레이 중이니?
	bool						_bLoop;						//루프 플레이니?
	LPD3DXANIMATIONSET			_pPrevPlayAnimationSet;		//OneShot 플레이시 한번 Animation 플레이되고 다시 되돌아갈 Animaiton
	float						_fCrossFadeTime;			//크로스페이드 타임
	float						_fLeftCrossFadeTime;		//남은크로스페이드 타임
	float						_fOutCrossFadeTime;			//OneShot Animation 끝나고 되돌아갈때 CrossFade 타임
	double						_AnimationPlayFactor;		//Animation 진행 Factor ( 0 이면 처음 1 일수록 마지막 )
	float						_fAnimDelta;

public:
	HRESULT	init(xMeshSkinned* pSkinnedMesh);
	void release();
	void update();
	void render(transform* pTransform);
	void renderBoneName(camera* pCam, transform* pTransform);

	void Play(string nowAnimName, string prevAnimName);
	void Play(string animName, float crossFadeTime = 0.0);
	void Play(int animIndex, float crossFadeTime = 0.0);
	void Play(LPD3DXANIMATIONSET animSet, float crossFadeTime = 0.0);
	void PlayOneShot(string animName, float inCrossFadeTime = 0.0, float outCrossFadeTime = 0.0f);
	void PlayOneShotAfterHold(string animName, float crossFadeTime = 0.0);
	void Stop() { _bPlay = false; }
	void SetPlaySpeed(float speed);

	void AddBoneTransform(std::string boneName, transform* pTransform);
	void AddApplyTransform(std::string boneName, transform* pTransform);
	void RemoveBoneTransform(std::string boneName);
	void RemoveApplyTransform(std::string boneName);

	double getFactor() { return _AnimationPlayFactor; }

public:
	skinnedAnimation();
	~skinnedAnimation();

private:
	//Animation 을 바꾼다.
	void SetAnimation(LPD3DXANIMATIONSET animSet);
};