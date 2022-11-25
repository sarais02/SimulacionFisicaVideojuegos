#include "ElasticBandForceGen.h"

ElasticBandForceGen::ElasticBandForceGen(double k, double resting_lenght, Particle* p):SpringForceGenerator(k, resting_lenght, p) {
	setName("ElasticBand");
}

void ElasticBandForceGen::updateForce(Particle* p, double duration) {
	Vector3 f = p->getPosition();
	f -= other_->getPosition();

	float lenght = f.normalize();
	lenght = (lenght - resting_lenght_);
	if (lenght <= 0.0f) return;
	
	f *= -(lenght * k_);
	p->addForce(f);
}