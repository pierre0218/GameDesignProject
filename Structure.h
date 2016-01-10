#include "FlyWin32.h"
//#include "SceneManager.h"
#include <map>
#include <vector>

using namespace std;

class Structure
{
public:
	CHARACTERid actorID;

	char* mModelName;
	FnCharacter mCharacter;
	int frameCount = 0;

	int numOfUnitsCreated = 0;

	Structure(char* modelName)
	{
		mModelName = modelName;
	}
	ACTIONid idleID, attackID, runID, damage1ID, damage2ID, dieID;

	static vector<Structure> all_structures;
	static map<CHARACTERid, Structure*> all_structures_map;

	void InitActions(char* idle, char* attack, char* run, char* damage1, char* damage2, char* die)
	{
		idleID = mCharacter.GetBodyAction(NULL, idle);
		runID = mCharacter.GetBodyAction(NULL, run);
		attackID = mCharacter.GetBodyAction(NULL, attack);

		mCharacter.SetCurrentAction(NULL, 0, idleID, 5.0f);
	}
	void Start()
	{
		// load the character
		FySetModelPath("Data\\NTU6\\Characters");
		FySetTexturePath("Data\\NTU6\\Characters");
		FySetCharacterPath("Data\\NTU6\\Characters");
		FySetGameFXPath("Data\\NTU6\\FX0");

		actorID = SceneManager::instance()->GetScene().LoadCharacter(mModelName);

		mCharacter.ID(actorID);
	}
	void Update(int skip)
	{
		frameCount++;
		mCharacter.Play(LOOP, (float)skip, FALSE, TRUE);

		float mPos[3];
		mCharacter.GetPosition(mPos);

		if (numOfUnitsCreated > 10)
			return;

		if (frameCount % (30 * 5) == 0)
		{
			int sign_x = -1;
			if (rand() % 2 == 0)
			{
				sign_x = 1;
			}

			int sign_y = -1;
			if (rand() % 2 == 0)
			{
				sign_y = 1;
			}

			int offset_x = sign_x*(rand() % 70 + 50);
			int offset_y = sign_y*(rand() % 70 + 50);

			UnitManager::instance()->CreateUnit("Robber02", "SpellHome_01", 0, 100, 70, mPos[0] + offset_x, mPos[1] + offset_y);

			int ii;
			for (ii = 0; ii < Character::all_units.size(); ii++)
			{
				if (Character::all_units_map[Character::all_units[ii].GetFnCharacterID()] != &Character::all_units[ii])
				{
					Character::all_units_map[Character::all_units[ii].GetFnCharacterID()] = &Character::all_units[ii];
				}
			}

			numOfUnitsCreated++;

			frameCount = 0;
		}
	}
};