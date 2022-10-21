#pragma once
#include "Particle.h"

class Proyectil :public Particle {

public:
    Proyectil();
    Proyectil(Vector3 Pos, Vector3 Vel);
    Proyectil(Vector3 Pos, Vector3 Vel, Vector4 c);
    Proyectil(Vector3 Pos, Vector3 Vel, Vector4 COLOR, Vector3 acceleration, double Damping);
    ~Proyectil();

};