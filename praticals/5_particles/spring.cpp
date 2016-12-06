#include "spring.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

// assign spring connections
Spring::Spring(Entity *from, Entity *to) : Component("Spring"), particleFrom(from), particleTo(to) {}

Spring::~Spring() {}

void Spring::CalculateForce()
{

}