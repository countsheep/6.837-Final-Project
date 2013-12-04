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
	void age();
	void grow();
	int getAge();
	float getScale();
	Vector3f getCenter();
	void draw(Matrix4f cam);

protected:
	Vector3f m_center;
	int m_age;
	float m_scale;
	float radius;

};

#endif
