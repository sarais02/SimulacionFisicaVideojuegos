#pragma once
#include "ForceGenerator.h"
#include <iostream>
#include "core.hpp"

using namespace std;

class ParticleDragGenerator :public ForceGenerator {
public:
	ParticleDragGenerator() {};
	ParticleDragGenerator(const float k1, const float k2);
	virtual void updateForce(Particle* p, double duration);
	virtual void updateForce(PxRigidDynamic* p, double duration);
	inline void setDrag(float k1, float k2) { _k1 = k1; _k2 = k2; }
	inline float getK1() { return _k1; }
	inline float getK2() { return _k2; }
	Vector3 calculateDrag(Vector3 v);

protected:
	float _k1;
	float _k2;
};