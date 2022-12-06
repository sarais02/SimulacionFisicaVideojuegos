#include "GaussianSolidRigidGen.h"
#include <algorithm>
#include <iostream>

GaussianSolidRigidGen::GaussianSolidRigidGen(Vector3 desTip_pos_, Vector3 desTip_vel_, double desTip_t_, PxScene* gScene_, PxPhysics* gPhysics_, WorldManager* wm) {
	setName("BASTA");
	gScene = gScene_;
	gPhysics = gPhysics_;
	desTip_pos = desTip_pos_;
	desTip_vel = desTip_vel_;
	desTip_t = desTip_t_;
	gnd = default_random_engine(r());
	SolidRigid* rg = new SolidRigid();
	setSolidRigid(rg);
	is_Gaussian = true;
	active = true;
	isDynamic_ = false;
	worldmanager = wm;
}

void GaussianSolidRigidGen::generateSolidRigid(list<SolidRigid*>& l)
{
	for (int i = 0; i < nParticles && worldmanager->getNParticles() <worldmanager->getMaxParticles(); i++) {
		auto posi = Vector3(desTip_pos.x * d(gnd), desTip_pos.y * d(gnd), desTip_pos.z * d(gnd));
		auto v = Vector3(desTip_vel.x * d(gnd), desTip_vel.y * d(gnd), desTip_vel.z * d(gnd));

		SolidRigid* rg = new SolidRigid();
		rg->solidType = molde->solidType; rg->timeAlive = molde->timeAlive; /*rg->item = molde->item;*/

		Vector3 velFinal;
		Vector3 posFinal = Vector3(molde->solidType->getGlobalPose().p.x + posi.x, molde->solidType->getGlobalPose().p.y + posi.y, molde->solidType->getGlobalPose().p.z + posi.z);

		PxRigidDynamic* di = static_cast<PxRigidDynamic*>(molde->solidType);
		if (di != nullptr) {
			velFinal = Vector3(di->getLinearVelocity().x + v.x, di->getLinearVelocity().y + v.y, di->getLinearVelocity().z + v.z);
			PxRigidDynamic* new_solid;
			new_solid = gPhysics->createRigidDynamic(PxTransform(posFinal));
			new_solid->setLinearVelocity(velFinal);
			new_solid->setAngularVelocity({ 0,0,0 });
			new_solid->setMass(di->getMass());
			//PxPhysics::createMaterial(0.5, 0.5, 0.1);
			
			Vector3 size = Vector3(1.0 + d(gnd), 1.0 + d(gnd), 1.0 + d(gnd));

			auto shape = molde->item->shape;
			new_solid->attachShape(*shape);
			//new_solid->setMassSpaceInertiaTensor(/*{ size.y * size.z,size.x * size.z,size.x * size.y }*/PxVec3(0.f));
			
			auto item = new RenderItem(shape, new_solid, molde->item->color);
			gScene->addActor(*new_solid);
			rg->item = item;
			isDynamic_ = true;
		}
		else { 
			velFinal = { 0,0,0 }; 
			PxRigidStatic* obj = gPhysics->createRigidStatic(PxTransform(posFinal));
			auto shape = molde->item->shape;
			obj->attachShape(*shape);
			auto item = new RenderItem(shape, obj, molde->item->color);
			gScene->addActor(*obj);
			rg->item = item;
		}
		l.push_back(rg);
		worldmanager->addParticles();
	}
	//changeActive();
}

void GaussianSolidRigidGen::increaseDesTipPos(Vector3 increase)
{
	desTip_pos += increase;
}

void GaussianSolidRigidGen::increaseDesTipVel(Vector3 increase)
{
	desTip_vel += increase;
	if (desTip_vel.x < 0) desTip_vel.x = 0;
	if (desTip_vel.y < 0) desTip_vel.y = 0;
	if (desTip_vel.z < 0) desTip_vel.z = 0;
}