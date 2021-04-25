#pragma once
#include "singletonBase.h"
// =============================================================
// 	## resourceManager ## (리소스 매니져)
// =============================================================
template<typename T, typename A>
class resourceManager : public singletonBase <A>
{
protected:
	typedef map<string, T> mapResource;
	mapResource _resourceList;	// 맵안에 담긴 리소스 목록들

public:
	// 리소스 추가
	T addResource(string fileName, void* param = NULL);
	T addResource(string fileName, string mtrlPath, void* param = NULL);
	// 리소스 삭제
	void removeResource(string fileName);
	// 모든 리소스 삭제
	void clearResource(void);

	// =============================================================
	// 	## 리소스 로드 해제관련 순수가상함수 ##
	// =============================================================
	virtual T loadResource(string fileName, void* param = NULL) = 0;
	virtual T loadResource(string fileName, string mtrlPath, void* param = NULL) = 0;
	virtual void releaseResource(T data) = 0;

	resourceManager() {}
	~resourceManager() {}
};

// 리소스 추가
template<typename T, typename A>
inline T resourceManager<T, A>::addResource(string fileName, void * param)
{
	mapResource::iterator find;
	// 이미 리소스가 추가가 되었는지 확인
	find = _resourceList.find(fileName);

	// 해당 리소스가 맵에 추가되어 있지 않다면 새로 만들어서 추가시키기
	if (find == _resourceList.end())
	{
		// 로드리소스 함수로 새로운 리소스 추가
		T newResource = this->loadResource(fileName, param);
		// 리소스 추가가 실패했다면 0리턴
		if (newResource == NULL) return NULL;
		// 리소스목록에 추가하기
		_resourceList.insert(make_pair(fileName, newResource));

		return newResource;
	}

	return find->second;
}

template<typename T, typename A>
inline T resourceManager<T, A>::addResource(string fileName, string mtrlPath, void * param)
{
	mapResource::iterator find;
	// 이미 리소스가 추가가 되었는지 확인
	find = _resourceList.find(fileName);

	// 해당 리소스가 맵에 추가되어 있지 않다면 새로 만들어서 추가시키기
	if (find == _resourceList.end())
	{
		// 로드리소스 함수로 새로운 리소스 추가
		T newResource = this->loadResource(fileName, mtrlPath, param);
		// 리소스 추가가 실패했다면 0리턴
		if (newResource == NULL) return NULL;
		// 리소스목록에 추가하기
		_resourceList.insert(make_pair(fileName, newResource));

		return newResource;
	}

	return find->second;
}

// 리소스 삭제
template<typename T, typename A>
inline void resourceManager<T, A>::removeResource(string fileName)
{
	mapResource::iterator find;
	// 이미 리소스가 추가가 되었는지 확인
	find = _resourceList.find(fileName);

	// 리소스가 있다면 지우기
	if (find != _resourceList.end())
	{
		// 찾은 리소스 해제
		this->releaseResource(find->second);
		// 해당하는 값을 삭제
		_resourceList.remove(find);
	}
}

// 모든 리소스 삭제
template<typename T, typename A>
inline void resourceManager<T, A>::clearResource(void)
{
	mapResource::iterator iter;
	for (iter = _resourceList.begin(); iter != _resourceList.end(); ++iter)
	{
		this->releaseResource(iter->second);
	}
	// 전체삭제
	_resourceList.clear();
}