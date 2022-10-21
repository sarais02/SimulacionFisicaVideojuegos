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
	inline string getName() {
		return name;
	}
	void setParticle(Particle* model);
	inline bool isActive() {
		return active;
	}
	inline void setActive(bool b) {
		active = b;
	}
	inline void changeActive() {
		active = !active;
		Vector3 pos = molde->getPosition();
		if (!active) molde->setPosition(Vector3(pos.x, pos.y + 4000, pos.z));
		else molde->setPosition(Vector3(pos.x, pos.y - 4000, pos.z));
	}
	void setNParticles(int n) {
		nParticles = n;
	}
	inline bool isGaussian() {
		return is_Gaussian;
	}
	

protected:

	string name;
	Vector3 meanPos, meanVel;
	double generationProbability;
	int nParticles=10;
	Particle* molde;
	bool active;
	bool is_Gaussian=false;
};