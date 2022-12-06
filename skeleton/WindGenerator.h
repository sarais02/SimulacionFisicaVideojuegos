#pragma once
#include "ParticleDragGenerator.h"

class WindGenerator :public ParticleDragGenerator {
public:
	WindGenerator(const float k1, const float k2, Vector3 v, float range=0, Vector3 centre=Vector3(0.0,0.0,0.0));
	virtual void updateForce(Particle* p, double duration);
	virtual void updateForce(PxRigidDynamic* p, double duration);
	void setVelocity(Vector3 v) {
		v = vel;
	}

protected:
	Vector3 vel;
	float rango;
	Vector3 maxPositive, maxNegative;

	bool inRange(Vector3 pos);
};