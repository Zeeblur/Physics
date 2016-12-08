#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <graphics_framework.h>
#include <phys_utils.h>

#include "game.h"
#include "spring.h"
#include "physics.h"
#include "sceneManager.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;
#define physics_tick 1.0 / 60.0


SceneManager myMan;

static vector<unique_ptr<Entity>> SceneList;
static unique_ptr<Entity> floorEnt;
static unique_ptr<Entity> springEnt;



unique_ptr<Entity> CreateParticle()
{
	unique_ptr<Entity> ent(new Entity());
	ent->SetPosition(vec3(0, 5.0 + (double)(rand() % 200) / 20.0, 0));
	unique_ptr<Component> physComponent(new cPhysics());
	unique_ptr<cShapeRenderer> renderComponent(new cShapeRenderer(cShapeRenderer::SPHERE));
	renderComponent->SetColour(phys::RandomColour());
	ent->AddComponent(physComponent);
	ent->AddComponent(unique_ptr<Component>(new cSphereCollider()));
	ent->AddComponent(unique_ptr<Component>(move(renderComponent)));
	return ent;
}

bool load_content()
{
	//phys::Init();
	
	myMan = SceneManager();
	myMan.Init();

	// create particle add to list
	SceneList.push_back(CreateParticle());
	SceneList.push_back(CreateParticle());

	// create floor ent and add as plane collider
	floorEnt = unique_ptr<Entity>(new Entity());
	floorEnt->AddComponent(unique_ptr<Component>(new cPlaneCollider()));

	springEnt = unique_ptr<Entity>(new Entity());
	//springEnt->AddComponent(unique_ptr<Component>(new Spring(uSceneList[0], SceneList[1])));

	return true;
}

bool update(float delta_time)
{

	static double t = 0.0;
	static double accumulator = 0.0;
	accumulator += delta_time;

	while (accumulator > physics_tick)
	{
		UpdatePhysics(t, physics_tick);
		myMan.update_grid(t, physics_tick);
		accumulator -= physics_tick;
		t += physics_tick;
	}

	// update all objects in scene
	for (auto &e : SceneList)
	{
		e->Update(delta_time);
	}

	// rotate camera over time
	static float rot = 0.0f;
	rot += 0.2f * delta_time;

	// update camera positions
	//myMan.SetCameraPos(rotate(vec3(15.0f, 12.0f, 15.0f), rot, vec3(0, 1.0f, 0)));
	myMan.Update(delta_time);

	return true;
}


bool render()
{
	myMan.render_floor();

	myMan.renderParticles();

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
}