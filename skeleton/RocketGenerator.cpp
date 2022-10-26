#include "RocketGenerator.h"

RocketGenerator::RocketGenerator(Vector3 pos, Vector3 vel) {

	active = true;
	gnd = default_random_engine(r());
	nParticles = 1;
	meanPos = pos;
	meanVel = vel;
	molde = new Particle();
}

void RocketGenerator::generateParticle(list<Particle*>& l) {
	Vector3 newpos, newvel;
	for (int i = 0; i < nParticles; i++) {
		newpos = molde->getPosition() + Vector3(meanPos.x * d(gnd), meanPos.y * d(gnd), meanPos.z * d(gnd));
		newvel = molde->getVelocity() + Vector3(meanVel.x * d(gnd), meanVel.y * d(gnd), meanVel.z * d(gnd));
		Firework* xy;
		switch (type_to_generate) {
		case Firework::BASIC:
			xy = types.at(0)->clone();
			break;
		case Firework::LINEAR:
			xy = types.at(1)->clone();
			break;
		case Firework::CIRCULAR:
			xy = types.at(2)->clone();
			break;
		}
		xy->setPosition(newpos);
		xy->setVelocity(newvel);
		l.push_back(xy);
	}
	type_to_generate = Firework::BASIC;
}

void RocketGenerator::ChangeType(Firework::FIREWORK_TYPE x) {
	type_to_generate = x;
}