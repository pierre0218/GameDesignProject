#include "CameraManager.h"
#include "SceneManager.h"
#include "MathHelper.h"

void CameraManager::Start()
{
	// create a viewport
	vID = FyCreateViewport(0, 0, 1024, 768);

	vp.ID(vID);

	// translate the camera
	cID = SceneManager::instance()->GetScene().CreateObject(CAMERA);
	
	camera.ID(cID);
	camera.SetNearPlane(5.0f);
	camera.SetFarPlane(100000.0f);

	float pos[3], fDir[3], uDir[3];
	// set camera initial position and orientation
	pos[0] = 4069.0f; pos[1] = -3208.0f; pos[2] = 93.046f;
	fDir[0] = -500.0f; fDir[1] = -0.0f; fDir[2] = -0.0f;
	uDir[0] = -0.116f; uDir[1] = -0.031f; uDir[2] = 0.993f;
	camera.SetPosition(pos);
	camera.SetDirection(fDir, uDir);

	rot_y = -60;
	rot_x = 0;

	CameraFocus();
}

void CameraManager::CameraFocus()
{
	float negativeDistance[3];
	negativeDistance[0] = -distance;
	negativeDistance[1] = 0;
	negativeDistance[2] = 0;

	float targetPos[3];
	targetPos[0] = 3569.0f; targetPos[1] = -3208.0f; targetPos[2] = 0.0f;

	float rotation[4];
	MathHelper::EulerToQuarternion(0, MathHelper::DegToRad(rot_y), MathHelper::DegToRad(rot_x), rotation);

	float forwardDir[3];
	MathHelper::QuaternionMultiVector(rotation, negativeDistance, forwardDir);

	float camPos[3], GlobalRight[3];
	GlobalRight[0] = 0;
	GlobalRight[1] = 1;
	GlobalRight[2] = 0;

	camPos[0] = targetPos[0] - forwardDir[0];
	camPos[1] = targetPos[1] - forwardDir[1];
	camPos[2] = targetPos[2] - forwardDir[2];

	float upDir[3];
	MathHelper::VectorCross(GlobalRight, forwardDir, upDir);

	camera.SetPosition(camPos);
	camera.SetDirection(forwardDir, upDir);
}

void CameraManager::MoveRight2D()
{
	float camPos[3], MoveDir[3];
	MoveDir[0] = 0;
	MoveDir[1] = 1;
	MoveDir[2] = 0;

	camera.GetPosition(camPos);

	camPos[0] = camPos[0] + moveSpeed*MoveDir[0];
	camPos[1] = camPos[1] + moveSpeed*MoveDir[1];
	camPos[2] = camPos[2] + moveSpeed*MoveDir[2];

	camera.SetPosition(camPos);
}

void CameraManager::MoveLeft2D()
{
	float camPos[3], MoveDir[3];
	MoveDir[0] = 0;
	MoveDir[1] = -1;
	MoveDir[2] = 0;

	camera.GetPosition(camPos);

	camPos[0] = camPos[0] + moveSpeed*MoveDir[0];
	camPos[1] = camPos[1] + moveSpeed*MoveDir[1];
	camPos[2] = camPos[2] + moveSpeed*MoveDir[2];

	camera.SetPosition(camPos);
}

void CameraManager::MoveUp2D()
{
	float camPos[3], MoveDir[3];
	MoveDir[0] = -1;
	MoveDir[1] = 0;
	MoveDir[2] = 0;

	camera.GetPosition(camPos);

	camPos[0] = camPos[0] + moveSpeed*MoveDir[0];
	camPos[1] = camPos[1] + moveSpeed*MoveDir[1];
	camPos[2] = camPos[2] + moveSpeed*MoveDir[2];

	camera.SetPosition(camPos);
}

void CameraManager::MoveDown2D()
{
	float camPos[3], MoveDir[3];
	MoveDir[0] = 1;
	MoveDir[1] = 0;
	MoveDir[2] = 0;

	camera.GetPosition(camPos);

	camPos[0] = camPos[0] + moveSpeed*MoveDir[0];
	camPos[1] = camPos[1] + moveSpeed*MoveDir[1];
	camPos[2] = camPos[2] + moveSpeed*MoveDir[2];

	camera.SetPosition(camPos);
}

void CameraManager::Update(int skip)
{

}

void CameraManager::ScreenPointToWorld(float* screenPos, float* worldPos)
{

}