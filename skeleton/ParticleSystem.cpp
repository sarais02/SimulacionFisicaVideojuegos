#include <iostream>
#include "ParticleSystem.h"
#include "GausianParticleGen.h"
#include "UniformParticleGen.h"
#include "CircleGenerator.h"
#include "RocketGenerator.h"

ParticleSystem::ParticleSystem() {
	gravity = Vector3(0.0, -10.0, 0.0);	
	pfr = ParticleForceRegistry();
	//particleGen_list.push_back(shared_ptr<ParticleGenerator>(new UniformParticleGenerator(Vector3(4.0, 1.0, 4.0), Vector3(0.2, 0.2, 0.0))));
	particles_list = list<Particle*>();
	auto xy = new Particle(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector4(1,1,1,1), Vector3(0, 0, 0), 0.9999, 1.0); 
	xy->setTimeAlive(100000);
	particles_list.push_back(xy);

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
}

void ParticleSystem::update(double t) {

	pfr.updateForces(t);
	for (auto g = particleGen_list.begin(); g != particleGen_list.end(); ++g)
	{
		if ((*g)->isActive()) {
			(*g)->generateParticle(particles_list);
		}
	}
	
	for (auto p = particles_list.begin();  p != particles_list.end(); ++p)
	{
		if ((*p)->update(t)) {
			//(*p)->integrate(t);
			if ((*p)->isFire())
				(*p)->changeSize((*p)->getTimeAlive() * t, (*p)->getTransform(), (*p)->getColor());
		}
		
	}
	auto p = particles_list.begin();
	while (p != particles_list.end()) {
		if (!(*p)->isAlive(t)) {
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

void ParticleSystem::generateHosepipeSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("HosePipeSystem");
	if (p != nullptr)
		p->changeActive();
	else { 
		auto s = new GausianParticleGen(Vector3(4.0, 1.0, 4.0), Vector3(0.2, 0.2, 0.0), 0.8, "HosePipeSystem");
		Particle* p = new Particle(Vector3(-50.0, 0.0, 0.0), Vector3(30.0, 15.0, -30.0), Vector4(0.0, 0.7, 0.96, 1.0));
		p->setTimeAlive(2.0);
		p->setAcceleration(gravity);
		//p->setMass(5.0);
		s->setParticle(p);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateFogSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("FogSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(20.0, 8.0, 5.0), 0.6, "FogSystem");
		Particle* molde = new Particle(Vector3(0.0, 30.0, 0.0), Vector3(2.5, 3.0, -2.5), Vector4(1.0, 1.0, 1.0, 0.25), Vector3(0,-5,0), 0.75, 0.3);
		molde->setTimeAlive(1.0);
		//p->setMass(5.0);
		s->setParticle(molde);
		s->setNParticles(30);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateFlamesSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("FlamesSystem");
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
		Particle* p = new Particle(Vector3(0.0, 0.0, 0.0), Vector3(5.0, 20.0, 0.0), Vector4(1.0, 1.0, 0.0, 1.0), gravity, 0.999);
		s->setTypesRockets(fireworks_pool);
		p->setColor(Vector4(255 / 250.0, 128 / 250.0, 0.0, 1.0));
		p->setTimeAlive(8.0);

		s->setParticle(p);
		s->setNParticles(1);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}