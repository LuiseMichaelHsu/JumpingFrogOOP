#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <chrono>
#include <string>

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// ³o­Óclass¬°¹CÀ¸ªº¹CÀ¸°õ¦æª«¥ó¡A¥D­nªº¹CÀ¸µ{¦¡³£¦b³o¸Ì
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
	
}

void CGameStateRun::OnBeginState()
{
	stage = 1;
	firstTime = 0;
	firstStart = 0;
	ghostMode = gameState.option;
	winsGame = false;
	cheatSheet = false;
}

void CGameStateRun::OnMove()							// ²¾°Ê¹CÀ¸¤¸¯À
{
	//Stage Changer
	if (character.GetTop() <= -50) {
		stage = stage + 1;
		character.SetTopLeft(character.GetLeft(), character.GetTop() + 980);
		jumpLimit = jumpLimit + 980;
	} else if (character.GetTop() >= 1000 && stage == 6) {
		stage = 100;
		character.SetTopLeft(character.GetLeft(), character.GetTop() - 990);
		jumpLimit = jumpLimit - 990;
	} else if (character.GetTop() >= 1000) {
		stage = stage - 1;
		character.SetTopLeft(character.GetLeft(), character.GetTop() - 990);
		jumpLimit = jumpLimit - 990;
	}

	//Transition to level 6
	if (character.GetLeft() >= 800 && stage == 5) {
		stage = 6;
	}

	//invisible left and right walls
	//Stops player from escaping the "world"
	if (character.GetLeft() <= 0) {
		character.SetTopLeft(0, character.GetTop());
	}
	else if (character.GetLeft() >= 930) {
		character.SetTopLeft(930, character.GetTop());
	}
	if (ghostMode == false) {
		//bounce
		if (wallBounce() == true && mcCollide() == false) {
			if (left) { //bounce towards left
				character.SetTopLeft(character.GetLeft() - sideStrength / 8, character.GetTop());
			}
			else if (right) { //bounce right
				character.SetTopLeft(character.GetLeft() + sideStrength / 8, character.GetTop());
			}
		}
		//prevent go through platforms
		else if (mcBump() == true && mcCollide() == false) {
			jumping = false;
			character.SetTopLeft(character.GetLeft(), character.GetTop()+10);
			inGameGravity(); //to force player to fall when collision is active (i.e. when player is right next to a wall)
		}
		else {
			inGameGravity(); //How Gravity should work in this game...
			inGameJumpingMechanic(); //How Jumping should work in this game (the idea is to keep "jumping until it hits where it should be according to the time")
		}
	}
}

