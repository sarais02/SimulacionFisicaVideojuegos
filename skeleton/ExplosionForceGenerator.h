#pragma once
#include "ForceGenerator.h"
#include <iostream>
#include <cmath>
using namespace std;

class ExplosionForceGenerator :public ForceGenerator {
public:
	ExplosionForceGenerator(const double K, int R, int c, Vector3 centre);
	virtual void updateForce(Particle* p, double duration);

protected:
	Vector3 vel;
	Vector3 centre;
	double K;
	int R;
	int c;
};