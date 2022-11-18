#include <iostream>
#include "ParticleSystem.h"
#include "GausianParticleGen.h"
#include "UniformParticleGen.h"
#include "CircleGenerator.h"
#include "RocketGenerator.h"
#include "GravityForceGenerator.h"
#include "ParticleDragGenerator.h"
#include "WindGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionForceGenerator.h"

ParticleSystem::ParticleSystem() {
	gravity = Vector3(0.0, -10.0, 0.0);	
	pfr = new ParticleForceRegistry();
	forceGen_list = list<shared_ptr<ForceGenerator>>();
	auto gravityForceGen = shared_ptr<ForceGenerator>(new GravityForceGenerator(gravity));
	forceGen_list.push_back(gravityForceGen);

	auto gravityForceGen2 = shared_ptr<ForceGenerator>(new GravityForceGenerator(Vector3(0, 6, 0)));
	gravityForceGen2.get()->setName("Gravity2");
	forceGen_list.push_back(gravityForceGen2);

	auto dragGen = shared_ptr<ParticleDragGenerator>(new ParticleDragGenerator(1.0, 0.0)); //k2=k1/10
	forceGen_list.push_back(dragGen);

	auto viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 0.0, { -10, 0, 0 }, 30, { 10, 45, 0 }));
	forceGen_list.push_back(viento);

	auto torbellino = shared_ptr<WhirlwindGenerator>(new WhirlwindGenerator({ 0, 0, 0 }, {0,20,0}, 1));
	forceGen_list.push_back(torbellino);

	auto torbellino2 = shared_ptr<WhirlwindGenerator>(new WhirlwindGenerator({ 0, 0, 0 }, { 0,25,0 }, 1.5, 20));
	torbellino2.get()->setName("TorbellinoGalaxy"); forceGen_list.push_back(torbellino2);

	auto explo = shared_ptr<ExplosionForceGenerator>(new ExplosionForceGenerator(500, 50, 3, {0,10.0,0}, 3));
	forceGen_list.push_back(explo); explosion = dynamic_cast<ExplosionForceGenerator*>(explo.get());
	
	particles_list = list<Particle*>();
	/*auto xy = new Particle(Vector3(0, 40, 0), Vector3(0, 0, 0), Vector4(1,1,1,1), Vector3(0, 0, 0), 0.9999, 1.0, 5); 
	xy->setTimeAlive(100000);
	particles_list.push_back(xy);*/

	shared_ptr<ParticleGenerator>it = shared_ptr<ParticleGenerator>(new GausianParticleGen(Vector3(2.50, -2.0, 2.50), Vector3(.0, .0, .0), 1.0, "GaussianGenerator"));
	particleGen_list.push_back(it);
	(*it).changeActive();

	it = shared_ptr<ParticleGenerator>(new CircleGenerator("C"));
	particleGen_list.push_back(it);
	(*it).changeActive();

	RocketGenerator* aux = new RocketGenerator({ 0,0,0 }, { 0,-10,0 });
	particleGen_list.push_back(shared_ptr<ParticleGenerator>(aux)); (*aux).changeActive();
	generateFireworkSystem();
	aux->setTypesRockets(fireworks_pool);

	//pfr.addRegistry(gravityFoceGen, xy);
}

ParticleSystem::~ParticleSystem() {

	while (!particleGen_list.empty()) {
		
		particleGen_list.pop_front();
	}
	while (!particles_list.empty()) {
		delete particles_list.front();
		particles_list.pop_front();
	}
	for (int i = 0; i < fireworks_pool.size(); i++) {
		delete fireworks_pool[i];
	}
	fireworks_pool.clear();
	//delete pfr;
}

void ParticleSystem::update(double t) {

	pfr->updateForces(t);
	for (auto g = particleGen_list.begin(); g != particleGen_list.end(); ++g)
	{
		if ((*g)->isActive()) {
			(*g)->generateParticle(particles_list);
		}
	}
	
	for (auto p = particles_list.begin();  p != particles_list.end(); ++p)
	{
		if ((*p)->update(t)) {
			if ((*p)->isFire())
				(*p)->changeSize((*p)->getTimeAlive() * t, (*p)->getTransform(), (*p)->getColor());
		}
		
	}
	auto p = particles_list.begin();
	while (p != particles_list.end()) {
		if (!(*p)->isAlive(t)) {
			pfr->deleteParticleRegistry((*p));
			onParticleDeath((*p));
			delete *p;
			p = particles_list.erase(p);
		}
		else p++;
	}
}

