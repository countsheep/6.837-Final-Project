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
	Vector3f getDim(){return halfDim;}
	Vector3f getCenter(){return m_center;}
	void setOrigin();
	bool isLeaf();
	void setPoint(Boid *b);
	int getOct(Vector3f p);
	OctTree *children[8];
	Boid* getPoint(){return point;}

protected:
	Vector3f m_center;
	Vector3f halfDim;
	Boid* point;

};

#endif
