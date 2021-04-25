 #include "stdafx.h"
#include "skinnedAnimation.h"
#include "transform.h"

skinnedAnimation::skinnedAnimation()
{
	_mapApplyBoneTransform.clear();
	_mapBoneTransform.clear();
}
skinnedAnimation::~skinnedAnimation()
{
}

HRESULT skinnedAnimation::init(xMeshSkinned * pSkinnedMesh)
{
	if (pSkinnedMesh == NULL) return E_FAIL;

	_pSkinnedMesh = pSkinnedMesh;

	//SKinned Mesh 에 Animation 를 복사한다.
	_pSkinnedMesh->_pAnimController->CloneAnimationController(
		_pSkinnedMesh->_pAnimController->GetMaxNumAnimationOutputs(),
		_pSkinnedMesh->_pAnimController->GetMaxNumAnimationSets(),
		_pSkinnedMesh->_pAnimController->GetMaxNumTracks(),
		_pSkinnedMesh->_pAnimController->GetMaxNumEvents(),
		&_pAnimController);

	//Animation 갯수를 얻는다.
	_AnimNum = _pAnimController->GetNumAnimationSets();

	for (UINT i = 0; i < _AnimNum; i++)
	{
		//에니메이션 셋 정보
		LPD3DXANIMATIONSET animSet;

		//i 번째 Animation 셋 정보를 얻는다.
		_pAnimController->GetAnimationSet(i, &animSet);

		//AnimationSet Vector 에 저장
		this->_vecAnimSet.push_back(animSet);

		//AnimationSet Map 에 저장
		this->_mapAnimSet.insert(make_pair(animSet->GetName(), animSet));
	}

	//일단 첫번째 Animation 으로 시작
	this->Play(0);
}

void skinnedAnimation::release()
{
	SAFE_RELEASE(_pAnimController);
}

void skinnedAnimation::update()
{
	//0 번 Track 정보를 얻는다
	_pAnimController->GetTrackDesc(0, &_Track_Desc_0);

	//_pNowPlayAnimationSet->GetPeriod() Animation 의 총시간을 얻는다.
	//현재 Animation 진행 Factor 를 갱신
	_AnimationPlayFactor = _Track_Desc_0.Position / _pNowPlayAnimationSet->GetPeriod()*2;

	//마지막에 도달했다면...
	if (_AnimationPlayFactor >= 0.98f)
	{
		if (this->_bLoop == false)
		{
			//돌아갈 Animation 이 있다면..
			if (this->_pPrevPlayAnimationSet != NULL)
			{
				_fCrossFadeTime = _fOutCrossFadeTime;
				_fLeftCrossFadeTime = _fOutCrossFadeTime;
				_bLoop = true;
				this->SetAnimation(_pPrevPlayAnimationSet);
				this->_pPrevPlayAnimationSet = NULL;
			}
			//돌아갈 Animation 이 없다면.
		}
		else
		{
			//_AnimationPlayFactor = 0;
		}
	}
	_AnimationPlayFactor = _AnimationPlayFactor - (int)_AnimationPlayFactor;	//정수부분 안생기게....

	if (_bPlay)
	{
		//애니메이션을 진행시킴
		//_pAnimController->AdvanceTime( _timeDelta, NULL );
		_fAnimDelta = _deltaTime;
	}
	else
	{
		_fAnimDelta = 0.0f;
	}

	//크로스 페이드가 진행중이라면..
	if (_fLeftCrossFadeTime > 0.0f)
	{
		//남은 크로스페이드 시간 뺀다
		this->_fLeftCrossFadeTime -= _deltaTime;

		//크로스페이드 가끝났다.
		if (_fLeftCrossFadeTime <= 0.0f)
		{
			_pAnimController->SetTrackWeight(0, 1);
			_pAnimController->SetTrackEnable(1, false);
		}
		else
		{
			float w1 = (_fLeftCrossFadeTime / _fCrossFadeTime);		//1번 Track 가중치
			float w0 = 1.0f - w1;									//0번 Track 가중치
																	//가중치 셋팅
			_pAnimController->SetTrackWeight(0, w0);
			_pAnimController->SetTrackWeight(1, w1);
		}
	}
}

