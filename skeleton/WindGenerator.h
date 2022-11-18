#pragma once
#include "ParticleDragGenerator.h"

class WindGenerator :public ParticleDragGenerator {
public:
	WindGenerator(const float k1, const float k2, Vector3 v, float range, Vector3 centre);
	virtual void updateForce(Particle* p, double duration);
	void setVelocity(Vector3 v) {
		v = vel;
	}

protected:
	Vector3 vel;
	float rango;
	Vector3 maxPositive, maxNegative;

	bool inRange(Vector3 pos);
};