#include "SpringForceGenerator.h"

SpringForceGenerator::SpringForceGenerator(double k, double resting_lenght, Particle* other): k_(k), resting_lenght_(resting_lenght), other_(other)
{
	setName("SpringForce");
}

void SpringForceGenerator::updateForce(Particle* p, double duration)
{
	if (!isActive()) return;

	Vector3 f = other_->getPosition() - p->getPosition();

	const float lenght = f.normalize();
	const float delta_x = lenght - resting_lenght_;

	f *= delta_x * k_;

	p->addForce(f);
}

void SpringForceGenerator::updateForce(PxRigidDynamic* p, double duration)
{
	if (!isActive()) return;

	Vector3 f = other_->getPosition() - p->getGlobalPose().p;

	const float lenght = f.normalize();
	const float delta_x = lenght - resting_lenght_;

	f *= delta_x * k_;

	p->addForce(f);
}