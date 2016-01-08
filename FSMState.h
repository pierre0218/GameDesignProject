#include <map>
#include <vector>

using namespace std;

enum Transition
{
	NullTransition = 0, // Use this transition to represent a non-existing transition in your system
	GotoAttack = 1,
	GotoIdle = 2,
	GotoWalk = 3,
	GotoChasing = 4
};

enum StateID
{
	NullStateID = 0, // Use this ID to represent a non-existing State in your system	
	AttackStateID = 1,
	IdleStateID = 2,
	WalkStateID = 3,
	ChasingStateID= 4
};

class FSMState
{
protected:
	map<Transition, StateID>* transitionMap;

public:
	StateID stateID;
	FSMState(){ transitionMap = new map<Transition, StateID>; };
	void AddTransition(Transition trans, StateID id);
	void DeleteTransition(Transition trans);
	StateID GetOutputState(Transition trans);
	virtual void DoBeforeEntering() { };
	virtual void DoBeforeLeaving() { };
	virtual void Reason(int skip){};
	virtual void Act(int skip){};
};