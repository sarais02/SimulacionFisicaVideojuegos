#pragma once
#include <list>
#include <Time.h>
#include <random>
#include "Particle.h"
#include "ParticleGenerator.h"
using namespace std;
class UniformParticleGenerator :public ParticleGenerator
{
	Vector3 _vel_width;
	Vector3 _pos_width;
	random_device r;
	std::default_random_engine gnd;
	uniform_real_distribution<double>d{ -0.5, 0.5 };
public:
	UniformParticleGenerator(Vector3 pos, Vector3 vel);
	~UniformParticleGenerator()=default;
	void generateParticle(list<Particle*>& l) override;
};