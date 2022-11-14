#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g):_gravity(g)
{
	setName("GravityForce");
}

void GravityForceGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getInverseMass()) < t) return;
	p->addForce(_gravity * p->getMass());
}
