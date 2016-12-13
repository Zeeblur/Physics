#pragma once
#include <glm/glm.hpp>

using namespace glm;

struct Atom;	// forward declare
class Particle;
struct CollisionInfo;

// object attached to each colliding object
class SphereCollider
{
private:
	double radius;
	Atom* parentAtom = nullptr;
	Particle* parentParticle = nullptr;
public:
	SphereCollider();
	SphereCollider(Atom &a);
	SphereCollider(Particle &p);

	dvec3 get_parent_pos();
	bool is_colliding(SphereCollider *s, CollisionInfo &col);// dvec3 &pos, dvec3 &norm, double &depth);
};