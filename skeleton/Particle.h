#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include <memory>

using namespace std;

class Particle
{
public:

	Particle() {};
	Particle(Vector3 Pos, Vector3 Vel, Vector4 c=Vector4(1.0, 1.0, 1.0, 1.0), Vector3 a=Vector3(0.0, -9.8, 0.0), 
		double dam=0.999, double size=1.0, double mass=5.0);
	
	~Particle();
	
	void integrate(double t); //Actualiza posicion
	virtual bool update(double t);
	void changeSize(double s, physx::PxTransform *pos, Vector4 c);
	virtual Particle* clone()const;
	void clearForce();
	void addForce(const Vector3& f);

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
	inline physx::PxTransform* getTransform() {
		return &pos;
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
		iniTimeAlive=timeAlive = t;
	}
	inline double getTimeAlive() {
		return timeAlive;
	}
	inline double getIniTimeAlive() {
		return iniTimeAlive;
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
	inline void setSize(double t) {
		size_ = t;
	}
	inline double getSize() {
		return size_;
	}
	inline bool isFire() {
		return isFire_;
	}
	inline void setIsFire(bool f) {
		isFire_ = f;
	}
	inline double getInverseMass() {
		return inverse_mass;
	}

protected:

	Vector3 vel, a;
	Vector3 force;
	Vector4 color;
	physx::PxTransform pos;
	//unique_ptr<RenderItem>renderitem;
	RenderItem *renderitem;
	double damping;
	double mass;
	double inverse_mass;
	type type_;
	double timeAlive, iniTimeAlive;
	double size_;
	bool isFire_;
};

