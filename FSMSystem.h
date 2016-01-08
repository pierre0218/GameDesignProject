#include <vector>
#include "FSMState.h"

using namespace std;

class FSMSystem
{
private:
	vector<FSMState*> states;

protected:

public:
	StateID currentStateID;
	FSMState* currentState;
	FSMSystem();
	void AddState(FSMState* s);
	void DeleteState(StateID id);
	void PerformTransition(Transition trans);
};