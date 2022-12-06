#pragma once

#include <map>
#include "ForceGenerator.h"

using namespace std;
using namespace physx;

//typedef pair<shared_ptr<ForceGenerator>, PxRigidDynamic*> FRPair;

class RigidForceRegistry :public multimap<shared_ptr<ForceGenerator>, PxRigidDynamic*> {
public:
	void updateForces(double duration) {
		for (auto it = begin(); it != end(); it++)
			it->first->updateForce(it->second, duration);
	}
	void addRegistry(shared_ptr<ForceGenerator> fg, PxRigidDynamic* p) {
		this->insert({ fg, p });
	}
	void deleteRigidRegistry(PxRigidDynamic* p) {
		auto it = begin();
		while (it != end()) {
			if (it->second == p) {
				it = this->erase(it);
			}
			else {
				it++;
			}
		}
	}
	shared_ptr<ForceGenerator> getForceGenenarion(string name) {
		for (auto it = begin(); it != end(); it++) {
			if (it->first.get()->getName() == name) return it->first;
		}
		return nullptr;
	}
	list<shared_ptr<ForceGenerator>> getDynamicForces(PxRigidDynamic* x) {
		list<shared_ptr<ForceGenerator>> aux;
		for (auto it = begin(); it != end(); it++)
			if (it->second == x)aux.push_back(it->first);
		return aux;
	}
};