#include "Cameramanager.h"

#include <render_framework\render_framework.h>
#include <GLM\glm.hpp>

#include "scenemanager.h"
#include "contentmanager.h"
#include "usercontrols.h"

using namespace std;
using namespace render_framework;
using namespace glm;

bool CameraManager::initialize()
{
	/* Set the projection matrix */
	// First get the aspect ratio (width/height)
	float aspect = (float)renderer::get_instance().get_screen_width()
		/ (float)renderer::get_instance().get_screen_height();

	// Set Earth Camera
	arc_ball_camera ecam = arc_ball_camera();
	// Use this to set the camera projection matrix
	ecam.set_projection(quarter_pi<float>(), // FOV
		aspect,              // Aspect ratio
		0.2f,                // Near plane
		1.5e10f);       // Far plane
	// Set the camera properties
	ecam.set_target(vec3(0.0,0.0,0.0));
	ecam.set_distance(20000.0f);
	ecam.set_rotationY(0.336f);
	registerCamera(ecam);

	arc_ball_camera scam = arc_ball_camera();
	// Use this to set the camera projection matrix
	scam.set_projection(quarter_pi<float>(), // FOV
		aspect,              // Aspect ratio
		0.2f,                // Near plane
		1.5e10f);           // Far plane
	// Set the camera properties
	registerCamera(scam);

	arc_ball_camera mcam = arc_ball_camera();
	// Use this to set the camera projection matrix
	mcam.set_projection(quarter_pi<float>(), // FOV
		aspect,              // Aspect ratio
		0.2f,                // Near plane
		1.5e10f);           // Far plane
	// Set the camera properties
	registerCamera(mcam);

	currentCamera = make_shared<arc_ball_camera>(cameras.at(0));

	printf("Camera manager initialized.\n");

	return true;
} // initialize()

// Update cameras
void CameraManager::update(float deltaTime) {
	// Update current camera position
	user_controls.moveCamera(currentCamera, deltaTime);
	currentCamera->update(deltaTime);
} // update()

arc_ball_camera CameraManager::getCameraAtIndex(int index) {
	return cameras.at(index);
} // getCameraAtIndex()

void CameraManager::setRenderCamera(arc_ball_camera cam) {
	currentCamera = make_shared<arc_ball_camera>(cam);
	renderer::get_instance().set_camera(currentCamera);
} // setRenderCamera(

void CameraManager::registerCamera(arc_ball_camera cam) {
	cameras.push_back(cam);
} // registerCamera(

void CameraManager::unregisterCamera(int index) {
	cameras.erase(cameras.begin()+index-1);
} // unregisterCamera()

/*
* Shuts down the CameraManagers
*/
void CameraManager::shutdown()
{
	// Set running to false
	_running = false;
} // shutdown()