#include "stdafx.h"
#include "xMeshSkinned.h"

char szTemp[128];
LPD3DXEFFECT xMeshSkinned::_sSkinnedMeshEffect = NULL;

xMeshSkinned::xMeshSkinned()
{
	_pNowPlayAnimationSet = NULL;
	_dwWorkingPaletteSize = 0;
	_pmWorkingPalette = NULL;
	_pSkinnedEffect = NULL;
	//Skinned Effect 로딩
	if (_sSkinnedMeshEffect == NULL)
	{
		_sSkinnedMeshEffect = RM_SHADER->addResource("Resources/Shader/SkinnedMesh.fx");
	}

	_normalTex = RM_TEXTURE->addResource("Resources/Mesh/normalDefault.png");
	_emissionTex = RM_TEXTURE->addResource("Resources/Mesh/emissionDefault.png");
}

HRESULT xMeshSkinned::init(string filePath, D3DXMATRIXA16 * pmatCorrection)
{
	//보정행렬이 들어왔다면...
	if (pmatCorrection) _matCorrection = *pmatCorrection;
	else D3DXMatrixIdentity(&_matCorrection);

	_pSkinnedEffect = _sSkinnedMeshEffect;

	//스킨드메쉬와 본의 계층 구조의 로딩방식이 재정의된 BONE_HIERARCHY 클래스 객체
	BONE_HIERARCHY boneHierachy;
	boneHierachy.SetLoadPath(filePath);
	boneHierachy.SetSkinnedMesh(this);

	//계층구조로 되어있는 XFile 메시 로딩....
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		filePath.c_str(),
		D3DXMESH_MANAGED,
		device,
		&boneHierachy,					//계층 구조의 로딩 방식이 재정의된 객체를 넘긴다.
		NULL,
		&_pRootbone,					//얻어온 루트 본프레임
		&_pAnimController				//얻어올 애니메이션 컨트롤러
	))) return E_FAIL;

	//본 매트릭스 포인터 생성
	InitBoneMatrixPointer((BONE*)_pRootbone);

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
		this->_mapAnimSet.insert(std::make_pair(animSet->GetName(), animSet));
	}

	//일단 첫번째 Animation 으로 시작
	this->Play(0);

	return S_OK;
}

HRESULT xMeshSkinned::init(string filePath, string mtrlPath, D3DXMATRIXA16 * matCorrection)
{
	//보정행렬이 들어왔다면...
	if (matCorrection) _matCorrection = *matCorrection;
	else D3DXMatrixIdentity(&_matCorrection);

	_pSkinnedEffect = _sSkinnedMeshEffect;

	//스킨드메쉬와 본의 계층 구조의 로딩방식이 재정의된 BONE_HIERARCHY 클래스 객체
	BONE_HIERARCHY boneHierachy;
	boneHierachy.SetLoadPath(filePath, mtrlPath);
	boneHierachy.SetSkinnedMesh(this);

	//계층구조로 되어있는 XFile 메시 로딩....
	if (FAILED(D3DXLoadMeshHierarchyFromX(
		filePath.c_str(),
		D3DXMESH_MANAGED,
		device,
		&boneHierachy,					//계층 구조의 로딩 방식이 재정의된 객체를 넘긴다.
		NULL,
		&_pRootbone,					//얻어온 루트 본프레임
		&_pAnimController				//얻어올 애니메이션 컨트롤러
	))) return E_FAIL;

	//본 매트릭스 포인터 생성
	InitBoneMatrixPointer((BONE*)_pRootbone);

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
		this->_mapAnimSet.insert(std::make_pair(animSet->GetName(), animSet));
	}

	//일단 첫번째 Animation 으로 시작
	this->Play(0);

	return S_OK;
}

void xMeshSkinned::release()
{
	//불러온 AnimationSet 해제
	for (int i = 0; i < this->_vecAnimSet.size(); i++)
	{
		SAFE_RELEASE(_vecAnimSet[i]);
	}

	
	//에니메이션 콘트롤러 해제
	SAFE_RELEASE(_pAnimController);

	//스킨드메쉬와 본의 계층 구조의 로딩방식이 재정의된 BONE_HIERARCHY 클래스 객체
	BONE_HIERARCHY boneHierachy;	//<- 예는 해제 방법도 알고 있다.
	D3DXFrameDestroy(_pRootbone, &boneHierachy);
}

