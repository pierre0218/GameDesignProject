#include "Character.h"
#include "SceneManager.h"
//#include "MathHelper.h"

Character::Character(char* modelName, int p)
{
	mModelName = modelName;
	party = p;
}

Character::Character(char* modelName, char* fxName, int p, int hp, float atkDistance)
{
	mModelName = modelName;
	mFXName = fxName;
	party = p;
	HP = hp;
	attackDistance = atkDistance;
}

void Character::Start()
{
	targetPos = new float[3];
	initPos = new float[3];
	fsm = new FSMSystem();
	idleID = 0;
	enemyTarget = 0;

	//HP = 100;
	// load the character
	FySetModelPath("Data\\NTU6\\Characters");
	FySetTexturePath("Data\\NTU6\\Characters");
	FySetCharacterPath("Data\\NTU6\\Characters");
	FySetGameFXPath("Data\\NTU6\\FX0");

	actorID = SceneManager::instance()->GetScene().LoadCharacter(mModelName);

	mCharacter.ID(actorID);

	dummyID = SceneManager::instance()->GetScene().CreateObject(MODEL);
	dummy2ID = SceneManager::instance()->GetScene().CreateObject(MODEL);
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

	if (atkFXID != NULL && playAtkFX) {
		atkFxFrameCount++;
		//SceneManager::instance()->GetScene().DeleteGameFXSystem(gFXID);
		FnGameFXSystem gxS(atkFXID);
		gxS.Play((float)skip, LOOP);

	}

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
	if (gFXID == NULL) {
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
	}
	else
	{
		FnGameFXSystem gxS(gFXID);
		gxS.SetParentObjectForAll(dummyID);// .SetPlayLocation(pos);
		gxS.PlayFrame(30);
	}
}

void Character::UnSelected()
{
	if (gFXID != NULL) {
		//SceneManager::instance()->GetScene().DeleteGameFXSystem(gFXID);
		FnGameFXSystem gxS(gFXID);
		gxS.SetParentObjectForAll(dummy2ID);// .SetPlayLocation(pos);
		gxS.PlayFrame(30);

	}
}

void Character::RemoveAttackFX()
{
	if (atkFXID != NULL) {
		FnGameFXSystem gxS(atkFXID);
		gxS.SetParentObjectForAll(dummy2ID);// .SetPlayLocation(pos);
		gxS.Pause();
		SceneManager::instance()->GetScene().DeleteGameFXSystem(atkFXID);
	}
}

void Character::PlayAttackFX(float* pos)
{
	if (atkFXID == NULL) {
		//SceneManager::instance()->GetScene().DeleteGameFXSystem(gFXID);
		// create a new game FX system
		atkFXID = SceneManager::instance()->GetScene().CreateGameFXSystem();

		FnGameFXSystem gxS(atkFXID);

		// play the FX on it
		BOOL4 beOK = gxS.Load(mFXName, TRUE);
		if (beOK) {
			gxS.SetPlayLocation(pos);
		}
	}
	else
	{
		FnGameFXSystem gxS(atkFXID);
		gxS.SetParentObjectForAll(dummyID);// .SetPlayLocation(pos);
	}

	playAtkFX = true;
}

void Character::StopAttackFX()
{
	if (atkFXID != NULL) {
		//SceneManager::instance()->GetScene().DeleteGameFXSystem(gFXID);
		FnGameFXSystem gxS(atkFXID);
		gxS.SetParentObjectForAll(dummy2ID);// .SetPlayLocation(pos);
		gxS.Pause();

	}

	playAtkFX = false;
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