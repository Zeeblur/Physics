#define GLM_ENABLE_EXPERIMENTAL
#include "springPhys.h"
#include <glm/glm.hpp>
#include "sceneManager.h"

using namespace std;
using namespace glm;

// constructor for spring
SpringPhys::SpringPhys(Atom &a, Atom &b) : particleFrom(&a), particleTo(&b)
{
	restLength = 1.0f;
	springConstant = 10.0f;
}

void SpringPhys::CalculateForce()
{
	// dist between particles
	dvec3 edgeDir = particleFrom->position - particleTo->position;

	// get extension magnitude 
	double extension = length(edgeDir) - restLength;

	edgeDir = normalize(edgeDir);

	// f = -kx
	dvec3 force = -springConstant * extension * edgeDir;

	// particleto.addfram

	particleFrom->force += force;
	particleTo->force -= force;

}
