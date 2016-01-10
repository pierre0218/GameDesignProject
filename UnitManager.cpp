#include "UnitManager.h"
#include "SceneManager.h"
//#include "MathHelper.h"

void UnitManager::Start()
{

}

void UnitManager::Update(int skip)
{
	int ii;
	for (ii = 0; ii < Character::all_units.size(); ii++)
	{
		Character::all_units[ii].Update(skip);
	}
}

CHARACTERid UnitManager::CreateUnit(char* model, char* fxName, int party, int HP, float atkDistance, float posX, float posY)
{
	float pos[3], fDir[3], uDir[3];
	pos[0] = posX; pos[1] = posY; pos[2] = 1000.0f;
	fDir[0] = 1.0f; fDir[1] = -1.0f; fDir[2] = 0.0f;
	uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;

	Character actor(model, fxName, party, HP, atkDistance);

	actor.Start();

	actor.GetFnCharacter().SetDirection(fDir, uDir);

	actor.GetFnCharacter().SetTerrainRoom(SceneManager::instance()->GetRoomID(), 10.0f);
	BOOL4 beOK = actor.GetFnCharacter().PutOnTerrain(pos);

	actor.InitActions("Idle", "NormalAttack1", "Run", "Damage1", "Damage2", "Die");

	Character::all_units.push_back(actor);
	Character::all_unit_ids.push_back(actor.GetFnCharacterID());
	//int size = Character::all_units.size();

	//Character::all_units_map[Character::all_units[size - 1].GetFnCharacterID()] = &Character::all_units[size - 1];
	return actor.GetFnCharacterID();
}

void UnitManager::RemoveUnit(CHARACTERid character)
{

}

CHARACTERid UnitManager::CheckMouseHit(float* worldPos)
{
	/*int ii;
	for (ii = 0; ii < Character::all_units.size(); ii++)
	{
		if (Character::all_units[ii].party != 0)
			continue;

		float pos[3];
		Character::all_units[ii].GetFnCharacter().GetPosition(pos);

		float distance = MathHelper::VectorDistance(worldPos, pos);
		if (distance < 70)
		{
			return ii;
		}
	}

	return -1;*/

	for (std::map<CHARACTERid, Character*>::iterator it = Character::all_units_map.begin(); it != Character::all_units_map.end(); ++it)
	{
		if (it->second->party != 0)
			continue;

		if (it->second->dead == true)
			continue;

		float pos[3];
		it->second->GetFnCharacter().GetPosition(pos);

		float distance = MathHelper::VectorDistance(worldPos, pos);
		if (distance < 70)
		{
			return it->first;
		}
	}

	return NULL;
}

