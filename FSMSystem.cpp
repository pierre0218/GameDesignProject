#include "FSMSystem.h"

FSMSystem::FSMSystem()
{

}

void FSMSystem::AddState(FSMState* s)
{
	// First State inserted is also the Initial state,
	//   the state the machine is in when the simulation begins
	if (states.size() == 0)
	{
		states.push_back(s);
		currentState = s;
		currentStateID = s->stateID;
		return;
	}

	// Add the state to the List if it's not inside it
	int ii;
	for (ii = 0; ii < states.size(); ii++)
	{
		if (states[ii]->stateID == s->stateID)
		{
			return;
		}
	}

	states.push_back(s);
}

void FSMSystem::DeleteState(StateID id)
{
	// Check for NullState before deleting
	if (id == StateID::NullStateID)
	{
		return;
	}

	// Search the List and delete the state if it's inside it
	vector<FSMState*>::const_iterator cii;
	for (cii = states.begin(); cii != states.end(); cii++)
	{
		if ((*cii)->stateID == id)
		{
			states.erase(cii);
			return;
		}
	}
}

void FSMSystem::PerformTransition(Transition trans)
{
	// Check for NullTransition before changing the current state
	if (trans == Transition::NullTransition)
	{
		return;
	}

	// Check if the currentState has the transition passed as argument
	StateID id = currentState->GetOutputState(trans);
	if (id == StateID::NullStateID)
	{
		return;
	}

	// Update the currentStateID and currentState		
	currentStateID = id;

	int ii;
	for (ii = 0; ii < states.size(); ii++)
	{
		if (states[ii]->stateID == currentStateID)
		{
			// Do the post processing of the state before setting the new one
			currentState->DoBeforeLeaving();

			currentState = states[ii];

			// Reset the state to its desired condition before it can reason or act
			currentState->DoBeforeEntering();
			break;
		}
	}
}

