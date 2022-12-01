#include "WhirlwindGenerator.h"

WhirlwindGenerator::WhirlwindGenerator(Vector3 v, Vector3 centre, float k, int c): WindGenerator(k, k, v,  0, centre), centre(centre), k(k), c(c)
{
	setName("Torbellino");
}

void WhirlwindGenerator::updateForce(Particle* p, double duration)
{
	if (fabs(p->getInverseMass()) < t || !isActive()) return;
	calculateVelocityWind(p->getPosition());
	//WindGenerator::updateForce(p, duration);
	Vector3 v = p->getVelocity() - vel;
	p->addForce(calculateDrag(v));
}

void WhirlwindGenerator::calculateVelocityWind(Vector3 pos)
{
	vel.x = k * (-(pos.z - centre.z));
	vel.y = k * (c - (pos.y - centre.y));
	vel.z = k * (pos.x - centre.x);
}
