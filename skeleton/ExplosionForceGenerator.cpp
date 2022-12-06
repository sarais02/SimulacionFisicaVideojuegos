#include "ExplosionForceGenerator.h"
#include "math.h"
#include "stdlib.h"
#include "conio.h"

ExplosionForceGenerator::ExplosionForceGenerator(const double K, int R, int c, Vector3 centre, double t): K(K), R(R), c(c), centre(centre), maxTime(t) {
	setName("Explosion"); active = false;
}

void ExplosionForceGenerator::updateForce(Particle* p, double duration)
{
	if (fabs(p->getInverseMass()) < t || !isActive()) return;

	currentTime += duration;
	if (currentTime > maxTime) currentTime = maxTime;

	R = ve * currentTime;

	double r = sqrt(pow(p->getPosition().x-centre.x, 2) + pow(p->getPosition().y - centre.y, 2) + pow(p->getPosition().z - centre.z, 2));

	if (r >= R) p->addForce({0,0,0});
	else {
		Vector3 f;
		double mult = (K / pow(r, 2));
		double mult2 = exp(-(duration / c));
		f.x = mult * (p->getPosition().x - centre.x);
		f.y = mult * (p->getPosition().y - centre.y);
		f.z = mult * (p->getPosition().z - centre.z);
		f = f * mult2;

		p->addForce(f);
	}
}

void ExplosionForceGenerator::updateForce(PxRigidDynamic* p, double duration)
{
	if (fabs(p->getInvMass()) < t || !isActive()) return;

	currentTime += duration;
	if (currentTime > maxTime) currentTime = maxTime;

	R = ve * currentTime;

	double r = sqrt(pow(p->getGlobalPose().p.x - centre.x, 2) + pow(p->getGlobalPose().p.y - centre.y, 2) + pow(p->getGlobalPose().p.z - centre.z, 2));

	if (r >= R) p->addForce({ 0,0,0 });
	else {
		Vector3 f;
		double mult = (K / pow(r, 2));
		double mult2 = exp(-(duration / c));
		f.x = mult * (p->getGlobalPose().p.x - centre.x);
		f.y = mult * (p->getGlobalPose().p.y - centre.y);
		f.z = mult * (p->getGlobalPose().p.z - centre.z);
		f = f * mult2;

		p->addForce(f);
	}
}
