#include <iostream>
#include "ParticleSystem.h"
#include "GausianParticleGen.h"
#include "UniformParticleGen.h"
#include "CircleGenerator.h"
#include "RocketGenerator.h"
#include "GravityForceGenerator.h"
#include "ParticleDragGenerator.h"
#include "WhirlwindGenerator.h"
#include "ExplosionForceGenerator.h"
#include "ElasticBandForceGen.h"

ParticleSystem::ParticleSystem():gameover(false) {
	gravity = Vector3(0.0, -10.0, 0.0);

	pfr = new ParticleForceRegistry();
	
	particles_list = list<Particle*>();
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
	delete pfr;
}

void ParticleSystem::update(double t, Vector3 pos) {

	pfr->updateForces(t);

	for (auto g = particleGen_list.begin(); g != particleGen_list.end(); ++g)
	{
		if ((*g)->isActive()) {
			(*g)->generateParticle(particles_list);
		}
		else if ((*g)->getName() == "FlamesSystem" && !(*g)->isActive()) {
			gameover = true;
		}
	}
	
	for (auto p = particles_list.begin();  p != particles_list.end(); ++p)
	{
		if ((*p)->update(t)) {
			if ((*p)->isFire()) {
				(*p)->changeSize((*p)->getTimeAlive() * t, (*p)->getTransform(), (*p)->getColor());
			}
			else if ((*p)->isWater()) {
				if ((*p)->getPosition().y >= pos.y + 10 && ((*p)->getPosition().x >= pos.x-5 && (*p)->getPosition().x <= pos.x+5)) {
					getParticleGen("FlamesSystem")->setActive(false);
				}
			}
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

void ParticleSystem::generateRain() {
	shared_ptr<ParticleGenerator> p = getParticleGen("RainSystem");
	if (p != nullptr)
		p->changeActive();
	else { 
		auto s = new GausianParticleGen(Vector3(1.0, 0.0, 1.0), Vector3(10.0, 10.0, 5.0), 0.8, "RainSystem");
		Particle* p = new Particle(Vector3(750.0, 30.0, 0.0), Vector3(0.0, 5.0, 0.0), Vector4(0.0, 0.7, 0.96, 1.0), {0,0,0}, 0.999, 0.5);
		p->setTimeAlive(10.0);
		p->setMass(0.5);
		s->setNParticles(4);
		p->setIsWater(true);
		pfr->addRegistry(getForceGen("GravityForce"), p);
		s->setParticle(p);
		s->addParticleForceRegistry(pfr);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::generateFogSystem(Vector3 pos, string n) {
	auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(10.0, 5.0, 2.5), 0.6, "FogSystem");
	Particle* molde = new Particle({pos.x+8, pos.y+2, pos.z}, Vector3(-1, 0.0, 0), Vector4(182.0 / 255, 149.0 / 255, 192.0 / 255, 0.25), Vector3(0, 0, 0), 0.75, 2.5, 5);
	molde->setTimeAlive(3.5);
	if(n!=" ") pfr->addRegistry(getForceGen(n), molde);
	s->setParticle(molde);
	s->addParticleForceRegistry(pfr);
	s->setNParticles(4);
	particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
}

void ParticleSystem::generateFlamesSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("FlamesSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(5.0, 2.5, 5.0), 0.6, "FlamesSystem");
		Particle* p = new Particle(Vector3(0.0, 10.0, 0.0), Vector3(1.0, 5.0, -1.0), Vector4(255 / 250.0, 128 / 250.0, 0.0, 0.5), Vector3(0, 0, 0), 0.9, 0.5);
		p->setTimeAlive(8.0);
		p->setIsFire(true);
		p->setMass(5.0);
		pfr->addRegistry(getForceGen("Gravity2"), p);
		pfr->addRegistry(getForceGen("Viento2"), p);
		pfr->addRegistry(getForceGen("Viento5"), p);
		pfr->addRegistry(getForceGen("Viento6"), p);
		pfr->addRegistry(getForceGen("Torbellino1"), p);
		s->setParticle(p);
		s->addParticleForceRegistry(pfr);
		s->setNParticles(4);
		
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
		s->setParticle(p);
		s->setNParticles(30);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
}

void ParticleSystem::shootFirework(Firework::FIREWORK_TYPE type, Vector3 pos) {
	Firework* x;
	switch (type) {
	case Firework::BASIC:
		x = fireworks_pool.at(0)->clone();
		x->setPosition(pos);

		particles_list.push_back(x);

		break;
	case Firework::LINEAR:
		x = fireworks_pool.at(1)->clone();
		x->setPosition(pos);

		particles_list.push_back(x);

		break;
	case Firework::CIRCULAR:
		x = fireworks_pool.at(2)->clone();
		x->setPosition(pos);

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
	auto it = shared_ptr<ParticleGenerator>(new CircleGenerator("C"));
	particleGen_list.push_back(it);
	(*it).changeActive();

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
	x->addGenerator(it);
	x->setTimeAlive(1.85);
	fireworks_pool.push_back(x);
}

void ParticleSystem::generateWhirlSystem() {
	shared_ptr<ParticleGenerator> p = getParticleGen("WhirlSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GausianParticleGen(Vector3(0.2, 0.1, 0.2), Vector3(7.0, 8.0, 5.0), 0.6, "WhirlSystem");
		Particle* molde = new Particle(Vector3(340.0, 10.0, 0.0), Vector3(2.5, 20.0, -2.5), Vector4(1.0, 1.0, 1.0, 0.25), Vector3(0,0, 0), 0.75, 0.25, 0.2);
		molde->setTimeAlive(4.0);
		pfr->addRegistry(getForceGen("GravityForce"), molde);
		pfr->addRegistry(getForceGen("Torbellino1"), molde);
		s->setParticle(molde);
		s->addParticleForceRegistry(pfr);
		s->setNParticles(10);
		particleGen_list.push_back(shared_ptr<ParticleGenerator>(s));
	}
};

void ParticleSystem::generateFlag(Vector3 pos)
{
	auto viento = shared_ptr<ForceGenerator>(new WindGenerator(5.0, 0.0, { -5.0, 0.0, 0.0 }, 40, pos)); viento->setName("WindAnchored");
	viento->setLoop(true); viento->setActive(true, 2.5);

	pos.y += 5;
	Particle* p3 = new Particle({ pos.x-2, pos.y, pos.z }, { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.2);
	p3->setTimeAlive(-1); pos.y -= 3;
	auto f3 = shared_ptr<ForceGenerator>(new AnchoredSpringFG(40, 5, pos)); f3->setName("Anchored3");
	Particle* p1 = new Particle({ pos.x - 2, pos.y, pos.z }, { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.2);
	p1->setTimeAlive(-1); pos.y -= 3;
	auto f1 = shared_ptr<ForceGenerator>(new AnchoredSpringFG(40, 5, pos)); f1->setName("Anchored1");
	Particle* p2 = new Particle({ pos.x - 2, pos.y, pos.z }, { 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 }, 0.85, 2.2);
	p2->setTimeAlive(-1); pos.y -= 3;
	auto f2 = shared_ptr<ForceGenerator>(new AnchoredSpringFG(40, 5, pos)); f2->setName("Anchored2");
	
	pfr->addRegistry(f3, p3); pfr->addRegistry(f1, p1); pfr->addRegistry(f2, p2);
	pfr->addRegistry(viento, p3); pfr->addRegistry(viento, p2); pfr->addRegistry(viento, p1);
	forceGen_list.push_back(f3); forceGen_list.push_back(f1); forceGen_list.push_back(f2); forceGen_list.push_back(viento);
	
	particles_list.push_back(p3); particles_list.push_back(p2); particles_list.push_back(p1);
}

void ParticleSystem::resetFire()
{
	getParticleGen("FlamesSystem")->setActive(true);
	for (auto it = forceGen_list.begin(); it != forceGen_list.end(); it++)
		(*it)->setIsOver(false);
}

void ParticleSystem::clearlevel()
{
	while (!particles_list.empty())
	{
		delete particles_list.front();
		particles_list.pop_front();
	}
	forceGen_list.clear();
	particleGen_list.clear();
	delete pfr;
	pfr = new ParticleForceRegistry();
}

void ParticleSystem::generateForces()
{
	forceGen_list = list<shared_ptr<ForceGenerator>>();
	auto gravityForceGen = shared_ptr<ForceGenerator>(new GravityForceGenerator(gravity));
	forceGen_list.push_back(gravityForceGen);

	auto gravityForceGen2 = shared_ptr<ForceGenerator>(new GravityForceGenerator(Vector3(0, 6, 0)));
	gravityForceGen2.get()->setName("Gravity2");
	forceGen_list.push_back(gravityForceGen2);

	auto gravityForceGen3 = shared_ptr<ForceGenerator>(new GravityForceGenerator(Vector3(-10, 0, 0)));
	gravityForceGen3.get()->setName("Gravity3");
	forceGen_list.push_back(gravityForceGen3);

	auto torbellino = shared_ptr<WhirlwindGenerator>(new WhirlwindGenerator({ 0, 0, 0 }, { 340,0,0 }, 0.5));
	forceGen_list.push_back(torbellino); torbellino->setName("Torbellino1");


	shared_ptr<ParticleGenerator>it = shared_ptr<ParticleGenerator>(new GausianParticleGen(Vector3(2.50, -2.0, 2.50), Vector3(.0, .0, .0), 1.0, "GaussianGenerator"));
	particleGen_list.push_back(it);
	(*it).changeActive();


	RocketGenerator* aux = new RocketGenerator({ 0,0,0 }, { 0,-10,0 });
	particleGen_list.push_back(shared_ptr<ParticleGenerator>(aux)); (*aux).changeActive();
	generateFireworkSystem();
	aux->setTypesRockets(fireworks_pool);
}

void ParticleSystem::generateBuoyancy() {
	
	auto s = shared_ptr<ParticleGenerator>(new GausianParticleGen({ 0,0,0 }, { 15,0,15 }, 3.0, "Flotacion"));
	particleGen_list.push_back(s);
	Particle* p1 = new Particle({ 0.0, 0.0, 0 }, { 0.0, 0.0, 0.0 }, { 1.0, 1.0, 0.0, 1.0 }, { 0.0, 0.0, 0.0 }, 0.999, 4);
	p1->setMass(10.0);
	p1->setTimeAlive(100.0);
	s->setParticle(p1);
	s->setNParticles(8);
	s->addParticleForceRegistry(pfr);
	
	pfr->addRegistry(getForceGen("GravityForce"), p1);

	auto f1 = shared_ptr<BuoyancyForceGenerator>(new BuoyancyForceGenerator(4, 0.03, 1000)); //v=0.02 equilibrio, v=0.01 se hunde
	forceGen_list.push_back(f1);

	pfr->addRegistry(f1, p1);
}

void ParticleSystem::createWinds()
{
	float range = 25;
	auto viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 5.0, { -5,0, 0 }, range, { 200,0+range/2,0 }));
	forceGen_list.push_back(viento); viento.get()->setName("Viento2");
	generateFogSystem({ 200,0 + range / 2,0 }, "Viento2");
	
	viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 5.0, { -5,0, 0 }, range, { 500,0+(range / 2),0}));
	forceGen_list.push_back(viento); viento.get()->setName("Viento4");
	generateFogSystem({ 500,0 + (range / 2),0 }, "Viento4");

	viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 5.0, { -5,0, 0 }, range, { 530,0 + (range / 2),0 }));
	forceGen_list.push_back(viento); viento.get()->setName("Viento5");
	generateFogSystem({ 530,0 + (range / 2),0 }, "Viento5");

	viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 5.0, { -5,0, 0 }, range, { 560,0 + (range / 2),0 }));
	forceGen_list.push_back(viento); viento.get()->setName("Viento6");
	generateFogSystem({ 560,0 + (range / 2),0 },"Viento6");
}

void ParticleSystem::createWinds2()
{
	float range = 25;
	auto viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 5.0, { -5,0, 0 }, range, { 600,0 + range / 2,0 }));
	forceGen_list.push_back(viento); viento.get()->setName("Viento2");
	generateFogSystem({ 600,0 + range / 2,0 }, "Viento2");

	viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 5.0, { -5,0, 0 }, range, { 5000,0 + (range / 2),0 }));
	forceGen_list.push_back(viento); viento.get()->setName("Viento4");

	viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 5.0, { -5,0, 0 }, range, { 5300,0 + (range / 2),0 }));
	forceGen_list.push_back(viento); viento.get()->setName("Viento5");

	viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 5.0, { -5,0, 0 }, range, { 5600,0 + (range / 2),0 }));
	forceGen_list.push_back(viento); viento.get()->setName("Viento6");
}