#include "ExplosionForceGenerator.h"
#include "math.h"
#include "stdlib.h"
#include "conio.h"

ExplosionForceGenerator::ExplosionForceGenerator(const double K, int R, int c, Vector3 centre): K(K), R(R), c(c), centre(centre)
{
	setName("Explosion");
}

void ExplosionForceGenerator::updateForce(Particle* p, double duration)
{
	if (fabs(p->getInverseMass()) < t) return;

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
