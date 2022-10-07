#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <memory>

using namespace std;

class Particle
{

public:

	Particle() {};
	Particle(Vector3 Pos, Vector3 Vel);
	Particle(Vector3 Pos, Vector3 Vel, Vector4 c);
	Particle(Vector3 Pos, Vector3 Vel, Vector3 a, double dam);

	~Particle();

	void integrate(double t); //Actualiza posicion
	inline void setVelocity(Vector3 Vel) {
		vel = Vel;
	}
	inline Vector3 getVelocity() {
		return vel;
	}
	inline void setPosition(Vector3 p) {
		pos.p = p;
	}
	inline Vector3 getPosition() {
		return pos.p;
	}
	inline void setMass(double mass_) {
		mass = mass_;
	}
	inline double getMass() {
		return mass;
	}
	inline void setAcceleration(Vector3 acc) {
		a = acc;
	}
	inline Vector3 getAcceleration() {
		return a;
	}
	inline void setDamping(double damping_) {
		damping = damping_;
	}
	inline double getDamping() {
		return damping;
	}
	enum type { UNUSED = 0, PROYECTIL };
	inline void setType(type t) {
		type_ = t;
	}

	inline void setTimeAlive(double t) {
		timeAlive = t;
	}
	inline double getTimeAlive() {
		return timeAlive;
	}

	inline bool isAlive(double t) {
		timeAlive -= t;
		return timeAlive > 0;
	};
	inline void setColor(Vector4 c) {
		color = c;
	}
	inline Vector4 getColor() {
		return color;
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
	double timeAlive;

};

