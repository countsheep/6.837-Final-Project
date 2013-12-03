#ifndef BOIDSYSTEM_H
#define BOIDSYSTEM_H

#include <vector>
#include <vecmath.h>
#include "BoundingBox.h"
#include "Boid.h"
#include "Image.h"

using namespace std;

class BoidSystem
{
public:

	BoidSystem(int numParticles, BoundingBox box);
	BoidSystem(BoundingBox box, Image* img);

	int m_numParticles;
	vector<Boid> m_mahBoids;
	Vector3f goalPos; //if we decide to make boids tend to a specific position
	// forces that move boids
	vector<Vector3f> forces;
	BoundingBox m_box;

	Vector3f defaultWind = Vector3f(0.05f, 0.0f, 0.0f);

	// 
	Vector3f getAvgVelocity();
	
	// getter method for center of mass
	Vector3f getCenterOfMass();
	Vector3f getCenterOfMassMinusB(int b);
	Vector3f getCenterOfMassOfBoids(vector<int> neighbors);
	Vector3f moveTowardCenterOfMass(int b);
	Vector3f moveAwayFromForceSphere(int b);
	Vector3f moveTowardsGoalPoint(int b);
	void getAvoidanceOffset(int b);
	Vector3f getAverageVelocity(int b);
	Vector3f inBounds(int b);
	Vector3f stepSystem();

	vector<int> getNearestNeighbors(int b);

	static float getDist(Vector3f p1, Vector3f p2);
	
	void draw();
	
};

#endif
