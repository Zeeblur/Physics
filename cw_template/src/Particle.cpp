#include "Particle.h"

Particle::Particle(const dvec3 &pos, const double &rad, const dvec4 &col) : position(pos), radius(rad)
{
	pgeom = geometry_builder::create_sphere();
	pmat = material();

	// set colour
	pmat.set_diffuse(col);
	pmat.set_emissive(dvec4(0));
	pmat.set_shininess(20.0f);
	pmat.set_specular(dvec4(1.0f));

	// set initial prev pos
	prev_position = position;

}

void Particle::render_particle(const effect &phong, const mat4 &projview, const directional_light &light)
{
	renderer::bind(phong);
	auto M = glm::translate(mat4(1.0f), vec3(position)); // T
	M *= glm::scale(mat4(1.0f), vec3(radius));			 // S
	auto MVP = projview * M;
	mat3 N(1.0f);

	renderer::bind(pmat, "mat");
	renderer::bind(light, "light");
	glUniformMatrix4fv(phong.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	glUniformMatrix4fv(phong.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
	glUniformMatrix3fv(phong.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
	renderer::render(pgeom);
}

dvec3 Particle::get_position()
{
	return position;
}

dvec3 Particle::get_prev_position()
{
	return prev_position;
}

void Particle::set_prev_pos(const dvec3 &pos)
{
	prev_position = pos;
}

void Particle::set_pos(const dvec3 &pos)
{
	position = pos;
}

void Particle::add_impulse(const dvec3 &impulse)
{
	force += impulse;
}

void Particle::clear_forces()
{
	force = dvec3(0);
}

double Particle::get_radius()
{
	return radius;
}

SphereCollider* Particle::get_collider()
{
	return &collider;
}

void Particle::set_collider(SphereCollider col)
{
	collider = col;
}