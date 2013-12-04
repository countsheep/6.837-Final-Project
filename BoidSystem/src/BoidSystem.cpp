#include "BoidSystem.h"
#include <iostream>
#include <math.h>

using namespace std;

BoidSystem::BoidSystem(int numParticles, BoundingBox box):m_box(box){
	m_numParticles = numParticles;
	
	for (int i = 0; i < m_numParticles; i++){
		Vector3f pos = m_box.getRandPosition();
		//pos.print();
		Boid* b = new Boid(pos, Vector3f::ZERO, m_box.getXDim() / 2.0f, 0.25f);
		m_mahBoids.push_back(b);
		goalPos = m_box.getCenter();
	}
}

BoidSystem::BoidSystem(BoundingBox box, Image* img):m_box(box){
	m_numParticles = img->Width() * img->Height();
	for (int i = 0; i < img->Width(); i++){
		for( int j = 0; j < img->Height(); j++){
			Vector3f pos = Vector3f(0.075f*i, 0.075f*j, 0.0f);
			Boid* b = new Boid(pos, Vector3f::ZERO, m_box.getXDim() / 2.0f, 0.25f);
			b->m_color = img->GetPixel(i, j);
			m_mahBoids.push_back(b);
			goalPos = m_box.getCenter();
		}
	}
}

//initialize color boid
BoidSystem::BoidSystem(BoundingBox box, vector<Boid*> boids, string main_color):m_box(box){
	m_numParticles = boids.size();
	m_mahBoids = boids;
	goalPos = m_box.getCenter();
	m_main_color = main_color;
	isColorBoid = true;
}

Vector3f BoidSystem::getAvgVelocity(){
	Vector3f vel = Vector3f::ZERO;
	for (int i = 0; i<m_mahBoids.size(); i++){
		vel += m_mahBoids[i]->m_velocity;
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
			pos += m_mahBoids[i]->m_position;
	}
	return pos/(m_mahBoids.size()-1);
}

Vector3f BoidSystem::getCenterOfMassOfBoids(vector<int> neighbors){
	Vector3f pos = Vector3f::ZERO;
	if(neighbors.size() == 0)
		return Vector3f::ZERO;
	for (int i = 0; i<neighbors.size(); i++){
			pos += m_mahBoids[neighbors[i]]->m_position;
	}
	return pos/(neighbors.size());
}

Vector3f BoidSystem::moveTowardCenterOfMass(int b){
	Vector3f c_m = getCenterOfMassOfBoids(getNearestNeighbors(b));
	return 0.01f * (c_m - m_mahBoids[b]->m_position);
}

Vector3f BoidSystem::moveAwayFromForceSphere(int b){

}

Vector3f BoidSystem::moveTowardsGoalPoint(int b){

}

//TOOD dislike the hardcoding with the direction change for avoidance
//causes weird bubbling
void BoidSystem::getAvoidanceOffset(int b){
	Vector3f vel = Vector3f::ZERO;
	Vector3f pos = m_mahBoids[b]->m_position;
	bool violated = false;
	float personal_bubble = m_mahBoids[b]->m_personal_bubble;
	for (int i = 0; i<m_mahBoids.size(); i++){
		if(i != b && getDist(m_mahBoids[i]->m_position, pos) < personal_bubble){
			vel -= (m_mahBoids[i]->m_position - pos);
			violated = true;
		}
	}
	if(violated == true){
		m_mahBoids[b]->m_avoidance_decay_counter = 5;
		m_mahBoids[b]->m_avoidanceVec = vel;
	}
}

Vector3f BoidSystem::getAverageVelocity(int b){
	Vector3f avg_vel = Vector3f::ZERO;
	for (int i = 0; i<m_mahBoids.size(); i++){
		if(i != b)
			avg_vel += m_mahBoids[i]->m_velocity;
	}
	return 0.125f * avg_vel / (m_mahBoids.size()-1);
}


//if boid crosses boundary lines, pushes boid back by 1% of the bounding box size
Vector3f BoidSystem::inBounds(int b){
	Vector3f vel = Vector3f::ZERO;
	Vector3f pos = m_mahBoids[b]->m_position;
	/*cout << "Size of bounding box " << endl;
	m_box.m_minCoords.print();
	m_box.m_maxCoords.print();*/

	// test x bounds
	if(pos.x() < m_box.m_minCoords.x()){
		vel += Vector3f(m_box.getXDim() * 0.01f, 0.0f, 0.0f);
		//return false;
	}
	else if(pos.x() > m_box.m_maxCoords.x()){
		vel -= Vector3f(m_box.getXDim() * 0.01f, 0.0f, 0.0f);
		//return false;
	}
	// test y bounds
	if(pos.y() < m_box.m_minCoords.y()){
		vel += Vector3f(m_box.getYDim() * 0.0f, 0.01f, 0.0f);
		//return false;
	}
	else if(pos.y() > m_box.m_maxCoords.y()){
		vel -= Vector3f(m_box.getYDim() * 0.0f, 0.01f, 0.0f);
		//return false;
	}
	// test z bounds
	if(pos.z() < m_box.m_minCoords.z()){
		vel += Vector3f(m_box.getZDim() * 0.0f, 0.0f, 0.01f);
		//return false;
	}
	else if(pos.z() > m_box.m_maxCoords.z()){
		vel -= Vector3f(m_box.getZDim() * 0.0f, 0.0f, 0.01f);
		//return false;
	}
	return vel;
	//return true;
}


