#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <memory>

using namespace std;

class Particle
{
public:

	Particle(Vector3 Pos, Vector3 Vel);
	~Particle();

	void integrate(double t); //Actualiza posicion

private:

	Vector3 vel;
	physx::PxTransform pos;
	unique_ptr<RenderItem>renderitem;
};

