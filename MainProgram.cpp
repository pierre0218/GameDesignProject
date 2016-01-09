#include "FlyWin32.h"
#include <vector>

#include "Character.h"
#include "SceneManager.h"
#include "CameraManager.h"
//#include "UnitManager.h"
#include "FXManager.h"
#include "SpriteManager.h"
//#include "MathHelper.h"

using namespace std;

SceneManager *SceneManager::s_instance = 0;
CameraManager *CameraManager::s_instance = 0;
FXManager *FXManager::s_instance = 0;
//UnitManager *UnitManager::s_instance = 0;
SpriteManager *SpriteManager::s_instance = 0;

Character* currentSelectCharacter;

vector<Character> Character::all_units;
vector<CHARACTERid> Character::all_unit_ids;
map<CHARACTERid, Character*> Character::all_units_map;


TEXTid textID = FAILED_ID;

// some globals
int frame = 0;

// hotkey callbacks
void QuitGame(BYTE, BOOL4);
void OnMouseLeftClick(int x, int y);
void OnMouseRightClick(int x, int y);
void OnMouseLeftDrag(int x, int y);
void OnMouseLeftRelease(int x, int y);

// timer callbacks
void GameAI(int);
void RenderIt(int);


void MousePosition(int posx, int posy);

float mousePos[2];
float mousePressedPos[2];
bool mouseLeftPressed = false;

const float ScreenWidth = 1024;
const float ScreenHeight = 768;

void CreateUnit(char* model, int party, float posX, float posY);
int CheckMouseHit(float* worldPos);
void AllUnitsUpdate(int skip);

void BuildUnitsMap();

OBJECTid spID0 = FAILED_ID;  // the sprite

/*------------------
the main program
-------------------*/
void FyMain(int argc, char **argv)
{
	// create a new world
	BOOL4 beOK = FyStartFlyWin32("NTU@2015 Final Project - Use Fly2", 0, 0, ScreenWidth, ScreenHeight, FALSE);

	SceneManager::instance()->Start();
	CameraManager::instance()->Start();
	FXManager::instance()->Start();
	//UnitManager::instance()->Start();

	// put the character on terrain
	float pos[3], fDir[3], uDir[3];
	pos[0] = 3569.0f; pos[1] = -3208.0f; pos[2] = 1000.0f;
//	UnitManager::instance()->CreateUnit("Robber02",pos[0]+50, pos[1]);
//	UnitManager::instance()->CreateUnit("Robber02", pos[0], pos[1]+50);

	CreateUnit("Lyubu2", 0, pos[0], pos[1]);
	CreateUnit("Robber02", 0, pos[0] + 50, pos[1]);
	CreateUnit("Robber02", 0, pos[0], pos[1] + 50);
	CreateUnit("Donzo2", 1, pos[0] + 100, pos[1]+300);

	spID0 = SpriteManager::instance()->CreateSprite("transparent_red", 0, 0, 500, 200, 0);

	// create a text object for displaying messages on screen
	textID = FyCreateText("Trebuchet MS", 18, FALSE, FALSE);

	// set Hotkeys
	FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);  // escape for quiting the game

	FyBindMouseMoveFunction(MousePosition);

	FyBindMouseFunction(LEFT_MOUSE, OnMouseLeftClick, OnMouseLeftDrag, OnMouseLeftRelease, NULL);
	FyBindMouseFunction(RIGHT_MOUSE, OnMouseRightClick, NULL, NULL, NULL);

	CameraManager::instance()->SetTarget(Character::all_units[0].GetFnCharacterID());

	BuildUnitsMap();

	// bind timers, frame rate = 30 fps
	FyBindTimer(0, 30.0f, GameAI, TRUE);
	FyBindTimer(1, 30.0f, RenderIt, TRUE);

	// invoke the system
	FyInvokeFly(TRUE);
}

