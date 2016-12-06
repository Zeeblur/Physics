#pragma once
#include "game.h"

class Spring : public Component
{
protected:
	std::unique_ptr<Entity> particleFrom;
	std::unique_ptr<Entity> particleTo;
	double springConstant;
	double dampingConstant;
	double restLength;
public:
	Spring(Entity *from, Entity *to);
	~Spring();

	void CalculateForce();
};