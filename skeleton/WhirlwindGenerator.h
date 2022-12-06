#pragma once
#include "WindGenerator.h"

class WhirlwindGenerator :public WindGenerator {
public:
	WhirlwindGenerator(Vector3 v, Vector3 centre, float k, int c = 50);
	virtual void updateForce(Particle* p, double duration);
	virtual void updateForce(PxRigidDynamic* p, double duration);
	void calculateVelocityWind(Vector3 pos);

protected:
	Vector3 centre;
	float k;
	int c;
};