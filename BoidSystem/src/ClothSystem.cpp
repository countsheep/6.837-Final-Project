#include "ClothSystem.h"
#include <iostream>
using namespace std;

bool useWind;
//TODO: Initialize here
ClothSystem::ClothSystem(int w, int h, Vector3f initPos)
{
	vel = Vector3f::ZERO;
	this->w = w;
	this->h = h;
	vector<Vector3f> init;
	for(unsigned i = 0; i < w; i++){
		for(unsigned j = 0; j < h; j++){
			init.push_back(Vector3f(initPos.x(), 0.25f*i+initPos.y(), 0.25f*j+initPos.z()));
			init.push_back(Vector3f(0.0f, 0.0f, 0.0f));
		}
	}
	//cout << w << " "<< h << endl;
	//cout << init.size() << endl;
	setState(init);
	useWind = false;
}

int ClothSystem::indexOf(int x, int y){
	return 2 * (x * w + y);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	//cout << " EVAL F "<<endl;
	vector<Vector3f> f; 
	for(unsigned i = 0; i < w; i++){
		for(unsigned j = 0; j < h; j++){
			if ((i==w-1&& j==h-1) || (i==w-1 && j==0)) {
				f.push_back(state[indexOf(i, j)+1]+vel);
			}
			else{
				f.push_back(state[indexOf(i, j)+1]);
			}
			if ((i==w-1&& j==h-1) || (i==w-1 && j==0)) {
				f.push_back(Vector3f(0.0f, 0.0f, 0.0f));
			} else {
				f.push_back(getForce(i, j, state));
			}
		}
	}
	return f;
}

Vector3f ClothSystem::getForce(int x, int y, vector<Vector3f> state){
	//cout << " A SPRING " << x << " " << y << end;
	//return Vector3f::ZERO;
	float mass = 1.0f;
	float gravity = 0.5f;
	float k_spring = 51.0f;
	float k_viscosity = 0.08f;
	float rest_length = 0.25f;
	float k_shear_spring = 4.0f;
	float k_flex_spring = 10.0f;
	float shear_rest_length = sqrt(2*(0.25f*0.25f));
	float flex_rest_length = 0.5f;
	Vector3f pos = state[indexOf(x, y)];
	Vector3f vel = state[indexOf(x, y)+1];
	//gravity
	Vector3f g = Vector3f(0.0f, -mass * gravity, 0.0f);
	//viscous drag
	Vector3f vis = -k_viscosity * vel;
	//spring
	Vector3f spring = Vector3f(0.0f, 0.0f, 0.0f);
	if (x != 0) { 
	        Vector3f d = pos - state[indexOf(x-1, y)];
	        spring += -k_spring * (d.abs() - rest_length) * d.normalized();
	}
	if (x != w-1) { 
	        Vector3f d = pos - state[indexOf(x+1, y)];
	        spring += -k_spring * (d.abs() - rest_length) * d.normalized();
	}
	if (y > 0) { 
	        Vector3f d = pos - state[indexOf(x, y-1)];
	        spring += -k_spring * (d.abs() - rest_length) * d.normalized();
	}
	if (y < h-1) {
	        Vector3f d = pos - state[indexOf(x, y+1)];
	        spring += -k_spring * (d.abs() - rest_length) * d.normalized();
	}
	if (x != 0) {
        if (y != 0) {
                Vector3f d = pos - state[indexOf(x-1, y-1)];                
                spring += -k_shear_spring * (d.abs() - shear_rest_length) * d.normalized();
        }
        if (y != h-1) {
                Vector3f d = pos - state[indexOf(x-1, y+1)];
                spring += -k_shear_spring * (d.abs() - shear_rest_length) * d.normalized();
        }
	}
	if (x < w-1) {
        if (y > 0) {
                Vector3f d = pos - state[indexOf(x+1, y-1)];
                spring += -k_shear_spring * (d.abs() - shear_rest_length) * d.normalized();
        }
        if (y < h-1) {
                Vector3f d = pos - state[indexOf(x+1, y+1)];
                spring += -k_shear_spring * (d.abs() - shear_rest_length) * d.normalized();
        }
	}
    if (x > 1) {
        Vector3f d = pos - state[indexOf(x-2, y)];
        spring += -k_flex_spring * (d.abs() - flex_rest_length) * d.normalized();
    }
    if (x < w-2) {
        Vector3f d = pos - state[indexOf(x+2, y)];
        spring += -k_flex_spring * (d.abs() - flex_rest_length) * d.normalized();
    }
    if (y > 1) {
        Vector3f d = pos - state[indexOf(x, y-2)];
        spring += -k_flex_spring * (d.abs() - flex_rest_length) * d.normalized();
    }
    if (y < h-2) {
        Vector3f d = pos - state[indexOf(x, y+2)];
        spring += -k_flex_spring * (d.abs() - flex_rest_length) * d.normalized();
    }
	Vector3f total_force = (g + vis + spring)/mass;
	//cout << "FORCE " << temp[0] << " " << temp[0] << " " << temp[1] << " " << temp[2] << endl;
	if(useWind == true){
		total_force = (Vector3f(0.0f, 0.0f, 0.5f) + g + vis + spring)/mass;
	}
	return total_force;
}

