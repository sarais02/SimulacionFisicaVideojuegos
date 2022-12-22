#pragma once
#include "PxPhysics.h"
#include "PxScene.h"
#include "RenderUtils.hpp"
#include "RigidForceRegistry.h"
#include "ParticleSystem.h"

using namespace std;
using namespace physx;
class SolidRigidGenerator;

struct SolidRigid {
	PxRigidActor* solidType;
	RenderItem* item;
	double timeAlive;
	vector<string>forcesNames;
	double tam;
};

class WorldManager {
public:
	WorldManager(PxScene* gScene, PxPhysics* gPhysics, Camera* cam);
	~WorldManager();

	void update(double t);
	void eliminarCuerpo(SolidRigid* s);
	inline int getNParticles() { return currentNParticles; }
	inline int getMaxParticles() { return max_Particles; }
	inline void setTimer(double t) { timer_fireworks = t; }

	//AÑADIR
	inline bool canGenerateObject() { return max_Particles > currentNParticles; }
	inline void addParticles() { currentNParticles++; }
	void addStaticBox(Vector3 pos, Vector3 tam, Vector4 color,char* name="suelo");
	void addStaticIce(Vector3 pos, Vector3 tam, Vector4 color,char* name="suelo");
	void addPlayer(Vector3 pos, float tam, Vector4 color);
	void addFlag(Vector3 pos);
	void resetPlayer();

	//INPUT
	void movePlayer();
	void jumpPlayer();
	inline bool getJump() { return jump; };
	inline void setJump(bool salto) { jump = salto; }

	//NIVELES
	void nextLevel();
	void level1();
	void level2();
	void clearLevel();
	inline void fireworks() {
		Vector3 pos = player->getGlobalPose().p;
		particleSystem->shootFirework(Firework::CIRCULAR, {pos.x, pos.y-60, pos.z+60});
	}
	inline void gameOver(bool g) { gameover = g; }
	inline bool isGameOver() { return gameover; }
	
	//OBTENER GENERADORES
	shared_ptr<SolidRigidGenerator> getParticleGen(string name);
	shared_ptr<ForceGenerator> getForceGen(string name);

protected:
	PxScene* gScene;
	PxPhysics* gPhysics;

	list<SolidRigid*> rStatic_list;
	list<SolidRigid*> rDynamic_list;
	list<shared_ptr<ForceGenerator>> forces_list;
	list<shared_ptr<SolidRigidGenerator>> generators_list;

	RigidForceRegistry* rfr;
	Camera* myCamera;
	ParticleSystem* particleSystem;
	PxRigidDynamic* player;

	bool gameover = false, jump = true;
	int max_Particles, currentNParticles;
	float max_vel;
	double timer_fireworks = 0.0;
	string level;
};