#include "GravityForceGenerator.h"

void GravityForceGenerator::updateForce(Particle* p, double t) {
	if (fabs(p->getInverseMass()) < 1e10) return;
		p->addForce(_gravity * p->getMass());
}