void BuildUnitsMap()
{
	int ii;
	for (ii = 0; ii < Character::all_units.size(); ii++)
	{
		Character::all_units_map[Character::all_units[ii].GetFnCharacterID()] = &Character::all_units[ii];
	}
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
	FXManager::instance()->Update(skip);
	//UnitManager::instance()->Update(skip);

	AllUnitsUpdate(skip);

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

void OnMouseRightClick(int x, int y)
{
	float worldPos[3], mousePos[2];

	mousePos[0] = x;
	mousePos[1] = y;

	if (currentSelectCharacter != NULL)
	{
		currentSelectCharacter->UnSelected();
		currentSelectCharacter = NULL;
	}
	
}

void OnMouseLeftClick(int x, int y)
{
	mouseLeftPressed = true;

	float worldPos[3], mousePos[2];

	mousePos[0] = x;
	mousePos[1] = y;

	mousePressedPos[0] = x;
	mousePressedPos[1] = y;

	CameraManager::instance()->ScreenPointToWorld(mousePos, worldPos);

	int unitIdx = CheckMouseHit(worldPos);
	if (unitIdx != -1)
	{
		if (currentSelectCharacter != NULL)
		{
			currentSelectCharacter->UnSelected();
		}

		currentSelectCharacter = &Character::all_units[unitIdx];
		currentSelectCharacter->Selected();
	}
	else
	{
		if (currentSelectCharacter != NULL)
		{
			currentSelectCharacter->SetTargetPos(worldPos);
		}
	}

	//mainActor.SetTargetPos(worldPos);

	//mainActor.Selected();

	//FXManager::instance()->CreateFX(worldPos);
}

void OnMouseLeftRelease(int x, int y)
{
	mouseLeftPressed = false;
	FnSprite sp;
	sp.ID(spID0);
	sp.SetSize(0, 0);
}


void OnMouseLeftDrag(int x, int y)
{
	if (mouseLeftPressed)
	{
		mousePos[0] = x;
		mousePos[1] = y;
		float w = mousePos[0] - mousePressedPos[0];
		float h = mousePos[1] - mousePressedPos[1];

		FnSprite sp;
		sp.ID(spID0);

		if (w > 0 && h > 0)
		{
			int ox = mousePressedPos[0];
			int oy = ScreenHeight - mousePos[1];

			sp.SetSize(w, h);
			sp.SetPosition(ox, oy, 0);
		}

		if (w < 0 && h > 0)
		{
			int ox = mousePos[0];
			int oy = ScreenHeight - mousePos[1];

			sp.SetSize(MathHelper::Abs(w), h);
			sp.SetPosition(ox, oy, 0);
		}

		if (w > 0 && h < 0)
		{
			int ox = mousePressedPos[0];
			int oy = ScreenHeight - (mousePos[1] - h);

			sp.SetSize(MathHelper::Abs(w), MathHelper::Abs(h));
			sp.SetPosition(ox, oy, 0);
		}

		if (w < 0 && h < 0)
		{
			int ox = mousePos[0];
			int oy = ScreenHeight - (mousePos[1] - h);

			sp.SetSize(MathHelper::Abs(w), MathHelper::Abs(h));
			sp.SetPosition(ox, oy, 0);
		}
	}
}

void CreateUnit(char* model, int party, float posX, float posY)
{
	float pos[3], fDir[3], uDir[3];
	pos[0] = posX; pos[1] = posY; pos[2] = 1000.0f;
	fDir[0] = 1.0f; fDir[1] = -1.0f; fDir[2] = 0.0f;
	uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;

	Character actor(model, party);

	actor.Start();

	actor.GetFnCharacter().SetDirection(fDir, uDir);

	actor.GetFnCharacter().SetTerrainRoom(SceneManager::instance()->GetRoomID(), 10.0f);
	BOOL4 beOK = actor.GetFnCharacter().PutOnTerrain(pos);

	actor.InitActions("Idle", "NormalAttack1", "Run", "Damage1", "Damage2", "Die");

	Character::all_units.push_back(actor);
	Character::all_unit_ids.push_back(actor.GetFnCharacterID());
}

int CheckMouseHit(float* worldPos)
{
	int ii;
	for (ii = 0; ii < Character::all_units.size(); ii++)
	{
		float pos[3];
		Character::all_units[ii].GetFnCharacter().GetPosition(pos);

		float distance = MathHelper::VectorDistance(worldPos, pos);
		if (distance < 70)
		{
			return ii;
		}
	}

	return -1;
}

void AllUnitsUpdate(int skip)
{
	int ii;
	for (ii = 0; ii < Character::all_units.size(); ii++)
	{
		Character::all_units[ii].Update(skip);
	}
}