#pragma once
#include "baseObject.h"

class playerPlane : public baseObject
{
public:
	virtual void baseObjectUpdate();

	playerPlane() {};
	~playerPlane() {};

};
