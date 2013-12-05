#ifndef OCTTREE_H
#define OCTTREE_H

#include <vecmath.h>
#include <vector>
#include "extra.h"
#include "Boid.h"

using namespace std;

class OctTree
{
public:
	OctTree(Vector3f center, Vector3f half);
	~OctTree();
	Vector3f getDim();
	Vector3f getCenter();
	void setOrigin();
	bool isLeaf();
	void setPoint(Boid* b);
	int getOct(Vector3f p);
	OctTree **children;
	Boid* getPoint(){return point;}
	OctTree *parent;
	bool upLevel(Vector3f p);
	OctTree* findOct(Boid* b);
	vector<Boid*> findClosestNeighbors();
	Boid* findBoidNearPoint(Vector3f p, Vector3f center, float d);
	void deleteChildren();
	vector<Boid*> collided;

protected:
	Vector3f m_center;
	Vector3f halfDim;
	Boid* point;
	int level = 0;
};

#endif