void CGameStateRun::OnInit()  								// ¹CÀ¸ªºªì­È¤Î¹Ï§Î³]©
{
	basicP1.smallRectangle.LoadBitmapByString({ "../resources/cobble.bmp" }, RGB(255, 255, 255));
	basicP2.smallRectangle.LoadBitmapByString({ "../resources/cobble.bmp" }, RGB(255, 255, 255));
	basicP3.smallRectangle.LoadBitmapByString({ "../resources/cobble.bmp" }, RGB(255, 255, 255));
	basicP4.smallRectangle.LoadBitmapByString({ "../resources/cobble.bmp" }, RGB(255, 255, 255));

	Cheating.LoadBitmapByString({ "../resources/cheatsheet.bmp" });
	Cheating.SetTopLeft(200, 200);

	backgroundStage1.LoadBitmapByString({ "../resources/firstStage.bmp" });
	backgroundStage1.SetTopLeft(0, 0);

	backgroundStage2.LoadBitmapByString({ "../resources/secondStage.bmp" });
	backgroundStage2.SetTopLeft(0, 0);

	backgroundStage3.LoadBitmapByString({ "../resources/level3.bmp" });
	backgroundStage3.SetTopLeft(0, 0);

	backgroundStage4.LoadBitmapByString({ "../resources/level4.bmp" });
	backgroundStage4.SetTopLeft(0, 0);

	backgroundStage5.LoadBitmapByString({ "../resources/level5.bmp" });
	backgroundStage5.SetTopLeft(0, 0);

	backgroundStage6.LoadBitmapByString({ "../resources/level6.bmp" });
	backgroundStage6.SetTopLeft(0, 0);

	backgroundStage7.LoadBitmapByString({ "../resources/level7.bmp" });
	backgroundStage7.SetTopLeft(0, 0);

	backgroundStage8.LoadBitmapByString({ "../resources/level8.bmp" });
	backgroundStage8.SetTopLeft(0, 0);

	backgroundStage9.LoadBitmapByString({ "../resources/level9.bmp" });
	backgroundStage9.SetTopLeft(0, 0);

	backgroundStage10.LoadBitmapByString({ "../resources/level10.bmp" });
	backgroundStage10.SetTopLeft(0, 0);

	flag.LoadBitmapByString({ "../resources/flag.bmp" });
	flag.SetTopLeft(0, 0);

	win.LoadBitmapByString({ "../resources/ggwp.bmp" });
	win.SetTopLeft(0, 0);

	//wrog 62x62
	character.LoadBitmapByString({ "../resources/wrog.bmp" }, RGB(255, 255, 255));
	character.SetTopLeft(500, 500);

	//big box platform to stand on 400 pixels height, and 400 pixels width
	bigBox.LoadBitmapByString({ "../resources/box.bmp" }, RGB(255, 255, 255));
	bigBox.SetTopLeft(0, 0);

	//wall 445 height, 80 width
	wallPlat.LoadBitmapByString({ "../resources/wall.bmp" }, RGB(255, 255, 255));
	wallPlat.SetTopLeft(0, 0);

	ground1.LoadBitmap();
	ground2.LoadBitmap();
	ground1.SetPos(0, 0);
	ground2.SetPos(0, 0);

	//small rectangle platform to stand on 80 pixels height, and 300 pixels width
	basicP1.LoadBitmap();
	basicP2.LoadBitmap();
	basicP3.LoadBitmap();
	basicP4.LoadBitmap();
	basicP1.SetPos(0, 0);
	basicP2.SetPos(0, 0);
	basicP3.SetPos(0, 0);
	basicP4.SetPos(0, 0);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB) {
		if (ghostMode == false) {
			ghostMode = true;
		}
		else {
			ghostMode = false;
		}
	}
	if (nChar == 67) {
		if (cheatSheet == false) {
			cheatSheet = true;
		}
		else {
			cheatSheet = false;
		}
	}
	if (stage == 5) {
		if (nChar == 13) {
			stage = 6;
		}
	}
	if (stage == 50) {
		if (nChar == 13) {
			winsGame = true;
		}
	}
	if (ghostMode == false) {
		if (nChar == VK_LEFT) {
			left = false;
			leftTime = clock();
		}
		if (nChar == VK_RIGHT) {
			right = false;
			rightTime = clock();
		}
		//another idea for the left and right button
		/*if (nChar == VK_LEFT) {
			leftTime = clock() - leftTime;
			if (leftTime < 100) {
				leftTime = 100;
			}
			if (leftTime > 800) {
				leftTime = 800;
			}
			leftTime = leftTime / 2;
			left = true;
			right = false;
			leftLimit = character.GetLeft() - leftTime;
		}
		if (nChar == VK_RIGHT) {
			rightTime = clock() - rightTime;
			if (rightTime < 100) {
				rightTime = 100;
			}
			if (rightTime > 800) {
				rightTime = 800;
			}
			rightTime = rightTime / 2;
			left = false;
			right = true;
			rightLimit = character.GetLeft() + rightTime;
		}*/
		if (nChar == VK_SPACE) {
			jumping = false;
			jumpTime = clock();
		}
	}
	else {
		if (nChar == VK_LEFT) {
			character.SetTopLeft(character.GetLeft() - 80, character.GetTop());
		}
		if (nChar == VK_RIGHT) {
			character.SetTopLeft(character.GetLeft() + 80, character.GetTop());
		}
		if (nChar == VK_UP) {
			character.SetTopLeft(character.GetLeft(), character.GetTop() - 80);
		}
		if (nChar == VK_DOWN) {
			character.SetTopLeft(character.GetLeft(), character.GetTop() + 80);
		}
		if (nChar == VK_SPACE) {
			if (stage < 10) {
				stage = stage + 1;
			};
		}
	}
}

