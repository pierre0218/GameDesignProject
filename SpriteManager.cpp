#include "SpriteManager.h"
#include "SceneManager.h"

void SpriteManager::Start()
{

}

void SpriteManager::Update(int skip)
{

}

OBJECTid SpriteManager::CreateSprite(char* sprite, int w, int h, int ox, int oy, int layer)
{
	OBJECTid spID0 = FAILED_ID;  // the sprite
	FnSprite sp;
	spID0 = SceneManager::instance()->GetScene2D().CreateObject(SPRITE);
	sp.ID(spID0);
	sp.SetSize(w, h);
	sp.SetImage(sprite, 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
	sp.SetPosition(ox, oy, layer);

	all_sprites.push_back(spID0);

	return spID0;
}