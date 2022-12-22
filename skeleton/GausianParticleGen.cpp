#include "GausianParticleGen.h"
#include <algorithm>
#include <iostream>

GausianParticleGen::GausianParticleGen(Vector3 desTip_vel_, Vector3 desTip_pos_, double desTip_t_, string name_) {
	setName(name_);
	desTip_pos = desTip_pos_;
	desTip_vel = desTip_vel_;
	desTip_t = desTip_t_;
	gnd = default_random_engine(r());
	Particle* p = new Particle();
	setParticle(p);
	is_Gaussian = true;
	active = true;
}

 void GausianParticleGen::generateParticle(list<Particle*>& l) {
	
	 for (int i = 0; i < nParticles && active; i++)
	 {
		 auto posi = Vector3(desTip_pos.x * d(gnd), desTip_pos.y * d(gnd), desTip_pos.z * d(gnd));
		 auto v = Vector3(desTip_vel.x * d(gnd), desTip_vel.y * d(gnd), desTip_vel.z * d(gnd));

		 Vector3 posFinal = Vector3(molde->getPosition().x + posi.x, molde->getPosition().y + posi.y, molde->getPosition().z + posi.z);
		 Vector3 velFinal = Vector3(molde->getVelocity().x + v.x, molde->getVelocity().y + v.y, molde->getVelocity().z + v.z);
		 
		 if (name=="Galaxy") galaxyColor(posFinal);
		 
		 if (name == "ExplosionSystem") setUpMeteor(posFinal);

		 auto p = molde->clone();
		 p->setMass(molde->getMass());
		 p->setIsFire(molde->isFire());
		 p->setIsWater(molde->isWater());
		 p->setTimeAlive(molde->getIniTimeAlive() + (desTip_t * d(gnd)));
		 p->setVelocity(velFinal);
		 p->setPosition(posFinal);
		 for (int i = 0; i < molde->forcesNames.size(); i++)
		 {
			 pfr->addRegistry(pfr->getForceGen(molde->forcesNames[i]), p);
			 if (name == "FlamesSystem" && pfr->getForceGen(molde->forcesNames[i])->isOver())
				 active = false;
		 }
		 l.push_back(p);
	 }
	 if (name == "Flotacion") changeActive();
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

 void GausianParticleGen::galaxyColor(Vector3 posFinal)
 {
	 auto r = rand() % 11 + 0;
	 if (posFinal.x < desTip_pos.x / 8) molde->setColor(Vector4(1.0, 1.0, 1.0, 1.0)); //blanco
	 else if (posFinal.x <= 2 * desTip_pos.x / 4) molde->setColor(Vector4(0.0, 1.0, 1.0, 1.0)); //cian
	 else if (posFinal.x < 5 * desTip_pos.x / 8) molde->setColor(Vector4(1.0, 0.0, 1.0, 1.0)); //rosa
	 else molde->setColor(Vector4(37.0 / 255, 36.0 / 255, 64.0 / 255, 1.0)); //azul
 }

 void GausianParticleGen::setUpMeteor(Vector3 pos) {
	 if (abs(pos.x - molde->getPosition().x)>(6 * desTip_pos.x) / 8.0 && abs(pos.y - molde->getPosition().y) > (6 * desTip_pos.y) / 8.0 && abs(pos.z - molde->getPosition().z) > (6 * desTip_pos.z) / 8.0) {
		 molde->setColor(Vector4(84/255.0, 56 / 255.0, 34 / 255.0, 1.0)); //marron
		 molde->setSize(0.8);
		 molde->setMass(0.25);
	 }
	 else if (abs(pos.x - molde->getPosition().x) < (3*desTip_pos.x) / 8.0 && abs(pos.y- molde->getPosition().y) < (3 * desTip_pos.x) / 8.0 && abs(pos.z - molde->getPosition().z) < (3 * desTip_pos.x) / 8.0) {
		 molde->setColor(Vector4(1.0, 1.0, 0.0, 1.0)); //amarillo
		 molde->setSize(0.5);
		 molde->setMass(50.5);
	 }
	 else {
		 molde->setColor(Vector4(1.0, 64 / 255.0, 0.0, 1.0)); //rojo/naranja
		 molde->setSize(0.3);
		 molde->setMass(2.0);
	 }
 }