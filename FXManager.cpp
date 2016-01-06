#include "FXManager.h"
#include "SceneManager.h"

void FXManager::Start()
{
	FySetGameFXPath("Data\\NTU6\\FX0");

	
}

void FXManager::Update(int skip)
{
	// play game FX
/*	if (gFXID != FAILED_ID) {
		FnGameFXSystem gxS(gFXID);
		BOOL4 beOK = gxS.Play((float)skip, ONCE);
		if (!beOK) {

			SceneManager::instance()->GetScene().DeleteGameFXSystem(gFXID);
			gFXID = FAILED_ID;
		}
	}*/
}

void FXManager::CreateFX(float* xyz)
{
	// remove the old one if necessary
	if (gFXID != NULL) {
		SceneManager::instance()->GetScene().DeleteGameFXSystem(gFXID);
	}

	// create a new game FX system
	gFXID = SceneManager::instance()->GetScene().CreateGameFXSystem();

	FnGameFXSystem gxS(gFXID);

	// play the FX on it
	BOOL4 beOK = gxS.Load("SpellHome_01", TRUE);
	if (beOK) {
		gxS.SetPlayLocation(xyz);
		gxS.PlayFrame(30);
	}
}