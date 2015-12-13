#include "Character.h"
#include "SceneManager.h"

Character::Character(char* modelName)
{
	mModelName = modelName;
}

void Character::Start()
{
	HP = 100;
	// load the character
	FySetModelPath("Data\\NTU6\\Characters");
	FySetTexturePath("Data\\NTU6\\Characters");
	FySetCharacterPath("Data\\NTU6\\Characters");
	actorID = SceneManager::instance()->GetScene().LoadCharacter(mModelName);

	mCharacter.ID(actorID);
	
}

void Character::InitActions(char* idle, char* attack, char* run, char* damage1, char* damage2, char* die)
{
	idleID = mCharacter.GetBodyAction(NULL, idle);
	runID = mCharacter.GetBodyAction(NULL, run);
	attackID = mCharacter.GetBodyAction(NULL, attack);

	mCharacter.SetCurrentAction(NULL, 0, idleID, 5.0f);
}

void Character::Update(int skip)
{
	if (gotHitTimer > 0 && !dead)
	{
		mCharacter.Play(ONCE, (float)skip, FALSE, TRUE);
		gotHitTimer -= 1;
		if (gotHitTimer <= 0)
		{
			gotHitTimer = 0;
			mCharacter.SetCurrentAction(NULL, 0, idleID, 5.0f);
		}
	}
	else if (dead)
	{
		mCharacter.Play(ONCE, (float)skip, FALSE, TRUE);
	}
	else
	{
		mCharacter.Play(LOOP, (float)skip, FALSE, TRUE);
	}
}

void Character::GetHit(int damage)
{
	HP -= damage;
	if (HP <= 0)
	{
		HP = 0;
		if (!dead)
		{
			mCharacter.SetCurrentAction(NULL, 0, dieID, 5.0f);
			dead = true;
		}
	}
	else
	{
		mCharacter.SetCurrentAction(NULL, 0, damage2ID, 5.0f);
		gotHit = true;
		gotHitTimer = 20;
	}
}