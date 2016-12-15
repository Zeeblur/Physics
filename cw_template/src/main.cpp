#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <graphics_framework.h>
#include "sceneManager.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;
#define physics_tick 1.0 / 60.0

// scenemanager instance
SceneManager myMan;

bool load_content()
{
	// initialises scene
	myMan = SceneManager();

	myMan.Init();
	
	return true;
}

bool update(float delta_time)
{
	// next physics tick
	static double t = 0.0;
	static double accumulator = 0.0;
	accumulator += delta_time;

	while (accumulator > physics_tick)
	{
		// update physics each tick
		myMan.update_physics(t, physics_tick);
		accumulator -= physics_tick;
		t += physics_tick;
	}

	// update camera positions and buffer data
	myMan.Update(delta_time);

	return true;
}

bool render()
{
	// call render on all objects in scene
	myMan.render();

	return true;
}

void main()
{
	// Create application
	app application;
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();

	// delete ptrs avoid mem leaks
	myMan.clean_memory();
}