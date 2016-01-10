#include "FlyWin32.h"
#include "Structure.h"

class StructureManager
{
	static StructureManager *s_instance;

public:
	static StructureManager *instance()
	{
		if (!s_instance)
			s_instance = new StructureManager;
		return s_instance;
	}

	void Start()
	{
	
	}
	void Update(int skip)
	{
		int ii;
		for (ii = 0; ii < Structure::all_structures.size(); ii++)
		{
			Structure::all_structures[ii].Update(skip);
		}
	}
	void CreateStructure(char* model, float posX, float posY)
	{
		float pos[3], fDir[3], uDir[3];
		pos[0] = posX; pos[1] = posY; pos[2] = 1000.0f;
		fDir[0] = 1.0f; fDir[1] = -1.0f; fDir[2] = 0.0f;
		uDir[0] = 0.0f; uDir[1] = 0.0f; uDir[2] = 1.0f;

		Structure actor(model);

		actor.Start();

		actor.mCharacter.SetDirection(fDir, uDir);

		actor.mCharacter.SetTerrainRoom(SceneManager::instance()->GetRoomID(), 10.0f);
		BOOL4 beOK = actor.mCharacter.PutOnTerrain(pos);

		actor.InitActions("Idle", "NormalAttack1", "Run", "Damage1", "Damage2", "Die");

		Structure::all_structures.push_back(actor);
		//Structure::all_structures_map[actor.actorID] = &actor;
	}
	
};

