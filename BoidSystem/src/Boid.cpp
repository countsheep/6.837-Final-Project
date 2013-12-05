#include "Boid.h"

using namespace std;


Boid::Boid(Vector3f initPos, Vector3f initVel, float maxSpeed, float personalBubble, bool img){
	m_img = img;
	start_pos = initPos;
	m_position = initPos;
	m_velocity = initVel;
	m_max_speed = maxSpeed;
	m_acceleration = Vector3f::ZERO;	
	m_personal_bubble = personalBubble;
}

void Boid::draw(){
	glPushMatrix();
	glTranslatef(m_position.x(), m_position.y(), m_position.z());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_color);
	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glEnable(GL_COLOR_MATERIAL);
	if (m_img){
	   glutSolidCube(0.075f);
	}
	else{
		glutSolidSphere(0.075f, 10.0f, 10.0f);
	}
	
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix();
}

//move 
//1. move towards center of mass
//2. try to keep distance away
//3. match velocity of other boids
//4. some kind of goal
// also, keep velocity under max speed

void Boid::move(vector<Vector3f> vels){
	//move boid appr 1% of the way towards the center of mass of the rest of flock
	//m_position.print();
	Vector3f total_vel = Vector3f::ZERO;
	for(int i = 0; i < vels.size(); i++){
		total_vel += vels[i];
	}
	// cap the max velocity of a boid
	if(total_vel.abs() > m_max_speed)
		total_vel = m_max_speed;
	m_position += total_vel;
	//cloth->vel = total_vel;

}
