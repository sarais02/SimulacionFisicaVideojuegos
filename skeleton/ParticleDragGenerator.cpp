#include "ParticleDragGenerator.h"


ParticleDragGenerator::ParticleDragGenerator(const float k1, const float k2): _k1(k1), _k2(k2) {
	setName("Drag");
}

void ParticleDragGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getInverseMass()) < t || !isActive()) return;
	p->addForce(calculateDrag(p->getVelocity()));
}

Vector3 ParticleDragGenerator::calculateDrag(Vector3 v) {
	float drag_coef = v.normalize();
	Vector3 dragF;
	drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
	dragF = -v * drag_coef;
	return dragF;
}