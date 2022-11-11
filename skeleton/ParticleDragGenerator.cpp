#include "ParticleDragGenerator.h"


ParticleDragGenerator::ParticleDragGenerator(const float k1, const float k2): _k1(k1), _k2(k2) {
	setName("Drag");
}

void ParticleDragGenerator::updateForce(Particle* p, double t) {
	if (fabs(p->getInverseMass()) < 1e-10) return;

	Vector3 v = p->getVelocity();
	float drag_coef = v.normalize();
	Vector3 dragF;
	drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
	dragF = -v * drag_coef;

	//cout << dragF.x << "\t" << dragF.y << "\t" << dragF.z << "\n";
	p->addForce(dragF);
}

Vector3 ParticleDragGenerator::calculateDrag(Vector3 v) {
	float drag_coef = v.normalize();
	Vector3 dragF;
	drag_coef = _k1 * drag_coef + _k2 * drag_coef * drag_coef;
	dragF = -v * drag_coef;
	return dragF;
}