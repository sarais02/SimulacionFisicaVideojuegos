#pragma once

#include "Particle.h"
#include <time.h>
#include <list>
#include "ParticleGenerator.h"
#include "Firework.h"
#include "ParticleForceRegistry.h"
#include "ExplosionForceGenerator.h"

using namespace std;

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	//llama al metodo create del generador de particulas, al integrate de cada particula
	void update(double t);

	shared_ptr<ParticleGenerator> getParticleGen(string name);
	shared_ptr<ForceGenerator> getForceGen(string name);
	void generateFireworkSystem();
	void generateHosepipeSystem();
	void generateFogSystem();
	void generateFlamesSystem();
	void generateCircleSystem();
	void generateRocketSystem();
	void increaseDesTip(Vector3 increase);
	void shootFirework(Firework::FIREWORK_TYPE type);
	void onParticleDeath(Particle* p);
	void generateWhirlSystem();
	void generateGalaxy();
	void activeExplosion(bool expl) { explosion->OnActive(expl); };
	void generateExplosionSystem();
protected:
	Vector3 bounds_max;
	Vector3 bounds_min;
	Vector3 gravity;
	list <Particle*> particles_list;
	list<shared_ptr<ParticleGenerator>>particleGen_list;
	list<shared_ptr<ForceGenerator>> forceGen_list;
	ExplosionForceGenerator* explosion;
	ParticleForceRegistry* pfr;
	vector<Firework*> fireworks_pool;
};
