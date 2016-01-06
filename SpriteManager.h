#include "FlyWin32.h"
#include <vector>

using namespace std;

class SpriteManager
{
	static SpriteManager *s_instance;
private:
	vector<OBJECTid> all_sprites;

public:
	static SpriteManager *instance()
	{
		if (!s_instance)
			s_instance = new SpriteManager;
		return s_instance;
	}

	void Start();
	void Update(int skip);
	OBJECTid CreateSprite(char* sprite, int w, int h, int ox, int oy, int layer);
};