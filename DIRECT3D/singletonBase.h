#pragma once
// =============================================================
// 	## singleTonBase ## (싱글톤 패턴)
// =============================================================
template<class T>
class singletonBase
{
protected:
	// 싱글톤 인스턴스 선언
	static T* singleton;

	singletonBase() {}
	~singletonBase() {}
public:
	static T* getSingleton(void);
	void releaseSingleton(void);
};

// 싱글톤 초기화
template<class T>
T* singletonBase<T>::singleton = 0;

// 싱글톤 가져오기
template<class T>
inline T * singletonBase<T>::getSingleton(void)
{
	// 싱글톤이 없으면 새로 생성하기 후 싱글톤 리턴
	if (!singleton) singleton = new T;
	return singleton;
}
// 싱글톤 메모리에서 해제하기
template<class T>
inline void singletonBase<T>::releaseSingleton(void)
{
	// 싱글톤이 있으면 메모리에서 해제
	if (singleton)
	{
		delete singleton;
		singleton = 0;
	}
}