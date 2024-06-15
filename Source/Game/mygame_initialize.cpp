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
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0, "Start Initialize...");	// �@�}�l��loading�i�׬�0%
	//
	// 
	background.LoadBitmapByString({ "resources/titleScreen.bmp" });
	background.SetTopLeft(0, 0);
	backgroundSlide.LoadBitmapByString({ "resources/titleScreen.bmp" });
	backgroundSlide.SetTopLeft(0, 0);
	sPointer.LoadBitmapByString({ "resources/wrog.bmp" });
	sPointer.SetTopLeft(0, 0);
	//
	// �}�l���J���
	//
	Sleep(1000);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
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