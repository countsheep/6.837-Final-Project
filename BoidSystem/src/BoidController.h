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
	BoidController(Image* img, Vector3f minBounds, Vector3f maxBounds, float scaler, bool o);
	vector<BoidSystem> m_systems;
	void draw();
	void stepSystem(vector<vector<Force*>> f, bool move_to_goal, bool move_away_from_goal, Vector3f goal);
	Image downSample(Image* img);
	float m_scale;
};

#endif
