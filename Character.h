#include "FlyWin32.h"
#include "FSMSystem.h"
#include "MathHelper.h"

class Character
{
	
	CHARACTERid actorID;

	GAMEFX_SYSTEMid gFXID = FAILED_ID;

	GAMEFX_SYSTEMid atkFXID = FAILED_ID;
	
	char* mModelName;
	char* mFXName;
	FnCharacter mCharacter;

	void MakeFSM();
public:
	Character(){};
	Character(char* modelName, int party);
	Character(char* modelName, char* fxName, int party, int HP, float attackDistance);
	ACTIONid idleID, attackID, runID, damage1ID, damage2ID, dieID;
	OBJECTid dummyID = FAILED_ID;
	OBJECTid dummy2ID = FAILED_ID;
	int HP = 0;
	int party;
	bool isWalking = false;
	bool dead;
	float* targetPos;
	float* initPos;
	FSMSystem* fsm;
	CHARACTERid enemyTarget;

	float chasingDistance = 200;
	float attackDistance = 70;

	bool gotHit;
	int gotHitTimer;
	int lastAttackFrame;

	int atkFxFrameCount = 0;

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

	void SetEnemyIdx(CHARACTERid id)
	{
		enemyTarget = id;
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

	void SetGotHitAnimation()
	{
		damage1ID = mCharacter.GetBodyAction(NULL, "Damage1");
		mCharacter.SetCurrentAction(NULL, 0, damage1ID, 5.0f);
	}

	void SetDieAnimation()
	{
		dieID = mCharacter.GetBodyAction(NULL, "Die");
		mCharacter.SetCurrentAction(NULL, 0, dieID, 5.0f);
	}

	bool CheckMouseHit(float* worldPos);

	void Selected();
	void UnSelected();

	void PlayAttackFX(float* pos);
	void StopAttackFX();

	void RemoveAttackFX();
	bool playAtkFX = false;

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
	void DoBeforeEntering()
	{
		//mCharacter = *Character::all_units_map[mCharacterid];
		Character::all_units_map[mCharacterid]->SetIdleAnimation();

	};
	void Reason(int skip)
	{

		for (std::map<CHARACTERid, Character*>::iterator it = Character::all_units_map.begin(); it != Character::all_units_map.end(); ++it)
		{
			if (Character::all_units_map[mCharacterid]->GetFnCharacterID() == it->second->GetFnCharacterID())
				continue;

			if (Character::all_units_map[mCharacterid]->party != it->second->party)
			{
				float playerPos[3], enemyPos[3];
				Character::all_units_map[mCharacterid]->GetFnCharacter().GetPosition(playerPos);
				it->second->GetFnCharacter().GetPosition(enemyPos);

				if (MathHelper::VectorDistance(playerPos, enemyPos) < Character::all_units_map[mCharacterid]->chasingDistance)
				{
					if (!it->second->dead)
					{
						Character::all_units_map[mCharacterid]->SetEnemyIdx(it->first);

						if (Character::all_units_map[mCharacterid]->fsm->currentState->stateID != StateID::ChasingStateID)
						{
							Character::all_units_map[mCharacterid]->SetTransition(Transition::GotoChasing);
						}

						break;
					}
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
	void Reason(int skip)
	{

	};
	void Act(int skip)
	{
		Character::all_units_map[mCharacterid]->GetFnCharacter().Play(LOOP, (float)skip, FALSE, TRUE);

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
		Character::all_units_map[Character::all_units_map[mCharacterid]->enemyTarget]->GetFnCharacter().GetPosition(enemyPos);

		float distanceX = enemyPos[0] - Character::all_units_map[mCharacterid]->initPos[0];
		float distanceY = enemyPos[1] - Character::all_units_map[mCharacterid]->initPos[1];

		float norm_sqr = distanceX*distanceX + distanceY*distanceY;
		float norm = sqrt(norm_sqr);

		playerDir[0] = distanceX;
		playerDir[1] = distanceY;
		playerDir[2] = fDir[2];
		if (MathHelper::Abs(enemyPos[0] - currentPos[0]) > Character::all_units_map[mCharacterid]->chasingDistance || MathHelper::Abs(enemyPos[1] - currentPos[1]) > Character::all_units_map[mCharacterid]->chasingDistance)
		{
			ACTIONid currentActid = Character::all_units_map[mCharacterid]->GetFnCharacter().GetCurrentAction(NULL);
			if (currentActid != Character::all_units_map[mCharacterid]->idleID)
			{
				Character::all_units_map[mCharacterid]->SetIdleAnimation();

				Character::all_units_map[mCharacterid]->GotoIdleState();
			}
		}
		else if (MathHelper::Abs(enemyPos[0] - currentPos[0]) > Character::all_units_map[mCharacterid]->attackDistance || MathHelper::Abs(enemyPos[1] - currentPos[1]) > Character::all_units_map[mCharacterid]->attackDistance)
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
	int frameCounter;
public:
	AttackState(Character& c)
	{
		mCharacterid = c.GetFnCharacterID();
		stateID = StateID::AttackStateID;
	};
	void DoBeforeEntering()
	{
		Character::all_units_map[mCharacterid]->SetAttackAnimation();
		frameCounter = Character::all_units_map[mCharacterid]->lastAttackFrame;

		float enemyPos[3];
		Character::all_units_map[Character::all_units_map[mCharacterid]->enemyTarget]->GetFnCharacter().GetPosition(enemyPos);


		Character::all_units_map[mCharacterid]->PlayAttackFX(enemyPos);
	}

	void DoBeforeLeaving()
	{
		Character::all_units_map[mCharacterid]->StopAttackFX();
	}
	void Reason(int skip)
	{
		
	};
	void Act(int skip)
	{
		frameCounter++;
		Character::all_units_map[mCharacterid]->lastAttackFrame = frameCounter;

		float currentPos[3], playerDir[3], fDir[3], upDir[3];
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetPosition(currentPos);
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetDirection(fDir, upDir);

		float enemyPos[3];
		Character::all_units_map[Character::all_units_map[mCharacterid]->enemyTarget]->GetFnCharacter().GetPosition(enemyPos);

		if (MathHelper::Abs(enemyPos[0] - currentPos[0]) > Character::all_units_map[mCharacterid]->attackDistance + 10 || MathHelper::Abs(enemyPos[1] - currentPos[1]) > Character::all_units_map[mCharacterid]->attackDistance + 10)
		{
			Character::all_units_map[mCharacterid]->SetTransition(Transition::GotoChasing);
		}
		else
		{
			fDir[0] = enemyPos[0] - currentPos[0];
			fDir[1] = enemyPos[1] - currentPos[1];

			Character::all_units_map[mCharacterid]->GetFnCharacter().SetDirection(fDir, upDir);

			Character::all_units_map[mCharacterid]->GetFnCharacter().Play(LOOP, (float)skip, FALSE, TRUE);

			FnObject dummy(Character::all_units_map[mCharacterid]->dummyID);
			dummy.SetPosition(currentPos);

			if (frameCounter % 60 == 0)
			{
				int enemyFrame = Character::all_units_map[Character::all_units_map[mCharacterid]->enemyTarget]->lastAttackFrame;
				Character::all_units_map[Character::all_units_map[mCharacterid]->enemyTarget]->HP -= 20;

				if (Character::all_units_map[Character::all_units_map[mCharacterid]->enemyTarget]->dead)
				{
					Character::all_units_map[mCharacterid]->GotoIdleState();
				}
				else
				{
					Character::all_units_map[Character::all_units_map[mCharacterid]->enemyTarget]->SetTransition(Transition::GotHit);
				}
				frameCounter = 0;
			}
		}
	};
};

class GotHitState : public FSMState
{
	CHARACTERid mCharacterid;
	int frameCounter;
public:
	GotHitState(Character& c)
	{
		mCharacterid = c.GetFnCharacterID();
		stateID = StateID::GotHitSateID;
	};
	void DoBeforeEntering()
	{
		if (Character::all_units_map[mCharacterid]->HP <= 0)
		{
			Character::all_units_map[mCharacterid]->SetTransition(Transition::Die);
		}
		else
		{
			Character::all_units_map[mCharacterid]->SetGotHitAnimation();
			frameCounter = 0;
		}
	}
	void Reason(int skip)
	{

	};
	void Act(int skip)
	{
		frameCounter++;

		float currentPos[3], playerDir[3], fDir[3], upDir[3];
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetPosition(currentPos);
		Character::all_units_map[mCharacterid]->GetFnCharacter().GetDirection(fDir, upDir);

		Character::all_units_map[mCharacterid]->GetFnCharacter().Play(LOOP, (float)skip, FALSE, TRUE);

		FnObject dummy(Character::all_units_map[mCharacterid]->dummyID);
		dummy.SetPosition(currentPos);

		if (frameCounter % 30 == 0)
		{
			Character::all_units_map[mCharacterid]->SetTransition(Transition::GotoAttack);
		}
	};
};

class DieState : public FSMState
{
	CHARACTERid mCharacterid;

public:
	DieState(Character& c)
	{
		mCharacterid = c.GetFnCharacterID();
		stateID = StateID::DieStateID;
	};
	void DoBeforeEntering()
	{
		Character::all_units_map[mCharacterid]->SetDieAnimation();
		Character::all_units_map[mCharacterid]->RemoveAttackFX();
	}
	void Reason(int skip)
	{

	};
	void Act(int skip)
	{

		Character::all_units_map[mCharacterid]->GetFnCharacter().Play(ONCE, (float)skip, FALSE, TRUE);
		Character::all_units_map[mCharacterid]->dead = true;
	};
};