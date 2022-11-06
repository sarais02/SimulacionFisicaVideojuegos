#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const Vector3& g):_gravity(g)
{
	setName("GravityForce");
}

void GravityForceGenerator::updateForce(Particle* p, double t) {
	if (fabs(p->getInverseMass()) < 1e-10) return;
	p->addForce(_gravity * p->getMass());
}
