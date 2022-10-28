#pragma once

#include <map>
#include "ForceGenerator.h"

typedef std::pair<shared_ptr<ForceGenerator>, Particle*>FRPair;

class ParticleForceRegistry : public std::multimap<shared_ptr<ForceGenerator>, Particle*> {
public:
	void updateForces(double duration) {
		for (auto it = begin(); it != end(); it++) {
			it->first->updateForce(it->second, duration);
		}
	}

	void addRegistry(ForceGenerator* fg, Particle* p) {
		this->insert(FRPair(fg, p));
	}
	void deleteParticleRegistry(Particle*p) {
		auto it = begin();
		while (it!=end())
		{
			if (it->second == p) { it = this->erase(it); }
			else ++it;
		}
	}
};