shared_ptr<ParticleGenerator> ParticleSystem::getParticleGen(string name) {

	for (auto it = particleGen_list.begin(); it != particleGen_list.end(); it++)
		if ((*it)->getName() == name) return (*it);
	
	return nullptr;
}

shared_ptr<ForceGenerator> ParticleSystem::getForceGen(string name) {

	for (auto it = forceGen_list.begin(); it != forceGen_list.end(); it++)
		if ((*it)->getName() == name) return (*it);

	return nullptr;
}

void ParticleSystem::generateHosepipeSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("HosePipeSystem");
	if (p != nullptr)
		p->changeActive();
	else { 
		auto s = new GausianParticleGen(Vector3(4.0, 1.0, 4.0), Vector3(0.2, 0.2, 0.0), 0.8, "HosePipeSystem");
		Particle* p = new Particle(Vector3(-50.0, 0.0, 0.0), Vector3(30.0, 15.0, -30.0), Vector4(0.0, 0.7, 0.96, 1.0));
		p->setTimeAlive(2.0);
		p->setMass(0.5);
		pfr->addRegistry(getForceGen("GravityForce"), p);
		pfr->addRegistry(getForceGen("Drag"), p);
		s->setParticle(p);
		s->addParticleForceRegistry(pfr);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateFogSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("FogSystem");
	if (p != nullptr)
		p->changeActive();
	else {

		auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(20.0, 8.0, 5.0), 0.6, "FogSystem");
		Particle* molde = new Particle(Vector3(0.0, 30.0, 0.0), Vector3(2.5, 8.0, -2.5), Vector4(1.0, 1.0, 1.0, 0.25), Vector3(0,/*-5*/0,0), 0.75, 0.25, 0.2);
		molde->setTimeAlive(1.0);		
		pfr->addRegistry(getForceGen("GravityForce"), molde);
		s->setParticle(molde);
		s->addParticleForceRegistry(pfr);
		s->setNParticles(15);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateFlamesSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("FlamesSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(10.0, 5.5, 5.0), 0.6, "FlamesSystem");
		Particle* p = new Particle(Vector3(0.0, 10.0, 0.0), Vector3(1.0, 5.0, -1.0), Vector4(255 / 250.0, 128 / 250.0, 0.0, 0.5), Vector3(0, 0, 0), 0.9, 0.5);
		p->setTimeAlive(6.0);
		p->setIsFire(true);
		p->setMass(5.0);
		pfr->addRegistry(getForceGen("Gravity2"), p);
		pfr->addRegistry(getForceGen("Viento"), p);
		s->setParticle(p);
		s->addParticleForceRegistry(pfr);
		s->setNParticles(30);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateCircleSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("CircleSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new CircleGenerator("CircleSystem");
		Particle* p = new Particle(Vector3(0.0, 20.0, 0.0), Vector3(0.0, 0.0, 0.0), Vector4(255 / 250.0, 128 / 250.0, 0.0, 1.0), gravity, 0.999, 1.0);
		p->setTimeAlive(3.5);
		//p->setMass(5.0);
		s->setParticle(p);
		s->setNParticles(30);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::increaseDesTip(Vector3 increase)
{
	for (auto it = particleGen_list.begin(); it != particleGen_list.end(); it++) {
		if ((*it)->isGaussian() && (*it)->isActive()) {
			GausianParticleGen* f = dynamic_cast <GausianParticleGen*>((*it).get());
			if (f != nullptr) f->increaseDesTipVel(increase);
		}
	}	
}

void ParticleSystem::shootFirework(Firework::FIREWORK_TYPE type) {
	Firework* x;
	switch (type) {
	case Firework::BASIC:
		x = fireworks_pool.at(0)->clone();
		x->setPosition({ 0,0,0 });

		particles_list.push_back(x);

		break;
	case Firework::LINEAR:
		x = fireworks_pool.at(1)->clone();
		x->setPosition({ 0,0,0 });

		particles_list.push_back(x);

		break;
	case Firework::CIRCULAR:
		x = fireworks_pool.at(2)->clone();
		x->setPosition({ 0,0,0 });

		particles_list.push_back(x);
		break;
	}
}

void ParticleSystem::onParticleDeath(Particle* p) {
	Firework* fk = dynamic_cast<Firework*>(p);
	if (fk != nullptr) {
		fk->explode(particles_list);
	}
}

void ParticleSystem::generateFireworkSystem() {
	//se rellena el firework_pool(debemos generalos a tomar por culo) y se inicializa el fireworksystrem
	
	auto x = new Firework(Vector3(10000000, 1000000000, 0), Vector3(0, 15, 0), Vector3(0, 1, 0), 0.9999, 1.0, Firework::BASIC);
	x->addGenerator(particleGen_list.front()); //x->addGenerator(rocket);
	x->setTimeAlive(3.0);
	fireworks_pool.push_back(x);

	x = new Firework(Vector3(10000000, 1000000000, 0), Vector3(0, 15, 0), Vector3(0, 2, 0), 0.9999, 1.0, Firework::LINEAR);
	x->addGenerator(particleGen_list.front());
	x->setTimeAlive(8.0);
	fireworks_pool.push_back(x);

	auto rocket = particleGen_list.begin(); rocket++;
	x = new Firework(Vector3(10000000, 1000000000, 0), Vector3(0, 30, 0), Vector3(0, 2, 0), 0.9999, 1.0, Firework::CIRCULAR);
	x->addGenerator(*rocket);
	x->setTimeAlive(8.0);
	fireworks_pool.push_back(x);
}

void ParticleSystem::generateRocketSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("RocketSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new RocketGenerator({ 0,0,0 }, { 10,-5,0 }); s->setName("RocketSystem");
		Particle* p = new Particle(Vector3(0.0, 0.0, 0.0), Vector3(5.0, 20.0, 0.0), Vector4(1.0, 1.0, 0.0, 1.0), {0,0,0}, 0.999);
		s->setTypesRockets(fireworks_pool);
		p->setColor(Vector4(255 / 250.0, 128 / 250.0, 0.0, 1.0));
		p->setTimeAlive(8.0);
		pfr->addRegistry(getForceGen("GravityForce"), p);
		s->setParticle(p);
		s->setNParticles(1);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateWhirlSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("WhirlSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(7.0, 8.0, 5.0), 0.6, "WhirlSystem");
		Particle* molde = new Particle(Vector3(0.0, 30.0, 0.0), Vector3(2.5, 8.0, -2.5), Vector4(1.0, 1.0, 1.0, 0.25), Vector3(0,0, 0), 0.75, 0.25, 0.2);
		molde->setTimeAlive(4.0);
		pfr->addRegistry(getForceGen("GravityForce"), molde);
		pfr->addRegistry(getForceGen("Torbellino"), molde);
		s->setParticle(molde);
		s->addParticleForceRegistry(pfr);
		s->setNParticles(1);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
};

void ParticleSystem::generateGalaxy() {
	shared_ptr<ParticleGenerator> p = getParticleGen("Galaxy");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(20.0, 8.0, 5.0), 0.6, "Galaxy");
		Particle* molde = new Particle(Vector3(0.0, 30.0, 0.0), Vector3(2.5, 8.0, -2.5), Vector4(1.0, 1.0, 1.0, 0.25), Vector3(0, 0, 0), 0.75, 0.25, 1.0);
		molde->setTimeAlive(15.0);
		pfr->addRegistry(getForceGen("GravityForce"), molde);
		pfr->addRegistry(getForceGen("TorbellinoGalaxy"), molde);
		s->setParticle(molde);
		s->addParticleForceRegistry(pfr);
		s->setNParticles(10);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
};

void ParticleSystem::generateExplosionSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("ExplosionSystem");
	if (p != nullptr) {
		p->changeActive();
		p->generateParticle(particles_list);
		p->changeActive();
	}
	else {
		Particle* p = new Particle(Vector3(0.0, 10.0, 0.0), Vector3(0.0, 0.0, 0.0), Vector4(255 / 250.0, 128 / 250.0, 0.0, 0.75), Vector3(0, 0, 0), 0.75, 0.5, 2.0);
		auto s = new GausianParticleGen(Vector3(.0, .0, .0), Vector3(5.0, 5.0, 5.0), 0.8, "ExplosionSystem");
		pfr->addRegistry(getForceGen("Explosion"), p);
		//pfr->addRegistry(getForceGen("GravityForce"), p);
		p->setTimeAlive(10.0);
		s->setParticle(p);
		s->setNParticles(500);
		s->addParticleForceRegistry(pfr);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
		s->generateParticle(particles_list);
		s->changeActive();
	}
}