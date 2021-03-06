#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>
#include <string.h>
#include "BoidSystem.h"
#include "Boid.h"
#include "BoundingBox.h"
#include "BoidController.h"

#include "extra.h"
#include "camera.h"

///TODO: include more headers if necessary


#include "Force.h"
#include "Image.h"
#include "OctTree.h"

using namespace std;

// Globals here.
namespace
{

    BoidSystem* boidSys;
    BoidController* boidController;
    float h = 0.02f;
    vector<vector<Force*>> forces;
    bool drawF = false;
    Vector3f f;
    bool move_to_goal = false;
    bool move_away_from_goal = false;
    bool move_to_start = false;
    Vector3f goal = Vector3f::ZERO;
    bool image = false;
    int max_boids = 600;
    bool oct = false;
    float scale = 1000;

  // initialize your particle systems
  ///TODO: read argv here. set timestepper , step size etc
  void initSystem(int argc, char * argv[])
  {
    // seed random number generator
    srand(time (0));
    BoundingBox box = BoundingBox(Vector3f(-7.0f, -7.0f, -7.0f), Vector3f(7.0f, 7.0f, 7.0f));

    //check if image command prompt thing is given 
    //then make an image boid
    char *input_file;

    Image *img;
    
    for (int i = 1; i < argc; i++) {
    	if (!strcmp(argv[i],"-image")) {
    		i++; assert (i < argc); 
    		input_file = argv[i];
    		img = Image::readBMP(input_file);
    		image = true;
    		
    	} else if (!strcmp(argv[i],"-oct")) {
    		oct = true;
    	
    	}
    	else if (!strcmp(argv[i], "-max_boids")) {
    		i++; assert (i < argc); 
    		max_boids = atoi(argv[i]);
    	}
    	else if (!strcmp(argv[i], "-scale")) {
    		i++; assert (i < argc); 
    		scale = atoi(argv[i]);
    	}
   }
   BoundingBox r_box(Vector3f(-7.0f, -7.0f, -7.0f), Vector3f(7.0f, 7.0f, 7.0f));
   
   
   if (image){
	   boidController = new BoidController(img, Vector3f(-7.0f, -7.0f, -7.0f), Vector3f(7.0f, 7.0f, 7.0f), scale, oct);
   }
   
   else{boidSys =  new BoidSystem(max_boids, r_box, oct);}

    
  }

 
  void stepSystem()
  {
    if (forces.size()>0){
    	if (forces[0].size()>0){
    		if (forces[0][0]->getAge()<1){
    			forces[0].erase(forces[0].begin());
    		}
    	}
    	else{forces.erase(forces.begin());}
    }
    for (int i = 0; i < forces.size(); i++){

    	vector<Force*> fv = forces[i];
    	for (int j = 0; j < fv.size(); j++){
    		Force *force = fv[j];
    		force->age();

    		force->grow();
    	}
    }
    if (drawF){
    	Force* f1 = forces.back().back();
    	if (f1->getAge()%20==0){
    		Force *fp = new Force(f, 55, 0.03f);
    		forces.back().push_back(fp);
    	}
    }
    if (image){
    boidController->stepSystem(forces, move_to_goal, move_away_from_goal, goal);
    }
    else{boidSys -> stepSystem(forces, move_to_goal, move_away_from_goal, goal);}
  }


  void drawSystem()
  {
  	if (image){boidController->draw();}
  	else{boidSys -> draw();}
    
  }
        

    //-------------------------------------------------------------------
    
        
    // This is the camera
    Camera camera;
    
    

    // These are state variables for the UI
    bool g_mousePressed = false;

    // Declarations of functions whose implementations occur later.
    void arcballRotation(int endX, int endY);
    void keyboardFunc( unsigned char key, int x, int y);
    void specialFunc( int key, int x, int y );
    void mouseFunc(int button, int state, int x, int y);
    void motionFunc(int x, int y);
    void reshapeFunc(int w, int h);
    void drawScene(void);
    void initRendering();

