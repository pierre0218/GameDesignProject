#include "FlyWin32.h"

#include "SceneManager.h"
#include "CameraManager.h"
#include "MathHelper.h"
#include "Character.h"

SceneManager *SceneManager::s_instance = 0;
CameraManager *CameraManager::s_instance = 0;


Character mainActor("Lyubu2");

TEXTid textID = FAILED_ID;

// some globals
int frame = 0;

// hotkey callbacks
void QuitGame(BYTE, BOOL4);
void OnMouseLeftClick(int x, int y);
void OnMouseLeftDrag(int x, int y);

// timer callbacks
void GameAI(int);
void RenderIt(int);


void MousePosition(int posx, int posy);

float mousePos[2];

const float ScreenWidth = 1024;
const float ScreenHeight = 576;


/*------------------
the main program
-------------------*/
void FyMain(int argc, char **argv)
{
	// create a new world
	BOOL4 beOK = FyStartFlyWin32("NTU@2015 Final Project - Use Fly2", 0, 0, ScreenWidth, ScreenHeight, FALSE);

	SceneManager::instance()->Start();
	CameraManager::instance()->Start();

	mainActor.Start();


	// put the character on terrain
	float pos[3], fDir[3], uDir[3];
	pos[0] = 3569.0f; pos[1] = -3208.0f; pos[2] = 1000.0f;
	fDir[0] = 1.0f; fDir[1] = -1.0f; fDir[2] = 0.0f;
	uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;
	mainActor.GetFnCharacter().SetDirection(fDir, uDir);

	mainActor.GetFnCharacter().SetTerrainRoom(SceneManager::instance()->GetRoomID(), 10.0f);
	beOK = mainActor.GetFnCharacter().PutOnTerrain(pos);

	mainActor.InitActions("Idle", "NormalAttack1", "Run", NULL, NULL, NULL);


	// create a text object for displaying messages on screen
	textID = FyCreateText("Trebuchet MS", 18, FALSE, FALSE);

	// set Hotkeys
	FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);  // escape for quiting the game

	FyBindMouseMoveFunction(MousePosition);

	FyBindMouseFunction(LEFT_MOUSE, OnMouseLeftClick, OnMouseLeftDrag, NULL, NULL);

	CameraManager::instance()->SetTarget(mainActor.GetFnCharacterID());

	// bind timers, frame rate = 30 fps
	FyBindTimer(0, 30.0f, GameAI, TRUE);
	FyBindTimer(1, 30.0f, RenderIt, TRUE);

	// invoke the system
	FyInvokeFly(TRUE);
}

float* GetMousePosition()
{
	return mousePos;
}


void MousePosition(int posx, int posy)
{
	mousePos[0] = posx;
	mousePos[1] = posy;
}

/*-------------------------------------------------------------
30fps timer callback in fixed frame rate for major game loop
--------------------------------------------------------------*/
void GameAI(int skip)
{
	SceneManager::instance()->Update(skip);
	CameraManager::instance()->Update(skip);

	mainActor.Update(skip);

	if ((mousePos[0] - ScreenWidth / 2) > ScreenWidth/2 - 50)
	{
		CameraManager::instance()->MoveRight2D();
	}
	else if ((mousePos[0] - ScreenWidth / 2) < -(ScreenWidth/2 - 50))
	{
		CameraManager::instance()->MoveLeft2D();
	}

	if ((ScreenHeight / 2 - mousePos[1]) > ScreenHeight/2 - 50)
	{
		CameraManager::instance()->MoveUp2D();
	}
	else if ((ScreenHeight / 2 - mousePos[1]) < -(ScreenHeight/2 - 50))
	{
		CameraManager::instance()->MoveDown2D();
	}
}


/*----------------------
perform the rendering
-----------------------*/
void RenderIt(int skip)
{
	CameraManager::instance()->Render();


	float pos[3], fDir[3], uDir[3];
	CameraManager::instance()->GetCamera().GetPosition(pos);
	CameraManager::instance()->GetCamera().GetDirection(fDir, uDir);

	// show frame rate
	static char string[128];
	if (frame == 0) {
		FyTimerReset(0);
	}

	if (frame / 10 * 10 == frame) {
		float curTime;

		curTime = FyTimerCheckTime(0);
		sprintf(string, "Fps: %6.2f", frame / curTime);
	}

	frame += skip;
	if (frame >= 1000) {
		frame = 0;
	}

	FnText text;
	text.ID(textID);

	text.Begin(CameraManager::instance()->GetViewportID());
	text.Write(string, 20, 20, 255, 0, 0);

	char posS[256], fDirS[256], uDirS[256], dubugText[256];
	sprintf(posS, "pos: %8.3f %8.3f %8.3f", pos[0], pos[1], pos[2]);
	sprintf(fDirS, "facing: %8.3f %8.3f %8.3f", fDir[0], fDir[1], fDir[2]);
	sprintf(uDirS, "up: %8.3f %8.3f %8.3f", uDir[0], uDir[1], uDir[2]);

	//sprintf(dubugText, "skip: %8.3f", skip);

	text.Write(posS, 20, 35, 255, 255, 0);
	text.Write(fDirS, 20, 50, 255, 255, 0);
	text.Write(uDirS, 20, 65, 255, 255, 0);
	//text.Write(dubugText, 20, 80, 255, 255, 0);

	text.End();

	// swap buffer
	FySwapBuffers();
}


/*------------------
quit the demo
-------------------*/
void QuitGame(BYTE code, BOOL4 value)
{
	if (code == FY_ESCAPE) {
		if (value) {
			FyQuitFlyWin32();
		}
	}
}

void OnMouseLeftClick(int x, int y)
{
	float worldPos[4], mousePos[2];

	mousePos[0] = x;
	mousePos[1] = y;

	CameraManager::instance()->ScreenPointToWorld(mousePos, worldPos);

	mainActor.SetTargetPos(worldPos);
}


void OnMouseLeftDrag(int x, int y)
{
	
}