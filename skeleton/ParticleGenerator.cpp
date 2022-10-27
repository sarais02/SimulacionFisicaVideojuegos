#include "ParticleGenerator.h"

void ParticleGenerator::setName(string n) {
	name = n;
}

void ParticleGenerator::setParticle(Particle* model) {
	//if (molde != nullptr)delete molde;
	molde = model;
}
