/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/
#include "BasicPlatform.h"
#include "GameState.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnKeyDown(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		void draw_text();
		CMovingBitmap background;
		CMovingBitmap backgroundSlide;
		CMovingBitmap sPointer;
		int state = 1;
		int option = 1;
		GameState gameState;
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void getDeltaTime();
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		const int gravity_constant = 10;
		int stage = 1;
		int sub_phase = 1;
		int sideStrength = 25;
		int jumpStrength = 25;
		int finalJump;
		int firstTime;
		int firstStart;
		bool ghostMode;
		GameState gameState;

		bool left = false;
		int leftLimit;
		clock_t leftTime;
		bool justJumpedLeft = false;

		bool right = false;
		int rightLimit;
		clock_t rightTime;
		bool justJumpedRight = false;

		bool jumping = false;
		int jumpLimit;
		clock_t jumpTime;

		float previousTime;
		float currentTime;
		float deltaTime;
		CMovingBitmap backgroundStage1;
		CMovingBitmap backgroundStage2;
		CMovingBitmap backgroundStage3;
		CMovingBitmap backgroundStage4;
		CMovingBitmap backgroundStage5;
		CMovingBitmap backgroundStage6;
		CMovingBitmap backgroundStage7;
		CMovingBitmap backgroundStage8;
		CMovingBitmap backgroundStage9;
		CMovingBitmap backgroundStage10;
		CMovingBitmap character;
		CMovingBitmap flag;
		CMovingBitmap win;
		bool winsGame;

		CMovingBitmap Cheating;
		bool cheatSheet;

		CMovingBitmap bigBox;
		CMovingBitmap wallPlat;

		BasicPlatform ground1;
		BasicPlatform ground2;

		BasicPlatform basicP1;
		BasicPlatform basicP2;
		BasicPlatform basicP3;
		BasicPlatform basicP4;

		void inGameGravity();
		void inGameJumpingMechanic();

		bool wallBounce();
		bool mcCollide();
		bool mcBump();
		bool smallPlatformCollision(CMovingBitmap);
		bool smallPlatformBump(CMovingBitmap);

		bool nextStage();

		double getDistance(int ,int ,int ,int );

		void EasyClear();
		void EndMsg();
		void WinMsg();
		void DebugModeMsg();
		void levelIndicator();
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
		void OnKeyDown(UINT, UINT, UINT);
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
		CMovingBitmap endBackground;
	};

}
