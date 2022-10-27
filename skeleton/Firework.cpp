#include "Firework.h"
#include<conio.h>
#include<math.h>

Firework::Firework() :Particle() {
};
Firework::Firework(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping, double size, FIREWORK_TYPE type_, Vector4 COLOR, int generations)
	:Particle(Pos,Vel,COLOR,acceleration,Damping),type(type_), nGenerations(generations) {

}
Firework* Firework::clone() const {
	Firework* newRK=new Firework(pos.p,vel,a,damping,size_,type, color, nGenerations);
	newRK->setTimeAlive(iniTimeAlive);
    newRK->setGenerations(2);
	for (auto it : gens)newRK->addGenerator(it);
	return newRK;
}

void Firework::explode(list<Particle*>& list){
    switch (type)
    {
    case Firework::BASIC:
        for (auto it : gens) {
            auto pt = clone();
            pt->deleteGens();
            pt->setAcceleration(Vector3(0, -10, 0));
            pt->setVelocity(pt->getVelocity()/2);
            auto r1 = rand() % 255 + 0;
            auto r2 = rand() % 255 + 0;
            auto r3 = rand() % 255 + 0;
            pt->setColor(Vector4(r1/255.0, r2 / 255.0, r3 / 255.0, 1.0));
            (*it).setNParticles(20);
            if (pt->getSize() > 0.5)pt->setSize(0.5);
            (*it).setParticle(pt);
            (*it).generateParticle(list);
            pt->setPosition(Vector3(10000, 100000, 0));
        }
        break;
    case Firework::LINEAR:
        for (auto it : gens) {
            Firework* pt = clone(); 
            pt->setAcceleration(Vector3(1, -10 / (nGenerations + 1), 1));
            pt->type = BASIC;
            pt->setVelocity(pt->getVelocity());
            auto r1 = rand() % 255 + 0;
            auto r2 = rand() % 255 + 0;
            auto r3 = rand() % 255 + 0;
            pt->setColor(Vector4(r1 / 255.0, r2 / 255.0, r3 / 255.0, 1.0));
            (*it).setParticle(pt);
            (*it).setNParticles(20);
            if (pt->getSize() > 0.5)pt->setSize(0.5);
            pt->setTimeAlive(getIniTimeAlive() / 2);
            (*it).generateParticle(list);
            pt->setPosition(Vector3(10000, 100000, 0));
        }
        break;
    case Firework::CIRCULAR:
        for (auto it : gens) {
            auto pt = clone(); pt->setAcceleration(Vector3(0, -15, 0));
            pt->setVelocity(pt->getVelocity() * 2);
            pt->setGenerations(getGenerations() + 1);
            if (getGenerations() >= 2) pt->type = BASIC;
            auto r1 = rand() % 255 + 0;
            auto r2 = rand() % 255 + 0;
            auto r3 = rand() % 255 + 0;
            pt->setColor(Vector4(r1 / 255.0, r2 / 255.0, r3 / 255.0, 1.0));
            (*it).setNParticles(20);
            (*it).setParticle(pt);
            (*it).generateParticle(list);
            pt->setPosition(Vector3(10000, 100000, 0));
        }
        break;
    }
}
