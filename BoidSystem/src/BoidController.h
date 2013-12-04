#ifndef BOIDCONTROLLER_H
#define BOIDCONTROLLER_H

#include <vector>
#include <vecmath.h>
#include <string>
#include "BoundingBox.h"
#include "BoidSystem.h"
#include "Boid.h"
#include "Image.h"
#include "Force.h"

using namespace std;

class BoidController
{
public:
	//give box dimensions
	BoidController(Image* img, Vector3f minBounds, Vector3f maxBounds);
	vector<BoidSystem> m_systems;
	void draw();
	void stepSystem(vector<vector<Force*>> f);
	Image downSample(Image* img);
};

#endif
