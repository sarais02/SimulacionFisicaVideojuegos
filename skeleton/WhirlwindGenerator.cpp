#include "WhirlwindGenerator.h"

WhirlwindGenerator::WhirlwindGenerator(Vector3 v, Vector3 centre, float k): WindGenerator(k, k, v), centre(centre), k(k)
{
	setName("Torbellino");
}

void WhirlwindGenerator::updateForce(Particle* p, double duration)
{
	if (fabs(p->getInverseMass()) < 1e-10) return;
	calculateVelocityWind(p->getPosition());
	WindGenerator::updateForce(p, duration);
}

void WhirlwindGenerator::calculateVelocityWind(Vector3 pos)
{
	vel.x = k * (-(pos.z - centre.z));
	vel.y = k * (50 - (pos.y - centre.y));
	vel.z = k * (pos.x - centre.x);
}
