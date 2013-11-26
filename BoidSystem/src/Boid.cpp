#include "Boid.h"

using namespace std;

Boid::Boid(Vector3f initPos, Vecto3f initVel, float maxSpeed, float personalBubble){
	m_position = initPos;
	m_velocity = initVel;
	m_max_speed = maxSpeed;
	m_acceleratioon = Vector3f::ZERO;	
	m_personal_bubble = personalBubble;
}

Boid::move(){
	
}

Boid::draw(){
	glPushMatrix();
	glTranslatef(m_position.x(), m_position.y(), m_position.z());
	glutSolidSphere(0.075f, 10.0f, 10.0f);
	glPopMatrix();
}