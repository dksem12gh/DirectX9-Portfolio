#include "stdafx.h"
#include "playerPlane.h"

void playerPlane::baseObjectUpdate()
{
	// 회전
	if (KEYMANAGER->isStayKeyDown(VK_LSHIFT))	
	{
		float rotDelta = D3DXToRadian(90) * _deltaTime;

		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_transform->rotateWorld(0, -rotDelta, 0);
		}
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_transform->rotateWorld(0, rotDelta, 0);
		}

	}
	else			// 이동
	{
		float moveDelta = 5.0f * _deltaTime;
		if (KEYMANAGER->isStayKeyDown(VK_LEFT))
		{
			_transform->movePositionWorld(-moveDelta, 0, 0);
		}
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			_transform->movePositionWorld(moveDelta, 0, 0);
		}
	}

}
