#pragma once

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

class transform
{
protected:
	// 공용체(메모리를 공동으로 사용할 수 있도록 해준다)
	// 배열로도 접근 가능하고, 방향벡터로도 접근가능하게 만들기
	// 방향벡터
	union
	{
		struct
		{
			D3DXVECTOR3 axis[3];
		};
		struct
		{
			// 바라보는 방향 값은 항상 직교화 되어 있어야 한다
			// 항상 정규화 되어 있다
			D3DXVECTOR3 right;
			D3DXVECTOR3 up;
			D3DXVECTOR3 forward;
		};
	};

	D3DXVECTOR3			_position;	// 위치벡터
	D3DXVECTOR3			_scale; 	// 크기벡터(스케일)
	D3DXMATRIXA16		_matFinal; 	// 최종행렬(월드행렬)


	// 로컬행렬
	// 부모를 기준으로한 상태적인 행렬(만약 부모가 없다면 matFinal 이랑 같다)
	D3DXMATRIXA16		_matLocal;
	// 부모 참조(null 이면 부모가 없는애)
	transform*			_parent;
	// 자식 참조(null 이면 자식이 없는애)
	transform*			_firstChild;
	// 형제 참조(null 이면 외아들)
	transform*			_nextSibling;

	// 트렌스폼 변환시 자동으로 업데이트 할꺼냐?
	// 만약 이게 false이면 사용자가 직접 updateTransform() 호출을 해야한다
	bool				_isAutoUpdate;

public:
	transform();
	~transform() {}

	// 특정 Child 를 내자식으로 붙인다
	void addChild(transform* pNewChild);
	// 특정 Transform 에 붙는다
	void attachTo(transform* pNewParent);
	// 부모와 안녕
	void releaseParent();

	// 위치를 월드 좌표계로 세팅
	void setWorldPosition(float x, float y, float z);
	void setWorldPosition(D3DXVECTOR3 pos);

	// 위치를 로컬 좌표계로 셋팅(부모가 있는 경우 부모의 상태적인 위치)
	void setLocalPosition(float x, float y, float z);
	void setLocalPosition(D3DXVECTOR3 pos);

	// 자기자신 기준으로 이동하기(상대좌표)
	void movePositionSelf(float dx, float dy, float dz);
	void movePositionSelf(D3DXVECTOR3 delta);

	// 월드 기준으로 이동하기(절대좌표)
	void movePositionWorld(float dx, float dy, float dz);
	void movePositionWorld(D3DXVECTOR3 delta);

	// 부모가 있는 경우 로컬 기준으로 이동 시킨다.
	void movePositionLocal(float dx, float dy, float dz);
	void movePositionLocal(D3DXVECTOR3 delta);

	// 스케일 세팅
	void setScale(float x, float y, float z);
	void setScale(D3DXVECTOR3 scale);

	// 스케일링
	void scaling(float dx, float dy, float dz);
	void scaling(D3DXVECTOR3 delta);

	// 월드 기준으로 회전
	void rotateWorld(float angleX, float angleY, float angleZ);
	void rotateWorld(D3DXVECTOR3 angle);

	// 자신의 축 기준으로 회전
	void rotateSelf(float angleX, float angleY, float angleZ);
	void rotateSelf(D3DXVECTOR3 angle);

	// 부모가 있는 경우 부모의 로컬 축 기준으로 회전
	void rotateLocal(float angleX, float angleY, float angleZ);
	void rotateLocal(D3DXVECTOR3 angle);

	// 특정 방향을 바라보게 회전해라
	void lookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));
	// 특정 방향을 바라보는데 angle 만큼 회전해라
	void lookDirection(D3DXVECTOR3 dir, float angle);
	// 특정 위치를 바라보게 회전해라
	void lookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));
	// 특정 위치를 바라보는데 angle 만큼 회전해라
	void lookPosition(D3DXVECTOR3 pos, float angle);

	// 사원수를 이용한 특정 회전값으로 회전량 세팅
	// void setRotate(float angleX, float angleY, float angleZ);
	void setRotateWorld(float eAngleX, float eAngleY, float aAngleZ);
	void setRotateLocal(float eAngleX, float eAngleY, float aAngleZ);

	//  회전 행렬을 넣어주면 그 회전 행렬대로 회전한다.
	void setRotateWorld(const D3DXMATRIXA16& matWorldRotate);
	void setRotateLocal(const D3DXMATRIXA16& matWorldRotate);

	//  회전 사원수를 넣어주면 그 회전값 대로 회전한다.
	void setRotateWorld(const D3DXQUATERNION& matWorldRotate);
	void setRotateLocal(const D3DXQUATERNION& matWorldRotate);

	// 자신의 회전 값을 from 과 to 사이의 회전량만큼 회전보간(구면보간) 하여 적용
	void rotateSlerp(transform& from, transform& to, float t);

	// 자신의 위치 값을 from 과 to 사이의 위치만큼 선형보간 하여 적용
	void positionLerp(transform& from, transform& to, float t);

	// 자신의 모든 정보를 from 과 to 사이의 정보만큼 보간 하여 적용
	void interpolate(transform& from, transform& to, float t);

	// 트렌스폼 업데이트 (트렌스폼에 정보들이 변경되었을때 사용한다)
	void updateTransform();

	// 디바이스에 자신의 트렌스폼을 월드로 세팅
	void setDeviceWorld();
	// 디바이스에 자신의 트렌스폼을 뷰로 세팅
	void setDeviceView();

	// 최종행렬(월드행렬) 가져오기
	D3DXMATRIXA16 getFinalMatrix() { return _matFinal; }
	void setWorldMatrix(const D3DXMATRIXA16 & matWorld);
	D3DXMATRIXA16 getWorldRotateMatrix();
	D3DXQUATERNION getWorldRotateQuaternion();
	// 위치 값을 얻는다
	D3DXVECTOR3 getWorldPosition();
	D3DXVECTOR3 getLocalPosition();
	// 축을 얻는다 (월드기준)
	void getScaledAxies(D3DXVECTOR3* pVecArr);
	void getUnitAxies(D3DXVECTOR3* pVecArr);
	D3DXVECTOR3 getScaledAxis(int axisNum);
	D3DXVECTOR3 getUnitAxis(int axisNum);
	D3DXVECTOR3 getForward(bool isNormalize = true);
	D3DXVECTOR3 getUp(bool isNormalize = true);
	D3DXVECTOR3 getRight(bool isNormalize = true);

	//  스케일 값 얻기
	D3DXVECTOR3 getScale() { return _scale; }

	// 카메라 디폴트 컨트롤
	void defaultControl();
	// 오브젝트 컨트롤
	void defaultMyControl();

	// 트렌스폼에 대한 기즈모 그리기
	void renderGizmo(float scale = 1.0f);
};

