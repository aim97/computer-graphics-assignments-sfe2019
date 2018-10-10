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
    point tmp;
    vector<point> v;
    v.pb(start);
    float angle, theta = 180 - ((n - 2) * 180) / n;
    for(int i = 0;i < n - 1;i++){
        angle = i * theta;
        tmp.X = start.X + l * cos(angle * PI / 180);
        tmp.Y = start.Y + l * sin(angle * PI / 180);
        v.pb(tmp);
        start = tmp;
    }
    
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
	
	drawPolygon(mp(colnum + shift.X,first_row + shift.Y), 1.0, 6);
	drawPolygon(mp(colnum + shift.X,second_row + shift.Y), 0.8, 8);
	drawPolygon(mp(colnum + shift.X,third_row + shift.Y), 0.6, 10);

	drawPolygon(mp(-colnum + shift.X,first_row + shift.Y), 1.0, 5);
	drawPolygon(mp(-colnum + shift.X,second_row + shift.Y), 0.8, 7);
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









