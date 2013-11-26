
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
	vector<Vector3f> init;
	init.push_back(Vector3f(1.0f, 1.0f, 0.0f));
	init.push_back(Vector3f(1.0f, 1.0f, 0.0f));
	setState(init);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	for(unsigned i = 0; i < state.size(); i++){
		f.push_back(Vector3f(-state[i][1], state[i][0], 0));
	}
	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
		Vector3f pos = getState()[0];//YOUR PARTICLE POSITION
	  glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
}
