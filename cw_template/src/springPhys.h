#pragma once

struct Atom; // forward declaration of struct

class SpringPhys
{
protected:
	Atom* particleFrom;
	Atom* particleTo;
	double springConstant;
	double dampingConstant;
	double restLength;
public:
	SpringPhys(Atom &a, Atom &b);

	void CalculateForce();
};