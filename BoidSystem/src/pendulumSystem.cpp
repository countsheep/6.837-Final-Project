
#include "pendulumSystem.h"
#include <iostream>
using namespace std;

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
	
	vector<Vector3f> init;
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		// for this system, we care about the position and the velocity
		init.push_back(Vector3f(i, i, i));
        init.push_back(Vector3f(0.0f, 0.0f ,0.0f));
	}
	setState(init);
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	//constants
	float mass = 1.0f;
	float gravity = 0.98f;
	float k_spring = 5.0f;
	float k_viscosity = 0.5f;
	float rest_length = 0.25f;

	vector<Vector3f> f;
	f.push_back(state[1]);
    f.push_back(Vector3f(0.0f, 0.0f, 0.0f));

	for(unsigned i = 1; i < m_numParticles; i++){
		//derivative of position is velocity
		f.push_back(state[2*i+1]);
        //gravity
        Vector3f g = Vector3f(0.0f, -mass * gravity, 0.0f);
        //viscous drag
        Vector3f vis = -k_viscosity * state[2*i+1];
        //spring
        Vector3f spring = Vector3f(0.0f, 0.0f, 0.0f);
        //for everything but first point has a pre-spring
        if (i != 0) {
            Vector3f d = state[2*i] - state[2*i-2];
            spring += -k_spring * (d.abs() - rest_length) * d.normalized();
        }
        //everything but last point
        if (i != m_numParticles - 1) {
            Vector3f d = state[2*i] - state[2*i+2];
            spring += -k_spring * (d.abs() - rest_length) * d.normalized();
        }
        f.push_back((g + vis + spring)/mass);
	}
	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	//draw points
	for (int i = 0; i < m_numParticles; i++) {
		Vector3f pos = getState()[2*i];//  position of particle i. YOUR CODE HERE
		Vector3f pos2 = getState()[2*(i+1)];
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
	//draw spring
	drawSprings();
}

void PendulumSystem::drawSprings()
{
    for (unsigned i = 0; i < m_numParticles-1; i++)
    {        
        glPushMatrix();
        Vector3f pos = getState()[2*i];//  position of particle i. YOUR CODE HERE
		Vector3f pos2 = getState()[2*(i+1)];
        glBegin(GL_LINES);
        glVertex3fv(pos);
        glVertex3fv(pos2);
        glEnd();
        glPopMatrix();
    }
}
