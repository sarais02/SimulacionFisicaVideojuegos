#include "Particle.h"

Particle::Particle(Vector3 Pos, Vector3 Vel): vel(Vel), pos(Pos)
{
	renderitem = make_unique<RenderItem>(CreateShape(physx::PxSphereGeometry(1.0)), &pos, Vector4(1.0, 0.0, 0.0, 1.0));
}

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 a_): vel(Vel), pos(Pos), a(a_)
{
	renderitem = make_unique<RenderItem>(CreateShape(physx::PxSphereGeometry(1.0)), &pos, Vector4(1.0, 0.0, 0.0, 1.0));
}

Particle::~Particle()
{
	renderitem.release(); renderitem = nullptr;
}

void Particle::integrate(double t)
{
	pos.p += vel * t;
	vel += a*t;
	vel *= powf(damping, t);
}
