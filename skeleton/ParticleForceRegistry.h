#pragma once

#include <map>
#include <string>
#include "ForceGenerator.h"

typedef std::pair<shared_ptr<ForceGenerator>, Particle*>FRPair;

class ParticleForceRegistry : public std::multimap<shared_ptr<ForceGenerator>, Particle*> {
public:
	void updateForces(double duration) {
		for (auto it = begin(); it != end(); it++) {
			it->first->updateForce(it->second, duration);
		}
	}

	void addRegistry(shared_ptr<ForceGenerator> fg, Particle* p) {
		this->insert(FRPair(fg, p));
		p->forcesNames.push_back(fg.get()->getName());
	}
	void deleteParticleRegistry(Particle*p) {
		auto it = begin();
		while (it!=end())
		{
			if (it->second == p) { it = this->erase(it); }
			else ++it;
		}
	}

	shared_ptr<ForceGenerator> getForceGen(string name) {
		for (auto it = begin(); it != end(); it++) {
			if (it->first.get()->getName() == name) return it->first;
		}
		return nullptr;
	}
};