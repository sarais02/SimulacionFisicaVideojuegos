#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Particle.h"
#include "Proyectil.h"
#include "ParticleSystem.h"

#include <iostream>



using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;
Particle* particle = NULL;

vector<Particle*>proyectiles;
//CREAR PARTICLE SYSTEM
ParticleSystem* particleSystem;
// Initialize physics engine
// Se define todo lo que queremos que aparezca en la escena
// Reglas del mundo fisico
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f); //Crear material del suelo

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	particleSystem = new ParticleSystem();
	srand(time(NULL));
	//particle = new Particle(Vector3(0.0,20.0,0), Vector3(5.0,15.0,0.0), Vector3(0.0, -9.8, 0.0));
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
// Definir lo que pasa en cada frame 
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	for (auto shot:proyectiles)
	{
		if(shot!=nullptr)
			shot->integrate(t);
		//if (shot->pos.y < 0.0f || shot->startTime + 5000 < GetLastFrame() || shot->particle.getPosition().z > 200.0f) {
	}
	particleSystem->update(t);
	/*if(particle!=nullptr)
		particle->integrate(t);*/
}

// Function to clean data
// Add custom code to the begining of the function
// Evita memory leaks
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	delete particle; particle = nullptr;
	for (auto shot : proyectiles)
	{
		if (shot != nullptr) {
			delete shot;
			shot = nullptr;
		}
	}
	delete particleSystem;
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	//case 'B': break;
	//case ' ':	break;
	case ' ':
	{
		break;
	}
	case 'K': //Artillero
	{
		auto bullet=new Proyectil(camera.p, GetCamera()->getDir()*30);
		bullet->setAcceleration(Vector3(0.0, -20.0, 0.0));
		bullet->setDamping(0.99);
		bullet->setMass(200.0);
		proyectiles.push_back(bullet);
		break;
	}
	case 'H': //Bola de fuego
	{
		auto bullet = new Proyectil(camera.p, GetCamera()->getDir() * 30, Vector4(255 / 250.0, 128 / 250.0, 0.0, 1.0));
		bullet->setAcceleration(Vector3(0.0, 0.6, 0.0));
		bullet->setDamping(0.9);
		bullet->setMass(1.0);
		proyectiles.push_back(bullet);
		break;
	}
	case 'F': //Laser
	{
		auto bullet = new Proyectil(camera.p, GetCamera()->getDir() * 30, Vector4(135/250.0, 206 / 250.0, 250 / 250.0, 1.0));
		bullet->setAcceleration(Vector3(0.0, 0.0, 0.0));
		bullet->setDamping(0.99);
		bullet->setMass(0.1);
		proyectiles.push_back(bullet);
		break;
	}
	case 'M':
		particleSystem->generateHosepipeSystem(); //MANGUERA
		break;
	case 'N':
		particleSystem->generateFogSystem();	//NIEBLA
		break;
	case 'L':
		particleSystem->generateFlamesSystem(); //FUEGO
		break;
	case 'R':
		particleSystem->generateRocketSystem(); //FUEGOS ARTIFICIALES RANDOM
		break;
	case 'Q':
		particleSystem->shootFirework(Firework::BASIC); //FUEGOS ARTIFICIALES
		break;
	case 'E':
		particleSystem->shootFirework(Firework::LINEAR); //FUEGOS ARTIFICIALES
		break;
	case 'T':
		particleSystem->shootFirework(Firework::CIRCULAR); //FUEGOS ARTIFICIALES
		break;
	case 'U':
		particleSystem->generateWhirlSystem(); //TORBELLINO
		break;
	case 'G':
		particleSystem->generateGalaxy(); //GALAXIA
		break;
	case 'J':
		particleSystem->generateExplosionSystem(); //EXPLOSION
		break;
	case 'Y':
		particleSystem->activeExplosion(true);
		break;
	case 'I':
		particleSystem->activeExplosion(false);
		break;
	case 'C':
		particleSystem->generateCircleSystem(); //CIRCULAR
		break;
	case '+':
		particleSystem->increaseDesTip(Vector3(1.0, 1.0, 0.0));
		break;
	case '-':
		particleSystem->increaseDesTip(Vector3(-1.0, -1.0, 0.0));
		break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}