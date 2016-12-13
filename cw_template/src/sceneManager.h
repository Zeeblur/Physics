#pragma once

#include <glm/glm.hpp>
#include <graphics_framework.h>
#include "springPhys.h"
#include "gui.h"
#include "Particle.h"

using namespace glm;
using namespace graphics_framework;

struct Atom
{
	dvec3 position;
	dvec3 normal;
	dvec3 prev_pos;

	dvec3 force;
	bool constraint;
};

class SceneManager {
private:

	// function to upate these positions.
	Atom atomlist[10][10];

	// store springs
	std::vector<SpringPhys> springs;

	// store particles
	std::vector<Particle> particles;

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


	void init_mesh();
	void init_springs();
	void init_particles();
	void render_floor();
	void render_mesh();
	void render_particles();
	bool is_colliding();
public:
	void Init();
	void render();

	void Update(double delta_time);

	void generate_indices();
	void update_physics(const double time, const double delta_time);

};

