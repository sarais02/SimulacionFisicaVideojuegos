#pragma once
#include "ParticleGenerator.h"
#include <random>

using namespace std;

class CircleGenerator : public ParticleGenerator
{
public:
	CircleGenerator(string name_);
	~CircleGenerator() = default;

	void generateParticle(list<Particle*>& l) override;
};