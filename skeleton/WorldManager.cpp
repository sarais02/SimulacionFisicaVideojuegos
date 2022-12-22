#include "WorldManager.h"
#include "PxRigidStatic.h"
#include "SolidRigidGenerator.h"
#include "GaussianSolidRigidGen.h"
#include "TorqueForce.h"
#include "ColorHSV.h"

WorldManager::WorldManager(PxScene* gScene, PxPhysics* gPhysics, Camera* cam) :gScene(gScene), gPhysics(gPhysics), myCamera(cam), max_vel(20.5)
{
	rStatic_list = list<SolidRigid*>();
	rDynamic_list = list<SolidRigid*>();

	forces_list = list<shared_ptr<ForceGenerator>>();
	generators_list = list<shared_ptr<SolidRigidGenerator>>();

	max_Particles = 350; currentNParticles = 0; level = "level1";

	rfr = new RigidForceRegistry();
	particleSystem = new ParticleSystem();

	level1();
}

WorldManager::~WorldManager() {

	while (!rDynamic_list.empty()) {
		eliminarCuerpo(rDynamic_list.front());
		rDynamic_list.pop_front();
	}
	while (!rStatic_list.empty()) {
		eliminarCuerpo(rStatic_list.front());
		rStatic_list.pop_front();
	}
	forces_list.clear();
	generators_list.clear();
	rfr->deleteRigidRegistry(player);
	delete rfr;
}