//To finalize how long or how far the player jumps
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (ghostMode == false) {
		if (nChar == VK_LEFT) {
			leftTime = clock() - leftTime;
			if (leftTime < 100) {
				leftTime = 100;
			}
			if (leftTime > 800) {
				leftTime = 800;
			}
			leftTime = leftTime / 2;
			left = true;
			leftLimit = character.GetLeft() - leftTime;
		}
		if (nChar == VK_RIGHT) {
			rightTime = clock() - rightTime;
			if (rightTime < 100) {
				rightTime = 100;
			}
			if (rightTime > 800) {
				rightTime = 800;
			}
			rightTime = rightTime / 2;
			right = true;
			rightLimit = character.GetLeft() + rightTime;
		}
		//if (nChar == VK_LEFT) {
		//	left = false;
		//	leftTime = clock();
		//}
		//if (nChar == VK_RIGHT) {
		//	right = false;
		//	rightTime = clock();
		//}
		if (nChar == VK_SPACE) {
			jumpTime = clock() - jumpTime;
			if (jumpTime < 200) {
				jumpTime = 200;
			}
			if (jumpTime > 800) {
				jumpTime = 800;
			}
			jumpTime = jumpTime / 2;
			jumping = true;
			jumpLimit = character.GetTop() - jumpTime;
		}
	}
}

//Gravity and side jump "side fall"
void CGameStateRun::inGameGravity()
{
	if (ghostMode == false) {
		if (mcCollide() == true) {
			character.SetTopLeft(character.GetLeft(), character.GetTop());
			justJumpedLeft = false;
			justJumpedRight = false;
		}
		else {
			if (jumping == false) {
				/**/
				if (justJumpedLeft == true && character.GetLeft() >= leftLimit * 2) {
					character.SetTopLeft(character.GetLeft() - sideStrength / 4, character.GetTop() + gravity_constant * 2);
				}
				else if (justJumpedRight == true && character.GetLeft() <= rightLimit * 2) {
					character.SetTopLeft(character.GetLeft() + sideStrength / 4, character.GetTop() + gravity_constant * 2);
				}
				else if (justJumpedLeft == false && justJumpedRight == false) {
					character.SetTopLeft(character.GetLeft(), character.GetTop() + gravity_constant * 2);
				}
				else {
					justJumpedLeft = false;
					justJumpedRight = false;
				}
				/*
				if (justJumpedLeft == false && justJumpedRight == false) {
					character.SetTopLeft(character.GetLeft(), character.GetTop() + gravity_constant * 3);
				}
				else {
					justJumpedLeft = false;
					justJumpedRight = false;
				}
				*/
			}
		}
	}
}

//Jump
void CGameStateRun::inGameJumpingMechanic()
{
	//jump straight up
	if (jumping == true && character.GetTop() >= jumpLimit && left == false && right == false) {
		character.SetTopLeft(character.GetLeft(), character.GetTop() - jumpStrength);
		justJumpedRight = false;
		justJumpedLeft = false;
	}
	//jump left
	else if (jumping == true && character.GetTop() >= jumpLimit && left == true && character.GetLeft() >= leftLimit) {
		character.SetTopLeft(character.GetLeft() - sideStrength, character.GetTop() - jumpStrength);
		justJumpedLeft = true;
		justJumpedRight = false;
	}
	//jump right
	else if (jumping == true && character.GetTop() >= jumpLimit && right == true && character.GetLeft() <= rightLimit) {
		character.SetTopLeft(character.GetLeft() + sideStrength, character.GetTop() - jumpStrength);
		justJumpedRight = true;
		justJumpedLeft = false;
	}
	//to make sure it jumps straight
	else {
		jumping = false;
		left = false;
		right = false;
	}
}

//Things that happens when the frog touches the wall
bool CGameStateRun::wallBounce() {
	if (mcBump() == true && justJumpedLeft == true) { //hit left
		character.SetTopLeft(character.GetLeft(), character.GetTop());
		left = false;
		right = true;
		return true;
	}
	else if (mcBump() == true && justJumpedRight == true) { //hit right
		character.SetTopLeft(character.GetLeft(), character.GetTop());
		right = false;
		left = true;
		return true;
	}
	else {
		return false;
	}
}

