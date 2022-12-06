#pragma once
#include <list>
#include <string>
#include "RigidForceRegistry.h"
#include "WorldManager.h"
using namespace std;
using namespace physx;
struct SolidRigid;

class SolidRigidGenerator
{
public:

	virtual void generateSolidRigid(list<SolidRigid*>& l) = 0;
	void setName(string n) {
		name = n;
	}
	inline string getName() {
		return name;
	}
	inline void  setSolidRigid(SolidRigid* s) {
		molde = s;
	}
	
	inline bool isActive() {
		return active;
	}
	inline void setActive(bool b) {
		active = b;
	}
	inline void changeActive() {
		active = !active;
		Vector3 pos = molde->solidType->getGlobalPose().p;
		if (!active) molde->solidType->setGlobalPose({ pos.x, pos.y + 4000, pos.x });
		else molde->solidType->setGlobalPose({ pos.x, pos.y - 4000, pos.x });
	}

	void setNParticles(int n) {
		nParticles = n;
	}
	
	inline bool isGaussian() {
		return is_Gaussian;
	}
	inline bool isDynamic() {
		return isDynamic_;
	}
	void addParticleForceRegistry(RigidForceRegistry* _pfr) { pfr = _pfr; }


protected:

	string name;
	int nParticles = 10;
	SolidRigid* molde;
	bool active;
	bool is_Gaussian = false, isDynamic_, isStatic;
	RigidForceRegistry* pfr;
	PxScene* gScene;
	PxPhysics* gPhysics;
};