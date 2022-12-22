#include "WhirlwindGenerator.h"

WhirlwindGenerator::WhirlwindGenerator(Vector3 v, Vector3 centre, float k, float range, int c): WindGenerator(k, k, v,  0, centre), centre(centre), k(k), c(c)
{
	setName("Torbellino");
	maxPositive = { centre.x + range, centre.y + range, centre.z + range };
	maxNegative = { centre.x - range, centre.y - range, centre.z - range };
}

void WhirlwindGenerator::updateForce(Particle* p, double duration)
{
	if (fabs(p->getInverseMass()) < t || !isActive()) return;
	else if (p->isFire() && !inRange(p->getPosition())) return;
	calculateVelocityWind(p->getPosition());
	Vector3 v = p->getVelocity() - vel;
	auto f = calculateDrag(v);
	if (f.x < (vel.x - 1) && p->isFire()) over = true;
	p->addForce(f);
}

void WhirlwindGenerator::updateForce(PxRigidDynamic* p, double duration)
{
	if (fabs(p->getInvMass()) < t || !isActive()) return;
	calculateVelocityWind(p->getGlobalPose().p);
	Vector3 v = p->getLinearVelocity() - vel;
	p->addForce(calculateDrag(v));
}

void WhirlwindGenerator::calculateVelocityWind(Vector3 pos)
{
	vel.x = k * (-(pos.z - centre.z));
	vel.y = k * (c - (pos.y - centre.y));
	vel.z = k * (pos.x - centre.x);
}