//to prevent player from going pass a platform
//to check if character should be able to stand on platform
bool CGameStateRun::mcCollide() {
	//collide first platform smallRectangle
	if (stage == 1) {
		if (smallPlatformCollision(ground1.smallRectangle) || smallPlatformCollision(ground2.smallRectangle) || smallPlatformCollision(basicP1.smallRectangle) || smallPlatformCollision(basicP2.smallRectangle) || smallPlatformCollision(basicP3.smallRectangle)) {
			return true;
		}
		else if (bigBox.GetTop() <= character.GetTop() + 70 && bigBox.GetTop() + 40 >= character.GetTop() + 70 && bigBox.GetLeft() <= character.GetLeft() + 70 && bigBox.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	} 
	else if (stage == 2) {
		if (smallPlatformCollision(basicP1.smallRectangle) || smallPlatformCollision(basicP2.smallRectangle) || smallPlatformCollision(basicP3.smallRectangle) || smallPlatformCollision(basicP4.smallRectangle)) {
			return true;
		}
	}
	else if (stage == 3) {
		if (smallPlatformCollision(basicP1.smallRectangle) || smallPlatformCollision(basicP2.smallRectangle) || smallPlatformCollision(basicP3.smallRectangle) || smallPlatformCollision(basicP4.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() <= character.GetTop() + 70 && wallPlat.GetTop() + 40 >= character.GetTop() + 70 && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 4) {
		if (smallPlatformCollision(basicP1.smallRectangle) || smallPlatformCollision(basicP2.smallRectangle) || smallPlatformCollision(basicP3.smallRectangle) || smallPlatformCollision(basicP4.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() <= character.GetTop() + 70 && wallPlat.GetTop() + 40 >= character.GetTop() + 70 && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 5) {
		if (smallPlatformCollision(basicP1.smallRectangle)) {
			return true;
		}
	}
	else if (stage == 6) {
		if (smallPlatformCollision(basicP1.smallRectangle) || smallPlatformCollision(basicP2.smallRectangle) || smallPlatformCollision(basicP3.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() <= character.GetTop() + 70 && wallPlat.GetTop() + 40 >= character.GetTop() + 70 && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 7) {
		if (smallPlatformCollision(basicP1.smallRectangle) || smallPlatformCollision(basicP2.smallRectangle) || smallPlatformCollision(basicP3.smallRectangle) || smallPlatformCollision(basicP4.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() <= character.GetTop() + 70 && wallPlat.GetTop() + 40 >= character.GetTop() + 70 && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 8) {
		if (smallPlatformCollision(basicP1.smallRectangle) || smallPlatformCollision(basicP2.smallRectangle) || smallPlatformCollision(basicP3.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() <= character.GetTop() + 70 && wallPlat.GetTop() + 40 >= character.GetTop() + 70 && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
		else if (bigBox.GetTop() <= character.GetTop() + 70 && bigBox.GetTop() + 40 >= character.GetTop() + 70 && bigBox.GetLeft() <= character.GetLeft() + 70 && bigBox.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 9) {
		if (smallPlatformCollision(basicP1.smallRectangle) || smallPlatformCollision(basicP2.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() <= character.GetTop() + 70 && wallPlat.GetTop() + 40 >= character.GetTop() + 70 && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
		else if (bigBox.GetTop() <= character.GetTop() + 70 && bigBox.GetTop() + 40 >= character.GetTop() + 70 && bigBox.GetLeft() <= character.GetLeft() + 70 && bigBox.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 10) {
		if (smallPlatformCollision(basicP1.smallRectangle) || smallPlatformCollision(basicP2.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() <= character.GetTop() + 70 && wallPlat.GetTop() + 40 >= character.GetTop() + 70 && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
		else if (bigBox.GetTop() <= character.GetTop() + 70 && bigBox.GetTop() + 40 >= character.GetTop() + 70 && bigBox.GetLeft() <= character.GetLeft() + 70 && bigBox.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	}
	return false;
}

//to check if the player should be able to bounce off the wall?
//if they touch the side of the platform
bool CGameStateRun::mcBump() {
	//collide first platform smallRectangle
	if (stage == 1) {
		if (smallPlatformBump(basicP1.smallRectangle) || smallPlatformBump(basicP2.smallRectangle) || smallPlatformBump(basicP3.smallRectangle)) {
			return true;
		}
		else if (bigBox.GetTop() + 400 >= character.GetTop() && bigBox.GetTop() + 40 <= character.GetTop() && bigBox.GetLeft() <= character.GetLeft() + 70 && bigBox.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 2) {
		if (smallPlatformBump(basicP1.smallRectangle) || smallPlatformBump(basicP2.smallRectangle) || smallPlatformBump(basicP3.smallRectangle) || smallPlatformBump(basicP4.smallRectangle)) {
			return true;
		}
	}
	else if (stage == 3) {
		if (smallPlatformBump(basicP1.smallRectangle) || smallPlatformBump(basicP2.smallRectangle) || smallPlatformBump(basicP3.smallRectangle) || smallPlatformBump(basicP4.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() + 445 >= character.GetTop() && wallPlat.GetTop() + 40 <= character.GetTop() && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 4) {
		if (smallPlatformBump(basicP1.smallRectangle) || smallPlatformBump(basicP2.smallRectangle) || smallPlatformBump(basicP3.smallRectangle) || smallPlatformBump(basicP4.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() + 445 >= character.GetTop() && wallPlat.GetTop() + 40 <= character.GetTop() && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 5) {
		if (smallPlatformBump(basicP1.smallRectangle)) {
			return true;
		}
	}
	else if (stage == 6) {
		if (smallPlatformBump(basicP1.smallRectangle) || smallPlatformBump(basicP2.smallRectangle) || smallPlatformBump(basicP3.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() + 445 >= character.GetTop() && wallPlat.GetTop() + 40 <= character.GetTop() && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 7) {
		if (smallPlatformBump(basicP1.smallRectangle) || smallPlatformBump(basicP2.smallRectangle) || smallPlatformBump(basicP3.smallRectangle) || smallPlatformBump(basicP4.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() + 445 >= character.GetTop() && wallPlat.GetTop() + 40 <= character.GetTop() && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 8) {
		if (smallPlatformBump(basicP1.smallRectangle) || smallPlatformBump(basicP2.smallRectangle) || smallPlatformBump(basicP3.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() + 445 >= character.GetTop() && wallPlat.GetTop() + 40 <= character.GetTop() && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
		else if (bigBox.GetTop() + 400 >= character.GetTop() && bigBox.GetTop() + 40 <= character.GetTop() && bigBox.GetLeft() <= character.GetLeft() + 70 && bigBox.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 9) {
		if (smallPlatformBump(basicP1.smallRectangle) || smallPlatformBump(basicP2.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() + 445 >= character.GetTop() && wallPlat.GetTop() + 40 <= character.GetTop() && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
		else if (bigBox.GetTop() + 400 >= character.GetTop() && bigBox.GetTop() + 40 <= character.GetTop() && bigBox.GetLeft() <= character.GetLeft() + 70 && bigBox.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 10) {
		if (smallPlatformBump(basicP1.smallRectangle) || smallPlatformBump(basicP2.smallRectangle)) {
			return true;
		}
		else if (wallPlat.GetTop() + 445 >= character.GetTop() && wallPlat.GetTop() + 40 <= character.GetTop() && wallPlat.GetLeft() <= character.GetLeft() + 70 && wallPlat.GetLeft() + 80 >= character.GetLeft()) {
			return true;
		}
		else if (bigBox.GetTop() + 400 >= character.GetTop() && bigBox.GetTop() + 40 <= character.GetTop() && bigBox.GetLeft() <= character.GetLeft() + 70 && bigBox.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	}
	return false;
}

bool CGameStateRun::smallPlatformCollision(CMovingBitmap platform) {
	//collide first platform smallRectangle
	if (platform.GetTop() <= character.GetTop() + 70 && platform.GetTop() + 30 >= character.GetTop() + 70 && platform.GetLeft() <= character.GetLeft() + 70 && platform.GetLeft() + 300 >= character.GetLeft()) {
		return true;
	}
	else {
		return false;
	}
}

//to bounce off platforms (OOP)
bool CGameStateRun::smallPlatformBump(CMovingBitmap platform) {
	//collide first platform smallRectangle
	if (platform.GetTop() + 60 >= character.GetTop() && platform.GetTop() + 20 <= character.GetTop() && platform.GetLeft() <= character.GetLeft() + 70 && platform.GetLeft() + 300 >= character.GetLeft()) {
		return true;
	}
	else if (platform.GetTop() + 80 >= character.GetTop() && platform.GetTop() + 60 <= character.GetTop() && platform.GetLeft() <= character.GetLeft() + 70 && platform.GetLeft() + 300 >= character.GetLeft()) {
		character.SetTopLeft(character.GetLeft(), character.GetTop());
		jumping = false;
		justJumpedLeft = false;
		justJumpedRight = false;
		return false;
	}
	else {
		return false;
	}
}

//not currently used
double CGameStateRun::getDistance(int x1, int y1, int x2, int y2){
	int xDistance = x2 - x1;
	int yDistance = y2 - y1;

	return sqrt(pow(xDistance, 2) + pow(yDistance, 2));
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // ³B²z·Æ¹«ªº°Ê§@
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// ³B²z·Æ¹«ªº°Ê§@
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// ³B²z·Æ¹«ªº°Ê§@
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // ³B²z·Æ¹«ªº°Ê§@
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// ³B²z·Æ¹«ªº°Ê§@
{
}

void CGameStateRun::EasyClear() {
	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 36, "times new roman", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 100, 400, "Congratulations, You Have Cleared Easy Levels");
	CTextDraw::Print(pDC, 100, 500, "Smash into the right wall to go to level 6");
	CTextDraw::Print(pDC, 100, 600, "P.S. Don't Fall");

	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::EndMsg() {
	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 36, "times new roman", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 50, 50, "Interact With The Pink Frog To Win The Game");

	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::WinMsg() {
	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 36, "times new roman", RGB(1, 255, 1));
	CTextDraw::Print(pDC, 60, 700, "Click Enter To Go Back To The Main Menu");

	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::DebugModeMsg() {
	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 20, "times new roman", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 0, 0, "Debug Mode Is On");

	CDDraw::ReleaseBackCDC();
}

void CGameStateRun::levelIndicator() {
	CDC* pDC = CDDraw::GetBackCDC();

	CTextDraw::ChangeFontLog(pDC, 20, "times new roman", RGB(255, 255, 255));
	CTextDraw::Print(pDC, 900, 0, "Level " + std::to_string(stage));

	CDDraw::ReleaseBackCDC();
}

//to display the game objects
void CGameStateRun::OnShow()
{
	if (stage == 1) {
		basicP1.SetPos(700, 700);
		basicP2.SetPos(700, 300);
		basicP3.SetPos(200, 100);
		ground1.SetPos(700, 960);
		ground2.SetPos(400, 960);
		bigBox.SetTopLeft(0, 580);

		backgroundStage1.ShowBitmap();
		if (firstStart <= 1) {
			character.SetTopLeft(500, 800);
			firstStart = firstStart + 1;
		}
		else {
			firstStart = firstStart + 1;
		}

		basicP1.OnShow();
		basicP2.OnShow();
		basicP3.OnShow();
		ground1.OnShow();
		ground2.OnShow();
		bigBox.ShowBitmap();

		character.ShowBitmap();
	}
	else if (stage == 2) {
		basicP1.SetPos(-200, 900);
		basicP2.SetPos(400, 600);
		basicP3.SetPos(700, 300);
		basicP4.SetPos(0, 120);

		backgroundStage2.ShowBitmap();

		basicP1.OnShow();
		basicP2.OnShow();
		basicP3.OnShow();
		basicP4.OnShow();

		character.ShowBitmap();
	}
	else if (stage == 3) {
		basicP1.SetPos(700, 900);
		basicP2.SetPos(700, 550);
		basicP3.SetPos(150, 200);
		basicP4.SetPos(700, 300);
		wallPlat.SetTopLeft(440, 250);

		backgroundStage3.ShowBitmap();

		basicP1.OnShow();
		basicP2.OnShow();
		basicP3.OnShow();
		basicP4.OnShow();
		wallPlat.ShowBitmap();

		character.ShowBitmap();
	}
	else if (stage == 4) {
		basicP1.SetPos(400, 900);
		basicP2.SetPos(400, 600);
		basicP3.SetPos(700, 300);
		basicP4.SetPos(150, 200);
		wallPlat.SetTopLeft(200, 350);

		backgroundStage4.ShowBitmap();

		basicP1.OnShow();
		basicP2.OnShow();
		basicP3.OnShow();
		basicP4.OnShow();
		wallPlat.ShowBitmap();

		character.ShowBitmap();
	}
	else if (stage == 5) {
		basicP1.SetPos(650, 900);

		backgroundStage5.ShowBitmap();

		EasyClear();

		basicP1.OnShow();
		character.ShowBitmap();
	}
	else if (stage == 6) {
		basicP1.SetPos(0, 900);
		basicP2.SetPos(0, 600);
		basicP3.SetPos(650, 200);
		wallPlat.SetTopLeft(440, 350);

		backgroundStage6.ShowBitmap();
		if (firstTime <= 1) {
			character.SetTopLeft(50, 850);
			firstTime = firstTime + 1;
		}
		else {
			firstTime = firstTime + 1;
		}

		basicP1.OnShow();
		basicP2.OnShow();
		basicP3.OnShow();
		wallPlat.ShowBitmap();

		character.ShowBitmap();
	}
	else if (stage == 7) {
		basicP1.SetPos(0, 900);
		basicP2.SetPos(650, 700);
		basicP3.SetPos(750, 390);
		basicP4.SetPos(200, 130);
		wallPlat.SetTopLeft(400, 150);

		backgroundStage7.ShowBitmap();

		basicP1.OnShow();
		basicP2.OnShow();
		basicP3.OnShow();
		basicP4.OnShow();
		wallPlat.ShowBitmap();

		character.ShowBitmap();
	}
	else if (stage == 8) {
		basicP1.SetPos(600, 900);
		basicP2.SetPos(900, 600);
		basicP3.SetPos(-150, 80);
		bigBox.SetTopLeft(0, 380);
		wallPlat.SetTopLeft(600, 300);

		backgroundStage8.ShowBitmap();

		basicP1.OnShow();
		basicP2.OnShow();
		basicP3.OnShow();
		wallPlat.ShowBitmap();
		bigBox.ShowBitmap();

		character.ShowBitmap();
	}
	else if (stage == 9) {
		basicP1.SetPos(-150, 150);
		basicP2.SetPos(230, 350);
		bigBox.SetTopLeft(400, 680);
		wallPlat.SetTopLeft(700, 70);

		backgroundStage9.ShowBitmap();

		basicP1.OnShow();
		basicP2.OnShow();
		bigBox.ShowBitmap();
		wallPlat.ShowBitmap();

		character.ShowBitmap();
	}
	else if (stage == 10) {
		basicP1.SetPos(150, 350);
		basicP2.SetPos(-100, 700);
		bigBox.SetTopLeft(600, 300);
		wallPlat.SetTopLeft(400, 650);
		flag.SetTopLeft(800, 235);

		backgroundStage10.ShowBitmap();

		EndMsg();

		basicP1.OnShow();
		basicP2.OnShow();
		bigBox.ShowBitmap();
		wallPlat.ShowBitmap();
		flag.ShowBitmap();

		if (character.GetLeft() >= flag.GetLeft()-50) {
			stage = 50;
		}

		character.ShowBitmap();
	}
	else if (stage == 50) {
		character.SetTopLeft(900, 500);
		win.ShowBitmap();
		WinMsg();
		if (winsGame == true) {
			GotoGameState(GAME_STATE_INIT);
		}
	}
	else if (stage == 100) {
		GotoGameState(GAME_STATE_OVER);
	}
	else if (stage == 201) {

	}//turtorial stage
	if (ghostMode == true) {
		DebugModeMsg();
		character.LoadBitmapByString({ "../resources/cheater.bmp" }, RGB(255, 255, 255));
	}
	else {
		character.LoadBitmapByString({ "../resources/wrog.bmp" }, RGB(255, 255, 255));
	}
	if (cheatSheet == true) {
		Cheating.ShowBitmap();
	}
	levelIndicator();
}
