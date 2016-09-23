#include "ik.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>
using namespace std;
using namespace glm;

int numberOfLinks = 0;
mat4 jacobian;

static void calc_jacob(const vec3 &target, std::vector<Link> &const links)
{
	// calculate jacobian for current pos

	// r cross etarget - end pos of link
	for (int i = 0; i < numberOfLinks; i++)
	{
		vec3 linkEndPos = links[i].m_end[3];
		vec3 rotAxis = links[i].m_axis;

		vec3 change = cross(rotAxis, (target - linkEndPos));

		
	}

	// calc pseudo inverse
	// approximate step deltaE
	// compute change in joint deltaTheta 
	// apply change - current angle = previous + change
}

static void ik_jacob_Update(const glm::vec3 &const target, std::vector<Link> &const links, const float linkLength)
{
	numberOfLinks = links.size();

	UpdateHierarchy();
	calc_jacob(target, links);

	// check distance to target
	const float distance = length(vec3(links[links.size() - 1].m_end[3]) - target);
		
	if (distance < 0.5f)
		return;

}