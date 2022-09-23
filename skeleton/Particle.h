#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <memory>

using namespace std;

class Particle
{
public:

	Particle(Vector3 Pos, Vector3 Vel);
	Particle(Vector3 Pos, Vector3 Vel, Vector3 a);
	~Particle();

	void integrate(double t); //Actualiza posicion

private:

	Vector3 vel;
	Vector3 a;
	physx::PxTransform pos;
	unique_ptr<RenderItem>renderitem;
	double damping = 0.999;
};

