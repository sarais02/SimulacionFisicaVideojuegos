#include "WindGenerator.h"

WindGenerator::WindGenerator(const float k1, const float k2, Vector3 v):ParticleDragGenerator(k1, k2), vel(v)
{
	setName("Viento");
}

void WindGenerator::updateForce(Particle* p, double duration)
{
	if (fabs(p->getInverseMass()) < 1e-10) return;

	Vector3 v= p->getVelocity()-vel;
	p->addForce(calculateDrag(v));
}
