#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <vector>
#include <vecmath.h>

using namespace std;

class BoundingBox
{
public:

	BoundingBox(Vector3f minCoords, Vector3f maxCoords); //minCoords is of the form (min_x, min_y, min_z)
	
	Vector3f m_minCoords;
	Vector3f m_maxCoords;

	// returns true if coord is within the bounds of the box
	bool isInBoundingBox(Vector3f coord);
	// returns a random position within the bounding box
	Vector3f getRandPosition();
	// returns the center coordinate of the bounding box
	Vector3f getCenter();

	Vector3f getForceAtPoint(Vector3f pos, int axis);

	float getXDim();
	float getYDim();
	float getZDim();
	float getDist(Vector3f p1, Vector3f p2);
	// TODO: decide where to put camera

//protected:
	
};

#endif
