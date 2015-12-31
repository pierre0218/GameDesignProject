#include "Character.h"
#include "SceneManager.h"
#include "MathHelper.h"

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

	walk();
}

void Character::walk()
{
	float currentPos[3], playerDir[3], fDir[3], upDir[3];
	mCharacter.GetPosition(currentPos);
	mCharacter.GetDirection(fDir, upDir);

	float distanceX = targetPos[0] - initPos[0];
	float distanceY = targetPos[1] - initPos[1];

	playerDir[0] = distanceX;
	playerDir[1] = distanceY;
	playerDir[2] = fDir[2];

	if (MathHelper::Abs(targetPos[0] - currentPos[0]) > 0.1 || MathHelper::Abs(targetPos[1] - currentPos[1]) > 0.1)
	{
		currentPos[0] += distanceX*0.02;
		currentPos[1] += distanceY*0.02;

		mCharacter.SetPosition(currentPos);
		mCharacter.SetDirection(playerDir, upDir);
	}
	else
	{
		mCharacter.SetCurrentAction(NULL, 0, dieID, 5.0f);
		isWalking = false;
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