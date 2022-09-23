#include "Proyectil.h"

Proyectil::Proyectil() :Particle(Vector3(0), Vector3(0)) {
	setType(Particle::type::PROYECTIL);
}
Proyectil::Proyectil(Vector3 Pos, Vector3 Vel) :Particle(Pos, Vel) {
	setType(Particle::type::PROYECTIL);
}
Proyectil::Proyectil(Vector3 Pos, Vector3 Vel, Vector4 COLOR) :Particle(Pos, Vel, COLOR) {
	setType(Particle::type::PROYECTIL);
}
Proyectil::Proyectil(Vector3 Pos, Vector3 Vel, Vector3 acceleration, double Damping) : Particle(Pos, Vel, acceleration, Damping) {
	setType(Particle::type::PROYECTIL);
}