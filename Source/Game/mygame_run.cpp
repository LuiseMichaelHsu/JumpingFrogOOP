#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <chrono>

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
	stage = 0;
}

void CGameStateRun::OnMove()							// ²¾°Ê¹CÀ¸¤¸¯À
{
	//Stage Changer
	if (character.GetTop() <= 0) {
		stage = stage + 1;
		character.SetTopLeft(character.GetLeft(), character.GetTop() + 980);
		jumpLimit = jumpLimit + 980;
	} else if(character.GetTop() >= 1000) {
		stage = stage - 1;
		character.SetTopLeft(character.GetLeft(), character.GetTop() - 980);
		jumpLimit = jumpLimit - 980;
	}
	
	//invisible left and right walls
	//Stops player from escaping the "world"
	if (character.GetLeft() <= 0) {
		character.SetTopLeft(0, character.GetTop());
	}
	else if (character.GetLeft() >= 930) {
		character.SetTopLeft(930, character.GetTop());
	}

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
		character.SetTopLeft(character.GetLeft(), character.GetTop());
		inGameGravity(); //to force player to fall when collision is active (i.e. when player is right next to a wall)
	}
	else {
		inGameGravity(); //How Gravity should work in this game...
		inGameJumpingMechanic(); //How Jumping should work in this game (the idea is to keep "jumping until it hits where it should be according to the time")
	}
}

void CGameStateRun::OnInit()  								// ¹CÀ¸ªºªì­È¤Î¹Ï§Î³]©
{
	backgroundStage1.LoadBitmapByString({ "../resources/firstStage.bmp" });
	backgroundStage1.SetTopLeft(0, 0);

	backgroundStage2.LoadBitmapByString({ "../resources/secondStage.bmp" });
	backgroundStage2.SetTopLeft(0, 0);

	//wrog 62x62
	character.LoadBitmapByString({ "../resources/wrog.bmp" }, RGB(255, 255, 255));
	character.SetTopLeft(500, 500);

	//platform width is 445 pixels
	basePlatform.LoadBitmapByString({ "../resources/platformBase.bmp" }, RGB(255, 255, 255));
	basePlatform.SetTopLeft(65, 960);

	//platform to stand on 200pixels width
	//platMid.LoadBitmapByString({ "../resources/platformStandMid.bmp" }, RGB(255, 255, 255));
	//platMid.SetTopLeft(700, 700);

	ground1.LoadBitmapByString({ "../resources/smallRectangle.bmp" }, RGB(255, 255, 255));
	ground1.SetTopLeft(700, 960);

	ground2.LoadBitmapByString({ "../resources/smallRectangle.bmp" }, RGB(255, 255, 255));
	ground2.SetTopLeft(400, 960);

	//big box platform to stand on 400 pixels height, and 400 pixels width
	bigBox1.LoadBitmapByString({ "../resources/bigBox.bmp" }, RGB(255, 255, 255));
	bigBox1.SetTopLeft(0, 580);

	//small rectangle platform to stand on 80 pixels height, and 300 pixels width
	smallRectangle1.LoadBitmapByString({ "../resources/smallRectangle.bmp" }, RGB(255, 255, 255));
	smallRectangle1.SetTopLeft(700, 700);

	//small rectangle platform to stand on 80 pixels height, and 300 pixels width
	smallRectangle2.LoadBitmapByString({ "../resources/smallRectangle.bmp" }, RGB(255, 255, 255));
	smallRectangle2.SetTopLeft(700, 300);

	//small rectangle platform to stand on 80 pixels height, and 300 pixels width
	smallRectangle3.LoadBitmapByString({ "../resources/smallRectangle.bmp" }, RGB(255, 255, 255));
	smallRectangle3.SetTopLeft(200, 100);

	smallRectangle4.LoadBitmapByString({ "../resources/smallRectangle.bmp" }, RGB(255, 255, 255));
	smallRectangle4.SetTopLeft(-200, 900);

	smallRectangle5.LoadBitmapByString({ "../resources/smallRectangle.bmp" }, RGB(255, 255, 255));
	smallRectangle5.SetTopLeft(400, 600);

	smallRectangle6.LoadBitmapByString({ "../resources/smallRectangle.bmp" }, RGB(255, 255, 255));
	smallRectangle6.SetTopLeft(600, 300);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
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

//To finalize how long or how far the player jumps
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
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

//Gravity and side jump "side fall"
void CGameStateRun::inGameGravity()
{
	if (mcCollide() == true) {
		character.SetTopLeft(character.GetLeft(), character.GetTop());
		justJumpedLeft = false;
		justJumpedRight = false;
	}
	else {
		if (jumping == false) {
			if (justJumpedLeft == true && character.GetLeft() >= leftLimit * 2) {
				character.SetTopLeft(character.GetLeft() - sideStrength / 4, character.GetTop() + gravity_constant * 3);
			}
			else if (justJumpedRight == true && character.GetLeft() <= rightLimit * 2) {
				character.SetTopLeft(character.GetLeft() + sideStrength / 4, character.GetTop() + gravity_constant * 3);
			}
			else if (justJumpedLeft == false && justJumpedRight == false) {
				character.SetTopLeft(character.GetLeft(), character.GetTop() + gravity_constant * 3);
			}
			else{
				justJumpedLeft = false;
				justJumpedRight = false;
			}
		}
	}
}

//Jump
void CGameStateRun::inGameJumpingMechanic()
{
	//jump straight
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
		if (smallPlatformCollision(ground1) || smallPlatformCollision(ground2) || smallPlatformCollision(smallRectangle1) || smallPlatformCollision(smallRectangle2) || smallPlatformCollision(smallRectangle3)) {
			return true;
		}
		else if (bigBox1.GetTop() <= character.GetTop() + 70 && bigBox1.GetTop() + 40 >= character.GetTop() + 70 && bigBox1.GetLeft() <= character.GetLeft() + 70 && bigBox1.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 2) {
		if (smallPlatformCollision(smallRectangle4) || smallPlatformCollision(smallRectangle5) || smallPlatformCollision(smallRectangle6)) {
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
		if (smallPlatformBump(smallRectangle1) || smallPlatformBump(smallRectangle2) || smallPlatformBump(smallRectangle3)) {
			return true;
		}
		else if (bigBox1.GetTop() + 400 >= character.GetTop() && bigBox1.GetTop() + 40 <= character.GetTop() && bigBox1.GetLeft() <= character.GetLeft() + 70 && bigBox1.GetLeft() + 400 >= character.GetLeft()) {
			return true;
		}
	}
	else if (stage == 2) {
		if (smallPlatformBump(smallRectangle4) || smallPlatformBump(smallRectangle5) || smallPlatformBump(smallRectangle6)) {
			return true;
		}
	}
	return false;
}

//to stand on platforms (OOP)
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

//to display the game objects
void CGameStateRun::OnShow()
{
	if (stage == 1) {
		backgroundStage1.ShowBitmap();
		smallRectangle1.ShowBitmap();
		smallRectangle2.ShowBitmap();
		smallRectangle3.ShowBitmap();
		bigBox1.ShowBitmap();
		character.ShowBitmap();
	}
	else if (stage == 2) {
		backgroundStage2.ShowBitmap();
		character.ShowBitmap();
		smallRectangle4.ShowBitmap();
		smallRectangle5.ShowBitmap();
		smallRectangle6.ShowBitmap();
	}
	else if (stage == 3) {
		GotoGameState(GAME_STATE_OVER);
	}
}
