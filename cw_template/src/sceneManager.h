#pragma once
#include <glm/glm.hpp>
#include <graphics_framework.h>

using namespace glm;
using namespace graphics_framework;

struct Atom
{
	dvec3 position;
	dvec3 normal;
};

class SceneManager {
private:
	Atom atomlist [8];

	free_camera cam;
	effect phong;
	mat4 PV;
	directional_light light;
	material mat;

	// buffer for render
	GLuint atom_buffer;
	GLuint m_vao;
	void Init_Mesh();
	

public:
	void Init();
	void  renderParticles();
	void Update(double delta_time);
	void SetCameraPos(const glm::vec3 &p0);
	void rendershit();
};