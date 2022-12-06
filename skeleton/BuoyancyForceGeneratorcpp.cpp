#include "BuoyancyForceGenerator.h"

BuoyancyForceGenerator::BuoyancyForceGenerator(float h, float v, float d) : height(h), volume(v), density(d)
{
    liquid_particle = new Particle({ 0.0, 0.0, 0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 0.0, 0.0, 0.0 });
    liquid_particle->changeShapeToPlane(liquid_particle->getTransform(), { 0.0, 0.0, 1.0, 1.0 });
    setName("Flotacion");
}

BuoyancyForceGenerator::~BuoyancyForceGenerator()
{
    delete liquid_particle;
    liquid_particle = nullptr;
}

void BuoyancyForceGenerator::updateForce(Particle* p, double t)
{
    if (!isActive()) return;

    float h = p->getPosition().y;
    float h0 = liquid_particle->getPosition().y;

    Vector3 f(0, 0, 0);
    float immersed = 0.0;

    if (h - h0 > height * 0.5)
    {
        immersed = 0.0;
    }
    else if (h0 - h > height * 0.5) {
        immersed = 1.0;
    }
    else
    {
        immersed = (h0 - h) / height + 0.5;
    }

    f.y = density * volume * immersed * gravity;

    p->addForce(f);
}

void BuoyancyForceGenerator::updateForce(PxRigidDynamic* p, double t)
{
    if (!isActive()) return;

    float h = p->getGlobalPose().p.y;
    float h0 = liquid_particle->getPosition().y;

    Vector3 f(0, 0, 0);
    float immersed = 0.0;

    if (h - h0 > height * 0.5)
    {
        immersed = 0.0;
    }
    else if (h0 - h > height * 0.5) {
        immersed = 1.0;
    }
    else
    {
        immersed = (h0 - h) / height + 0.5;
    }

    f.y = density * volume * immersed * gravity;

    p->addForce(f);
}
