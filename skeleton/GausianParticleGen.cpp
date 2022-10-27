#include "GausianParticleGen.h"
#include <iostream>

GausianParticleGen::GausianParticleGen(Vector3 desTip_vel_, Vector3 desTip_pos_, double desTip_t_, string name_) {
	setName(name_);
	desTip_pos = desTip_pos_;
	desTip_vel = desTip_vel_;
	desTip_t = desTip_t_;
	gnd = default_random_engine(r());
	Particle* p = new Particle(Vector3(-50.0, 0.0, 0.0), Vector3(30.0, 15.0, -30.0), Vector4(1.0, 1.0, 1.0, 1.0));
	p->setTimeAlive(5.0);
	p->setMass(5.0);
	setParticle(p);
	is_Gaussian = true;
	active = true;
}

 void GausianParticleGen::generateParticle(list<Particle*>& l) {
	
	 for (int i = 0; i < nParticles; i++)
	 {
		 auto posi = Vector3(desTip_pos.x * d(gnd), desTip_pos.y * d(gnd), desTip_pos.z * d(gnd));
		 auto v = Vector3(desTip_vel.x * d(gnd), desTip_vel.y * d(gnd), desTip_vel.z * d(gnd));

		 Vector3 posFinal = Vector3(molde->getPosition().x + posi.x, molde->getPosition().y + posi.y, molde->getPosition().z + posi.z);
		 Vector3 velFinal = Vector3(molde->getVelocity().x + v.x, molde->getVelocity().y + v.y, molde->getVelocity().z + v.z);

		 //auto p = new Particle(posFinal, velFinal, molde->getColor(), molde->getAcceleration(), molde->getDamping(), molde->getSize());
		 auto p = molde->clone();
		 p->setMass(molde->getMass());
		 p->setIsFire(molde->isFire());
		 p->setTimeAlive(molde->getIniTimeAlive() + (desTip_t * d(gnd)));
		 p->setVelocity(velFinal);
		 p->setPosition(posFinal);
		 l.push_back(p);
	 }
 }

 void GausianParticleGen::increaseDesTipPos(Vector3 increase)
 {
	 desTip_pos += increase;
 }

 void GausianParticleGen::increaseDesTipVel(Vector3 increase)
 {
	 desTip_vel += increase;
	 if (desTip_vel.x < 0) desTip_vel.x = 0;
	 if (desTip_vel.y < 0) desTip_vel.y = 0;
	 if (desTip_vel.z < 0) desTip_vel.z = 0;
 }
