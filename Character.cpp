#include "Character.h"
#include "SceneManager.h"
//#include "MathHelper.h"

Character::Character(char* modelName, int p)
{
	mModelName = modelName;
	party = p;
}

void Character::Start()
{
	targetPos = new float[3];
	initPos = new float[3];
	fsm = new FSMSystem();
	idleID = 0;
	enemyTarget = 0;

	HP = 100;
	// load the character
	FySetModelPath("Data\\NTU6\\Characters");
	FySetTexturePath("Data\\NTU6\\Characters");
	FySetCharacterPath("Data\\NTU6\\Characters");
	FySetGameFXPath("Data\\NTU6\\FX0");

	actorID = SceneManager::instance()->GetScene().LoadCharacter(mModelName);

	mCharacter.ID(actorID);

	dummyID = SceneManager::instance()->GetScene().CreateObject(MODEL);
	dead = false;

	MakeFSM();
}

void Character::MakeFSM()
{
	FSMState* idleState = new IdleState(*this);
	idleState->AddTransition(Transition::GotoAttack, StateID::AttackStateID);
	idleState->AddTransition(Transition::GotoWalk, StateID::WalkStateID);
	idleState->AddTransition(Transition::GotoChasing, StateID::ChasingStateID);

	FSMState* walkingState = new WalkingState(*this);
	walkingState->AddTransition(Transition::GotoIdle, StateID::IdleStateID);
	walkingState->AddTransition(Transition::GotoChasing, StateID::ChasingStateID);

	FSMState* attackState = new AttackState(*this);
	attackState->AddTransition(Transition::GotoIdle, StateID::IdleStateID);
	attackState->AddTransition(Transition::GotoChasing, StateID::ChasingStateID);

	FSMState* chasingState = new ChasingState(*this);
	chasingState->AddTransition(Transition::GotoIdle, StateID::IdleStateID);
	chasingState->AddTransition(Transition::GotoAttack, StateID::AttackStateID);
	chasingState->AddTransition(Transition::GotoWalk, StateID::WalkStateID);

	fsm->AddState(idleState);
	fsm->AddState(walkingState);
	fsm->AddState(attackState);
	fsm->AddState(chasingState);
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
	/*if (gotHitTimer > 0 && !dead)
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
	}*/

	//walk();

	fsm->currentState->Reason(skip);
	fsm->currentState->Act(skip);

	float currentPos[3];
	mCharacter.GetPosition(currentPos);

	FnObject dummy(dummyID);
	dummy.SetPosition(currentPos);
}

void Character::walk()
{
	float currentPos[3], playerDir[3], fDir[3], upDir[3];
	mCharacter.GetPosition(currentPos);
	mCharacter.GetDirection(fDir, upDir);

	float distanceX = targetPos[0] - initPos[0];
	float distanceY = targetPos[1] - initPos[1];

	float norm_sqr = distanceX*distanceX + distanceY*distanceY;
	float norm = sqrt(norm_sqr);

	playerDir[0] = distanceX;
	playerDir[1] = distanceY;
	playerDir[2] = fDir[2];

	if (MathHelper::Abs(targetPos[0] - currentPos[0]) > 5 || MathHelper::Abs(targetPos[1] - currentPos[1]) > 5)
	{
		if (MathHelper::Abs(distanceX) > 0 && MathHelper::Abs(distanceY) > 0)
		{
			currentPos[0] = distanceX*5 / norm + currentPos[0];
			currentPos[1] = distanceY*5 / norm + currentPos[1];
		}

		mCharacter.SetPosition(currentPos);
		mCharacter.SetDirection(playerDir, upDir);
	}
	else
	{
		ACTIONid currentActid = mCharacter.GetCurrentAction(NULL);
		if (currentActid != idleID)
		{
			mCharacter.SetCurrentAction(NULL, 0, idleID, 5.0f);
			dead = false;
			isWalking = false;
		}
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

void Character::Selected()
{
	// remove the old one if necessary
	//if (gFXID == NULL) {
		//SceneManager::instance()->GetScene().DeleteGameFXSystem(gFXID);
		// create a new game FX system
		gFXID = SceneManager::instance()->GetScene().CreateGameFXSystem();

		FnGameFXSystem gxS(gFXID);

		float pos[3];
		mCharacter.GetPosition(pos);

		// play the FX on it
		BOOL4 beOK = gxS.Load("SpellHome_01", TRUE);
		if (beOK) {
			gxS.SetParentObjectForAll(dummyID);// .SetPlayLocation(pos);
			gxS.PlayFrame(30);
		}
	//}
}

void Character::UnSelected()
{
	if (gFXID != NULL) {
		SceneManager::instance()->GetScene().DeleteGameFXSystem(gFXID);
	}
}

bool Character::CheckMouseHit(float* worldPos)
{
	float pos[3];
	mCharacter.GetPosition(pos);

	float distance = MathHelper::VectorDistance(worldPos, pos);
	if (distance < 70)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Character::operator==(Character& rhs)
{ 
	return actorID == rhs.GetFnCharacterID();
}
bool Character::operator!=(Character& rhs)
{ 
	return !(actorID == rhs.GetFnCharacterID());
}
bool Character::operator!=(int rhs)
{
	return !(actorID == rhs);
}