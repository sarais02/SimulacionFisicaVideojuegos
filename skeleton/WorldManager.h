#pragma once
#include "PxPhysics.h"
#include "PxScene.h"
#include "RenderUtils.hpp"
#include "ForceGenerator.h"
#include "RigidForceRegistry.h"
#include "ExplosionForceGenerator.h"

#include <list>

using namespace std;
using namespace physx;
class SolidRigidGenerator;

struct SolidRigid {
	PxRigidActor* solidType;
	RenderItem* item;
	double timeAlive;
};

class WorldManager {
public:
	WorldManager(PxScene* gScene, PxPhysics* gPhysics);
	~WorldManager();

	void update(double t);
	void eliminarCuerpo(SolidRigid* s);
	void setMaterialToObject(PxRigidActor* x, Vector3 mat);
	void addStaticBox(Vector3 pos, Vector3 tam, Vector4 color);
	void addDynamicBall(Vector3 pos, double tam, Vector3 vel, Vector4 color);
	inline void explotar() { dynamic_cast<ExplosionForceGenerator*>(list_forces.front().get())->setActive(true, 5.0); };

	shared_ptr<SolidRigidGenerator> getParticleGen(string name);
	void generateSystem();
	void addParticles() {
		currentNParticles++;
	}
	int getNParticles() {
		return currentNParticles;
	}
	int getMaxParticles() {
		return max_Particles;
	}

protected:
	PxScene* gScene;
	PxPhysics* gPhysics;
	list<SolidRigid*> rStatic_list;
	list<SolidRigid*> rDynamic_list;
	list<shared_ptr<ForceGenerator>> list_forces;
	list<shared_ptr<SolidRigidGenerator>> generators_list;
	RigidForceRegistry* rfr;
	int max_Particles, currentNParticles;
};