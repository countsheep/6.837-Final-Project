#include "BoidSystem.h"
#include <iostream>
#include <math.h>

using namespace std;

BoidSystem::BoidSystem(int numParticles, BoundingBox box):m_box(box){
	m_numParticles = numParticles;
	
	for (int i = 0; i < m_numParticles; i++){
		Vector3f pos = m_box.getRandPosition();
		//pos.print();
		Boid b = Boid(pos, Vector3f::ZERO, m_box.getXDim() / 2.0f, 0.15f);
		m_mahBoids.push_back(b);
		goalPos = m_box.getCenter();
	}
}

Vector3f BoidSystem::getAvgVelocity(){
	Vector3f vel = Vector3f::ZERO;
	for (int i = 0; i<m_mahBoids.size(); i++){
		vel += m_mahBoids[i].m_velocity;
	}
	return vel/m_mahBoids.size();
}

Vector3f BoidSystem::getCenterOfMass(){
	return getCenterOfMassMinusB(-1);
}

Vector3f BoidSystem::getCenterOfMassMinusB(int b){
	Vector3f pos = Vector3f::ZERO;
	for (int i = 0; i<m_mahBoids.size(); i++){
		if(i != b)
			pos += m_mahBoids[i].m_position;
	}
	return pos/(m_mahBoids.size()-1);
}

Vector3f BoidSystem::moveTowardCenterOfMass(int b){
	Vector3f c_m = getCenterOfMassMinusB(b);
	return 0.005f * (c_m - m_mahBoids[b].m_position);
}

//TOOD dislike the hardcoding with the direction change for avoidance
//causes weird bubbling
Vector3f BoidSystem::getAvoidanceOffset(int b){
	Vector3f vel = Vector3f::ZERO;
	Vector3f pos = m_mahBoids[b].m_position;
	float personal_bubble = m_mahBoids[b].m_personal_bubble;
	for (int i = 0; i<m_mahBoids.size(); i++){
		if(i != b && getDist(m_mahBoids[i].m_position, pos) < personal_bubble)
			vel -= (m_mahBoids[i].m_position - pos);
	}
	return vel;
}

Vector3f BoidSystem::getAverageVelocity(int b){
	Vector3f avg_vel = Vector3f::ZERO;
	for (int i = 0; i<m_mahBoids.size(); i++){
		if(i != b)
			avg_vel += m_mahBoids[i].m_velocity;
	}
	return 0.125f * avg_vel / (m_mahBoids.size()-1);
}


//if boid crosses boundary lines, pushes boid back by 1% of the bounding box size
bool BoidSystem::inBounds(int b){
	Vector3f vel = Vector3f::ZERO;
	Vector3f pos = m_mahBoids[b].m_position;
	cout << "Size of bounding box " << endl;
	m_box.m_minCoords.print();
	m_box.m_maxCoords.print();
	// test x bounds
	if(pos.x() < m_box.m_minCoords.x()){
		//vel += Vector3f(m_box.getXDim() * 0.01f, 0.0f, 0.0f);
		return false;
	}
	else if(pos.x() > m_box.m_maxCoords.x()){
		//vel -= Vector3f(m_box.getXDim() * 0.01f, 0.0f, 0.0f);
		return false;
	}
	// test y bounds
	if(pos.y() < m_box.m_minCoords.y()){
		//vel += Vector3f(m_box.getYDim() * 0.0f, 0.01f, 0.0f);
		return false;
	}
	else if(pos.y() > m_box.m_maxCoords.y()){
		//vel -= Vector3f(m_box.getYDim() * 0.0f, 0.01f, 0.0f);
		return false;
	}
	// test z bounds
	if(pos.z() < m_box.m_minCoords.z()){
		//vel += Vector3f(m_box.getZDim() * 0.0f, 0.0f, 0.01f);
		return false;
	}
	else if(pos.z() > m_box.m_maxCoords.z()){
		//vel -= Vector3f(m_box.getZDim() * 0.0f, 0.0f, 0.01f);
		return false;
	}
	//return vel;
	return true;
}


Vector3f BoidSystem::stepSystem(){
	for (int i = 0; i<m_mahBoids.size(); i++){
		vector<Vector3f> vels;
		//vels.push_back(stayInBounds(i));
		if(inBounds(i)){
			vels.push_back(moveTowardCenterOfMass(i));
			vels.push_back(getAvoidanceOffset(i));
			vels.push_back(getAverageVelocity(i));
			vels.push_back(defaultWind);
		}
		m_mahBoids[i].move(vels);
		m_mahBoids[i].stepSystem();
	}
}

float BoidSystem::getDist(Vector3f p1, Vector3f p2){
	return sqrt(pow(p1.x()-p2.x(), 2.0f) + pow(p1.y()-p2.y(), 2.0f) + pow(p1.z()-p2.z(), 2.0f));
}

void BoidSystem::draw(){
	for (int i = 0; i<m_mahBoids.size(); i++){
		m_mahBoids[i].draw();
	}
}
