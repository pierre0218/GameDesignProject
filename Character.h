#include "FlyWin32.h"

class Character
{
	ACTIONid idleID, attackID, runID, damage1ID, damage2ID, dieID;
	CHARACTERid actorID;
	bool gotHit;
	int gotHitTimer;
	bool dead;
	char* mModelName;
	FnCharacter mCharacter;
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
};