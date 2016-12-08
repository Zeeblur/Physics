#pragma once

#include <glm/glm.hpp>
#include <graphics_framework.h>
#include "springPhys.h"

using namespace glm;
using namespace graphics_framework;

struct Atom
{
	dvec3 position;
	dvec3 normal;
	dvec3 prev_pos;

	dvec3 force;
};

class SceneManager {
private:

	// function to upate these positions.
	Atom atomlist[6][6];

	std::vector<SpringPhys> springs;

	free_camera cam;
	effect phong;
	mat4 PV;
	directional_light light;
	material mat;


	// buffer for render
	GLuint atom_buffer;
	GLuint m_vao;
	GLuint elementbuffer;
	std::vector<unsigned int> indices;
	void Init_Mesh();
	void init_springs();

	unsigned int width;
	unsigned int height;

	float current_x, current_y, initialX, initialY;
	
	bool firstMouse = true;
	void update_camera(float delta_time);

	// updates positions of atoms
	
	// calculate acc from forces for atom
	dvec3 calculate_acceleration(const Atom &a);
public:
	void Init();
	void renderParticles();
	void Update(double delta_time);
	void SetCameraPos(const glm::vec3 &p0);
	void render_floor();
	void generate_indices();
	void update_grid(const double time, const double delta_time);
};
