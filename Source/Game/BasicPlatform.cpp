#include "stdafx.h"
#include "BasicPlatform.h"

namespace game_framework {
	BasicPlatform::BasicPlatform()
	{
		x = y = 0;
	}

	void BasicPlatform::LoadBitmap()
	{
		smallRectangle.LoadBitmapByString({ "../resources/cobble.bmp" }, RGB(255, 255, 255));
	}

	void BasicPlatform::SetPos(int xpos, int ypos)
	{
		x = xpos;
		y = ypos;
	}

	void BasicPlatform::OnShow()
	{
		smallRectangle.SetTopLeft(x, y);
		smallRectangle.ShowBitmap();
	}
}