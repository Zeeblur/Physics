#pragma once

#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "sphereCollider.h"

using namespace glm;
using namespace graphics_framework;


class Particle
{
private:
	dvec3 position;
	dvec3 prev_position;
	dvec3 force;
	dvec3 velocity;

	double radius;  // bounding sphere

	geometry pgeom;
	material pmat;

	SphereCollider collider;

public:
	Particle(const dvec3 &pos, const double &rad, const dvec4 &col);
	void render_particle(const effect &phong, const mat4 &projview, const directional_light &light);
	
	// getters and setters for positions
	dvec3 get_position();
	dvec3 get_prev_position();
	void set_prev_pos(const dvec3 &pos);
	void set_pos(const dvec3 &pos);
	double get_radius();

	dvec3 get_velocity();
	void set_velocity(const dvec3 &vel);

	// add and clear forces
	void add_impulse(const dvec3 &impulse);
	void clear_forces();

	void set_collider(SphereCollider col);
	SphereCollider* get_collider();
	
};