    // This function is called whenever a "Normal" key press is
    // received.
    void keyboardFunc( unsigned char key, int x, int y )
    {
        switch ( key )
        {
        case 27: // Escape key
            exit(0);
            break;
        case ' ':
        {
            Matrix4f eye = Matrix4f::identity();
            camera.SetRotation( eye );
            camera.SetCenter( Vector3f::ZERO );
            break;
        }

        default:
            cout << "Unhandled key press " << key << "." << endl;        
        }

        glutPostRedisplay();
    }

    // This function is called whenever a "Special" key press is
    // received.  Right now, it's handling the arrow keys.
    void specialFunc( int key, int x, int y )
    {
        switch ( key )
        {

        }
        //glutPostRedisplay();
    }

    //  Called when mouse button is pressed.
    void mouseFunc(int button, int state, int x, int y)
    {

    	int key = glutGetModifiers();

        if (state == GLUT_DOWN)
        {
            g_mousePressed = true;
            
            switch (button)
            {
            case GLUT_LEFT_BUTTON:
                if (key == GLUT_ACTIVE_CTRL){
                	camera.MouseClick(Camera::LEFT, x, y);
                }
                else if(key == GLUT_ACTIVE_ALT){
                    move_to_goal = true;
                    Vector3f center = Vector3f::ZERO;
                    if (image){
						for (int i = 0; i < boidController -> m_systems.size();i++){
							center = center + boidController -> m_systems[i].getCenterOfMass();
						}
						center = center/boidController -> m_systems.size();
					}
					else{
						center = boidSys ->getCenterOfMass();
					}
                    goal = camera.Camera::getForcePoint(center, x, y);
                }
                else if(key == GLUT_ACTIVE_SHIFT){
                    move_away_from_goal = true;
                    Vector3f center = Vector3f::ZERO;
                   if (image){
						for (int i = 0; i < boidController -> m_systems.size();i++){
							center = center + boidController -> m_systems[i].getCenterOfMass();
						}
						center = center/boidController -> m_systems.size();
					}
					else{
						center = boidSys ->getCenterOfMass();
					}
                    goal = camera.Camera::getForcePoint(center, x, y);
                }
                else{
                	Vector3f center = Vector3f::ZERO;

                	if (drawF == false){
                		if (image){
							for (int i = 0; i < boidController -> m_systems.size();i++){
								center = center + boidController -> m_systems[i].getCenterOfMass();
							}
							center = center/boidController -> m_systems.size();
							}
							else{
								center = boidSys ->getCenterOfMass();
							}
		            	Vector3f force = camera.Camera::getForcePoint(center, x, y);
		            	f = force;
                	}
                	drawF = true;
                	vector<Force*> fv;
                	Force *fp = new Force(f, 55, 0.03f);
                	fv.push_back(fp);
                	forces.push_back(fv);
                }
                break;
            case GLUT_MIDDLE_BUTTON:
                camera.MouseClick(Camera::MIDDLE, x, y);
                break;
            case GLUT_RIGHT_BUTTON:
                camera.MouseClick(Camera::RIGHT, x,y);
            default:
                break;
            }                       
        }
        else
        {
            camera.MouseRelease(x,y);
            g_mousePressed = false;
            drawF = false;
            move_to_start = false;
            move_to_goal = false;
            move_away_from_goal = false;
        }
        glutPostRedisplay();
    }

    // Called when mouse is moved while button pressed.
    void motionFunc(int x, int y)
    {
        camera.MouseDrag(x,y);
        Vector3f center = Vector3f::ZERO;
        if (image){
			for (int i = 0; i < boidController -> m_systems.size();i++){
				center = center + boidController -> m_systems[i].getCenterOfMass();
			}
			center = center/boidController -> m_systems.size();
		}
		else{
			center = boidSys ->getCenterOfMass();
		}
    	Vector3f force = camera.Camera::getForcePoint(center, x, y);
    	f = force;       
    
        glutPostRedisplay();
    }

