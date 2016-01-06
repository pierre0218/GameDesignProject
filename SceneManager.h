#include "FlyWin32.h"

class SceneManager
{
	static SceneManager *s_instance;
	SCENEid sID;  // the 3D scene
	SCENEid sID2; // the 2D scene
	OBJECTid tID; //the terrain
	ROOMid terrainRoomID = FAILED_ID;
	FnScene scene;
	FnScene scene2D;
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

	FnScene GetScene2D()
	{
		return scene2D;
	}

	SCENEid GetUISceneID()
	{
		return sID2;
	}

	FnObject GetTerrain()
	{
		return terrain;
	}
	OBJECTid GetTerrainID()
	{
		return tID;
	}
};