void WorldManager::update(double duration) {

	if (timer_fireworks > 0.0) timer_fireworks -= duration;
	if (timer_fireworks < 0.0) {
		timer_fireworks = 0.0;
		clearLevel();
		nextLevel();
	}
	rfr->updateForces(duration);
	auto f = particleSystem->getParticleGen("FlamesSystem");

	if (player->getGlobalPose().p.y < -30 || !f->isActive()) resetPlayer(); //Reseteo del jugador
	f->getMolde()->setPosition(player->getGlobalPose().p); //El fuego sigue al jugador

	myCamera->setEye(player->getGlobalPose().p, 5, 150); //Update de la camara

	particleSystem->update(duration, player->getGlobalPose().p);

	for (auto t = generators_list.begin(); t != generators_list.end(); t++) {
		auto gen = *t;
		if (gen->isActive()) {
			if (gen->type == STATIC) gen->generateSolidRigid(rStatic_list);
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
		if (cuerpo->timeAlive == -1) pa++;
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
	//rfr->deleteRigidRegistry(static_cast<PxRigidDynamic*>(cuerpo->solidType));
	cuerpo->solidType->release();
	DeregisterRenderItem(cuerpo->item);
	delete cuerpo;
	currentNParticles--;
}

void WorldManager::addStaticBox(Vector3 pos, Vector3 tam, Vector4 color, char* name) {
	
	PxRigidStatic* obj = gPhysics->createRigidStatic(PxTransform(pos));
	PxShape* shape = CreateShape(PxBoxGeometry(tam), gPhysics->createMaterial(0, 0, 0.0)); obj->setName(name);
	obj->attachShape(*shape);
	auto item = new RenderItem(shape, obj, color);
	gScene->addActor(*obj);
	SolidRigid* rg = new SolidRigid();
	rg->solidType = obj; rg->timeAlive = -1; rg->item = item;
	rStatic_list.push_back(rg);
}

void WorldManager::addStaticIce(Vector3 pos, Vector3 tam, Vector4 color, char* name)
{
	PxRigidStatic* obj = gPhysics->createRigidStatic(PxTransform(pos));
	PxShape* shape = CreateShape(PxBoxGeometry(tam), gPhysics->createMaterial(50.0, 100.0, 1.0)); obj->setName(name);
	obj->attachShape(*shape);
	auto item = new RenderItem(shape, obj, color);
	gScene->addActor(*obj);
	SolidRigid* rg = new SolidRigid();
	rg->solidType = obj; rg->timeAlive = -1; rg->item = item;
	rStatic_list.push_back(rg);
}

void WorldManager::addPlayer(Vector3 pos, float tam, Vector4 color) {
	
	player = gPhysics->createRigidDynamic(PxTransform(pos));
	player->setLinearVelocity(Vector3(0.0, 0.0, 0.0)); player->setAngularVelocity({ 0,0,0 });
	player->setAngularDamping(0.0); player->setLinearDamping(0.5);
	player->setName("player");
	player->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z);
	
	auto shape = CreateShape(PxSphereGeometry(tam), gPhysics->createMaterial(5, 10, 0.0)); player->attachShape(*shape);
	
	PxRigidBodyExt::setMassAndUpdateInertia(*player, 10.0);
	hsv col = { 24.0f, 0.9f, 0.55f };
	rgb rgb = hsv2rgb(col);
	Vector4 colo = { rgb.r, rgb.g, rgb.b, 1.0 };
	auto x = new RenderItem(shape, player, colo);

	gScene->addActor(*player);
	
	SolidRigid* rg = new SolidRigid();
	rg->solidType = player; rg->timeAlive = -1; rg->item = x;

	rDynamic_list.push_back(rg);

	particleSystem->generateFlamesSystem();
}


void WorldManager::nextLevel()
{
	if (level == "level1") { //Si estoy en el nivel 1
		level = "level2";
		level2();
	}
	else { //Si estoy en el nivel 2
		level = "level1";
		level1();
	}
}

void WorldManager::level1()
{
	gameover = false;
	particleSystem->generateForces();
	particleSystem->createWinds();
	particleSystem->generateRain();
	particleSystem->generateWhirlSystem();
	addPlayer({ 0 ,4,0 }, 3.5, { 0.0, 0.0, 1.0, 1.0 });
	myCamera->setEye(player->getGlobalPose().p, 5, 150);

	Vector4 color_plataformas = { 1.0, 165.0 / 255, 0.0, 1.0 };
	Vector4 color1 = { 234.0 / 255, 137.0 / 255, 154.0 / 255, 1.0 };
	Vector4 color2 = { 189.0 / 255, 236.0 / 255, 182.0 / 255, 1.0 };

	addStaticBox({ 00,0,0 }, { 50,1,50 }, { color1 });
	addStaticBox({ 80,0,0 }, { 30,10,50 }, color2);

	addStaticBox({ 120,25,0 }, { 10,2,20 }, color_plataformas);

	addStaticBox({ 150,40,0 }, { 10,2,20 }, color_plataformas);
	addStaticBox({ 180,60,0 }, { 10,2,20 }, color_plataformas);
	addStaticBox({ 200,0,0 }, { 100,5,50 }, color2); //Viento

	addStaticBox({ 310,25,0 }, { 10,1,5 }, color_plataformas);
	addStaticBox({ 340,40,0 }, { 10,1,5 }, color_plataformas);
	addStaticBox({ 340,-1,0 }, { 40,1,50 }, color1);//Torbellino
	addStaticBox({ 400,0,0 }, { 30,10,50 }, color2);

	addStaticBox({ 450,35,0 }, { 15,2,20 }, color_plataformas);
	addStaticBox({ 530,50,0 }, { 30,2,20 }, color_plataformas);
	addStaticBox({ 530,0,0 }, { 100,5,50 }, color2); //Viento

	addStaticBox({ 750,60,0 }, { 10,1,5 }, color_plataformas);
	addStaticBox({ 710,40,0 }, { 10,1,5 }, color_plataformas);
	addStaticBox({ 690,20,0 }, { 10,1,5 }, color_plataformas);
	addStaticBox({ 800,0,0 }, { 200,3,50 }, color1);
	addFlag({ 880,14, 0 });
}

void WorldManager::level2()
{
	gameover = false;
	particleSystem->generateForces();
	particleSystem->createWinds2();
	particleSystem->generateWhirlSystem();
	addPlayer({ 0 ,4,0 }, 3.5, { 0.0, 0.0, 1.0, 1.0 });
	myCamera->setEye(player->getGlobalPose().p, 5, 150);

	Vector4 color_plataformas = { 1.0, 165.0 / 255, 0.0, 1.0 };
	Vector4 color1 = { 234.0 / 255, 137.0 / 255, 154.0 / 255, 1.0 };
	Vector4 color2 = { 189.0 / 255, 236.0 / 255, 182.0 / 255, 1.0 };
	Vector4 hielo = { 178.0 / 255, 218.0 / 255, 250.0 / 255, 1.0 };
	Vector4 agua = {.0 , .0, 1.0, 1.0 };


	addStaticBox({ -10,0,0 }, { 70,1,50 }, { color1 });
	
	//AGUA Y FLOTACION
	auto f1 = shared_ptr<BuoyancyForceGenerator>(new BuoyancyForceGenerator(8, 0.03, 1000, {80, -20,0}));
	forces_list.push_back(f1);

	PxRigidDynamic* obj = gPhysics->createRigidDynamic(PxTransform({ 80,5,0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(Vector3(8, 3, 6)), gPhysics->createMaterial(0, 0, 0.0)); obj->setName("suelo");
	obj->attachShape(*shape);
	obj->setLinearVelocity(Vector3(0.0, 0.0, 0.0));obj->setAngularVelocity({ 0,0,0 });
	obj->setAngularDamping(0.0); obj->setLinearDamping(0.999);
	obj->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z | PxRigidDynamicLockFlag::eLOCK_LINEAR_X);
	auto item = new RenderItem(shape, obj, color_plataformas);
	gScene->addActor(*obj);
	SolidRigid* rg = new SolidRigid();
	rg->solidType = obj; rg->timeAlive = -1; rg->item = item;
	rDynamic_list.push_back(rg);

	rfr->addRegistry(f1, obj);

	auto f2 = shared_ptr<BuoyancyForceGenerator>(new BuoyancyForceGenerator(8, 0.03, 1000, { 110, -20,0 }));
	forces_list.push_back(f2);

	PxRigidDynamic* obj2 = gPhysics->createRigidDynamic(PxTransform({ 110,5,0 })); obj2->setName("suelo");
	obj2->attachShape(*shape);
	obj2->setLinearVelocity(Vector3(0.0, 0.0, 0.0)); obj2->setAngularVelocity({ 0,0,0 });
	obj2->setAngularDamping(0.0); obj2->setLinearDamping(0.999);
	obj2->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z | PxRigidDynamicLockFlag::eLOCK_LINEAR_X);
	item = new RenderItem(shape, obj2, color_plataformas);
	gScene->addActor(*obj2);
	rg = new SolidRigid();
	rg->solidType = obj2; rg->timeAlive = -1; rg->item = item;
	rDynamic_list.push_back(rg);

	rfr->addRegistry(f2, obj2);

	auto f3 = shared_ptr<BuoyancyForceGenerator>(new BuoyancyForceGenerator(8, 0.03, 1000, { 140, -20,0 }));
	forces_list.push_back(f3);

	PxRigidDynamic* obj3 = gPhysics->createRigidDynamic(PxTransform({ 140,5,0 })); obj3->setName("suelo");
	obj3->attachShape(*shape);
	obj3->setLinearVelocity(Vector3(0.0, 0.0, 0.0)); obj3->setAngularVelocity({ 0,0,0 });
	obj3->setAngularDamping(0.0); obj3->setLinearDamping(0.999);
	obj3->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z | PxRigidDynamicLockFlag::eLOCK_LINEAR_X);
	item = new RenderItem(shape, obj3, color_plataformas);
	gScene->addActor(*obj3);
	rg = new SolidRigid();
	rg->solidType = obj3; rg->timeAlive = -1; rg->item = item;
	rDynamic_list.push_back(rg);

	rfr->addRegistry(f3, obj3);

	addStaticBox({ 220,0,0 }, { 60,10,50 }, color2);
	
	addStaticBox({ 300,35,0 }, { 17,2,5 }, color_plataformas);
	addStaticBox({ 350,0,0 }, { 70,5,50 }, color1); //Viento
	
	addStaticBox({ 550,30,0 }, { 12,1,5 }, color_plataformas);
	addStaticBox({ 575,50,0 }, { 12,1,5 }, color_plataformas);
	addStaticIce({ 500,0,0 }, { 200,2,40 }, hielo);

	addStaticBox({ 720,0,0 }, { 35,10,50 }, color2);
	addFlag({725,14, 0 });
}

