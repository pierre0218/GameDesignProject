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

void CameraManager::Render()
{
	// render the whole scene
	vp.Render3D(cID, TRUE, TRUE);

	// render the sprites
	vp.RenderSprites(SceneManager::instance()->GetUISceneID(), FALSE, TRUE);  // no clear the background but clear the z buffer
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
	/*float viewMatrix[16], projectionMatrix[16];
	camera.GetViewingMatrix(viewMatrix);
	camera.GetProjectionMatrix(projectionMatrix);

	float pvMatrix[16], invertPVMatrix[16];
	
	MathHelper::Matrix16Multiply(projectionMatrix, viewMatrix, pvMatrix);
	MathHelper::InvertMatrix16(pvMatrix, invertPVMatrix);

	float in[4];
	float winZ = 1.0;


	in[0] = (2.0f*((float)(screenPos[0] - 0) / (screenW - 0))) - 1.0f,
	in[1] = (2.0f*((float)(screenPos[1] - 0) / (screenH - 0))) - 1.0f;
	in[2] = 2.0* winZ - 1.0;
	in[3] = 1.0;

	MathHelper::Matrix16MultiplyVector(invertPVMatrix, in, worldPos);

	worldPos[3] = 1.0 / worldPos[3];

	worldPos[0] *= worldPos[3];
	worldPos[1] *= worldPos[3];
	worldPos[2] *= worldPos[3];

	float camPos[3];
	camera.GetPosition(camPos);

	worldPos[0] = camPos[0] + camPos[0] * worldPos[0];
	worldPos[1] = camPos[1] + camPos[1] * worldPos[1];*/

	GEOMETRYid hitPoint = vp.HitPosition(SceneManager::instance()->GetTerrainID(), cID, (int)(screenPos[0]), (int)(screenPos[1]), worldPos);
}