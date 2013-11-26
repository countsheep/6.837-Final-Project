#ifndef BOIDSYSTEM_H
#define BOIDSYSTEM_H

#include <vector>
#include <vecmath.h>

using namespace std;

class BoidSystem
{
public:

	BoidSystem(int numParticles=0);

	int m_numParticles;
	vector<Boid> m_mahBoids;
	Vector3f goalPos; //if we decide to make boids tend to a specific position
	
	// 
	Vector3f getAvgVelocity();
	
	// getter method for center of mass
	Vector3f getCenterOfMass();
	
	void draw() = 0;
	
protected:

	//vector<Vector3f> m_vVecState;
	
};

#endif
