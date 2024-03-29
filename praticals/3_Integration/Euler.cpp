#include <glm/glm.hpp>
#include "main.h"
using namespace std;
using namespace glm;

static dvec3 gravity = dvec3(0, -10.0, 0);

// Phys Tick
void UpdatePhysics_Euler(const double t, const double dt) {
  
  // for each object in the scene
  for (size_t i = 0; i < balls.size(); i++)
  {
    // *********************************
    // Apply Accleration to Velocity
	  balls[i].velocity += gravity * dt;
    // Apply Velocity to position
	  balls[i].position += balls[i].velocity * dt;
    // *********************************
    
    //super simple bounce function
    if (balls[i].position.y <= 0.0f) {
      balls[i].velocity.y = -balls[i].velocity.y;
    }
  }
}