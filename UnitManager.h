#include <vector>
#include "FlyWin32.h"
#include "Character.h"

using namespace std;

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

	vector<Character> all_units;

	void Start();
	void Update(int skip);
	void CreateUnit(char* model, float posX, float posY);
	void RemoveUnit(CHARACTERid character);

	int CheckMouseHit(float* worldPos);
};