void WorldManager::clearLevel()
{
	while (!rStatic_list.empty())
	{
		eliminarCuerpo(rStatic_list.front());
		rStatic_list.pop_front();
	}

	while (!rDynamic_list.empty())
	{
		eliminarCuerpo(rDynamic_list.front());
		rDynamic_list.pop_front();
	}

	forces_list.clear();
	delete rfr;
	rfr = new RigidForceRegistry();

	generators_list.clear();

	particleSystem->clearlevel();
}

void WorldManager::movePlayer()
{
	player->setLinearVelocity(player->getLinearVelocity() + Vector3(5.5, 0, 0));
	if (player->getLinearVelocity().x > max_vel) player->setLinearVelocity({ max_vel, player->getLinearVelocity().y, player->getLinearVelocity().z });
}

void WorldManager::jumpPlayer(){
	if (jump) return;
	player->setLinearVelocity(player->getLinearVelocity() + Vector3(0,30,0));
	jump = true;
}

void WorldManager::resetPlayer()
{
	player->setGlobalPose({ 0 , 6, 0 });
	player->setLinearVelocity(Vector3(0.0, 0.0, 0.0));
	player->setAngularVelocity({ 0,0,0 });
	
	particleSystem->resetFire();
}

void WorldManager::addFlag(Vector3 pos)
{
	//Palo
	PxRigidStatic* obj = gPhysics->createRigidStatic(PxTransform(pos));
	PxShape* shape = CreateShape(PxBoxGeometry(Vector3(1, 10, 1)));
	obj->attachShape(*shape); obj->setName("palo");
	auto item = new RenderItem(shape, obj, Vector4(1.0,  1.0, 1.0, 1.0));
	gScene->addActor(*obj);
	SolidRigid* rg = new SolidRigid();
	rg->solidType = obj; rg->timeAlive = -1; rg->item = item;
	rStatic_list.push_back(rg);
	
	//Bandera
	pos.z += 5;
	pos.y += 10;
	particleSystem->generateFlag(pos);
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