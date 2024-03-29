#define GLM_ENABLE_EXPERIMENTAL
#include "springPhys.h"
#include <glm/glm.hpp>
#include "sceneManager.h"

using namespace std;
using namespace glm;

// constructor for spring
SpringPhys::SpringPhys(Atom &a, Atom &b) : particleFrom(&a), particleTo(&b)
{
	restLength = 0.7f;
	springConstant = 100.0f;
	dampingConstant = 10.0f;
}

void SpringPhys::CalculateForce()
{
	// dist between particles
	dvec3 edgeDir = particleFrom->position - particleTo->position;

	// get extension magnitude 
	double extension = (length(edgeDir) - restLength);

	edgeDir = normalize(edgeDir);

	// f = -kx
	dvec3 force = -springConstant * extension * edgeDir;

	// need to account for damping force

	// analytical approach to damping force (find closing velocity)
	// edgedir dot v1 - egdedir dot v2

	double closingVel = dot(particleFrom->velocity, edgeDir) - dot(particleTo->velocity, edgeDir);
	
	// damping force is constant * velocity
	dvec3 dampingforce = -closingVel * dampingConstant * edgeDir;

	// add force to particles
	particleFrom->force += force +dampingforce;
	particleTo->force -= force +dampingforce;

} 
