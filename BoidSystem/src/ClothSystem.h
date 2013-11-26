#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include "extra.h"
#include <vector>

#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int w, int h);
	vector<Vector3f> evalF(vector<Vector3f> state);
	Vector3f getForce(int x, int y, vector<Vector3f> state);
	void draw();
	void toggleWind();

private:
	int w;
	int h;
	int indexOf(int x, int y);
	void drawSprings();
	bool useWind;
};


#endif
