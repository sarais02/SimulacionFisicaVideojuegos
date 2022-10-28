#pragma once

#include "Particle.h"
#include <list>
#include <random>
#include <string>

class ForceGenerator {
public:
	virtual void updateForce(Particle* p, double duration) = 0;
protected:
	std::string _name;
	double t = -1e10;
};