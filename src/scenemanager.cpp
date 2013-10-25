#include "scenemanager.h"

#include <render_framework\render_framework.h>
#include <iostream>
#include <GLM\glm.hpp>

#include "cameramanager.h"
#include "contentmanager.h"

using namespace std;
using namespace render_framework;
using namespace glm;

/*
* Initializes the scene manager
*/
bool SceneManager::initialize()
{

	// Initialize the renderer
	if (!renderer::get_instance().initialise()) {
		printf("Renderer failed to initialize.");
		return false;
	}

	// Load Camera manager
	if (!CameraManager::get_instance().initialize()) {
		printf("Camera manager failed to initialize.");
		return false;
	}

	// Load Content manager
	if (!ContentManager::get_instance().initialize()) {
		printf("Content manager failed to initialize.");
		return false;
	}

	// Set Scene Clear colour to cyan
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	// Create the projection matrix
	// get the aspect ration (Width/height)
	float aspect = (float)renderer::get_instance().get_screen_width()
		/ (float)renderer::get_instance().get_screen_height();

	// Use aspect to create projection matrix
	auto projection = perspective(degrees(quarter_pi<float>()), // FOV
		aspect,						// aspect ratio
		2.414f,						// Near plane
		10000.0f);					// far plane
	// Set the projection matrix
	renderer::get_instance().set_projection(projection);

	// Create the view matrix
	auto view = lookAt(vec3(20.0f, 20.0f, 20.0f),
		vec3(0.0f, 0.0f, 0.0),
		vec3(0.0f, 1.0, 0.0f));
	// Set the view matrix
	renderer::get_instance().set_view(view);

	_running = true;

	return true;
}

/*
* Updates all registers objects in the scene
*/
void SceneManager::updateScene(float deltaTime)
{
	printf("Updating scene.\n");

	CameraManager::get_instance().update(deltaTime);
	ContentManager::get_instance().update(deltaTime);
}


/*
* Render registered objects
*/
void SceneManager::renderScene(float deltaTime)
{
	printf("Rendering.\n");

	if (renderer::get_instance().begin_render())
	{
		int i;
		for (i = 0; i < ContentManager::get_instance().propListSize(); ++i) {
			printf("propList: %d index: %d", ContentManager::get_instance().propListSize(), i);
			shared_ptr<mesh> prop = make_shared<mesh>(ContentManager::get_instance().getPropAt(i));
			renderer::get_instance().render(prop);
		}
	}
	// End the render
	renderer::get_instance().end_render();
}

/*
* Shuts down the SceneManagers
*/
void SceneManager::shutdown()
{
	// Set running to false
	_running = false;
}