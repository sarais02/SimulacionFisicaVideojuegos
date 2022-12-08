#pragma once
#include "ForceGenerator.h"

class TorqueForce : public ForceGenerator {
protected:
    Vector3 punto;
    double k, R;

public:
    TorqueForce(Vector3 punto, double k, double R);
    inline virtual void updateForce(Particle* particle, double duration) override {};
    virtual void updateForce(PxRigidDynamic* solid, double duration) override;
};