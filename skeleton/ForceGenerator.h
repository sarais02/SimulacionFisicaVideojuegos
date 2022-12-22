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
	inline bool isOver() {
		return over;
	}
	inline void setIsOver(bool o) {
		over = o;
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
	void setLoop(bool l) {
		loop = l;
	}
	bool canUpdateForce(double duration) {
		if (loop && !active) { 
			bool x = timeLimit <= timer + duration;
			timer += duration;
			active = x;
			if (x) {
				timer = 0;
			}
			return x;
		}
		else if (!active) return false;
		else if (timeLimit == -1) return true;

		bool x = timeLimit >= timer + duration;
		timer += duration;
		active = x;
		if (!x) {
			timer = 0;
		}
		return x;
	}

	
protected:
	std::string _name;
	double t = -1e10;
	bool active = true, over=false, loop=false;
	double timer = 0.0, timeLimit=-1;
};