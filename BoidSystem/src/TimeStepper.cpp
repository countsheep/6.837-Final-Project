#include "TimeStepper.h"

///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> end_state;
	vector<Vector3f> current_state = particleSystem->getState();
	vector<Vector3f> f = particleSystem->evalF(current_state);
	for(unsigned i = 0; i < current_state.size(); i++){
		end_state.push_back(current_state[i] + stepSize * f[i]);
	}
	particleSystem->setState(end_state);
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> X = particleSystem->getState();
	vector<Vector3f>f0 = particleSystem->evalF(X);

	vector<Vector3f> X_h;
	for(unsigned i = 0; i < X.size(); i++){
		X_h.push_back(X[i]+stepSize*f0[i]);
	}
	vector<Vector3f>f1 = particleSystem->evalF(X_h);

	vector<Vector3f> end_state;
	for(unsigned i = 0; i < X.size(); i++){
		end_state.push_back(X[i] + (stepSize / 2.0f) * (f0[i] + f1[i]));
	}
	particleSystem->setState(end_state);
}
