#include "ParticleGenerator.h"

void ParticleGenerator::setName(string n) {
	name = n;
	active = false;
}

void ParticleGenerator::setParticle(Particle* model) {
	molde = model;
}
