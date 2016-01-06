#include "FlyWin32.h"

class CameraManager
{
	static CameraManager *s_instance;
	VIEWPORTid vID;                 // the major viewport
	OBJECTid cID;              // the main camera
	FnCamera camera;
	FnViewport vp;
	CHARACTERid targetid;
	const float CameraBaseHeight = 300;
	float cameraHeight = CameraBaseHeight;
	//camera control parameters
	float distance = 1500;
	float moveSpeed = 10;
	float rot_x = 0;
	float rot_y = -30;

public:
	static CameraManager *instance()
	{
		if (!s_instance)
			s_instance = new CameraManager;
		return s_instance;
	}

	void Start();
	void Update(int skip);
	void CameraFocus();
	void MoveRight2D();
	void MoveLeft2D();
	void MoveUp2D();
	void MoveDown2D();


	FnCamera GetCamera()
	{
		return camera;
	}
	VIEWPORTid GetViewportID()
	{
		return vID;
	}
	void SetTarget(CHARACTERid tid)
	{
		targetid = tid;
	}
	void Render();
	void ScreenPointToWorld(float* screenPos, float* worldPos);
};