#pragma once
#include "core.hpp"
#include "ForceGenerator.h"

class SpringForceGenerator : public ForceGenerator {
public:

	SpringForceGenerator(double k, double resting_lenght, Particle* other);
	SpringForceGenerator(double k, double resting_lenght, Vector3 v);
	virtual void updateForce(Particle* p, double duration);
	inline void setK(double k) {
		k_ = k;
	}

protected:
	double k_;
	double resting_lenght_;
	Particle* other_;
};