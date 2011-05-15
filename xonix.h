#pragma once

#include "geometry.h"
#include "detector.h"
#include "observer.h"
#include <unordered_map>

class Xonix {
private:
	int tick;
public:
	Xonix();
	void step();
};
