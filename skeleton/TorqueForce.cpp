#include "TorqueForce.h"

TorqueForce::TorqueForce(Vector3 punto, double k, double R) : punto(punto), k(k), R(R) {
	setName("Torque");
}
void TorqueForce::updateForce(PxRigidDynamic* solid, double duration) {
	if (!isActive() || fabs(solid->getInvMass()) < 1e-10 || !canUpdateForce(t)) return;

	auto pos = solid->getGlobalPose().p;

	auto difX = pos.x - punto.x;
	auto difY = pos.y - punto.y;
	auto difZ = pos.z - punto.z;

	auto r = pow(difX, 2) + pow(difY, 2) + pow(difZ, 2);

	if (r > pow(R, 2)) return;

	solid->addTorque(Vector3(difX, difY, difZ) * k);
}