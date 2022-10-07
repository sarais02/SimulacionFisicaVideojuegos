#include <iostream>
#include "ParticleSystem.h"
#include "GausianParticleGen.h"

ParticleSystem::ParticleSystem() {
	gravity = Vector3(0.0, -10.0, 0.0);	
	particleGen_list.push_back(new GausianParticleGen(Vector3(4.0, 1.0, 4.0), Vector3(0.2, 0.2, 0.0)));
	particles_list = list<Particle*>();
}

ParticleSystem::~ParticleSystem() {

	while (!particleGen_list.empty()) {
		delete particleGen_list.front();
		particleGen_list.pop_front();
	}
	while (!particles_list.empty()) {
		delete particles_list.front();
		particles_list.pop_front();
	}
}

void ParticleSystem::update(double t) {

	for (auto g = particleGen_list.begin(); g != particleGen_list.end(); ++g)
	{
		(*g)->generteParticle(particles_list);
		std::cout << particles_list.size() << endl;
	}
	for (auto p = particles_list.begin();  p != particles_list.end(); ++p)
	{
		(*p)->setAcceleration(gravity);
		if((*p)->isAlive(t))
			(*p)->integrate(t);
		else {
			auto aux = p; p++;
			delete* aux;
			*aux = nullptr;
			particles_list.erase(aux);
		}
	}
}