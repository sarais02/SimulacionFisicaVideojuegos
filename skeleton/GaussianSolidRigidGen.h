#pragma once
#include "SolidRigidGenerator.h"
#include <random>
#include <Time.h>
using namespace std;

class GaussianSolidRigidGen : public SolidRigidGenerator
{
public:
	GaussianSolidRigidGen(WorldManager* wm, typeRigid typee, Vector3 desTip_pos, Vector3 desTip_vel, double desTip_t_, PxScene* gScene, PxPhysics* gPhysics, bool rgb, double mass = 0.0);
	~GaussianSolidRigidGen();

	void generateSolidRigid(list<SolidRigid*>& l) override;
	void increaseDesTipPos(Vector3 increase);
	void increaseDesTipVel(Vector3 increase);

private:

	Vector3 desTip_pos, desTip_vel;
	double desTip_t;
	normal_distribution<double>d{ 0,1 };
	random_device r;
	default_random_engine gnd;
	bool rgb;
};