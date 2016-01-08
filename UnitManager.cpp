#include "UnitManager.h"
#include "SceneManager.h"
//#include "MathHelper.h"

void UnitManager::Start()
{

}

void UnitManager::Update(int skip)
{
	int ii;
	for (ii = 0; ii < all_units.size(); ii++)
	{
		all_units[ii].Update(skip);
	}
}

void UnitManager::CreateUnit(char* model, float posX, float posY)
{
	float pos[3], fDir[3], uDir[3];
	pos[0] = posX; pos[1] = posY; pos[2] = 1000.0f;
	fDir[0] = 1.0f; fDir[1] = -1.0f; fDir[2] = 0.0f;
	uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;

	Character actor(model,0);

	actor.Start();

	actor.GetFnCharacter().SetDirection(fDir, uDir);

	actor.GetFnCharacter().SetTerrainRoom(SceneManager::instance()->GetRoomID(), 10.0f);
	BOOL4 beOK = actor.GetFnCharacter().PutOnTerrain(pos);

	actor.InitActions("Idle", "NormalAttack1", "Run", NULL, NULL, NULL);

	all_units.push_back(actor);
}

void UnitManager::RemoveUnit(CHARACTERid character)
{

}

int UnitManager::CheckMouseHit(float* worldPos)
{
	int ii;
	for (ii = 0; ii < all_units.size(); ii++)
	{
		float pos[3];
		all_units[ii].GetFnCharacter().GetPosition(pos);

		float distance = 0;// MathHelper::VectorDistance(worldPos, pos);
		if (distance < 70)
		{
			return ii;
		}
	}

	return NULL;
}