void xMeshSkinned::update()
{
	//0 번 Track 정보를 얻는다
	_pAnimController->GetTrackDesc(0, &_Track_Desc_0);

	//_pNowPlayAnimationSet->GetPeriod() Animation 의 총시간을 얻는다.
	//현재 Animation 진행 Factor 를 갱신
	_AnimationPlayFactor = _Track_Desc_0.Position / _pNowPlayAnimationSet->GetPeriod();

	//마지막에 도달했다면...
	if (_AnimationPlayFactor >= 1.0)
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
			else
			{
				this->Stop();
			}
		}
	}
	_AnimationPlayFactor = _AnimationPlayFactor - (int)_AnimationPlayFactor;	//정수부분 안생기게....

	if (_bPlay)
	{
		//애니메이션을 진행시킴
		_pAnimController->AdvanceTime(_deltaTime, NULL);
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

void xMeshSkinned::render(transform * pTrans)
{

	D3DXMATRIXA16 matFinal;
	//Transform 이 들어왔다면...
	if (pTrans != NULL)
	{
		D3DXMATRIXA16 matWorld = pTrans->getFinalMatrix();
		matFinal = _matCorrection * matWorld;
	}
	else
	{
		matFinal = _matCorrection;
	}

	_pSkinnedEffect->SetTechnique("SkinnedMesh");

	//SkinnedEffect WorldMatrix 적용
	_pSkinnedEffect->SetMatrix("matWorld", &matFinal);

	//행렬 업데이트 
	UpdateMatrices((BONE*)_pRootbone, &matFinal);

	if (GET_OUTLINE_RS)
	{
		this->setTechniqueName("Base");
		this->outlineRenderBone((BONE*)_pRootbone);
	}
	this->setTechniqueName("ReciveShadow");
	//루트 본부터 타고 들어간다.
	RenderBone((BONE*)_pRootbone);
}

//본을 보여준다.
void xMeshSkinned::renderBoneName(camera * pCam, transform * transform)
{
	D3DXMATRIXA16 matFinal;

	//Transform 이 들어왔다면...
	if (transform != NULL)
	{
		D3DXMATRIXA16 matWorld = transform->getFinalMatrix();
		matFinal = _matCorrection * matWorld;
	}
	else
	{
		matFinal = _matCorrection;
	}

	//행렬 업데이트 
	UpdateMatrices((BONE*)_pRootbone, &matFinal);

	//루트 본부터 타고 들어간다.
	RenderChildBoneName(pCam, (BONE*)_pRootbone, NULL);
}

void xMeshSkinned::Play(string animName, float crossFadeTime)
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

void xMeshSkinned::Play(int animIndex, float crossFadeTime)
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

void xMeshSkinned::PlayOneShot(string animName, float inCrossFadeTime, float outCrossFadeTime)
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

void xMeshSkinned::PlayOneShotAfterHold(std::string animName, float crossFadeTime)
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

void xMeshSkinned::SetPlaySpeed(float speed)
{
	_pAnimController->SetTrackSpeed(0, speed);
}

//Animation 정보를 출력한다.
void xMeshSkinned::ShowAnimationName(int x, int y)
{
	int i = 0;
	for (i = 0; i < _AnimNum; i++)
	{
		FONTMANAGER->fontOut(_vecAnimSet[i]->GetName(), x, y + i * 20, 0xffffffff);
	}
}

//본이름으로 본찾는다.
BONE * xMeshSkinned::GetFineBONE(std::string boneName)
{
	return (BONE*)D3DXFrameFind(_pRootbone, boneName.c_str());
}

//본 행렬을 셋팅한다.
void xMeshSkinned::InitBoneMatrixPointer(BONE * pBone)
{
	if (pBone == NULL) return;

	
	//해당 본프레임에 메쉬 컨테이너 가 존재한다면...
	if (pBone->pMeshContainer)
	{
		//메쉬 컨테이너를 BONE_MESH 형으로 형변환
		BONE_MESH* pBoneMesh = (BONE_MESH*)pBone->pMeshContainer;
		
		//메쉬에 스키닝 정보가 있다면
		if (pBoneMesh->pSkinInfo)
		{
			//해당 스킨에 적용되는 본의 갯수만큼
			int numBones = pBoneMesh->pSkinInfo->GetNumBones();

			//Matrix 포인터 할당
			pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIXA16*[numBones];
			
			//적용되는 폰 프레임의 매트릭스를 찾아 해당 본프레임의 행렬 포인터를 물린다.
			for (int i = 0; i < numBones; i++)
			{
				//해당 스킨에 적용되는 i 번째의 본이름으로 해당 본 프레임을 루프 본부터 찾아 들어간가
				BONE* pB = (BONE*)D3DXFrameFind(_pRootbone, pBoneMesh->pSkinInfo->GetBoneName(i));

				//찾았다면
				if (pB != NULL)
				{
					pBoneMesh->ppBoneMatrixPtrs[i] = &pB->CombinedTransformationMatrix;
				}
				else
				{
					pBoneMesh->ppBoneMatrixPtrs[i] = NULL;
				}
			}
		}
	}

	//이웃 본이 존재한다면...
	if (pBone->pFrameSibling)
		InitBoneMatrixPointer((BONE*)pBone->pFrameSibling);

	//자식 본이 존재한다면...
	if (pBone->pFrameFirstChild)
		InitBoneMatrixPointer((BONE*)pBone->pFrameFirstChild);
}

void xMeshSkinned::UpdateMatrices(BONE * pBone, D3DXMATRIXA16 * pParentMatrix)
{
	if (pBone == NULL) return;

	//부모 행렬이 있다면..
	if (pParentMatrix != NULL)
	{
		D3DXMATRIXA16 matTransformation = pBone->TransformationMatrix;

		//적용 Transform 이 물려 있다면...
		if (pBone->pApplyTransform != NULL)
		{
			D3DXMATRIXA16 pApplyMatrix = pBone->pApplyTransform->getWorldRotateMatrix();
			matTransformation = matTransformation * pApplyMatrix;
			pBone->pApplyTransform = NULL;
		}

		//pBone->CombinedTransformationMatrix = pBone->_transformationMatrix * *(pParentMatrix);
		//pBone 의 행렬 부모행렬을 곱하여 pBone 의 최종 행렬을 구한다.
		D3DXMatrixMultiply(
			&pBone->CombinedTransformationMatrix,
			&matTransformation,
			pParentMatrix);
	}

	//부모 행렬이 없다면..
	else
	{
		//자신의 행렬이 현재 행렬이 된다.
		pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
	}

	//본 Transform 이 물려 있다면....
	if (pBone->pBoneTransform != NULL)
	{
		pBone->pBoneTransform->setWorldMatrix(pBone->CombinedTransformationMatrix);

		pBone->pBoneTransform->setScale(1, 1, 1);
		pBone->pBoneTransform = NULL;
	}

	//이웃 본이 존재한다면...
	if (pBone->pFrameSibling)
	{
		//이웃은 같은레벨이기 때문에 부모가 같다 따라서 매개변로 받은 부모의 행렬을 넘기면 된다.
		UpdateMatrices((BONE*)pBone->pFrameSibling, pParentMatrix);
	}
	//자식 본이 존재한다면...
	if (pBone->pFrameFirstChild)
	{
		UpdateMatrices((BONE*)pBone->pFrameFirstChild, &pBone->CombinedTransformationMatrix);
	}
}

//본을 그린다.
void xMeshSkinned::RenderBone(BONE * pBone)
{
	if (pBone == NULL) return;

	//본에 메쉬 컨테이너가 존재한다면 그려야한다.
	if (pBone->pMeshContainer)
	{
		//본에 있는 메쉬 컨테이너는 BONE_MESH 이다
		BONE_MESH* pBoneMesh = (BONE_MESH*)pBone->pMeshContainer;

		
		//본 컴비네이션 정보가  
		if (pBoneMesh->BufBoneCombos != NULL)
		{
			//해당 본의 컴비네이션 정보를 얻는다.
			LPD3DXBONECOMBINATION pBoneComb =
				(LPD3DXBONECOMBINATION)(pBoneMesh->BufBoneCombos->GetBufferPointer());

			//본에 물려있는 메쉬의 서브셋갯수을 속성그룹수와 같다
			for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
			{
				//해당 속성의 팔래트 엔트리 수만큼 돌면서 작업용 팔래트 행렬 갱신
				for (DWORD palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
				{
					//적용되는 행렬 ID 를 얻는다
					DWORD dwMatrixIndex = pBoneComb[i].BoneId[palEntry];

					//행렬 인덱스가 유효하다면...
					if (dwMatrixIndex != UINT_MAX)
					{
						//작업 앵렬을 만든다.
						D3DXMatrixMultiply(&_pmWorkingPalette[palEntry],
							&(pBoneMesh->pBoneOffsetMatices[dwMatrixIndex]),
							pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);
					}
				}

				//위에서 셋팅됭 작업행렬을 Effect 팔래스에 적용한다.
				_pSkinnedEffect->SetMatrixArray("amPalette", _pmWorkingPalette, pBoneMesh->NumPaletteEntries);

				//적용되는 정점의 본최대 영향수 를 대입 최대 영향수  -1 
				_pSkinnedEffect->SetInt("CurNumBones", pBoneMesh->MaxNumFaceInfls - 1);

				//메터리얼 인덱스
				DWORD materialIndex = pBoneComb[i].AttribId;

			

				UINT uiPasses, uiPass;
				_pSkinnedEffect->Begin(&uiPasses, 0);
				//텍스쳐 셋팅
				
				_pSkinnedEffect->SetTexture("Diffuse_Tex", pBoneMesh->DiffuseTexs[materialIndex]);
				_pSkinnedEffect->SetTexture("Normal_Tex", pBoneMesh->NormalTexs[materialIndex]);
				_pSkinnedEffect->SetTexture("Specular_Tex", pBoneMesh->SpecularTexs[materialIndex]);
				_pSkinnedEffect->SetTexture("Emission_Tex", pBoneMesh->EmissionTexs[materialIndex]);
				_pSkinnedEffect->SetTexture("Gloss_Tex", pBoneMesh->GlossTex[materialIndex]);
				
				_pSkinnedEffect->SetFloat("fSpecPower", pBoneMesh->Materials[materialIndex].Power);
				
				for (uiPass = 0; uiPass < uiPasses; ++uiPass)
				{
					
					_pSkinnedEffect->BeginPass(uiPass);
					pBoneMesh->WorkingMesh->DrawSubset(i);
					_pSkinnedEffect->EndPass();
				}
				_pSkinnedEffect->End();
			}
		}
	}

	//이웃 본이 존재한다면...
	if (pBone->pFrameSibling) RenderBone((BONE*)pBone->pFrameSibling);

	//자식 본이 존재한다면...
	if (pBone->pFrameFirstChild) RenderBone((BONE*)pBone->pFrameFirstChild);
}

void xMeshSkinned::outlineRenderBone(BONE * pBone)
{
	if (pBone == NULL) return;

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	D3DXVECTOR2 lineWidth = D3DXVECTOR2(0.003f, 0.003f);

	for (int i = 0; i < 2; i++)
	{
		//본에 메쉬 컨테이너가 존재한다면 그려야한다.
		if (pBone->pMeshContainer)
		{
			//본에 있는 메쉬 컨테이너는 BONE_MESH 이다
			BONE_MESH* pBoneMesh = (BONE_MESH*)pBone->pMeshContainer;


			//본 컴비네이션 정보가  
			if (pBoneMesh->BufBoneCombos != NULL)
			{
				//해당 본의 컴비네이션 정보를 얻는다.
				LPD3DXBONECOMBINATION pBoneComb =
					(LPD3DXBONECOMBINATION)(pBoneMesh->BufBoneCombos->GetBufferPointer());

				//본에 물려있는 메쉬의 서브셋갯수을 속성그룹수와 같다
				for (DWORD i = 0; i < pBoneMesh->NumAttributesGroup; i++)
				{
					//해당 속성의 팔래트 엔트리 수만큼 돌면서 작업용 팔래트 행렬 갱신
					for (DWORD palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
					{
						//적용되는 행렬 ID 를 얻는다
						DWORD dwMatrixIndex = pBoneComb[i].BoneId[palEntry];

						//행렬 인덱스가 유효하다면...
						if (dwMatrixIndex != UINT_MAX)
						{
							//작업 앵렬을 만든다.
							D3DXMatrixMultiply(&_pmWorkingPalette[palEntry],
								&(pBoneMesh->pBoneOffsetMatices[dwMatrixIndex]),
								pBoneMesh->ppBoneMatrixPtrs[dwMatrixIndex]);

							D3DXMATRIXA16 matTrans;
							D3DXMatrixTranslation(&matTrans, lineWidth.x, lineWidth.y, 0.0f);

							_pmWorkingPalette[palEntry] *= matTrans;
						}
					}

					//위에서 셋팅됭 작업행렬을 Effect 팔래스에 적용한다.
					_pSkinnedEffect->SetMatrixArray("amPalette", _pmWorkingPalette, pBoneMesh->NumPaletteEntries);

					//적용되는 정점의 본최대 영향수 를 대입 최대 영향수  -1 
					_pSkinnedEffect->SetInt("CurNumBones", pBoneMesh->MaxNumFaceInfls - 1);

					//메터리얼 인덱스
					DWORD materialIndex = pBoneComb[i].AttribId;



					UINT uiPasses, uiPass;
					_pSkinnedEffect->Begin(&uiPasses, 0);
					//텍스쳐 셋팅

					_pSkinnedEffect->SetTexture("Diffuse_Tex", pBoneMesh->DiffuseTexs[materialIndex]);
					_pSkinnedEffect->SetTexture("Normal_Tex", pBoneMesh->NormalTexs[materialIndex]);
					_pSkinnedEffect->SetTexture("Specular_Tex", pBoneMesh->SpecularTexs[materialIndex]);
					_pSkinnedEffect->SetTexture("Emission_Tex", pBoneMesh->EmissionTexs[materialIndex]);
					_pSkinnedEffect->SetTexture("Gloss_Tex", pBoneMesh->GlossTex[materialIndex]);

					_pSkinnedEffect->SetFloat("fSpecPower", pBoneMesh->Materials[materialIndex].Power);

					for (uiPass = 0; uiPass < uiPasses; ++uiPass)
					{

						_pSkinnedEffect->BeginPass(uiPass);
						pBoneMesh->WorkingMesh->DrawSubset(i);
						_pSkinnedEffect->EndPass();
					}
					_pSkinnedEffect->End();

					//해당 속성의 팔래트 엔트리 수만큼 돌면서 작업용 팔래트 행렬 갱신
					for (DWORD palEntry = 0; palEntry < pBoneMesh->NumPaletteEntries; palEntry++)
					{
						//적용되는 행렬 ID 를 얻는다
						DWORD dwMatrixIndex = pBoneComb[i].BoneId[palEntry];

						//행렬 인덱스가 유효하다면...
						if (dwMatrixIndex != UINT_MAX)
						{
							D3DXMATRIXA16 matTrans;
							D3DXMatrixTranslation(&matTrans, -lineWidth.x, -lineWidth.y, 0.0f);

							_pmWorkingPalette[palEntry] *= matTrans;
						}
					}
				}
			}
		}

		lineWidth = -lineWidth;
	}

	//이웃 본이 존재한다면...
	if (pBone->pFrameSibling) outlineRenderBone((BONE*)pBone->pFrameSibling);

	//자식 본이 존재한다면...
	if (pBone->pFrameFirstChild) outlineRenderBone((BONE*)pBone->pFrameFirstChild);

	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void xMeshSkinned::RenderChildBoneName(camera * pCam, BONE * pBone, D3DXVECTOR3 * parentPos)
{
	if (pBone == NULL) return;

	//본의 Transform  그려
	if (pBone->pBoneTransform != NULL)
	{
		pBone->pBoneTransform->renderGizmo();
	}
	//본위치
	D3DXVECTOR3 bonePos = D3DXVECTOR3(
		pBone->CombinedTransformationMatrix._41,
		pBone->CombinedTransformationMatrix._42,
		pBone->CombinedTransformationMatrix._43);

	sprintf(szTemp, "%s", pBone->Name);

	DWORD color = 0xff808000;
	DWORD textColor = 0xffffff00;


	//루트본이라면.
	if (pBone == _pRootbone)
	{
		color = 0xff808080;
		textColor = 0xffffffff;
		//본위치에 Gizmo
		GIZMOMANAGER->sphereMake(bonePos, 0.2f, color);
	}

	//본위치에 Gizmo
	GIZMOMANAGER->sphereMake(bonePos, 0.1f, color);

	//부모위치에서 내위치의 선
	if (parentPos != NULL) GIZMOMANAGER->lineMake(bonePos, *parentPos, color);

	//본스크린 위치
	D3DXVECTOR2 boneScreenPos;
	if (pCam->getWorldPosToScreenPos(&boneScreenPos, &bonePos))
	{
		FONTMANAGER->fontOut(szTemp, (int)boneScreenPos.x, (int)boneScreenPos.y, textColor);
	}

	//이웃 본이 존재한다면...
	if (pBone->pFrameSibling) RenderChildBoneName(pCam, (BONE*)pBone->pFrameSibling, parentPos);

	//자식 본이 존재한다면...
	if (pBone->pFrameFirstChild) RenderChildBoneName(pCam, (BONE*)pBone->pFrameFirstChild, &bonePos);
}

//Animation 을 바꾼다.
void xMeshSkinned::SetAnimation(LPD3DXANIMATIONSET animSet)
{
	//이미 플레이되고 있는 Animation 이라면 실행이 되지 않는다.
	if (this->_pNowPlayAnimationSet != NULL &&
		animSet == this->_pNowPlayAnimationSet)
		return;

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
	//크로스 페이드가 없다면...
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

void xMeshSkinned::setCamera(camera * camera)
{
	//Skinned Effect 로딩
	if (_sSkinnedMeshEffect == NULL)
	{
		_sSkinnedMeshEffect = RM_SHADER->addResource("Resources/Shader/SkinnedMesh.fx");
	}

	D3DXMATRIXA16 matViewProj = camera->getViewProjectionMatrix();
	_sSkinnedMeshEffect->SetMatrix("matViewProjection", &matViewProj);
	D3DXVECTOR3 vEyePos = camera->getWorldPosition();
	_sSkinnedMeshEffect->SetVector("vEyePos", &D3DXVECTOR4(vEyePos, 1));
}

void xMeshSkinned::setBaseLight(lightDirection * pDirLight)
{
	_sSkinnedMeshEffect->SetMatrix("baseDirectionLight", &pDirLight->getLightMatrix());
}

void xMeshSkinned::setLighting(light ** pLights, int lightNum)
{
	//라이트 행렬 세팅
	D3DXMATRIXA16 matLight[10];
	for (int i = 0; i < lightNum; i++)
	{
		matLight[i] = pLights[i]->getLightMatrix();
	}
	//Effect 에 라이팅 세팅
	_sSkinnedMeshEffect->SetMatrixArray("matLights", matLight, 10);
	_sSkinnedMeshEffect->SetInt("LightNum", lightNum);
}

void xMeshSkinned::setLighting(vector<light*>* pLights)
{
	//라이트 행렬 세팅
	D3DXMATRIXA16 matLight[10];
	for (int i = 0; i < pLights->size(); i++)
	{
		matLight[i] = (*(pLights))[i]->getLightMatrix();
	}
	//Effect 에 라이팅 세팅
	_sSkinnedMeshEffect->SetMatrixArray("matLights", matLight, 10);
	_sSkinnedMeshEffect->SetInt("LightNum", pLights->size());
}

void xMeshSkinned::setBaseLighting(light ** ppLights)
{
	
	D3DXMATRIXA16 matLight = (*ppLights)->getLightMatrix();
	_sSkinnedMeshEffect->SetMatrix("baseDirectionLight", &matLight);

	//_sSkinnedMeshEffect->SetMatrix("matLightViewProjection", &matLight);
}

void xMeshSkinned::setTechniqueName(string name)
{
	_sSkinnedMeshEffect->SetTechnique(name.c_str());
}