#ifndef BOIDSYSTEM_H
#define BOIDSYSTEM_H

#include <vector>
#include <vecmath.h>
#include "BoundingBox.h"
#include "Boid.h"
#include "Image.h"
#include <string>
#include "Force.h"
#include "OctTree.h"

using namespace std;

class BoidSystem
{
public:

	BoidSystem(int numParticles, BoundingBox box, bool o);
	BoidSystem(BoundingBox box, Image* img, bool o);
	BoidSystem(BoundingBox box, vector<Boid*> boids, string main_color, bool o);

	int m_numParticles;
	vector<Boid*> m_mahBoids;
	Vector3f goalPos; //if we decide to make boids tend to a specific position
	// forces that move boids
	vector<Vector3f> forces;
	BoundingBox m_box;

	Vector3f defaultWind = Vector3f(0.05f, 0.0f, 0.0f);

	//for color boids, save main color
	bool isColorBoid = false;
	string m_main_color;

	// 
	Vector3f getAvgVelocity();
	
	// getter method for center of mass
	Vector3f getCenterOfMass();
	Vector3f getCenterOfMassMinusB(int b);
	Vector3f getCenterOfMassOfBoids(vector<int> neighbors);
	Vector3f getCenterOfMassOfBoidsB(vector<Boid*> neighbors);
	
	Vector3f moveTowardCenterOfMass(int b);
	Vector3f moveTowardCenterOfMassB(int b);
	Vector3f moveAwayFromForceSphere(int b);
	Vector3f moveTowardsGoalPoint(int b);
	void getAvoidanceOffset(int b);
	Vector3f getAverageVelocity(int b);
	Vector3f inBounds(int b);
	void stepSystem(vector<vector<Force*>> f, bool move_to_goal, bool move_away_from_goal, Vector3f goal);

	vector<Boid*> getNearestNeighborsOct(int b);
	vector<int> getNearestNeighbors(int b);

	static float getDist(Vector3f p1, Vector3f p2);
	
	void draw();
	
	OctTree* oct;
	
	bool useOct;
	bool image;
	
};

#endif
