#include "FlyWin32.h"

class FXManager
{
	static FXManager *s_instance;
	GAMEFX_SYSTEMid gFXID = FAILED_ID;
public:
	static FXManager *instance()
	{
		if (!s_instance)
			s_instance = new FXManager;
		return s_instance;
	}

	void Start();
	void Update(int skip);

	void CreateFX(float* xyz);

};