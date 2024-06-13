#include "stdafx.h"
#include "GameState.h"

namespace game_framework {
	GameState::GameState()
	{
		state = option = false;
	}

	void GameState::SetState(bool statusST)
	{
		state = statusST;
	}
	
	void GameState::SetOption(bool statusOP)
	{
		option = statusOP;
	}
}