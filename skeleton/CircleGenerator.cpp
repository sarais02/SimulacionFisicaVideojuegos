#include "CircleGenerator.h"
#include <math.h>
#include <stdio.h>

#define PI 3.14159265

CircleGenerator::CircleGenerator(string name_){
	setName(name_);
	is_Gaussian = false;
	active = true;
	Particle* p = new Particle(Vector3(-50.0, 0.0, 0.0), Vector3(30.0, 15.0, -30.0), Vector4(1.0, 1.0, 1.0, 1.0));
	p->setTimeAlive(5.0);
	p->setMass(5.0);
	setParticle(p);
}

void CircleGenerator::generateParticle(list<Particle*>& l) {

	auto random = rand() % 30 + 10;
	double angle = 90;
	for (int i = 0; i < nParticles; i++)
	{
		auto v = Vector3(random*cos(angle * PI / 180.0), 0, random * sin(angle * PI / 180.0));

		Vector3 posFinal = Vector3(molde->getPosition().x, molde->getPosition().y, molde->getPosition().z);
		Vector3 velFinal = Vector3(molde->getVelocity().x + v.x, molde->getVelocity().y + v.y, molde->getVelocity().z + v.z);
		auto p = molde->clone();
		p->setPosition(posFinal);
		p->setVelocity(velFinal);
		p->setMass(molde->getMass());
		p->setIsFire(molde->isFire());
		p->setTimeAlive(molde->getTimeAlive());
		l.push_back(p);
		angle += 360.0 / nParticles;
	}
	changeActive();
}