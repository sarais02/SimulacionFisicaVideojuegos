#pragma once
#include "ForceGenerator.h"
#include <iostream>
#include <cmath>
using namespace std;

class ExplosionForceGenerator :public ForceGenerator {
public:
	ExplosionForceGenerator(const double K, int R, int c, Vector3 centre, double time);
	virtual void updateForce(Particle* p, double duration);
	void OnActive(bool activee) { active = activee; currentTime = 0; };
protected:
	Vector3 vel;
	Vector3 centre;
	double K, currentTime, maxTime;
	const double ve = 343;
	int R;
	int c;
	
};