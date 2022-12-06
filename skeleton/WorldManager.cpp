#include "WorldManager.h"
#include "SolidRigidGenerator.h"
#include "GaussianSolidRigidGen.h"

WorldManager::WorldManager(PxScene* gScene, PxPhysics* gPhysics):gScene(gScene), gPhysics(gPhysics)
{
	rStatic_list = list<SolidRigid*>();
	rDynamic_list = list<SolidRigid*>();

	max_Particles = 500;

	rfr = new RigidForceRegistry();
	addStaticBox({ 0,0,0 }, { 100,1,100 }, { 1.0,1.0,1.0,1.0 });
	//addDynamicBall({ 0,50,0 }, 5.0, { 10,-10,0 }, { 1.0,1.0,1.0,1.0 });	

	addStaticBox({ 100,0,0 }, { 1,50,100 }, { 1.0,1.0,1.0,1.0 });
	addStaticBox({ -100,0,0 }, { 1,50,100 }, { 1.0,1.0,1.0,1.0 });
	addStaticBox({ 0,0,100 }, { 100,50,1 }, { 1.0,1.0,1.0,1.0 });
	addStaticBox({ 0,0,-100 }, { 100,50,1 }, { 1.0,1.0,1.0,1.0 });
}

WorldManager::~WorldManager() {
	while (!rDynamic_list.empty()) {
		eliminarCuerpo(rDynamic_list.front());
		delete rDynamic_list.front()->item;
		rDynamic_list.pop_front();
	}
	while (!rStatic_list.empty()) {
		eliminarCuerpo(rStatic_list.front());
		delete rStatic_list.front()->item;
		rStatic_list.pop_front();
	}
	list_forces.clear();
	delete rfr;
}

void WorldManager::update(double duration) {
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

	for (auto g = generators_list.begin(); g != generators_list.end(); ++g)
	{
		if ((*g)->isActive()) {
			if((*g)->isDynamic())
				(*g)->generateSolidRigid(rDynamic_list);
			else
				(*g)->generateSolidRigid(rStatic_list);
		}
	}

	rfr->updateForces(duration);
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
	//Plane as a solid rigid
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
	/*auto it = shared_ptr<ForceGenerator>(new ExplosionForceGenerator(200, 1000, { 0,50,0 }, 5.0));
	list_forces.push_back(it); it->setName("ExplosionForceGenerator"); it->setActive(false);
	rfr->addRegistry(it, new_solid);*/
	rDynamic_list.push_back(rg);
}

shared_ptr<SolidRigidGenerator> WorldManager::getParticleGen(string name)
{
	for (auto it = generators_list.begin(); it != generators_list.end(); it++)
		if ((*it)->getName() == name) return (*it);

	return nullptr;
}

void WorldManager::generateSystem()
{
	shared_ptr<SolidRigidGenerator> p = getParticleGen("FirstSystem");
	if (p != nullptr)
		p->changeActive();
	else {
		auto s = new GaussianSolidRigidGen(Vector3(25.0, 5.0, 25.0), Vector3(4.0, 1.0, 4.0),  0.8, gScene, gPhysics, this);
		s->setName("FirstSystem");
		s->setNParticles(10);

		PxRigidDynamic* new_solid;
		new_solid = gPhysics->createRigidDynamic(PxTransform(Vector3(00.0, 50.0, 0.0)));
		new_solid->setLinearVelocity(Vector3(5.0, -15.0, 5.0));
		new_solid->setAngularVelocity({ 0,0,0 });
		new_solid->setMass(0.00005);

		auto shape = CreateShape(PxSphereGeometry(1.0)); new_solid->attachShape(*shape);

		
		auto x = new RenderItem(shape, new_solid, { 0,0,1,1 });
		

		SolidRigid* rg = new SolidRigid();
		rg->solidType = new_solid; rg->item = x; rg->timeAlive = 5;
		s->setSolidRigid(rg);
		
		generators_list.push_back(shared_ptr<SolidRigidGenerator>(s));
	}
}
