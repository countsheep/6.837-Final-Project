#include "BoundingBox.h"
#include <ctime>
#include <stdlib.h>
#include <math.h> 
#include <iostream>
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

//axis is 0 to indicate x, 1 = y, 2 = z, default is z axis
Vector3f BoundingBox::getForceAtPoint(Vector3f pos, int axis){
	float ratio = 0.5;
	// if(pos.x() < m_maxCoords.x()*ratio && pos.y() < m_maxCoords.y()*ratio && pos.z() < m_maxCoords.z()*ratio &&
	//    pos.x() > m_minCoords.x()*ratio && pos.y() > m_minCoords.y()*ratio && pos.z() > m_minCoords.z()*ratio){
	// 	return (pos - getCenter()) * 0.005f;
	// }
	//else{
	float radius = getDist(pos, getCenter());
	float theta = 1.0f * M_PI/180.0; //in radians, hardcoded to 5 degrees
	Matrix3f rotate_z = Matrix3f(cos(theta), -sin(theta), 0,
								 sin(theta), cos(theta), 0,
								 0, 0, 1.0f);
	Matrix3f rotate_y = Matrix3f(cos(theta), 0, sin(theta),
								 0, 1, 0,
								 -sin(theta), 0, cos(theta));
	Matrix3f rotate_x = Matrix3f(1, 0, 0,
								 0, cos(theta), -sin(theta),
								 0, sin(theta), cos(theta));
	Vector3f new_point;
	if(axis == 0){
		new_point = rotate_x * pos;
	}
	else if(axis == 1){
		new_point = rotate_y * pos;
	}
	else{
		new_point = rotate_z * pos;
	}
	return new_point - pos;
	//}

}

float BoundingBox::getXDim(){
	return m_maxCoords.x() - m_minCoords.x();
}

float BoundingBox::getYDim(){
	return m_maxCoords.y() - m_minCoords.y();
}

float BoundingBox::getZDim(){
	return m_maxCoords.z() - m_minCoords.z();
}

float BoundingBox::getDist(Vector3f p1, Vector3f p2){
	return abs(sqrt(pow(p1.x()-p2.x(), 2.0f) + pow(p1.y()-p2.y(), 2.0f) + pow(p1.z()-p2.z(), 2.0f)));
}

