#pragma once

class baseObject;

class Frustum
{
private :
	D3DXVECTOR3		_RHWPos[8];		// viewProj 변환전 위치
	D3DXVECTOR3		_pos[8];		// 변환 후 위치
	D3DXPLANE		_plane[6];		// 절두체 평면

public :
	// 오브젝트가 프러스텀 안에 있는지 체크
	bool isInFrustum(baseObject* pObject);
	// 프러스텀 정보 업데이트
	void updateFrustum(D3DXMATRIXA16* matViewProj);
	// 프러스텀 기즈모
	void renderGizmo();
	
	Frustum();
	~Frustum();
};