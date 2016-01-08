#include "FlyWin32.h"
#include "FSMSystem.h"
#include "MathHelper.h"

class Character
{
	
	CHARACTERid actorID;

	GAMEFX_SYSTEMid gFXID = FAILED_ID;
	

	bool gotHit;
	int gotHitTimer;
	
	char* mModelName;
	FnCharacter mCharacter;

	void walk();
	void MakeFSM();
public:
	Character(){};
	Character(char* modelName, int party);
	ACTIONid idleID, attackID, runID, damage1ID, damage2ID, dieID;
	OBJECTid dummyID = FAILED_ID;
	int HP = 0;
	int party;
	bool isWalking = false;
	bool dead;
	float* targetPos;
	float* initPos;
	FSMSystem* fsm;
	int enemyTarget;

	static vector<Character> all_units;
	static vector<CHARACTERid> all_unit_ids;
	static map<CHARACTERid, Character*> all_units_map;

	void InitActions(char* idle, char* attack, char* run, char* damage1, char* damage2, char* die);
	void Start();
	void Update(int skip);
	void GetHit(int damage);

	void SetTransition(Transition t) { fsm->PerformTransition(t); }

	void SetParty(int i)
	{
	    party = i;
	}

	void SetEnemyIdx(int idx)
	{
		enemyTarget = idx;
	}

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

			//mCharacter.SetCurrentAction(NULL, 0, runID, 5.0f);

			SetTransition(Transition::GotoWalk);

			isWalking = true;
		}
	}

	void GotoIdleState()
	{
		SetTransition(Transition::GotoIdle);
	}

	void GotoAttackState()
	{
		SetTransition(Transition::GotoAttack);
	}

	void SetIdleAnimation()
	{
		idleID = mCharacter.GetBodyAction(NULL, "Idle");
		mCharacter.SetCurrentAction(NULL, 0, idleID, 5.0f);
	}

	void SetRunAnimation()
	{
		runID = mCharacter.GetBodyAction(NULL, "Run");
		mCharacter.SetCurrentAction(NULL, 0, runID, 5.0f);
	}

	void SetAttackAnimation()
	{
		attackID = mCharacter.GetBodyAction(NULL, "NormalAttack1");
		mCharacter.SetCurrentAction(NULL, 0, attackID, 5.0f);
	}

	bool CheckMouseHit(float* worldPos);

	void Selected();
	void UnSelected();

	bool operator==(Character& rhs);
	bool operator!=(Character& rhs);
	bool operator!=(int rhs);
};

class IdleState : public FSMState
{
	CHARACTERid mCharacterid;
public:
	IdleState(Character& c)
	{ 
		mCharacterid = c.GetFnCharacterID();
		stateID = StateID::IdleStateID;
	};
	void Reason(int skip)
	{
		int ii;
		for (ii = 0; ii < Character::all_units.size(); ii++)
		{
			if (Character::all_units_map[mCharacterid]->GetFnCharacterID() == Character::all_units[ii].GetFnCharacterID())
				continue;

			if (Character::all_units_map[mCharacterid]->party != Character::all_units[ii].party)
			{
				float playerPos[3], enemyPos[3];
				Character::all_units_map[mCharacterid]->GetFnCharacter().GetPosition(playerPos);
				Character::all_units[ii].GetFnCharacter().GetPosition(enemyPos);

				if (MathHelper::VectorDistance(playerPos, enemyPos) < 200)
				{
					Character::all_units_map[mCharacterid]->SetEnemyIdx(ii);

					if (Character::all_units_map[mCharacterid]->fsm->currentState->stateID != StateID::ChasingStateID)
					{
						Character::all_units_map[mCharacterid]->SetTransition(Transition::GotoChasing);
					}

					break;
				}
			}
		}
	};
	void Act(int skip)
	{
		Character::all_units_map[mCharacterid]->GetFnCharacter().Play(LOOP, (float)skip, FALSE, TRUE);
	};
};

class WalkingState : public FSMState
{
	CHARACTERid mCharacterid;
public:
	WalkingState(Character& c)
	{ 
		mCharacterid = c.GetFnCharacterID();
		stateID = StateID::WalkStateID;
	};
	void DoBeforeEntering() 
	{
		//mCharacter = *Character::all_units_map[mCharacterid];
		Character::all_units_map[mCharacterid]->SetRunAnimation();

	};
	void Reason(int skip){};
	void Act(int skip)
	{
		float currentPos[3], playerDir[3], fDir[3], upDir[3];
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetPosition(currentPos);
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetDirection(fDir, upDir);

		float distanceX = Character::all_units_map[mCharacterid]->targetPos[0] - Character::all_units_map[mCharacterid]->initPos[0];
		float distanceY = Character::all_units_map[mCharacterid]->targetPos[1] - Character::all_units_map[mCharacterid]->initPos[1];

		float norm_sqr = distanceX*distanceX + distanceY*distanceY;
		float norm = sqrt(norm_sqr);

		playerDir[0] = distanceX;
		playerDir[1] = distanceY;
		playerDir[2] = fDir[2];

		if (MathHelper::Abs(Character::all_units_map[mCharacterid]->targetPos[0] - currentPos[0]) > 5 || MathHelper::Abs(Character::all_units_map[mCharacterid]->targetPos[1] - currentPos[1]) > 5)
		{
			if (MathHelper::Abs(distanceX) > 0 && MathHelper::Abs(distanceY) > 0)
			{
				currentPos[0] = distanceX * 5 / norm + currentPos[0];
				currentPos[1] = distanceY * 5 / norm + currentPos[1];
			}

			Character::all_units_map[mCharacterid]->GetFnCharacter().SetPosition(currentPos);
			Character::all_units_map[mCharacterid]->GetFnCharacter().SetDirection(playerDir, upDir);
		}
		else
		{
			ACTIONid currentActid = Character::all_units_map[mCharacterid]->GetFnCharacter().GetCurrentAction(NULL);
			if (currentActid != Character::all_units_map[mCharacterid]->idleID)
			{
				Character::all_units_map[mCharacterid]->SetIdleAnimation();

				Character::all_units_map[mCharacterid]->dead = false;
				Character::all_units_map[mCharacterid]->isWalking = false;

				Character::all_units_map[mCharacterid]->GotoIdleState();
			}
		}

		Character::all_units_map[mCharacterid]->GetFnCharacter().Play(LOOP, (float)skip, FALSE, TRUE);

		FnObject dummy(Character::all_units_map[mCharacterid]->dummyID);
		dummy.SetPosition(currentPos);
	};
};

