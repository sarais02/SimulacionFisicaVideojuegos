#pragma once
#include "ParticleDragGenerator.h"

class WindGenerator :public ParticleDragGenerator {
public:
	WindGenerator(const float k1, const float k2, Vector3 v);
	virtual void updateForce(Particle* p, double duration);
	void setVelocity(Vector3 v) {
		v = vel;
	}

protected:
	Vector3 vel;
};