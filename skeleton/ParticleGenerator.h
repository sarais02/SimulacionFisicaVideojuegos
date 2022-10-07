#pragma once

#include "Particle.h"
#include <time.h>
#include <list>
#include <string>
using namespace std;

class ParticleGenerator
{
public:
	//Vector3 pos, Vector3 vel, double _mass
	virtual void generteParticle(list<Particle*>& l)=0;
	void setName(string n);

protected:

	string name;
};