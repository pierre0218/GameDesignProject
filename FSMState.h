#include <map>

using namespace std;

enum Transition
{
	NullTransition = 0 // Use this transition to represent a non-existing transition in your system
};

enum StateID
{
	NullStateID = 0, // Use this ID to represent a non-existing State in your system	
};

class FSMState
{
protected:
	map<Transition, StateID> transitionMap;

public:
	StateID stateID;

	void AddTransition(Transition trans, StateID id);
	void DeleteTransition(Transition trans);
	StateID GetOutputState(Transition trans);
	void DoBeforeEntering() { };
	void DoBeforeLeaving() { };
	void Reason();
	void Act();
};