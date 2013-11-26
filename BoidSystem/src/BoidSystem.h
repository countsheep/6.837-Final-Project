#ifndef BOIDSYSTEM_H
#define BOIDSYSTEM_H

#include <vector>
#include <vecmath.h>
#include "BoundingBox.h"
#include "Boid.h"

using namespace std;

class BoidSystem
{
public:

	BoidSystem(int numParticles, BoundingBox box);

	int m_numParticles;
	vector<Boid> m_mahBoids;
	Vector3f goalPos; //if we decide to make boids tend to a specific position
	// forces that move boids
	vector<Vector3f> forces;
	BoundingBox m_box;

	// 
	Vector3f getAvgVelocity();
	
	// getter method for center of mass
	Vector3f getCenterOfMass();
	
	void draw();
	
};

#endif