void skinnedAnimation::render(transform * pTransform)
{
	//현재 자신의 Animation 정보로 셋팅
	_pAnimController->AdvanceTime(_fAnimDelta, NULL);
	_fAnimDelta = 0.0f;		//애니메이션을 진행시켰기 때문에 delta 량은 0 으로....

							//각 본에 행렬 대입
	MAP_BONETRANSFORM::iterator iterBone;
	for (iterBone = _mapBoneTransform.begin(); iterBone != _mapBoneTransform.end(); ++iterBone)
	{
		iterBone->first->pBoneTransform = iterBone->second;
	}
	for (iterBone = _mapApplyBoneTransform.begin(); iterBone != _mapApplyBoneTransform.end(); ++iterBone)
	{
		iterBone->first->pApplyTransform = iterBone->second;
	}
	_pSkinnedMesh->render(pTransform);
}

void skinnedAnimation::renderBoneName(camera * pCam, transform * pTransform)
{
	//현재 자신의 Animation 정보로 셋팅
	_pAnimController->AdvanceTime(_fAnimDelta, NULL);
	_fAnimDelta = 0.0f;		//애니메이션을 진행시켰기 때문에 delta 량은 0 으로....

							//각 본에 행렬 대입
	MAP_BONETRANSFORM::iterator iterBone;
	for (iterBone = _mapBoneTransform.begin(); iterBone != _mapBoneTransform.end(); ++iterBone)
	{
		iterBone->first->pBoneTransform = iterBone->second;
	}
	for (iterBone = _mapApplyBoneTransform.begin(); iterBone != _mapApplyBoneTransform.end(); ++iterBone)
	{
		iterBone->first->pApplyTransform = iterBone->second;
	}
	_pSkinnedMesh->renderBoneName(pCam, pTransform);
}

void skinnedAnimation::Play(string nowAnimName, string prevAnimName)
{
	_bPlay = true;
	_bLoop = false;

	MAP_ANIMSET::iterator nowFind = this->_mapAnimSet.find(nowAnimName);
	MAP_ANIMSET::iterator prevFind = this->_mapAnimSet.find(prevAnimName);
	if (nowFind != this->_mapAnimSet.end() && prevFind != this->_mapAnimSet.end())
	{
		//다음애니메이션 기억
		this->_pPrevPlayAnimationSet = prevFind->second;

		//크로스 페이드 타임 기억
		_fCrossFadeTime = 0;
		_fLeftCrossFadeTime = 0;

		this->SetAnimation(nowFind->second);
	}
}

