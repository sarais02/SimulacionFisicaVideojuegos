#pragma once

#include "Particle.h"
#include <time.h>
#include <list>
#include "ParticleGenerator.h"
#include "Firework.h"
#include "ParticleForceRegistry.h"
#include "ExplosionForceGenerator.h"
#include "SpringForceGenerator.h"
#include "AnchoredSpringFG.h"
#include "BuoyancyForceGenerator.h"
#include "WindGenerator.h"

using namespace std;

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	//llama al metodo create del generador de particulas, al integrate de cada particula
	void update(double t, Vector3 pos);
	void onParticleDeath(Particle* p);

	//OBTENER GENERADORES
	shared_ptr<ParticleGenerator> getParticleGen(string name);
	shared_ptr<ForceGenerator> getForceGen(string name);

	//GENERADORES
	void generateFireworkSystem();
	void generateRain();
	void generateFogSystem(Vector3 pos, string=" ");
	void generateFlamesSystem();
	void generateCircleSystem();
	void generateWhirlSystem();
	void generateBuoyancy();
	void generateFlag(Vector3 pos);

	//NIVEL
	void shootFirework(Firework::FIREWORK_TYPE type, Vector3 pos);
	void createWinds();
	void createWinds2();
	void resetFire();
	void clearlevel();
	void generateForces();
	inline bool isGameOver() { return gameover; }

protected:
	Vector3 gravity;

	list <Particle*> particles_list;
	list<shared_ptr<ParticleGenerator>>particleGen_list;
	list<shared_ptr<ForceGenerator>> forceGen_list;

	ParticleForceRegistry* pfr;

	vector<Firework*> fireworks_pool;
	bool gameover;
};