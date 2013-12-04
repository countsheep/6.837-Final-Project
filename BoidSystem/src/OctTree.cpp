#include "OctTree.h"
#include <cmath>
#include <iostream>

using namespace std;

OctTree::OctTree(Vector3f center, Vector3f half):m_center(center), halfDim(half){
	point = NULL;
	for(int i=0; i<8; ++i) {
		children[i] = NULL;
	}
}

int OctTree::getOct(Vector3f p) {
	int oct = 0;
	if(p.x() >= m_center.x()){oct += 4;}
	if(p.y() >= m_center.y()){oct += 2;}
	if(p.z() >= m_center.z()){oct += 1;}
	return oct;
}

bool OctTree::isLeaf(){
	return children[0] == NULL;
}

void OctTree::setPoint(Boid *b){
	if (isLeaf()){
		if (point == NULL){
			point = b;
			return;
		}
		else{
			Boid* savePoint = point;
			point = NULL;
			
			for (int i = 0; i < 8; i++){
				float nx;
				float ny;
				float nz;
				if (i < 3){nx = m_center.x() - halfDim.x()*0.5f;}
				else{nx = m_center.x() + halfDim.x()*0.5f;}
				if (i % 2 == 0){nz = m_center.z() - halfDim.z()*0.5f;}
				else{nz = m_center.z() + halfDim.z()*0.5f;}
				if ((i==0)||(i==1)||(i==4)||(i==5)){ny = m_center.y() - halfDim.y()*0.5f;}
				else{ny = m_center.y() + halfDim.y()*0.5f;}
				
				children[i] = new OctTree(Vector3f(nx, ny, nz), halfDim*0.5f);
			}
			children[getOct(savePoint -> m_position)] -> setPoint(savePoint);
			children[getOct(point -> m_position)] -> setPoint(point);
		}
	}
	else{children[getOct(point -> m_position)] -> setPoint(point);}
}