void ClothSystem::toggleWind(){
	if(useWind==true){
		useWind = false;
		cout << "Turned off wind" << endl;
	}
	else{
		useWind = true;
		cout << "Turned on wind" << endl;
	}
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
	/*for(unsigned i = 0; i < w; i++){
		for(unsigned j = 0; j < h; j++){
			Vector3f pos = getState()[indexOf(i, j)];
			glPushMatrix();
			glTranslatef(pos[0], pos[1], pos[2] );
			glutSolidSphere(0.075f,10.0f,10.0f);
			glPopMatrix();
		}
	}
	drawSprings();*/
	glLineWidth(3.0f);
	glDisable(GL_LIGHTING);
	for (int i = 0; i < w; i++){
		glBegin(GL_LINE_STRIP);
		glColor3f(0.5f,0.5f,0.5f);
		for (int j = 0; j < h; j++){
			Vector3f vertex = getState()[indexOf(i, j)];
			glVertex3f(vertex.x(), vertex.y(), vertex.z());
		}
		glEnd();
	}
	for (int j = 0; j < h; j++){
		glBegin(GL_LINE_STRIP);
		glColor3f(0.5f,0.5f,0.5f);
		for (int i = 0; i < w; i++){
			Vector3f vertex = getState()[indexOf(i, j)];
			glVertex3f(vertex.x(), vertex.y(), vertex.z());
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
	glLineWidth(1.0f);
}

void ClothSystem::drawSprings(){
	for(unsigned i = 0; i < w; i++){
		for(unsigned j = 0; j < h; j++){
			Vector3f pos = getState()[indexOf(i, j)];
			if(i!=0){
				Vector3f pos_l = getState()[indexOf(i-1, j)];
		        glBegin(GL_LINES);
		        glVertex3fv(pos);
		        glVertex3fv(pos_l);
		        glEnd();
			}
			if(j!=0){
				Vector3f pos_u = getState()[indexOf(i, j-1)];
		        glBegin(GL_LINES);
		        glVertex3fv(pos);
		        glVertex3fv(pos_u);
		        glEnd();
			}
			if(i!=w-1){
				Vector3f pos_r = getState()[indexOf(i+1, j)];
		        glBegin(GL_LINES);
		        glVertex3fv(pos);
		        glVertex3fv(pos_r);
		        glEnd();
			}
			if(j!=h-1){
				Vector3f pos_b = getState()[indexOf(i, j+1)];
		        glBegin(GL_LINES);
		        glVertex3fv(pos);
		        glVertex3fv(pos_b);
		        glEnd();
			}
		}
	}
}

