#pragma once

#include "../Library/gameutil.h"

namespace game_framework {
	class GameState
	{
	public:
		GameState();
		void SetState(bool statusST);
		void SetOption(bool statusOP);
		bool state;
		bool option;
	};
}