#pragma once

#include "../Library/gameutil.h"

namespace game_framework {
	class BasicPlatform
	{
	public:
		BasicPlatform();
		void LoadBitmap();
		void SetPos(int xpos, int ypos);
		void OnShow();
		CMovingBitmap smallRectangle;
	protected:
		int x, y;
	};
}