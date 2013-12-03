#ifndef FORCE_H
#define FORCE_H

#include <vecmath.h>
#include <vector>
#include "extra.h"

using namespace std;

class Force
{
public:
	Force(Vector3f center, int age, float scale);
	float getRadius(){return radius;}
	void addAge(){m_age+=1;}
	int getAge(){return m_age;}
	float getScale(){return m_scale;}
	Vector3f getCenter(){return m_center;}
	void draw();

protected:
	Vector3f m_center;
	int m_age;
	float m_scale;
	float radius;

};

#endif
