#pragma once
#include "ForceGenerator.h"

class BuoyancyForceGenerator :public ForceGenerator {
public:
	BuoyancyForceGenerator(float h, float v, float d);
	virtual ~BuoyancyForceGenerator();

	virtual void updateForce(Particle* p, double t);

protected:
	float height, volume, density, gravity = 9.8;
	Particle* liquid_particle;
};