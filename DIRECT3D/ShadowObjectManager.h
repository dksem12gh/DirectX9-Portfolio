#pragma once
#include "singletonBase.h"
#include "baseObject.h"

class ShadowObjectManager :
	public singletonBase<ShadowObjectManager>
{
private:
	vector<baseObject*> _shadowObject;
public:
	void addShadowObject(baseObject* object) { _shadowObject.push_back(object); }
	vector<baseObject*>* getObjectVector(void) { return &_shadowObject; }

	void clearList(void) { _shadowObject.clear(); }

	ShadowObjectManager();
	~ShadowObjectManager();
};

