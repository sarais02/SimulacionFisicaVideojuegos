#pragma once
#include "ParticleGenerator.h"
#include "Firework.h"
#include <random>

class RocketGenerator :public ParticleGenerator {

	Firework::FIREWORK_TYPE type_to_generate;
	vector<Firework*> types;
	
	std::random_device r;
	std::default_random_engine gnd;
	uniform_real_distribution<double>d{ -0.5,0.5 };
public:
	RocketGenerator(Vector3 pos, Vector3 vel);
	void generateParticle(list<Particle*>& l) override;
	void ChangeType(Firework::FIREWORK_TYPE x);
	void setTypesRockets(vector<Firework*> f) { types = f; };
};

