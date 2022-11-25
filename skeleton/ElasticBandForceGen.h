#pragma once
#include "SpringForceGenerator.h"

class ElasticBandForceGen :public SpringForceGenerator {
public:
	ElasticBandForceGen(double k, double resting_lenght, Particle* p);
	~ElasticBandForceGen() { delete other_; };
	virtual void updateForce(Particle* p, double duration);
};