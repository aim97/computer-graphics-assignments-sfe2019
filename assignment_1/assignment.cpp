// shapes.cpp : This file contains the 'main' function. Program execution begins and ends there.

//#include "pch.h"
#include <bits/stdc++.h>

//Include OpenGL header files, so that we can use OpenGL
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define X first
#define Y second
#define point pair<float, float>
#define mp make_pair
#define pb push_back
#define PI 3.14159265359

using namespace std;

//Called when a key is pressed
void handleKeypress(unsigned char key, //The key that was pressed
	int x, int y) {    //The current mouse coordinates
	switch (key) {
	case 27: //Escape key
		exit(0); //Exit the program
	}
}

//Initializes 3D rendering
void initRendering() {
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
}

//Called when the window is resized
void handleResize(int w, int h) {
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
		(double)w / (double)h, //The width-to-height ratio
		1.0,                   //The near z clipping coordinate
		200.0);                //The far z clipping coordinate
}

void drawPolygon(point start, float l, int n){
    point tmp;        // create a tmp point
    vector<point> v;  // create a vector to be used to hold the points of the polygon
    v.pb(start);      // add the start point to the vector
	// angle : this is the slope angle of currently drawn line (created by last point and next one) with x-axis
	// theta : the angle between two consecutive lines in the same polygon
	// for n=4 --> theta=90, n=5 --> theta = 72, ...
    float angle, theta = 180 - ((n - 2) * 180) / n; 
    for(int i = 0;i < n - 1;i++){
        angle = i * theta; // get value of angle
        tmp.X = start.X + l * cos(angle * PI / 180); // compute X value of next point
        tmp.Y = start.Y + l * sin(angle * PI / 180); // compute Y value of next point
        v.pb(tmp);                                   // add computed point to vector v
        start = tmp;                                 // set start to hold the new computed point
    }
    
	// draw the polygon
    glBegin(GL_POLYGON);
        for(auto cur: v)
            glVertex3f(cur.X, cur.Y, -10.0);
    glEnd();
}

void drawScene() {
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	// draw scene objects
	float colnum = 1.8,
	      first_row = 2.2,
	      second_row = 0.0,
	      third_row = -2.2;
	point shift = mp(-0.3, -0.5);
	
	// hexagon
	drawPolygon(mp(colnum + shift.X,first_row + shift.Y), 1.0, 6);
	// octagon
	drawPolygon(mp(colnum + shift.X,second_row + shift.Y), 0.8, 8);
	// decagon
	drawPolygon(mp(colnum + shift.X,third_row + shift.Y), 0.6, 10);
	
	// pentagon
	drawPolygon(mp(-colnum + shift.X,first_row + shift.Y), 1.0, 5);
	// heptagon
	drawPolygon(mp(-colnum + shift.X,second_row + shift.Y), 0.8, 7);
	// nonagon
	drawPolygon(mp(-colnum + shift.X,third_row + shift.Y), 0.6, 9);


	glutSwapBuffers(); //Send the 3D scene to the screen
}


int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400); //Set the window size

	//Create the window
	glutCreateWindow("Basic Shapes");
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutMainLoop(); //Start the main loop.  glutMainLoop doesn't return.
	return 0; //This line is never reached
}









