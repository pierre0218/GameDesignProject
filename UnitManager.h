#include "FlyWin32.h"
#include "Character.h"

class UnitManager
{
	static UnitManager *s_instance;

public:
	static UnitManager *instance()
	{
		if (!s_instance)
			s_instance = new UnitManager;
		return s_instance;
	}

	void Start();
	void Update(int skip);
	CHARACTERid CreateUnit(char* model, char* fxName, int party, int HP, float atkDistance, float posX, float posY);
	void RemoveUnit(CHARACTERid character);

	CHARACTERid CheckMouseHit(float* worldPos);
};

