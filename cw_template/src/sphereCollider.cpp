#include "sphereCollider.h"
#include "sceneManager.h"
#include "Particle.h"

using namespace glm;

SphereCollider::SphereCollider(Atom &a) : parentAtom(&a)
{
	// set default radius for atom sphere collider to be 0.1
	radius = 0.1;
}

SphereCollider::SphereCollider(Particle &p) : parentParticle(&p)
{
	// set particle radius
	radius = p.get_radius();
}

// default constructor
SphereCollider::SphereCollider() {}

dvec3 SphereCollider::get_parent_pos()
{
	dvec3 pos;

	if (parentAtom)
	{
		pos = parentAtom->position;
	}
	else if (parentParticle)
	{
		pos = parentParticle->get_position();
	}

	return pos;
}

static int count = 0;

bool SphereCollider::is_colliding(SphereCollider *s, CollisionInfo &col)// dvec3 &pos, dvec3 &norm, double &depth)
{
	count++;
	// takes in collision data to change and sphere to check against
	
	dvec3 thisSphereP = this->get_parent_pos();

	//std::cout << thisSphereP.x << " " << thisSphereP.y << " " << thisSphereP.z << std::endl;
	dvec3 collidingSphereP = s->get_parent_pos();

	// distance between points
	dvec3 edgeDir = thisSphereP - collidingSphereP;
	double distance = length(edgeDir);

	// if distance is less than the radius combined collision is true
	double radii = (this->radius + s->radius);

	if (distance <= radii)
	{
		col.depth = radii - distance;
		col.normal = -normalize(edgeDir);
		col.position = thisSphereP - col.normal * (radius - col.depth * 0.5f);
		return true;
	}


	return false;
}