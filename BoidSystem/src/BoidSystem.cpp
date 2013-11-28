#include "BoidSystem.h"
#include <iostream>
#include <math.h>

using namespace std;

BoidSystem::BoidSystem(int numParticles, BoundingBox box):m_box(box){
	m_numParticles = numParticles;
	
	for (int i = 0; i < m_numParticles; i++){
		Vector3f pos = m_box.getRandPosition();
		pos.print();
		Boid b = Boid(pos, Vector3f::ZERO, 53.0f, 0.15f);
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

Vector3f BoidSystem::stepSystem(){
	//cout << "STEP SYSTEM";
	for (int i = 0; i<m_mahBoids.size(); i++){
		m_mahBoids[i].move(getCenterOfMassMinusB(i), getAvoidanceOffset(i), getAverageVelocity(i), defaultWind);
		m_mahBoids[i].stepSystem();
	}
}

float BoidSystem::getDist(Vector3f p1, Vector3f p2){
	//cout << "distance finding";
	//p1.print();
	//p2.print();
	//cout << sqrt(pow(p1.x()-p2.x(), 2.0f) + pow(p1.y()-p2.y(), 2.0f) + pow(p1.z()-p2.z(), 2.0f));
	return sqrt(pow(p1.x()-p2.x(), 2.0f) + pow(p1.y()-p2.y(), 2.0f) + pow(p1.z()-p2.z(), 2.0f));
}

void BoidSystem::draw(){
	for (int i = 0; i<m_mahBoids.size(); i++){
		m_mahBoids[i].draw();
	}
}
