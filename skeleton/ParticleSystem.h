#pragma once

#include "Particle.h"
#include <time.h>
#include <list>
#include "ParticleGenerator.h"

using namespace std;

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	//llama al metodo create del generador de particulas, al integrate de cada particula
	void update(double t);

	ParticleGenerator* getParticleGen(string name);
	void generateFireworkSystem();
	void generateHosepipeSystem();
	void generateFogSystem();
	void generateFlamesSystem();
	void increaseDesTip(Vector3 increase);

protected:
	Vector3 bounds_max;
	Vector3 bounds_min;
	Vector3 gravity;
	list <Particle*> particles_list;
	list<ParticleGenerator*>particleGen_list;
};
