#pragma once
#include <glm/glm.hpp>
#include <vector>

class MatN
{
public:
	std::vector<float> m;

	MatN();
	MatN(const MatN& rhs);

	//MatN(const vec3& row1, const vec3& row2, const vec3& row3);
};