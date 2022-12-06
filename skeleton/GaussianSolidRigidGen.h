#pragma once
#include "SolidRigidGenerator.h"
#include <random>
struct SolidRigid;

using namespace std;

class GaussianSolidRigidGen : public SolidRigidGenerator
{
public:
	GaussianSolidRigidGen(Vector3 desTip_pos, Vector3 desTip_vel, double desTip_t_, PxScene* gScene, PxPhysics* gPhysics, WorldManager* wm);
	~GaussianSolidRigidGen() = default;

	void generateSolidRigid(list<SolidRigid*>& l) override;
	void increaseDesTipPos(Vector3 increase);
	void increaseDesTipVel(Vector3 increase);

private:
	Vector3 desTip_vel;
	Vector3 desTip_pos;
	double desTip_t;
	normal_distribution<double>d{ 0,1 };
	random_device r;
	default_random_engine gnd;
	PxScene* gScene;
	PxPhysics* gPhysics;
	WorldManager* worldmanager;
};