#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0, "Start Initialize...");	// 一開始的loading進度為0%
	//
	// 
	background.LoadBitmapByString({ "../resources/titleScreen.bmp" });
	background.SetTopLeft(0, 0);
	backgroundSlide.LoadBitmapByString({ "../resources/titleScreen.bmp" });
	backgroundSlide.SetTopLeft(0, 0);
	sPointer.LoadBitmapByString({ "../resources/wrog.bmp" });
	sPointer.SetTopLeft(0, 0);
	//
	// 開始載入資料
	//
	Sleep(1000);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
	//
	// 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
	//
}

void CGameStateInit::OnBeginState()
{
	state = 1;
	option = 1;
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 13) {
		if (option == 1) {
			GotoGameState(GAME_STATE_RUN);
		}
	}
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
}

void CGameStateInit::OnShow()
{
	if (state == 1) {
		background.ShowBitmap();
		draw_text();
	}
}

void CGameStateInit::draw_text() {
	CDC* pDC = CDDraw::GetBackCDC();

	/* Print title */
	CTextDraw::ChangeFontLog(pDC, 36, "times new roman", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 290, 100, "Press Enter key to start");

	CDDraw::ReleaseBackCDC();
}