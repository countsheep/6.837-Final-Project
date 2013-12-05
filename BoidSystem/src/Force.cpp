#include "Force.h"
#include <cmath>
#include <iostream>

using namespace std;
const float DEG2RAD = 3.14159/180;

Force::Force(Vector3f center, int age, float scale){
	m_center = center;
	m_age = age;
	m_scale = scale;
	radius = 0.0f;
}

void Force::age(){
	m_age--;
}

void Force::grow(){
	radius += 0.05;
}

int Force::getAge(){return m_age;}
float Force::getScale(){return m_scale;}
Vector3f Force::getCenter(){return m_center;}

void Force::draw(Matrix4f cam){
	
	glBegin(GL_LINE_LOOP);
 	glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector3f(0.7f, 0.7f, 0.7f));
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glEnable(GL_COLOR_MATERIAL);
   for (int i=0; i <360; i++)
   {
      float degInRad = i*DEG2RAD;
      Vector3f v = (cam*Vector4f(cos(degInRad)*radius,sin(degInRad)*radius, 0.0f, 0.0f)).xyz();
      glVertex3f(v.x(), v.y(), v.z());
   }
 	glDisable(GL_COLOR_MATERIAL);
   glEnd();
}
