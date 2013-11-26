#include "BoidSystem.h"
#include <iostream>

using namespace std;

BoidSystem::BoidSystem(int numParticles, BoundingBox box):m_box(box){
	m_numParticles = numParticles;
	
	for (int i = 0; i < m_numParticles; i++){
		Vector3f pos = m_box.getRandPosition();
		//pos.print();
		Boid b = Boid(pos, Vector3f::ZERO, 53.0f, 0.5f);
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
	Vector3f pos = Vector3f::ZERO;
	for (int i = 0; i<m_mahBoids.size(); i++){
		pos += m_mahBoids[i].m_position;
	}
	return pos/m_mahBoids.size();
}

void BoidSystem::draw(){
	for (int i = 0; i<m_mahBoids.size(); i++){
		m_mahBoids[i].draw();
	}
}
