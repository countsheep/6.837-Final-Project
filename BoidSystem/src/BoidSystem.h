#ifndef BOIDSYSTEM_H
#define BOIDSYSTEM_H

#include <vector>
#include <vecmath.h>

using namespace std;

class BoidSystem
{
public:

	BoidSystem(int numParticles, BoundingBox box);

	int m_numParticles;
	vector<Boid> m_mahBoids;
	Vector3f goalPos; //if we decide to make boids tend to a specific position
	BoundingBox m_box;

	// 
	Vector3f getAvgVelocity();
	
	// getter method for center of mass
	Vector3f getCenterOfMass();
	
protected:

	void draw() = 0;
	
};

#endif
