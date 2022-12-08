#include "WorldManager.h"
#include "PxRigidStatic.h"
#include "SolidRigidGenerator.h"
#include "ExplosionForceGenerator.h"
#include "GaussianSolidRigidGen.h"
#include "WindGenerator.h"
#include "TorqueForce.h"

WorldManager::WorldManager(PxScene* gScene, PxPhysics* gPhysics) :gScene(gScene), gPhysics(gPhysics)
{
	rStatic_list = list<SolidRigid*>();
	rDynamic_list = list<SolidRigid*>();

	forces_list = list<shared_ptr<ForceGenerator>>();
	generators_list = list<shared_ptr<SolidRigidGenerator>>();

	max_Particles = 1500;
	currentNParticles = 0;

	rfr = new RigidForceRegistry();

	addStaticBox({ 0,0,0 }, { 100,1,100 }, { 1.0,1.0,1.0,1.0 });
	//addDynamicBall({ 0,50,0 }, 5.0, { 10,-10,0 }, { 1.0,1.0,1.0,1.0 });	

	addStaticBox({ 100,0,0 }, { 1,50,100 }, { 1.0,1.0,1.0,1.0 });
	addStaticBox({ -100,0,0 }, { 1,50,100 }, { 1.0,1.0,1.0,1.0 });
	addStaticBox({ 0,0,100 }, { 100,50,1 }, { 1.0,1.0,1.0,1.0 });
	addStaticBox({ 0,0,-100 }, { 100,50,1 }, { 1.0,1.0,1.0,1.0 });

	auto viento = shared_ptr<WindGenerator>(new WindGenerator(5.0, 5.0, { -10,0, 0 }, 50, { 0, 0, 0 }));
	forces_list.push_back(viento);

	auto tor = shared_ptr<ForceGenerator>(new TorqueForce({ 0,0,0 }, 10, 100));
	forces_list.push_back(tor);
}

WorldManager::~WorldManager() {
	while (!rDynamic_list.empty()) {
		eliminarCuerpo(rDynamic_list.front());
		//delete rDynamic_list.front()->item;
		rDynamic_list.pop_front();
	}
	while (!rStatic_list.empty()) {
		eliminarCuerpo(rStatic_list.front());
		//delete rStatic_list.front()->item;
		rStatic_list.pop_front();
	}
	forces_list.clear();
	generators_list.clear();
	delete rfr;
}

void WorldManager::update(double duration) {
	rfr->updateForces(duration);

	for (auto t = generators_list.begin(); t != generators_list.end(); t++) {
		auto gen = *t;
		if (gen->isActive()) {
			if (gen->type == STATIC)gen->generateSolidRigid(rStatic_list);
			else gen->generateSolidRigid(rDynamic_list);
		}
	}

	auto p = rDynamic_list.begin();
	while (p != rDynamic_list.end()) {
		auto cuerpo = *p;
		if (cuerpo->timeAlive == -1) p++;
		else {
			cuerpo->timeAlive -= duration;
			if (cuerpo->timeAlive <= 0) {
				eliminarCuerpo(cuerpo);
				p = rDynamic_list.erase(p);
			}
			else p++;
		}
	}
	auto pa = rStatic_list.begin();
	while (pa != rStatic_list.end()) {
		auto cuerpo = *pa;
		if (cuerpo->timeAlive == -1)pa++;
		else {
			cuerpo->timeAlive -= duration;
			if (cuerpo->timeAlive <= 0) {
				eliminarCuerpo(cuerpo);
				pa = rStatic_list.erase(pa);
			}
			else pa++;
		}
	}
}

void WorldManager::eliminarCuerpo(SolidRigid* cuerpo) {
	DeregisterRenderItem(cuerpo->item);
	delete cuerpo;
	currentNParticles--;
}

void WorldManager::setMaterialToObject(PxRigidActor* x, Vector3 mat) {
	dynamic_cast<PxRigidDynamic*>(x)->setMassSpaceInertiaTensor(mat);
}

void WorldManager::addStaticBox(Vector3 pos, Vector3 tam, Vector4 color) {
	if (!canGenerateObject())return;
	currentNParticles++;

	PxRigidStatic* obj = gPhysics->createRigidStatic(PxTransform(pos));
	PxShape* shape = CreateShape(PxBoxGeometry(tam));
	obj->attachShape(*shape);
	auto item = new RenderItem(shape, obj, color);
	gScene->addActor(*obj);
	SolidRigid* rg = new SolidRigid();
	rg->solidType = obj; rg->timeAlive = -1; rg->item = item;
	rStatic_list.push_back(rg);
}

