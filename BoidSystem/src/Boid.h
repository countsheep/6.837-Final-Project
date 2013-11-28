#ifndef BOID_H
#define BOID_H

#include <vector>
#include <vecmath.h>
#include "extra.h"

using namespace std;

class Boid
{
public:

	Boid(Vector3f initPos, Vector3f initVel, float maxSpeed, float personalBubble); //default start accel is zero

	Vector3f m_position;
	Vector3f m_velocity;
	float m_max_speed;
	Vector3f m_acceleration; 
	float m_personal_bubble; //radius around self
	
	// setter method for the system's state
	void setPosition(Vector3f & newPosition){ m_position = newPosition; };
	void setVelocity(Vector3f  & newVelocity){ m_velocity = newVelocity; };
	void setAcceleration(Vector3f  & newAcceleration){ m_acceleration = newAcceleration; };
	
	// reset position, velocity, and/or acceleration if pertinent
	// c_m represents the center of mass of all the other boid individuals
	void move(Vector3f c_m, vector<Vector3f> vels);
	void draw();
	
};

#endif
