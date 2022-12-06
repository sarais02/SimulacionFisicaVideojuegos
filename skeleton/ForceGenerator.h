#pragma once

#include "Particle.h"
#include <list>
#include <random>
#include <string>
#include <iostream>

using namespace physx;

class ForceGenerator {
public:
	virtual void updateForce(Particle* p, double duration) = 0;
	virtual void updateForce(PxRigidDynamic* p, double duration) = 0;
	virtual void setName(string n) {
		_name = n;
	}
	virtual string getName() {
		return _name;
	}
	inline virtual void changeActive() {
		active = !active;
	}
	inline bool isActive() {
		return active;
	}
	void resetTimer() {
		timer = 0.0;
	}
	void setTimeLimit(double t) {
		timeLimit = t;
	}
	void setActive(bool x, double limit=-1) {
		active = x;
		timeLimit = limit;
		resetTimer();
	}
	bool canUpdateForce(double duration) {
		if (!active) return false;
		else if (timeLimit == -1) return true;

		bool x = timeLimit >= timer + duration;
		timer += duration;
		active = x;
		if (!x)cout << "OFF" << "\n";
		return x;
		return true;
	}

	
protected:
	std::string _name;
	double t = -1e10;
	bool active = true;
	double timer = 0.0, timeLimit=-1;
};