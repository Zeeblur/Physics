#pragma once

#include <glm/glm.hpp>
#include <graphics_framework.h>
#include "springPhys.h"
#include "gui.h"
#include "Particle.h"
#include "sphereCollider.h"

using namespace glm;
using namespace graphics_framework;

struct Atom
{
	dvec3 position;
	dvec3 normal = dvec3(0.0, 1.0, 0.0); // default normal
	dvec3 prev_pos;

	dvec3 force;

	dvec3 velocity;
	bool constraint;

	SphereCollider collider;

	// default constructor
	Atom(){}

	// constructor
	Atom(const dvec3 &pos, const bool &fixed) : position(pos), constraint(fixed)
	{
		prev_pos = pos;
	}

	void set_collider(const SphereCollider &col)
	{
		collider = col;
	}
};

struct CollisionInfo
{
	dvec3 position;
	dvec3 normal;
	double depth;

	Atom* a;
	Particle* p;
};

class SceneManager {
private:

	// function to upate these positions.
	Atom atomlist[20][20];

	// store springs
	std::vector<SpringPhys> springs;

	// store particles
	std::vector<Particle*> particles;

	// store collision data
	std::vector<CollisionInfo> collisions;

	// vars for rendering
	free_camera cam;
	effect phong;
	effect effG;
	mat4 PV;
	directional_light light;
	material mat;

	// buffer for render
	GLuint atom_buffer;
	GLuint m_vao;
	GLuint elementbuffer;
	std::vector<unsigned int> indices;

	// grid values for mesh
	unsigned int width;
	unsigned int height;

	// positions for the mouse movement
	double current_x, current_y, initialX, initialY;
	
	bool firstMouse = true;
	void update_camera(double delta_time);

	// updates positions of atoms
	
	// calculate acc from forces for atom
	dvec3 calculate_acceleration(const Atom &a);


	bool showgui = false;

	void init_mesh();
	void init_springs();
	void init_particles();
	void render_floor();
	void render_mesh();
	void render_particles();
	void resolve_collison(CollisionInfo &col);
public:
	void Init();
	void render();

	void Update(double delta_time);

	void generate_indices();
	void update_physics(const double time, const double delta_time);

	void reset_ball();
	void add_ball();
	void clear_balls();
	void clean_memory();
};