class ChasingState : public FSMState
{
	CHARACTERid mCharacterid;
public:
	ChasingState(Character& c)
	{
		mCharacterid = c.GetFnCharacterID();
		stateID = StateID::ChasingStateID;
	};
	void DoBeforeEntering()
	{
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetPosition(Character::all_units_map[mCharacterid]->initPos);
		Character::all_units_map[mCharacterid]->SetRunAnimation();
	}
	void Reason(int skip){};
	void Act(int skip)
	{
		float currentPos[3], playerDir[3], fDir[3], upDir[3];
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetPosition(currentPos);
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetDirection(fDir, upDir);

		float enemyPos[3];
		Character::all_units[Character::all_units_map[mCharacterid]->enemyTarget].GetFnCharacter().GetPosition(enemyPos);

		float distanceX = enemyPos[0] - Character::all_units_map[mCharacterid]->initPos[0];
		float distanceY = enemyPos[1] - Character::all_units_map[mCharacterid]->initPos[1];

		float norm_sqr = distanceX*distanceX + distanceY*distanceY;
		float norm = sqrt(norm_sqr);

		playerDir[0] = distanceX;
		playerDir[1] = distanceY;
		playerDir[2] = fDir[2];
		if (MathHelper::Abs(enemyPos[0] - currentPos[0]) > 200 || MathHelper::Abs(enemyPos[1] - currentPos[1]) > 200)
		{
			ACTIONid currentActid = Character::all_units_map[mCharacterid]->GetFnCharacter().GetCurrentAction(NULL);
			if (currentActid != Character::all_units_map[mCharacterid]->idleID)
			{
				Character::all_units_map[mCharacterid]->SetIdleAnimation();

				Character::all_units_map[mCharacterid]->GotoIdleState();
			}
		}
		else if (MathHelper::Abs(enemyPos[0] - currentPos[0]) > 70 || MathHelper::Abs(enemyPos[1] - currentPos[1]) > 70)
		{
			if (MathHelper::Abs(distanceX) > 0 && MathHelper::Abs(distanceY) > 0)
			{
				currentPos[0] = distanceX * 2 / norm + currentPos[0];
				currentPos[1] = distanceY * 2 / norm + currentPos[1];
			}

			Character::all_units_map[mCharacterid]->GetFnCharacter().SetPosition(currentPos);
			Character::all_units_map[mCharacterid]->GetFnCharacter().SetDirection(playerDir, upDir);
		}
		else
		{
			ACTIONid currentActid = Character::all_units_map[mCharacterid]->GetFnCharacter().GetCurrentAction(NULL);
			if (currentActid != Character::all_units_map[mCharacterid]->idleID)
			{
				Character::all_units_map[mCharacterid]->GotoAttackState();
			}
		}

		Character::all_units_map[mCharacterid]->GetFnCharacter().Play(LOOP, (float)skip, FALSE, TRUE);

		FnObject dummy(Character::all_units_map[mCharacterid]->dummyID);
		dummy.SetPosition(currentPos);
	};
};

class AttackState : public FSMState
{
	CHARACTERid mCharacterid;
public:
	AttackState(Character& c)
	{
		mCharacterid = c.GetFnCharacterID();
		stateID = StateID::AttackStateID;
	};
	void DoBeforeEntering()
	{
		Character::all_units_map[mCharacterid]->SetAttackAnimation();
	}
	void Reason(int skip)
	{
		int ii;
		for (ii = 0; ii < Character::all_units.size(); ii++)
		{
			if (Character::all_units_map[mCharacterid]->GetFnCharacterID() == Character::all_units[ii].GetFnCharacterID())
				continue;

			if (Character::all_units_map[mCharacterid]->party != Character::all_units[ii].party)
			{
				int a = 0;
			}
		}
	};
	void Act(int skip)
	{
		float currentPos[3], playerDir[3], fDir[3], upDir[3];
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetPosition(currentPos);
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetDirection(fDir, upDir);

		Character::all_units_map[mCharacterid]->GetFnCharacter().Play(LOOP, (float)skip, FALSE, TRUE);

		FnObject dummy(Character::all_units_map[mCharacterid]->dummyID);
		dummy.SetPosition(currentPos);
	};
};