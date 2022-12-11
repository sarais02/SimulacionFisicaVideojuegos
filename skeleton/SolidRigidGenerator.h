#pragma once
#include <list>
#include <string>
#include "PxPhysics.h"
#include "RigidForceRegistry.h"
#include "WorldManager.h"
#include "PxScene.h"
#include "RenderUtils.hpp"

using namespace std;
using namespace physx;
struct SolidRigid;

enum typeRigid { STATIC, DYNAMIC };

class SolidRigidGenerator
{
public:
	typeRigid type;
	virtual void generateSolidRigid(list<SolidRigid*>& l) = 0;
	void setName(string n) {
		name = n;
	}
	inline string getName() {
		return name;
	}
	inline void  setSolidRigid(SolidRigid* s) {
		if (molde != nullptr) {
			DeregisterRenderItem(molde->item);
			delete molde;
		}
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
	void setPhysx(PxPhysics* gP) { gPhysics = gP; };
	void setNParticles(int n) {
		nParticles = n;
	}

	void addParticleForceRegistry(RigidForceRegistry* _pfr) { pfr = _pfr; }

	SolidRigid* clone(SolidRigid* rg) {

		SolidRigid* nuevo = new SolidRigid();
		double t = rg->tam;
		nuevo->timeAlive = rg->timeAlive; nuevo->tam = t;
		
		auto pos = rg->solidType->getGlobalPose().p;
		auto di = static_cast<PxRigidDynamic*>(rg->solidType);
		PxRigidActor* new_solid;
		auto typee = rg->item->shape->getGeometryType();
		PxShape* shape;
		switch (typee)
		{
		case physx::PxGeometryType::eSPHERE:
			if (type == DYNAMIC) {
				auto x = gPhysics->createRigidDynamic(PxTransform(pos));
				x->setLinearVelocity(di->getLinearVelocity());
				x->setAngularVelocity(di->getAngularVelocity());
				//x->setMass(di->getMass());
				x->setMassSpaceInertiaTensor(PxVec3(5.f));
				//x->setMassSpaceInertiaTensor(PxVec3(0.f, 0.f, 10.f));
				for (int i = 0; i < molde->forcesNames.size(); i++) {
					nuevo->forcesNames.push_back(molde->forcesNames[i]);
					pfr->addRegistry(pfr->getForceGenenarion(molde->forcesNames[i]), x);
				}
				PxRigidBodyExt::setMassAndUpdateInertia(*x, 60.0);
				//x->setLinearDamping(0.9999);
				new_solid = x;
			}
			else {
				new_solid = gPhysics->createRigidDynamic(PxTransform(pos));
			}

			shape = CreateShape(PxSphereGeometry(/*2.0*/molde->tam));
			break;

		case physx::PxGeometryType::eBOX:
			if (type == DYNAMIC) {
				auto x = gPhysics->createRigidDynamic(PxTransform(pos));
				x->setLinearVelocity(di->getLinearVelocity());
				x->setAngularVelocity(di->getAngularVelocity());
				//x->setMass(di->getMass());
				//x->setMassSpaceInertiaTensor(PxVec3(0.f));
				//x->setMassSpaceInertiaTensor(PxVec3(0.f, 0.f, 10.f));
				for (int i = 0; i < molde->forcesNames.size(); i++) {
					nuevo->forcesNames.push_back(molde->forcesNames[i]);
					pfr->addRegistry(pfr->getForceGenenarion(molde->forcesNames[i]), x);
				}
				new_solid = x;
			}
			else {
				new_solid = gPhysics->createRigidDynamic(PxTransform(pos));
			}

			shape = CreateShape(PxBoxGeometry(Vector3(3.5, 3.5, 3.5)), gPhysics->createMaterial(10, 10, 0.2));
			break;
		}
		//guardo las cosas
		new_solid->attachShape(*shape);
		//new_solid->setMassSpaceInertiaTensor({ size.y * size.z,size.x * size.z,size.x * size.y });
		nuevo->item = new RenderItem(shape, new_solid, rg->item->color);
		gScene->addActor(*new_solid);
		nuevo->solidType = new_solid;

		//if (di != nullptr) { //Le meto las fuerzas
		//	auto aux2 = static_cast<PxRigidDynamic*>(nuevo->solidType);
		//	for (int i = 0; i < molde->forcesNames.size(); i++) {
		//		nuevo->forcesNames.push_back(molde->forcesNames[i]);
		//		pfr->addRegistry(pfr->getForceGenenarion(molde->forcesNames[i]), aux2);
		//	}
		//}

		return nuevo;
	};

protected:

	string name;
	int nParticles = 20;
	SolidRigid* molde = nullptr;
	bool active = false;
	bool is_Gaussian = false;
	RigidForceRegistry* pfr;
	PxScene* gScene;
	PxPhysics* gPhysics;
	WorldManager* worldmanager;
	double mass;
};