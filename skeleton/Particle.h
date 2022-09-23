#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <memory>

using namespace std;

class Particle
{
	

public:

	Particle(Vector3 Pos, Vector3 Vel);
	Particle(Vector3 Pos, Vector3 Vel, Vector4 c);
	Particle(Vector3 Pos, Vector3 Vel, Vector3 a, double dam);

	~Particle();

	void integrate(double t); //Actualiza posicion
	inline void setVelocity(Vector3 Vel) {
		vel = Vel;
	}
	inline void setMass(double mass_) {
		mass = mass_;
	}
	inline void setAcceleration(Vector3 acc) {
		a = acc;
	}
	inline void setDamping(double damping_) {
		damping = damping_;
	}
	enum type { UNUSED = 0, PROYECTIL };
	inline void setType(type t) {
		type_ = t;
	}

private:

	Vector3 vel;
	Vector3 a;
	Vector4 color;
	physx::PxTransform pos;
	unique_ptr<RenderItem>renderitem;
	double damping;
	double mass;
	type type_;
};

