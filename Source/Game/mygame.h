/*
 * mygame.h: ���ɮ��x�C��������class��interface
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


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
	protected:
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		void draw_text();
		CMovingBitmap background;
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void getDeltaTime();
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@ 
		void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		const int gravity_constant = 10;
		int stage = 1;
		int sub_phase = 1;
		int sideStrength = 25;
		int jumpStrength = 25;
		int finalJump;

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
		CMovingBitmap character;
		CMovingBitmap basePlatform;
		CMovingBitmap platMid;
		CMovingBitmap bigBox1;
		CMovingBitmap ground1;
		CMovingBitmap ground2;
		CMovingBitmap wall1;
		CMovingBitmap wall2;
		CMovingBitmap wall3;
		CMovingBitmap wall4;
		CMovingBitmap smallRectangle1;
		CMovingBitmap smallRectangle2;
		CMovingBitmap smallRectangle3;
		CMovingBitmap smallRectangle4;
		CMovingBitmap smallRectangle5;
		CMovingBitmap smallRectangle6;

		void inGameGravity();
		void inGameJumpingMechanic();

		bool wallBounce();
		bool mcCollide();
		bool mcBump();
		bool smallPlatformCollision(CMovingBitmap);
		bool smallPlatformBump(CMovingBitmap);

		bool nextStage();

		double getDistance(int ,int ,int ,int );
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();
		void OnLButtonDown(UINT nFlags, CPoint point);
	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		int counter;	// �˼Ƥ��p�ƾ�
		CMovingBitmap endBackground;
	};

}
