#include "WindGenerator.h"
#include <iostream>

WindGenerator::WindGenerator(const float k1, const float k2, Vector3 v, float range, Vector3 centre):ParticleDragGenerator(k1, k2), vel(v), rango(range)
{
	setName("Viento");
	maxPositive = {centre.x + rango, centre.y + rango, centre.z + rango };
	maxNegative = { centre.x - rango, centre.y - rango, centre.z - rango };
}

void WindGenerator::updateForce(Particle* p, double duration)
{
	if (fabs(p->getInverseMass()) < 1e-10 || !isActive() || !inRange(p->getPosition()) ||  !canUpdateForce(duration)) return;

	Vector3 v = p->getVelocity()-vel;
	p->addForce(calculateDrag(v));
}

void WindGenerator::updateForce(PxRigidDynamic* p, double duration)
{
	if (fabs(p->getInvMass()) < 1e-10 || !isActive() || !inRange(p->getGlobalPose().p) || !canUpdateForce(duration)) return;

	Vector3 v = p->getLinearVelocity() - vel;
	cout << "FUERZA PARTICULA X:" << p->getLinearVelocity().x << " y:" << p->getLinearVelocity().y << " z:" << p->getLinearVelocity().z << "\n";
	Vector3 puta = calculateDrag(v);
	p->addForce(puta);
	cout << "FUERZA X:" << puta.x << " y:" << puta.y << " z:" << puta.z << "\n";
}

bool WindGenerator::inRange(Vector3 pos)
{
	return (pos.x < maxPositive.x && pos.x > maxNegative.x
		&& pos.y < maxPositive.y && pos.y > maxNegative.y
		&& pos.z < maxPositive.z && pos.z > maxNegative.z);
}
