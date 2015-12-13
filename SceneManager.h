#include "FlyWin32.h"

class SceneManager
{
	static SceneManager *s_instance;
	SCENEid sID;  // the 3D scene
	OBJECTid tID; //the terrain
	ROOMid terrainRoomID = FAILED_ID;
	FnScene scene;
	FnObject terrain;
public:
	static SceneManager *instance()
	{
		if (!s_instance)
			s_instance = new SceneManager;
		return s_instance;
	}

	void Start();
	void Update(int skip);
	ROOMid GetRoomID()
	{
		return terrainRoomID;
	}

	FnScene GetScene()
	{
		return scene;
	}

	FnObject GetTerrain()
	{
		return terrain;
	}
};