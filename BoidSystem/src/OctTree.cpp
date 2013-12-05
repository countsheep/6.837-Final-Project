#include "OctTree.h"
#include <cmath>
#include <iostream>

using namespace std;

OctTree::OctTree(Vector3f center, Vector3f half) {
	m_center = center;
	halfDim = half;
	point = NULL;
	children = new OctTree* [8];
	for(int i=0; i<8; ++i) {
		children[i] = NULL;
	}
	parent = NULL;
}
OctTree::~OctTree() {
	delete children;
}
Vector3f OctTree::getDim(){return halfDim;}
Vector3f OctTree::getCenter(){return m_center;}

int OctTree::getOct(Vector3f p) {
	int oct = 0;
	if(p.x() >= m_center.x()){oct += 4;}
	if(p.y() >= m_center.y()){oct += 2;}
	if(p.z() >= m_center.z()){oct += 1;}
	return oct;
}

bool OctTree::upLevel(Vector3f p){
	Vector3f dif = p-getCenter();
	if (dif.abs()> getDim().abs()){
		return true;
	}
	return false;
}

OctTree* OctTree::findOct(Boid* b){
	if (isLeaf()){
		if (b == point){return this;}
		for (int i = 0; i < collided.size(); i++){
			if (b == collided[i]){return this;}
		}
		return NULL;
	}
	int i = getOct(b ->  m_position);
	return children[i] ->findOct(b);
}

bool OctTree::isLeaf(){
	for (int i = 0; i < 8; i++){
		if (children[i] != NULL){
			return false;
		}
	}
	return true;
	//return children[0] == NULL;
}


void OctTree::setPoint(Boid* b){
	
	if (isLeaf()){
		if (point == NULL){
			
			//cout << b <<" "<< level << endl;
			point = b;
		}
		else{
			//cout << "in else" << endl;
			//cout << b << " " << children[0] << endl;
			if (b -> m_position == point -> m_position){
				collided.push_back(b);
			}
			else{
				Boid* savePoint = point;
				point = NULL;
				//cout << "here" << endl;
				//cout << "level " << level << endl;
				for (int i = 0; i < 8; i++){
					float nx;
					float ny;
					float nz;
					if (i < 4){nx = m_center.x() - halfDim.x()*0.5f;}
					else{nx = m_center.x() + halfDim.x()*0.5f;}
					if (i % 2 == 0){nz = m_center.z() - halfDim.z()*0.5f;}
					else{nz = m_center.z() + halfDim.z()*0.5f;}
					if ((i==0)||(i==1)||(i==4)||(i==5)){ny = m_center.y() - halfDim.y()*0.5f;}
					else{ny = m_center.y() + halfDim.y()*0.5f;}
					//cout << i << endl;
					children[i] = new OctTree(Vector3f(nx, ny, nz), halfDim*0.5f);
					//cout << "made new child" << endl;
					children[i] -> parent = this;
					children[i]->level = level+1;
				}
				//cout << getOct(savePoint -> m_position) << " set old to " << savePoint << " "; 
				//savePoint -> m_position.print();
				children[getOct(savePoint -> m_position)] -> setPoint(savePoint);
				//cout << getOct(b -> m_position) << " set new " << b << " ";
				//b -> m_position.print();
				children[getOct(b -> m_position)] -> setPoint(b);
				}
			}
		}
	
	else{
		
		children[getOct(b -> m_position)] -> setPoint(b);
	}

}

Boid* OctTree::findBoidNearPoint(Vector3f p, Vector3f center, float d){
	if (isLeaf()){return point;}
	if (upLevel(p)){
		if (parent == NULL){return NULL;}
		else{return parent -> findBoidNearPoint(p, center, d);}
	}
	else{
		Vector3f np;
		float nd;
		Vector3f dif = center-getCenter();
		float d2 = dif.abs();
		if ( d > d2){
			np = (center+getCenter())/2.0f;
			nd = np.abs();
		}
		else{
			np = p;
			nd = d;
		}
		int i = getOct(np);
		Boid* b = children[i]-> findBoidNearPoint(np, center, nd);
		for (int j = 0; j < 8; j++){
			if (b==NULL){
				i += 1; 
				OctTree *child = children[i%8] ;
				Vector3f c = child->getCenter() - getCenter();
				b = child -> findBoidNearPoint(np-c, center, (np-c).abs());
			}
		}
		return b;
	}
}
vector<Boid*> OctTree::findClosestNeighbors(){
	//cout << "in find level: " << level <<endl;
	
	vector<Boid*> neighbors;
	//cout << neighbors.size() <<endl;
	if (point != NULL){
		vector<Vector3f> points;
		vector<float> dis;
		Vector3f pos = point -> m_position;
		Vector3f dim = getDim();
		Vector3f v;
	
		dis.push_back(dim.x());
		dis.push_back(dim.x());
		v = pos + dim.x();
		points.push_back(v);
		v = pos - dim.x();
		points.push_back(v);
	
		dis.push_back(dim.y());
		dis.push_back(dim.y());
		v = pos + dim.y();
		points.push_back(v);
		v = pos - dim.y();
		points.push_back(v);
	
		dis.push_back(dim.z());
		dis.push_back(dim.z());
		v = pos + dim.z();
		points.push_back(v);
		v = pos - dim.z();
		points.push_back(v);
	
		for (int i = 0; i < 8; i++){
			//cout << i << endl;
			Boid *b = findBoidNearPoint(points[i], pos, dis[i]);
			if ( b != NULL){
				neighbors.push_back(b);
			}
			//cout << "ending it" << endl;
		}
	}
	
	return neighbors;
	
}

void OctTree::deleteChildren(){
	if(isLeaf()){
		return;
	}
	else{
		for(int i=0; i<8; ++i) {
			children[i] -> deleteChildren();
		}
		delete [] children;
		for(int i=0; i<8; ++i) {
			children[i] = NULL;
		}
	}
}
	

