#include "Particle.h"


Particle::Particle(Vector3 Pos, Vector3 Vel, Vector4 c, Vector3 a_, double dam, double size, double mass) : vel(Vel), pos(Pos), isFire_(false),
																								color(c), a(a_), damping(dam), size_(size), mass(mass), inverse_mass(1.0/mass)
{
	renderitem = new RenderItem(CreateShape(physx::PxSphereGeometry(size_)), &pos, c);
}


Particle::~Particle()
{
	DeregisterRenderItem(renderitem);
}

void Particle::integrate(double t)
{
	pos.p += vel * t;
	Vector3 totalAcceleration = a;
	totalAcceleration += force * inverse_mass;
	// Update linear velocity
	vel += totalAcceleration* t;
	// Impose drag (damping)
	vel *= powf(damping, t);
	clearForce();
}

void Particle::changeSize(double s, physx::PxTransform *pos, Vector4 c) {
	//renderitem->shape->getGeometry().sphere().radius = s;
	//renderitem->shape->setGeometry(physx::PxSphereGeometry(s));
	DeregisterRenderItem(renderitem);
	renderitem = new RenderItem(CreateShape(physx::PxSphereGeometry(s)), pos, c);
}

bool Particle::update(double t) {
	if (isAlive(t)) { integrate(t); return true; }
	return false;
}

Particle* Particle::clone() const {
	Particle* p = new Particle(pos.p, vel, color, a, damping, size_);
	p->setTimeAlive(iniTimeAlive);
	p->setIsFire(isFire_);
	return p;
}

void Particle::clearForce() {
	force = Vector3(0);
}

void Particle::addForce(const Vector3& f) {
	force += f;
}