#pragma once
#include "ForceGenerator.h"

class BuoyancyForceGenerator :public ForceGenerator {
public:
	BuoyancyForceGenerator(float h, float v, float d);
	virtual ~BuoyancyForceGenerator();

	virtual void updateForce(Particle* p, double t);
	inline void setLiquidParticle(Particle* p) {
		liquid_particle = p;
	}
	inline void increaseVolume(float v) {
		volume += v;
	}
	virtual void setVolume(float v) {
		volume = v;
		if (volume == 0) volume = 0.0;
	}
	virtual float getVolume() {
		return volume;
	}
	virtual void setHeight(float h) {
		height = h;
		if (height == 0) height = 0.0;
	}
	virtual float getHeight() {
		return height;
	}

protected:
	float height, volume, density, gravity = 9.8;
	Particle* liquid_particle;
};