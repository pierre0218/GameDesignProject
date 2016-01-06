#include "FlyWin32.h"

class Character
{
	ACTIONid idleID, attackID, runID, damage1ID, damage2ID, dieID;
	CHARACTERid actorID;

	GAMEFX_SYSTEMid gFXID = FAILED_ID;
	OBJECTid dummyID = FAILED_ID;

	bool gotHit;
	int gotHitTimer;
	bool dead;
	char* mModelName;
	FnCharacter mCharacter;

	float targetPos[3];
	float initPos[3];
	void walk();
	bool isWalking = false;

public:
	Character(char* modelName);
	int HP = 0;
	void InitActions(char* idle, char* attack, char* run, char* damage1, char* damage2, char* die);
	void Start();
	void Update(int skip);
	void GetHit(int damage);

	CHARACTERid GetFnCharacterID()
	{
		return actorID;
	}

	FnCharacter GetFnCharacter()
	{
		return mCharacter;
	}

	void SetTargetPos(float* pos)
	{
		if (pos[2] > 0 && pos[2] < 10)
		{
			targetPos[0] = pos[0];
			targetPos[1] = pos[1];
			targetPos[2] = pos[2];

			mCharacter.GetPosition(initPos);
			char* name = mCharacter.GetName();

			mCharacter.SetCurrentAction(NULL, 0, runID, 5.0f);

			isWalking = true;
		}
	}

	bool CheckMouseHit(float* worldPos);

	void Selected();
	void UnSelected();

	bool operator==(Character& rhs);
	bool operator!=(Character& rhs);
	bool operator!=(int rhs);
};