void skinnedAnimation::Play(string animName, float crossFadeTime)
{
	_bPlay = true;
	_bLoop = true;

	MAP_ANIMSET::iterator find = this->_mapAnimSet.find(animName);
	if (find != this->_mapAnimSet.end())
	{
		//크로스 페이드 타임 기억
		_fCrossFadeTime = crossFadeTime;
		_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}

void skinnedAnimation::Play(int animIndex, float crossFadeTime)
{
	_bPlay = true;
	_bLoop = true;

	if (animIndex < this->_AnimNum)
	{
		//크로스 페이드 타임 기억
		_fCrossFadeTime = crossFadeTime;
		_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(_vecAnimSet[animIndex]);
	}
}

void skinnedAnimation::Play(LPD3DXANIMATIONSET animSet, float crossFadeTime)
{
	_bPlay = true;
	_bLoop = true;

	//크로스 페이드 타임 기억
	_fCrossFadeTime = crossFadeTime;
	_fLeftCrossFadeTime = crossFadeTime;

	this->SetAnimation(animSet);
}

void skinnedAnimation::PlayOneShot(string animName, float inCrossFadeTime, float outCrossFadeTime)
{
	_bPlay = true;
	_bLoop = false;

	MAP_ANIMSET::iterator find = this->_mapAnimSet.find(animName);
	if (find != this->_mapAnimSet.end())
	{
		//현재 Animaiton 을 기억한다.
		this->_pPrevPlayAnimationSet = this->_pNowPlayAnimationSet;

		//크로스 페이드 타임 기억
		_fCrossFadeTime = inCrossFadeTime;
		_fLeftCrossFadeTime = inCrossFadeTime;

		//나갈때 크로스페이드 타입 기억
		_fOutCrossFadeTime = outCrossFadeTime;

		this->SetAnimation(find->second);
	}
}

void skinnedAnimation::PlayOneShotAfterHold(string animName, float crossFadeTime)
{
	_bPlay = true;
	_bLoop = false;

	MAP_ANIMSET::iterator find = this->_mapAnimSet.find(animName);
	if (find != this->_mapAnimSet.end())
	{
		//돌아갈 Animation 은 없다
		this->_pPrevPlayAnimationSet = NULL;

		//크로스 페이드 타임 기억
		_fCrossFadeTime = crossFadeTime;
		_fLeftCrossFadeTime = crossFadeTime;

		this->SetAnimation(find->second);
	}
}

void skinnedAnimation::SetPlaySpeed(float speed)
{
	_pAnimController->SetTrackSpeed(0, speed);
}

void skinnedAnimation::AddBoneTransform(string boneName, transform * pTransform)
{
	BONE* pBone = this->_pSkinnedMesh->GetFineBONE(boneName);
	this->_mapBoneTransform.insert(make_pair(pBone, pTransform));
}

void skinnedAnimation::AddApplyTransform(string boneName, transform * pTransform)
{
	BONE* pBone = this->_pSkinnedMesh->GetFineBONE(boneName);
	this->_mapApplyBoneTransform.insert(std::make_pair(pBone, pTransform));
}

void skinnedAnimation::RemoveBoneTransform(string boneName)
{
	BONE* pBone = this->_pSkinnedMesh->GetFineBONE(boneName);
	MAP_BONETRANSFORM::iterator boneIter = _mapBoneTransform.find(pBone);
	if (boneIter != _mapBoneTransform.end())
	{
		_mapBoneTransform.erase(boneIter);
	}
}

void skinnedAnimation::RemoveApplyTransform(string boneName)
{
	BONE* pBone = this->_pSkinnedMesh->GetFineBONE(boneName);
	MAP_BONETRANSFORM::iterator boneIter = _mapApplyBoneTransform.find(pBone);
	if (boneIter != _mapApplyBoneTransform.end())
	{
		_mapApplyBoneTransform.erase(boneIter);
	}
}

//Animation 을 바꾼다.
void skinnedAnimation::SetAnimation(LPD3DXANIMATIONSET animSet)
{
	//이미 플레이되고 있는 Animation 이라면 실행이 되지 않는다.
	if (this->_pNowPlayAnimationSet != NULL &&
		animSet == this->_pNowPlayAnimationSet) return;

	//크로스 페이드가 존재한다면..
	if (this->_fCrossFadeTime > 0.0f)
	{
		//현제 Animation 을 1 번Track 으로 셋팅
		_pAnimController->SetTrackAnimationSet(1, _pNowPlayAnimationSet);
		_pAnimController->SetTrackPosition(1, _Track_Desc_0.Position);	//이전에 플레이 되던 위치로 셋팅
		_pAnimController->SetTrackEnable(1, true);						//1 번 Track 활성화
		_pAnimController->SetTrackWeight(1, 1.0f);						//1 번 Track 가중치
		_pAnimController->SetTrackSpeed(1, _Track_Desc_0.Speed);		//속도 

																		//지금셋팅되는 Animation 을 0 번Track 으로 셋팅
		_pNowPlayAnimationSet = animSet;
		_pAnimController->SetTrackAnimationSet(0, animSet);
		_pAnimController->SetTrackPosition(0, 0.0f);
		_pAnimController->SetTrackWeight(0, 0.0f);						//가중치는 0 으로 

																		//현재 플레이 되고 있는 AnimationSet 갱신
		this->_pNowPlayAnimationSet = animSet;
	}
	//크로스 페이드가 없다면
	else
	{
		//Track 의 포지션을 맨압으로 돌린다.
		this->_pAnimController->SetTrackPosition(
			0,			//Track 번호 
			0.0			//PlayPosition
		);

		//매개변수로 받은 AnimationSet 으로 Animation 0 번 Track에 플레이
		this->_pAnimController->SetTrackAnimationSet(0, animSet);

		//현재 플레이 되고 있는 AnimationSet 갱신
		this->_pNowPlayAnimationSet = animSet;
	}
}