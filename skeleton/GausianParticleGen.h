#pragma once
#include "ParticleGenerator.h"
#include <random>

using namespace std;

class GausianParticleGen: public ParticleGenerator
{
public:
	GausianParticleGen(Vector3 desTip_vel, Vector3 desTip_pos, double desTip_t_, string name_);
	~GausianParticleGen() { delete molde; };

	void generateParticle(list<Particle*>& l) override;
	void increaseDesTipPos(Vector3 increase);
	void increaseDesTipVel(Vector3 increase);
	void galaxyColor(Vector3 pos);
	void setUpMeteor(Vector3 pos);
	
private:
	//Cuanto más, más ancho es el camino
	Vector3 desTip_vel, desTip_pos;
	double desTip_t;
	normal_distribution<double>d{ 0,1 };
	random_device r;
	default_random_engine gnd;
};
