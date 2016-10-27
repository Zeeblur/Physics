#include "sceneManager.h"
#include <glm/glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace glm;
using namespace graphics_framework;

namespace sm
{
	// store camera and effect instances
	free_camera _cam;
	effect effB;
	effect effG;
	effect effP;
	mat4 PV;
	directional_light light;
	material mat;

	//scenelist
	vector<unique_ptr<Entity>> SceneList;

	// return camera pointer
	camera* GetCam() { return &_cam; }

	// Initialse camera, lighting and effects
	void Init()
	{
		effB = effect();
		effB.add_shader("shaders/phys_basic.vert", GL_VERTEX_SHADER);
		effB.add_shader("shaders/phys_basic.frag", GL_FRAGMENT_SHADER);
		effB.build();
		effP = effect();
		effP.add_shader("shaders/phys_phong.vert", GL_VERTEX_SHADER);
		effP.add_shader("shaders/phys_phong.frag", GL_FRAGMENT_SHADER);
		effP.build();
		effG = effect();
		effG.add_shader("shaders/phys_grid.vert", GL_VERTEX_SHADER);
		effG.add_shader("shaders/phys_grid.frag", GL_FRAGMENT_SHADER);
		effG.build();
		_cam.set_position(vec3(10.0f, 10.0f, 10.0f));
		_cam.set_target(vec3(0.0f, 0.0f, 0.0f));
		auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
		_cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);

		light.set_ambient_intensity(vec4(0.5f, 0.5f, 0.5f, 1.0f));
		light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		light.set_direction(vec3(0.0f, 1.0f, 0.0f));
		mat = material(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), 25.0f);

	}
	
	// update camera
	void Update(double delta_time)
	{
		// update proj/view matrix
		PV = _cam.get_projection() * _cam.get_view();
		_cam.update(static_cast<float>(delta_time));
	}

	unique_ptr<Entity> CreateParticle(const vec3 &pos)
	{
		unique_ptr<Entity> ent(new Entity());
		ent->SetPosition(vec3(0, 5.0 + (double)(rand() % 200) / 20.0, 0));
		//unique_ptr<Component> physComponent(new cPhysics());
		unique_ptr<cShapeRenderer> renderComponent(new cShapeRenderer(cShapeRenderer::SPHERE));
		renderComponent->SetColour(phys::RandomColour());
		//ent->AddComponent(physComponent);
		//ent->AddComponent(unique_ptr<Component>(new cSphereCollider()));
		ent->AddComponent(unique_ptr<Component>(move(renderComponent)));
		return ent;
	}
}