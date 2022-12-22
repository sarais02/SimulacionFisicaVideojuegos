#include "GaussianSolidRigidGen.h"
#include <math.h>
#include <list>
#include <Time.h>
#include <random>

GaussianSolidRigidGen::GaussianSolidRigidGen(WorldManager* wm, typeRigid typee, Vector3 desTip_pos_, Vector3 desTip_vel_, double desTip_t_, PxScene* gScene_, PxPhysics* gPhysics_, bool rgb_, double masss)
{
	worldmanager = wm; 
	type = typee;
	desTip_pos = desTip_pos_; 
	desTip_vel = desTip_vel_;
	mass = masss;
	gScene = gScene_;
	gPhysics = gPhysics_;
	active = true;
	rgb = rgb_;
}

GaussianSolidRigidGen::~GaussianSolidRigidGen() {
	setSolidRigid(nullptr);
	worldmanager = nullptr;
	pfr = nullptr;
}

void GaussianSolidRigidGen::generateSolidRigid(list<SolidRigid*>& l) {

	Vector3 velFinal, posmolde = molde->solidType->getGlobalPose().p;
	auto di = static_cast<PxRigidDynamic*>(molde->solidType);

	for (int i = 0; i < nParticles && worldmanager->getNParticles()<worldmanager->getMaxParticles(); i++) {
		
		auto posi = Vector3(desTip_pos.x * d(gnd), desTip_pos.y * d(gnd), desTip_pos.z * d(gnd));
		auto v = Vector3(desTip_vel.x * d(gnd), desTip_vel.y * d(gnd), desTip_vel.z * d(gnd));

		if (rgb) {
			auto r = rand() % 255 + 0;
			auto g = rand() % 255 + 0;
			auto b = rand() % 255 + 0;
			molde->item->color = Vector4(r / 255.0, g / 255.0, b / 255.0, 1.0);
		}

		Vector3 posFinal = Vector3(posmolde.x + posi.x, posmolde.y + posi.y, posmolde.z + posi.z);

		SolidRigid* nueva = clone(molde);


		nueva->solidType->setGlobalPose(PxTransform(posFinal));
		if (type == DYNAMIC) {
			velFinal = Vector3(di->getLinearVelocity().x + v.x, di->getLinearVelocity().y + v.y, di->getLinearVelocity().z + v.z);
			auto aux = static_cast<PxRigidDynamic*>(nueva->solidType);
			//aux->setMass(di->getMass() + abs(d(gnd) * mass));
			aux->setLinearVelocity(velFinal);
		}

		worldmanager->addParticles();
		l.push_back(nueva);
	}
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