void WorldManager::addDynamicBall(Vector3 pos, double tam, Vector3 vel, Vector4 color) {
	if (!canGenerateObject())return;
	currentNParticles++;

	PxRigidDynamic* new_solid;
	new_solid = gPhysics->createRigidDynamic(PxTransform(pos));
	new_solid->setLinearVelocity(vel);
	new_solid->setAngularVelocity({ 0,0,0 });

	auto shape = CreateShape(PxSphereGeometry(tam)); new_solid->attachShape(*shape);
	//new_solid->setMassSpaceInertiaTensor({ size.y * size.z,size.x * size.z,size.x * size.y });
	auto x = new RenderItem(shape, new_solid, { 0,0,1,1 });
	gScene->addActor(*new_solid);

	SolidRigid* rg = new SolidRigid();
	rg->solidType = new_solid; rg->timeAlive = -1; rg->item = x;
	//auto it = shared_ptr<ForceGenerator>(new ExplosionForceGenerator(200, 1000, { 0,50,0 }, 5.0));

	rDynamic_list.push_back(rg);
}

void WorldManager::changeActiveForces()
{
	for (auto it = forces_list.begin(); it != forces_list.end(); it++) {
		(*it)->changeActive();
	}
}

void WorldManager::generateSystem()
{
	shared_ptr<SolidRigidGenerator> p = getParticleGen("FirstSystem");

	if (p != nullptr)
		p->changeActive();

	else {
		auto s = new GaussianSolidRigidGen(this, DYNAMIC, { 50,10,50 }, { 4,1,4 },0.8, gScene, gPhysics, false);
		s->setName("FirstSystem");
		s->setNParticles(10);

		PxRigidDynamic* molde;
		molde = gPhysics->createRigidDynamic(PxTransform({ 0,50,0 }));
		molde->setLinearVelocity({ 0,/*-15*/0,0 });
		molde->setAngularVelocity({ 0,0,0 });
		//molde->setMass(0.00005);

		double size = 2.0;

		auto shape = CreateShape(PxSphereGeometry(size));
		molde->attachShape(*shape);
		//new_solid->setMassSpaceInertiaTensor({ size.y * size.z,size.x * size.z,size.x * size.y });
		auto iten = new RenderItem(shape, molde, { 0,0,1,1 });

		SolidRigid* rg = new SolidRigid();
		rg->solidType = molde; rg->timeAlive = 15; rg->item = iten; /*rg->tam = size*/;
		s->setSolidRigid(rg);
		s->addParticleForceRegistry(rfr);
		rfr->addRegistry(getForceGen("Viento"), molde);
		rg->forcesNames.push_back("Viento");
		generators_list.push_back(shared_ptr<SolidRigidGenerator>(s));
	}
}

void WorldManager::generateTorqueSystem()
{
	shared_ptr<SolidRigidGenerator> p = getParticleGen("TorqueSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = shared_ptr<SolidRigidGenerator>(new GaussianSolidRigidGen(this, DYNAMIC, { -50,1,50 }, { 50,0,50 }, 1.0, gScene, gPhysics, true)); 
		s->setName("TorqueSystem");
		s->setNParticles(10);
		
		PxRigidDynamic* molde;
		molde = gPhysics->createRigidDynamic(PxTransform({ 0,50,0 }));
		molde->setLinearVelocity({ 0,0,0 });
		molde->setAngularVelocity({ 0,0,0 });
		molde->setMass(60);

		double size = 3.5;
		PxShape* shape = CreateShape(PxBoxGeometry(Vector3(size, size, size)));
		//auto shape = CreateShape(PxSphereGeometry(size));
		molde->attachShape(*shape);
		
		auto item = new RenderItem(shape, molde, { 0,0,1,1 });

		SolidRigid* rg = new SolidRigid();
		rg->solidType = molde; rg->timeAlive = 30; rg->item = item;
		s->setSolidRigid(rg);
		s->addParticleForceRegistry(rfr);
		rfr->addRegistry(getForceGen("Torque"), molde);
		rg->forcesNames.push_back("Torque");

		generators_list.push_back(shared_ptr<SolidRigidGenerator>(s));
	}
}

shared_ptr<SolidRigidGenerator> WorldManager::getParticleGen(string name)
{
	for (auto it = generators_list.begin(); it != generators_list.end(); it++)
		if ((*it)->getName() == name) return (*it);

	return nullptr;
}

shared_ptr<ForceGenerator> WorldManager::getForceGen(string name)
{
	for (auto it = forces_list.begin(); it != forces_list.end(); it++)
		if ((*it)->getName() == name) return (*it);

	return nullptr;
}