void BoidSystem::stepSystem(vector<vector<Force*>> f){
	for (int i = 0; i<m_mahBoids.size(); i++){
		vector<Vector3f> vels;
		getAvoidanceOffset(i);
		//vels.push_back(stayInBounds(i));
		//if(inBounds(i)){
			vels.push_back(moveTowardCenterOfMass(i));
			if(m_mahBoids[i]->m_avoidance_decay_counter > 0){
				vels.push_back(m_mahBoids[i]->m_avoidanceVec*0.1f*m_mahBoids[i]->m_avoidance_decay_counter);
				m_mahBoids[i]->m_avoidance_decay_counter -=1;
			}
			vels.push_back(getAverageVelocity(i));
			//if color boid, swirl force around different axes
			if(isColorBoid){
				int axis = 0;
				if(m_main_color == "green"){
					axis = 1;
				}
				else if(m_main_color == "blue"){
					axis = 2;
				}
				vels.push_back(m_box.getForceAtPoint(m_mahBoids[i]->m_position, axis));
			}
			else{
				vels.push_back(m_box.getForceAtPoint(m_mahBoids[i]->m_position, -1));//defaultWind);
			}
			vels.push_back(inBounds(i));
		//}
		for (int i = 0; i < f.size(); i++){
			vector<Force*> fv = f[i];
			for (int j = 0; j < fv.size(); j++){
				Force *force = fv[j];
				Vector3f b_pos = m_mahBoids[i]->m_position;
				Vector3f fc = force->getCenter();
				Vector3f dif = b_pos-fc;
				if (dif.abs() >= force -> getRadius()){
					Vector3f f_vel = (2.0f/(force -> getRadius()*dif.abs()+1.0f))*(force->getScale())*(dif.normalized());
					vels.push_back(f_vel);
					
				}
			}
		}
		m_mahBoids[i]->move(vels);
		m_mahBoids[i]->stepSystem();
	}
}

vector<int> BoidSystem::getNearestNeighbors(int b){
	// vector<int> neighbors;
	// for(int i = 0; i < m_mahBoids.size(); i++){
	// 	if(i != b && getDist(m_mahBoids[b].m_position, m_mahBoids[i].m_position) < m_box.getXDim() * 0.05f){
	// 		neighbors.push_back(i);
	// 	}
	// }
	vector<int> neighbors;
	float furthest_close = 0.0f;
	int furthest_close_boid = -1;
	vector<float> dists;
	for(int i = 0; i < m_mahBoids.size(); i++){
		float dist = getDist(m_mahBoids[b]->m_position, m_mahBoids[i]->m_position);
		// if there are less than five neighbors, just add this boid
		if(neighbors.size() < 8){
			neighbors.push_back(i);
			dists.push_back(dist);
			if(dist > furthest_close){
				furthest_close = dist;
				furthest_close_boid = i;
			}
		}
		//otherwise check that it is closer than the furthest neighbor
		else if(i != b && dist < furthest_close){
			//cout << "furthest_close is " << furthest_close << " and dist is " << dist << endl;
			//cout << "size is "<<neighbors.size() <<endl;
			furthest_close = 0.0f;
			int new_furthest_close_boid = -1;
			float new_furthest_close = 0.0f;
			for(int j = 0; j < neighbors.size(); j++){
				//cout << "should delete "<< furthest_close_boid <<endl; 
				//cout << "this loops is "<< neighbors[j] << endl;
				if(neighbors[j]==furthest_close_boid){
					//cout << "deleting" <<endl;
					//cout << "size of dist before erase " << dists.size() <<endl;
					dists.erase(dists.begin() + j);
					neighbors.erase(neighbors.begin() + j);
					//cout << "size of dist after erase " << dists.size() <<endl;
				}
				else if(dists[j] > new_furthest_close){
					//set the current new furthest close boid
					new_furthest_close_boid = neighbors[j];
					//set the current new furthest close dist
					new_furthest_close = dists[j];
				}
			}
			if(dist > new_furthest_close){
				new_furthest_close = dist;
				new_furthest_close_boid = i;
			}
			furthest_close = new_furthest_close;
			furthest_close_boid = new_furthest_close_boid;
			dists.push_back(dist);
			neighbors.push_back(i);
		}
	}

	return neighbors;
}

float BoidSystem::getDist(Vector3f p1, Vector3f p2){
	return abs(sqrt(pow(p1.x()-p2.x(), 2.0f) + pow(p1.y()-p2.y(), 2.0f) + pow(p1.z()-p2.z(), 2.0f)));
}

void BoidSystem::draw(){
	for (int i = 0; i<m_mahBoids.size(); i++){
		m_mahBoids[i]->draw();
	}
}
