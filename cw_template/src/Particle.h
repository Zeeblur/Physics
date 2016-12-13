#pragma once

#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace glm;
using namespace graphics_framework;

class Particle
{
private:
	dvec3 position;
	dvec3 prev_position;
	dvec3 force;

	double radius;  // bounding sphere

	geometry pgeom;
	material pmat;

public:
	Particle(const dvec3 &pos, const double &rad, const dvec4 &col);
	void render_particle(const effect &phong, const mat4 &projview, const directional_light &light);
	
	// getters and setters for positions
	dvec3 get_position();
	dvec3 get_prev_position();
	void set_prev_pos(const dvec3 &pos);
	void set_pos(const dvec3 &pos);

	// add and clear forces
	void add_impulse(const dvec3 &impulse);
	void clear_forces();
};