#pragma once

class xMesh;
class skinnedAnimation;

class baseObject
{
public:
	xMesh*			_pMesh;			// 내가 사용할 메쉬
	transform*		_transform;		// 트렌스폼
	boundBox		_boundBox;		// 충돌박스
	bool			_ignoreCreateShadow;	// 쉐도우맵 만들때 무시되냐?
	skinnedAnimation*	_skinnedAnim;		//스킨드 애니메이션

protected:
	bool			_isActive;		//활성화 여부

public:
	virtual void update();
	virtual void render();

	// 랜더링 될 메쉬를 세팅한다.
	void setMesh(xMesh*	pMesh);
	// 활성화 세팅
	void setActive(bool isActive);

	// 활성화 여부를 얻는다.
	bool getActive() { return _isActive; }

	// 셋팅된 메쉬에 따라 바운드박스를 재계산 한다.
	void computeBoundBox();

	// override로 필요하면 재정의 해서 사용하기
	virtual void baseObjectEnable() {}			// BaseObject 가 활성화 될때 실행
	virtual void baseObjectDisable() {}			// BaseObject 가 비활성화 될때 실행
	virtual void baseObjectUpdate() {}			// BaseObject 가 Update 때 실행
	virtual void baseObjectNoActiveUpdate() {}	// BaseObject 가 비활성화시 업데이트 실행
	virtual void baseObjectRender();			// BaseObject 를 그릴때 실행

	xMeshStatic* getStaticMesh() { return dynamic_cast<xMeshStatic*>(_pMesh); }
	xMeshSkinned* getSkinnedMesh() { return dynamic_cast<xMeshSkinned*>(_pMesh); }
	baseObject();
	baseObject(xMesh* pMesh)
	{
		this->setMesh(pMesh);
		this->setActive(true);
	}
	~baseObject();
};