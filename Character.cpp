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
	attackState->AddTransition(Transition::GotHit, StateID::GotHitSateID);
	attackState->AddTransition(Transition::GotoWalk, StateID::WalkStateID);
	

	FSMState* chasingState = new ChasingState(*this);
	chasingState->AddTransition(Transition::GotoIdle, StateID::IdleStateID);
	chasingState->AddTransition(Transition::GotoAttack, StateID::AttackStateID);
	chasingState->AddTransition(Transition::GotoWalk, StateID::WalkStateID);

	FSMState* gotHitState = new GotHitState(*this);
	gotHitState->AddTransition(Transition::GotoAttack, StateID::AttackStateID);
	gotHitState->AddTransition(Transition::Die, StateID::DieStateID);

	FSMState* dieState = new DieState(*this);

	fsm->AddState(idleState);
	fsm->AddState(walkingState);
	fsm->AddState(attackState);
	fsm->AddState(chasingState);
	fsm->AddState(gotHitState);
	fsm->AddState(dieState);
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
		//mCharacter.Play(ONCE, (float)skip, FALSE, TRUE);
		gotHitTimer -= 1;
		if (gotHitTimer <= 0)
		{
			gotHitTimer = 0;
			gotHit = false;
			//mCharacter.SetCurrentAction(NULL, 0, idleID, 5.0f);
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


void Character::GetHit(int damage)
{
	HP -= damage;
	if (HP <= 0)
	{
		HP = 0;
		if (!dead)
		{
			//mCharacter.SetCurrentAction(NULL, 0, dieID, 5.0f);
			dead = true;
		}
	}
	else
	{
		//mCharacter.SetCurrentAction(NULL, 0, damage2ID, 5.0f);
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