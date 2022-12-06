#pragma once

#include "ForceGenerator.h"
#include "core.hpp"

class GravityForceGenerator: public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& g);
	virtual void updateForce(Particle* p, double t);
	virtual void updateForce(PxRigidDynamic* p, double t);
	inline void setGravity(Vector3 g){
		_gravity = g;
	}
	//~GravityForceGenerator() {};
protected:
	Vector3 _gravity;
};