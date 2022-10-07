#pragma once
#include "ParticleGenerator.h"
#include <random>

using namespace std;

class GausianParticleGen: public ParticleGenerator
{
public:
	GausianParticleGen(Vector3 desTip_vel, Vector3 desTip_pos);
	~GausianParticleGen();

	void generteParticle(list<Particle*>& l) override;

private:
	//Cuanto más, más ancho es el camino
	Vector3 desTip_vel;
	Vector3 desTip_pos;
	Particle molde;
	normal_distribution<double>d{ 0,1 };

	void setMolde();
};
