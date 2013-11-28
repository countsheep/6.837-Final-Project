#include "Boid.h"

using namespace std;


Boid::Boid(Vector3f initPos, Vector3f initVel, float maxSpeed, float personalBubble){
	m_position = initPos;
	m_velocity = initVel;
	m_max_speed = maxSpeed;
	m_acceleration = Vector3f::ZERO;	
	m_personal_bubble = personalBubble;
}


void Boid::draw(){
	glPushMatrix();
	glTranslatef(m_position.x(), m_position.y(), m_position.z());
	glutSolidSphere(0.075f, 10.0f, 10.0f);
	glPopMatrix();
}

//move 
//1. move towards center of mass
//2. try to keep distance away
//3. match velocity of other boids
//4. some kind of goal
// also, keep velocity under max speed

void Boid::move(Vector3f c_m, Vector3f vel_rule2, Vector3f vel_rule3, Vector3f vel_rule4){
	//move boid appr 1% of the way towards the center of mass of the rest of flock
	//m_position.print();
	Vector3f vel_rule1 = 0.01f * (c_m - m_position);
	Vector3f total_vel = vel_rule1 + vel_rule2 + vel_rule3 + vel_rule4;
	m_position += total_vel;

	//TODO: remain in boudning box
}