    // Called when the window is resized
    // w, h - width and height of the window in pixels.
    void reshapeFunc(int w, int h)
    {
        camera.SetDimensions(w,h);

        camera.SetViewport(0,0,w,h);
        camera.ApplyViewport();

        // Set up a perspective view, with square aspect ratio
        glMatrixMode(GL_PROJECTION);

        camera.SetPerspective(50);
        glLoadMatrixf( camera.projectionMatrix() );
    }

    // Initialize OpenGL's rendering modes
    void initRendering()
    {
        glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
        glEnable(GL_LIGHTING);     // Enable lighting calculations
        glEnable(GL_LIGHT0);       // Turn on light #0.

        glEnable(GL_NORMALIZE);

        // Setup polygon drawing
        glShadeModel(GL_SMOOTH);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

        // Clear to black
        glClearColor(0,0,0,1);
    }

    // This function is responsible for displaying the object.
    void drawScene(void)
    {
        // Clear the rendering window
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode( GL_MODELVIEW );  
        glLoadIdentity();              

        // Light color (RGBA)
        GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
        GLfloat Lt0pos[] = {3.0,3.0,5.0,1.0};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
        glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

        glLoadMatrixf( camera.viewMatrix() );

        // THIS IS WHERE THE DRAW CODE GOES.

        drawSystem();
		for (int i = 0; i < forces.size(); i++){
			vector<Force*> fv = forces[i];
			for (int j = 0; j < fv.size(); j++){
				Force *force = fv[j];
				//glPushMatrix();
				//glLoadIdentity();
				glPushMatrix();
				Vector3f trans = force -> getCenter();

				glTranslatef(trans.x(), trans.y(), trans.z());
				Matrix4f m = camera.viewMatrix();
				m.inverse();
				force->draw(m);

				glPopMatrix();
			}
		}

        // This draws the coordinate axes when you're rotating, to
        // keep yourself oriented.
        if( g_mousePressed )
        {
            glPushMatrix();
            Vector3f eye = camera.GetCenter();
            glTranslatef( eye[0], eye[1], eye[2] );

            // Save current state of OpenGL
            glPushAttrib(GL_ALL_ATTRIB_BITS);

            // This is to draw the axes when the mouse button is down
            glDisable(GL_LIGHTING);
            glLineWidth(3);
            glPushMatrix();
            glScaled(5.0,5.0,5.0);
            glBegin(GL_LINES);
            glColor4f(1,0.5,0.5,1); glVertex3f(0,0,0); glVertex3f(1,0,0);
            glColor4f(0.5,1,0.5,1); glVertex3f(0,0,0); glVertex3f(0,1,0);
            glColor4f(0.5,0.5,1,1); glVertex3f(0,0,0); glVertex3f(0,0,1);

            glColor4f(0.5,0.5,0.5,1);
            glVertex3f(0,0,0); glVertex3f(-1,0,0);
            glVertex3f(0,0,0); glVertex3f(0,-1,0);
            glVertex3f(0,0,0); glVertex3f(0,0,-1);

            glEnd();
            glPopMatrix();
            
            glPopAttrib();
            glPopMatrix();
        }
                 
        // Dump the image to the screen.
        glutSwapBuffers();
    }

    void timerFunc(int t)
    {

        stepSystem();
        glutPostRedisplay();

        glutTimerFunc(t, &timerFunc, t);
    }

    

    
    
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char* argv[] )
{
    //setup opengl
    glutInit( &argc, argv );

    // We're going to animate it, so double buffer 
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 600, 600 );
    
    camera.SetDimensions( 600, 600 );

    camera.SetDistance( 10 );
    camera.SetCenter( Vector3f::ZERO );
    
    glutCreateWindow("Assignment 3");

    // Initialize OpenGL parameters.
    initRendering();

    // Setup particle system
    initSystem(argc,argv);

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

    // Set up callback functions for mouse
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);

    // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Trigger timerFunc every 20 msec
    glutTimerFunc(20, timerFunc, 20);

        
    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();

    return 0;	// This line is never reached.
}
