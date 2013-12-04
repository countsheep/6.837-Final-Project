#include "BoidController.h"

using namespace std;


BoidController::BoidController(Image* img, Vector3f minBounds, Vector3f maxBounds){
	// first boidsystem will have a dominant color of red, second will be green, etc. 
	vector<Boid*> red;
	vector<Boid*> green;
	vector<Boid*> blue;
	Image reduced_img = downSample(img);
	for (int i = 0; i < reduced_img.Width(); i++){
		for( int j = 0; j < reduced_img.Height(); j++){
			Vector3f pos = Vector3f(0.075f*(i - reduced_img.Width()/2), 0.075f*(j - reduced_img.Height()/2), 0.0f);
			Boid* b = new Boid(pos, Vector3f::ZERO, 10.0f, 0.25f);
			b->m_color = reduced_img.GetPixel(i, j);
			//choose which flock to add boid to
			if(b->m_color.x() >= b->m_color.y() && b->m_color.x() >= b->m_color.z()){
				red.push_back(b);
			}
			else if(b->m_color.y() >= b->m_color.x() && b->m_color.y() >= b->m_color.z()){
				green.push_back(b);
			}
			else{
				blue.push_back(b);
			}
		}
	}
	//initialize boid systems
	BoundingBox r_box(minBounds, maxBounds);
	BoundingBox g_box(minBounds, maxBounds);
	BoundingBox b_box(minBounds, maxBounds);
	BoidSystem r_sys(r_box, red, "red");
	BoidSystem g_sys(g_box, green, "green");
	BoidSystem b_sys(b_box, blue, "blue");
	m_systems.push_back(r_sys);
	m_systems.push_back(g_sys);
	m_systems.push_back(b_sys);
}

void BoidController::draw(){
	for(int i = 0; i < m_systems.size(); i++){
		m_systems[i].draw();
	}
}

//move 
//1. move towards center of mass
//2. try to keep distance away
//3. match velocity of other boids
//4. some kind of goal
// also, keep velocity under max speed

void BoidController::stepSystem(vector<vector<Force*>> f){
	for(int i = 0; i < m_systems.size(); i++){
		m_systems[i].stepSystem(f);
	}
}

Image BoidController::downSample(Image* img){
	float total_scale = (img->Width() * img->Height())/1000;
	int scale = sqrt(total_scale);
	Image img_out( img->Width()/scale , img->Height()/scale );

	//downsampling 
    for(int i = 0; i < img->Width()/scale; i++){
        for(int j = 0; j < img->Height()/scale; j++){
            Vector3f sum;
            for(int k = 0; k < scale; k++){
            	for(int l = 0; l < scale; l++){
            		sum += img->GetPixel(scale * i + k, scale * j + l);
            	}
            }
            img_out.SetPixel(i, j, sum/pow(scale, 2));
        }
    }
	return img_out;
}
