#include "UniformParticleGen.h"

UniformParticleGenerator::UniformParticleGenerator(Vector3 pos_, Vector3 vel_): _pos_width(pos_), _vel_width(vel_) {
	setName("UniformGenerator");
	gnd = default_random_engine(r());

	Particle* p = new Particle(Vector3(-50.0, 0.0, 0.0), Vector3(30.0, 15.0, -30.0), Vector4(1.0, 1.0, 1.0, 1.0));
	p->setTimeAlive(5.0);
	p->setMass(5.0);
	setParticle(p);
}

void UniformParticleGenerator::generteParticle(list<Particle*>& l) {
	for (int i = 0; i < nParticles; i++)
	{
		auto posi = Vector3(_pos_width.x * d(gnd), _pos_width.y * d(gnd), _pos_width.z * d(gnd));
		auto v = Vector3(_vel_width.x * d(gnd), _vel_width.y * d(gnd), _vel_width.z * d(gnd));

		Vector3 posFinal = Vector3(molde->getPosition().x + posi.x, molde->getPosition().y + posi.y, molde->getPosition().z + posi.z);
		Vector3 velFinal = Vector3(molde->getVelocity().x + v.x, molde->getVelocity().y + v.y, molde->getVelocity().z + v.z);

		auto p = new Particle(posFinal, velFinal, molde->getColor());
		p->setDamping(molde->getDamping());
		p->setMass(molde->getMass());
		p->setTimeAlive(molde->getTimeAlive());
		l.push_back(p);
	}
}
