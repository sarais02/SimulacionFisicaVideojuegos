#include "GausianParticleGen.h"
#include <iostream>

GausianParticleGen::GausianParticleGen(Vector3 desTip_vel_, Vector3 desTip_pos_) {
	setName("GaussianGenerator");
	desTip_pos = desTip_pos_;
	desTip_vel = desTip_vel_;

	setMolde();
}

 void GausianParticleGen::generteParticle(list<Particle*>& l) {
	 random_device r;
	 default_random_engine gnd(r());
	 for (int i = 0; i < 10; i++)
	 {
		 auto posi = Vector3(desTip_pos.x * d(gnd), desTip_pos.y * d(gnd), desTip_pos.z * d(gnd));
		 auto v = Vector3(desTip_vel.x * d(gnd), desTip_vel.y * d(gnd), desTip_vel.z * d(gnd));

		 Vector3 posFinal = Vector3(molde.getPosition().x + posi.x, molde.getPosition().y + posi.y, molde.getPosition().z + posi.z);
		 Vector3 velFinal = Vector3(molde.getVelocity().x + v.x, molde.getVelocity().y + v.y, molde.getVelocity().z + v.z);

		 auto p = new Particle(posFinal, velFinal, molde.getColor());
		 p->setDamping(molde.getDamping());
		 p->setMass(molde.getMass());
		 p->setTimeAlive(molde.getTimeAlive());
		 l.push_back(p);
		
	 }
	
	 cout << l.size() << "\n";
	
 }

void GausianParticleGen::setMolde() {

	molde.setColor(Vector4(1.0, 1.0, 1.0, 1.0));
	molde.setTimeAlive(5.0);
	//molde.setAcceleration((Vector3(0.0, -10.0, 0))); //Hay que sumarsela en el update
	molde.setMass(5);
	molde.setVelocity(Vector3(30.0, 15.0, -30.0));
	molde.setPosition(Vector3(-50.0, 0.0, 0.0));
	molde.setDamping(0.999);
}