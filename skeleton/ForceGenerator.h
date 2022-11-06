#pragma once

#include "Particle.h"
#include <list>
#include <random>
#include <string>

class ForceGenerator {
public:
	virtual void updateForce(Particle* p, double duration) = 0;
	virtual void setName(string n) {
		_name = n;
	}
	virtual string getName() {
		return _name;
	}
protected:
	std::string _name;
	double t = -1e10;
};