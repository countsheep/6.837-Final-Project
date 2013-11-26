#include "BoidSystem.h"

using namespace std;

BoidSystem::BoidSystem(int numParticles, BoundingBox box){
	m_numParticles = numParticles;
	m_box = box;
	for (int i = 0; i < m_numParticles; i++){
		Boid b = Boid(m_box.getRandPosition(), Vector3f::ZERO, 53.0f, 2.0f);
		m_mahBoids.push_back(b);
		goalPos = m_box.getCenter();
	}
}

float BoidSystem::getAvgVelocity(){
	Vector3f vel = Vector3f::ZERO;
	for (int i = 0; i<m_mahBoids.size(); i++){
		vel += m_mahBoids[i].m_velocity;
	}
	return vel/m_mahBoids.size();
}

float BoidSystem::getCenterOfMass(){
	Vector3f pos = Vector3f::ZERO;
	for (int i = 0; i<m_mahBoids.size(); i++){
		pos += m_mahBoids[i].m_position;
	}
	return pos/m_mahBoids.size();
}

BoidSystem::draw(){
	for (int i = 0; i<m_mahBoids.size(); i++){
		m_mahBoids[i].draw();
	}
}
