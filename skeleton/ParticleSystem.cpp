#include <iostream>
#include "ParticleSystem.h"
#include "GausianParticleGen.h"
#include "UniformParticleGen.h"

ParticleSystem::ParticleSystem() {
	gravity = Vector3(0.0, -10.0, 0.0);	
	//particleGen_list.push_back(new UniformParticleGenerator(Vector3(4.0, 1.0, 4.0), Vector3(0.2, 0.2, 0.0)));
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
		if ((*g)->isActive()) {
			(*g)->generteParticle(particles_list);
		}
	}
	
	for (auto p = particles_list.begin();  p != particles_list.end(); ++p)
	{
		if ((*p)->isAlive(t)) {
			(*p)->integrate(t);
			if((*p)->isFire())
				(*p)->changeSize((*p)->getTimeAlive()*t, (*p)->getTransform(), (*p)->getColor());
		}
	}
	auto p = particles_list.begin();
	while (p != particles_list.end()) {
		if (!(*p)->isAlive(t)) {
			delete *p;
			p = particles_list.erase(p);
		}
		else
			p++;
	}
}

ParticleGenerator* ParticleSystem::getParticleGen(string name) {

	for (auto it = particleGen_list.begin(); it != particleGen_list.end(); it++)
		if ((*it)->getName() == name) return (*it);
	
	return nullptr;
}

void ParticleSystem::generateHosepipeSystem() {
	ParticleGenerator* p = getParticleGen("HosePipeSystem");
	if (p != nullptr)
		p->changeActive();
	else { 
		auto s = new GausianParticleGen(Vector3(4.0, 1.0, 4.0), Vector3(0.2, 0.2, 0.0), 0.8, "HosePipeSystem");
		Particle* p = new Particle(Vector3(-50.0, 0.0, 0.0), Vector3(30.0, 15.0, -30.0), Vector4(0.0, 0.7, 0.96, 1.0));
		p->setTimeAlive(2.0);
		p->setAcceleration(gravity);
		//p->setMass(5.0);
		s->setParticle(p);
		particleGen_list.push_back(s); 
	}
}

void ParticleSystem::generateFogSystem() {
	ParticleGenerator* p = getParticleGen("FogSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(20.0, 8.0, 5.0), 0.6, "FogSystem");
		Particle* molde = new Particle(Vector3(0.0, 30.0, 0.0), Vector3(2.5, 3.0, -2.5), Vector4(1.0, 1.0, 1.0, 0.25), Vector3(0,-5,0), 0.75, 0.3);
		molde->setTimeAlive(1.0);
		//p->setMass(5.0);
		s->setParticle(molde);
		s->setNParticles(30);
		particleGen_list.push_back(s);
	}
}

void ParticleSystem::generateFlamesSystem() {
	ParticleGenerator* p = getParticleGen("FlamesSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(10.0, 5.5, 5.0), 0.6, "FlamesSystem");
		Particle* p = new Particle(Vector3(0.0, 10.0, 0.0), Vector3(1.0, 5.0, -1.0), Vector4(255 / 250.0, 128 / 250.0, 0.0, 0.5), Vector3(0, 6, 0), 0.9, 0.5);
		p->setTimeAlive(6.0);
		p->setIsFire(true);
		//p->setMass(5.0);
		s->setParticle(p);
		s->setNParticles(30);
		particleGen_list.push_back(s);
	}
}

void ParticleSystem::increaseDesTip(Vector3 increase)
{
	for (auto it = particleGen_list.begin(); it != particleGen_list.end(); it++) {
		if ((*it)->isGaussian() && (*it)->isActive()) {
			GausianParticleGen* f = dynamic_cast <GausianParticleGen*>((*it));
			if (f != nullptr) f->increaseDesTipVel(increase);
		}
	}	
}