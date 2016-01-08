#include "FSMState.h"

void FSMState::AddTransition(Transition trans, StateID id)
{
	// Check if anyone of the args is invalid
	if (trans == Transition::NullTransition)
	{
		return;
	}

	if (id == StateID::NullStateID)
	{
		return;
	}

	// Since this is a Deterministic FSM,
	//   check if the current transition was already inside the map
	if (transitionMap->count(trans))
	{
		return;
	}

	(*transitionMap)[trans] = id;
}

void FSMState::DeleteTransition(Transition trans)
{
	// Check for NullTransition
	if (trans == Transition::NullTransition)
	{
		return;
	}

	// Check if the pair is inside the map before deleting
	if (transitionMap->count(trans))
	{
		transitionMap->erase(trans);
		return;
	}
}

StateID FSMState::GetOutputState(Transition trans)
{
	// Check if the map has this transition
	if (transitionMap->count(trans))
	{
		return (*transitionMap)[trans];
	}
	return StateID::NullStateID;
}
