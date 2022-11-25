#include "AnchoredSpringFG.h"

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const Vector3& anchor_pos):SpringForceGenerator(k, resting, nullptr)
{
	other_ = new Particle(anchor_pos, { 0,0,0 }, { 1.0, 1.0, 0.0, 1.0 });
	other_->changeShapeToCube(0.75, other_->getTransform(), { 1.0, 1.0, 0.0, 1.0 });
	setName("AnchoredSpring");
}
