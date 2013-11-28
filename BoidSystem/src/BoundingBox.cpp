#include "BoundingBox.h"
#include <ctime>
#include <stdlib.h>
using namespace std;

BoundingBox::BoundingBox(Vector3f minCoords, Vector3f maxCoords){
	m_minCoords = minCoords;
	m_maxCoords = maxCoords;
}

bool BoundingBox::isInBoundingBox(Vector3f coord){
	if(coord.x() < m_maxCoords.x() && coord.y() < m_maxCoords.y() && coord.z() < m_maxCoords.z() &&
	   coord.x() > m_minCoords.x() && coord.y() > m_minCoords.y() && coord.z() > m_minCoords.z()){
		return true;
	}
	return false;
}

Vector3f BoundingBox::getRandPosition(){
	float x = m_minCoords.x() + (m_maxCoords.x() - m_minCoords.x()) * ((float)rand()/RAND_MAX);
	float y = m_minCoords.y() + (m_maxCoords.y() - m_minCoords.y()) * ((float)rand()/RAND_MAX);
	float z = m_minCoords.z() + (m_maxCoords.z() - m_minCoords.z()) *((float)rand()/RAND_MAX);
	return Vector3f(x, y, z);
}

Vector3f BoundingBox::getCenter(){
	return Vector3f( (m_minCoords.x()+m_maxCoords.x())/2.0f, (m_minCoords.y()+m_maxCoords.y())/2.0f, (m_minCoords.z()+m_maxCoords.z())/2.0f );
}
