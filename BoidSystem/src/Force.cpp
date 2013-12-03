#include "Force.h"
#include <cmath>

using namespace std;
const float DEG2RAD = 3.14159/180;

Force::Force(Vector3f center, int age, float scale){
	m_center = center;
	m_age = age;
	m_scale = scale;
	radius = 0.0f;
}

void Force::draw(){
	glBegin(GL_LINE_LOOP);
 
   for (int i=0; i <360; i++)
   {
      float degInRad = i*DEG2RAD;
      glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
   }
 
   glEnd();
}
